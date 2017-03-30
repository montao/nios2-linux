/*
 * File:         drivers/char/bfin_spi_adc.c
 * Based on:
 * Author:       Michael Hennerich, Luke Yang
 *
 * Created:
 * Description:
 *
 *
 * Modified:
 *               Copyright 2004-2008 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/spi/spi.h>
#include <linux/spinlock.h>
#include <linux/dma-mapping.h>

#define CMD_SPI_SET_BAUDRATE  		_IOW('f', 0, unsigned long)
#define CMD_SPI_GET_SYSTEMCLOCK   	_IOR('f', 1, unsigned long)
#define CMD_SPI_SET_WRITECONTINUOUS     _IOW('f', 2, unsigned long)

struct spi_adc {
	int             opened;
	unsigned short  *buffer;
	int             hz;
	int             cont;
	struct spi_device *spidev;
	dma_addr_t dma_handle;
};

static struct spi_adc spi_adc;
static DEFINE_SPINLOCK(spiadc_lock);

static long adc_spi_ioctl(struct file *filp, uint cmd, unsigned long arg)
{
	long ret = 0;
	unsigned long value;
	struct spi_adc *spi_adc = filp->private_data;

	switch (cmd) {
	case CMD_SPI_GET_SYSTEMCLOCK:
		value = get_sclk();
		ret = copy_to_user((unsigned long *)arg, &value, sizeof(unsigned long)) ? -EFAULT : 0;
		break;
	case CMD_SPI_SET_BAUDRATE:
		if (arg > (133000000 / 4))
			return -EINVAL;
		spi_adc->hz = arg;
		break;
	case CMD_SPI_SET_WRITECONTINUOUS:
		spi_adc->cont = (unsigned char)arg;
		break;
	default:
		return -EINVAL;
	}
	return ret;
}

static ssize_t adc_spi_read(struct file *filp, char *buf, size_t count, loff_t *pos)
{
	int stat;
	struct spi_adc *spi_adc = filp->private_data;
	u8 *buffer;

	struct spi_transfer	t = {
			.len		= count,
			.speed_hz	= spi_adc->hz,
		};
	struct spi_message	m;

	if (count <= 0)
		return -EINVAL;

	buffer = dma_alloc_coherent(NULL, count, &spi_adc->dma_handle, GFP_KERNEL);

	if (buffer == NULL)
		return -ENOMEM;

	t.rx_buf = buffer;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);

	stat = spi_sync(spi_adc->spidev, &m);

	if (stat == 0) {
		unsigned long	missing;

		missing = copy_to_user(buf, buffer, count);
		if (count && missing == count)
			stat = -EFAULT;
		else
			stat = count - missing;
	}

	dma_free_coherent(NULL, count, buffer, spi_adc->dma_handle);

	return stat;
}

static ssize_t adc_spi_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int stat;
	unsigned long missing;
	struct spi_adc *spi_adc = filp->private_data;
	u8 *buffer;

	struct spi_transfer	t = {
			.len		= count,
			.speed_hz	= spi_adc->hz,
		};
	struct spi_message	m;

	if (count <= 0)
		return -EINVAL;

	buffer = dma_alloc_coherent(NULL, count, &spi_adc->dma_handle, GFP_KERNEL);

	if (buffer == NULL)
		return -ENOMEM;

	t.tx_buf = buffer;

	if (spi_adc->cont)  /* dirty hack for continuous DMA output mode */
		t.tx_dma = 0xFFFF;

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);

	missing = copy_from_user(buffer, buf, count);

	if (missing == 0) {
		stat = spi_sync(spi_adc->spidev, &m);
		if (stat == 0)
			stat = count;
	} else
		stat = -EFAULT;

	dma_free_coherent(NULL, count, buffer, spi_adc->dma_handle);

	return stat;
}

static int adc_spi_open(struct inode *inode, struct file *filp)
{
	unsigned long flags;

	spin_lock_irqsave(&spiadc_lock, flags);
	if (spi_adc.opened){
		spin_unlock_irqrestore(&spiadc_lock, flags);
		return -EMFILE;
	}

	if (spi_adc.spidev == 0) {
		spin_unlock_irqrestore(&spiadc_lock, flags);
		pr_err("spi_adc: SPI driver failed to register\n");
		return -ENODEV;
	}

	spi_adc.opened = 1;

	filp->private_data = &spi_adc;
	spin_unlock_irqrestore(&spiadc_lock, flags);

	return 0;
}

static int adc_spi_release(struct inode *inode, struct file *filp)
{
	unsigned long flags;
	struct spi_adc *spi_adc = filp->private_data;

	spin_lock_irqsave(&spiadc_lock, flags);
	spi_adc->opened = 0;
	spi_adc->hz = 0;
	spi_adc->cont = 0;
	spin_unlock_irqrestore(&spiadc_lock, flags);

	return 0;
}

static const struct file_operations spi_adc_fops = {
	.owner = THIS_MODULE,
	.read = adc_spi_read,
	.write = adc_spi_write,
	.unlocked_ioctl = adc_spi_ioctl,
	.open = adc_spi_open,
	.release = adc_spi_release,
};

static int __devinit spi_adc_probe(struct spi_device *spi)
{
	spi_adc.spidev = spi;

	return 0;
}

static int __devexit spi_adc_remove(struct spi_device *spi)
{
	pr_info("spi_adc: goodbye\n");
	return 0;
}

static struct spi_driver spi_adc_driver = {
	.driver = {
		.name	= "bfin_spi_adc",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
	},
	.probe	= spi_adc_probe,
	.remove	= __devexit_p(spi_adc_remove),
};

static struct miscdevice adc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "spi",
	.fops  = &spi_adc_fops
};

static int __init spi_adc_init(void)
{
	int result;
	result = misc_register(&adc_dev);
	if (result < 0) {
		pr_err("spi_adc: failed to register\n");
		return result;
	}

	return spi_register_driver(&spi_adc_driver);
}
module_init(spi_adc_init);

static void __exit spi_adc_exit(void)
{
	spi_unregister_driver(&spi_adc_driver);
	misc_deregister(&adc_dev);
}
module_exit(spi_adc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SPI-ADC/DAC Driver");
MODULE_AUTHOR("Luke Yang");
