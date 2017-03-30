/*
 * Preempt kernel latency tset driver
 *
 * Author: (C) 2010 by Bob Liu (lliubbo@gmail.com)
 *
 * This is a simple char-device interface driver for gptimer TIMER2.
 * It primarily serves for the preempt kernel latency test application 
 * in userspace to test the preempt kernel (CONFIG_PREEMPT is on) 
 * latency on blackfin.
 *
 * Behaviour
 * This driver config the gptimer2 sendout interrupt every second, the
 * isr handle record the time and send SIGIO to userapp preempt_test.
 * preempt_test will read the recorded time when scheduled and 
 * compare with the actual time to calculate the latency.
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/time.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/gptimers.h>
#include <asm/irq.h>
#include <asm/bfin-global.h>

#define BFIN_LATENCYTEST_IOCTL_MAGIC 'r'
#define BFIN_LATENCYTEST_START       _IO(BFIN_LATENCYTEST_IOCTL_MAGIC, \
						6)
#define BFIN_LATENCYTEST_STOP        _IO(BFIN_LATENCYTEST_IOCTL_MAGIC, \
						8)
#define TIMER_MAJOR 	  	     0
#define DRV_NAME    	             "bfin_latencytest"

static struct timeval interrupt_time;
static struct fasync_struct *bfin_async_queue;

static int timer_ioctl(struct inode *inode, struct file *filp, uint cmd,
		unsigned long arg)
{
	unsigned long n;
	switch (cmd) {
		case BFIN_LATENCYTEST_START:
			set_gptimer_config(TIMER2_id, OUT_DIS | PWM_OUT |
					PERIOD_CNT | IRQ_ENA);
			n = get_sclk();
			set_gptimer_period(TIMER2_id, n); /* one second */
			enable_gptimers(TIMER2bit);
			break;
		case BFIN_LATENCYTEST_STOP:
			disable_gptimers(TIMER2bit);
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

static irqreturn_t timer_isr(int irq, void *dev_id)
{
	unsigned long state = get_gptimer_status(0);
	if (state & TIMER_STATUS_TIMIL2) {
		do_gettimeofday(&interrupt_time);
		set_gptimer_status(0, TIMER_STATUS_TIMIL2);
		kill_fasync(&bfin_async_queue, SIGIO, POLL_IN);
	}
	return IRQ_HANDLED;
}

static ssize_t timer_read(struct file *file, char *buf, size_t count,
		loff_t *ppos)
{
	ssize_t retval;

	if(count != sizeof(struct timeval))
		return -EINVAL;
	retval = -EIO;
	if (copy_to_user(buf, &interrupt_time, sizeof(struct timeval)) == 0)
		retval = sizeof(struct timeval);
	return retval;
}

static int timer_open(struct inode *inode, struct file *filp)
{
	int minor = MINOR(inode->i_rdev);
	int err = 0;

	err = request_irq(IRQ_TIMER2, timer_isr, IRQF_DISABLED, DRV_NAME,
			(void *)minor);
	if (err < 0) {
		printk(KERN_ERR "request_irq(%d) failed\n", IRQ_TIMER2);
		return err;
	}
	pr_debug(DRV_NAME ": device(%d) opened\n", minor);
	return 0;
}

static int timer_fasync(int fd, struct file *file, int on)
{
	return fasync_helper(fd, file, on, &bfin_async_queue);
}

static int timer_close(struct inode *inode, struct file *filp)
{
	int minor = MINOR(inode->i_rdev);
	disable_gptimers(TIMER2bit);
	free_irq(IRQ_TIMER2, (void *)minor);
	pr_debug(DRV_NAME ": device closed\n");
	return 0;
}

static const struct file_operations fops = {
	.owner   = THIS_MODULE,
	.ioctl   = timer_ioctl,
	.read    = timer_read,
	.fasync  = timer_fasync,
	.open    = timer_open,
	.release = timer_close,
};

static struct miscdevice bfin_latencytest_misc_device = {
	.minor    = MISC_DYNAMIC_MINOR,
	.name     = DRV_NAME,
	.fops     = &fops,
};

static int __init timer_initialize(void)
{
	int ret;
	ret = misc_register(&bfin_latencytest_misc_device);
	if (ret) {
		pr_debug(KERN_ERR "unable to register a misc device\n");
		return ret;
	}

	pr_debug(KERN_INFO "initialized\n");
	return 0;
}
module_init(timer_initialize);

static void __exit timer_cleanup(void)
{
	misc_deregister(&bfin_latencytest_misc_device);
	pr_debug(DRV_NAME ": module unloaded\n");
}
module_exit(timer_cleanup);

MODULE_AUTHOR("Bob Liu <lliubbo@gmail.com>");
MODULE_DESCRIPTION("latencytest char-device interface for preempt test");
MODULE_LICENSE("GPL");
