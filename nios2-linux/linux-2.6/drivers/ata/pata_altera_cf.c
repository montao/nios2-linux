/*
 *  pata_altera_cf.c - PATA driver for Altera SOPC Builder CompactFlash core.
 *
 *  Copyright (C) 2009 MEV Limited <http://www.mev.co.uk>
 *  Author: Ian Abbott <abbotti@mev.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  This file was based on/inspired by:
 *    drivers/ata/pata_platform.c
 *    drivers/ata/pata_pcmcia.c
 *    drivers/ata/pata_ixp4xx_cf.c
 *    altcf.c from the Microtronix Nios II Linux distribution.
 *
 *  This is a platform device driver and expects two memory resources and
 *  two IRQ resources per platform device in the following:
 *
 *    IDE memory resource -- 64 byte memory area consisting of IDE registers
 *                           on 4-byte boundaries.
 *    IDE IRQ resource    -- IRQ resource consisting of IRQ number and desired
 *                           IRQ flags for the IDE interface.  The IRQ number
 *                           can be set to 0 to avoid setting up the IRQ.
 *    CF CONTROL memory resource -- 16 byte resource consisting of the CF
 *                           CONTROL registers on 4-byte boundaries.
 *    CF CONTROL IRQ resource -- IRQ resource consisting of IRQ number and
 *                           desired IRQ flags for the CF CONTROL interface.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <scsi/scsi_host.h>
#include <linux/ata.h>
#include <linux/libata.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/of.h>

#define DRV_NAME "pata_altera_cf"
#define DRV_VERSION "0.01"


#define ALTCF_SHIFT		2

/* IDE registers */
#define ALTCF_IDE_LEN		(16 << ALTCF_SHIFT)
#define ALTCF_IDE_REG_DATA	(ATA_REG_DATA << ALTCF_SHIFT)
#define ALTCF_IDE_REG_ERR	(ATA_REG_ERR << ALTCF_SHIFT)
#define ALTCF_IDE_REG_NSECT	(ATA_REG_NSECT << ALTCF_SHIFT)
#define ALTCF_IDE_REG_LBAL	(ATA_REG_LBAL << ALTCF_SHIFT)
#define ALTCF_IDE_REG_LBAM	(ATA_REG_LBAM << ALTCF_SHIFT)
#define ALTCF_IDE_REG_LBAH	(ATA_REG_LBAH << ALTCF_SHIFT)
#define ALTCF_IDE_REG_DEVICE	(ATA_REG_DEVICE << ALTCF_SHIFT)
#define ALTCF_IDE_REG_STATUS	(ATA_REG_STATUS << ALTCF_SHIFT)
#define ALTCF_IDE_REG_FEATURE	(ATA_REG_FEATURE << ALTCF_SHIFT)
#define ALTCF_IDE_REG_CMD	(ATA_REG_CMD << ALTCF_SHIFT)
#define ALTCF_IDE_REG_CTL	(14 << ALTCF_SHIFT)

/* CF CONTROL registers */
#define ALTCF_CFC_LEN		(4 << ALTCF_SHIFT)
#define ALTCF_CFC_REG_CFCTL	(0 << ALTCF_SHIFT)
#define ALTCF_CFC_REG_IDECTL	(1 << ALTCF_SHIFT)

/* CFCTL register bitmasks */
#define ALTCF_CFCTL_DET		0x01	/* CF detected */
#define ALTCF_CFCTL_PWR		0x02	/* power */
#define ALTCF_CFCTL_RST		0x04	/* reset */
#define ALTCF_CFCTL_IDET	0x08	/* "CF detect change" intr enable */

/* IDECTL register bitmasks */
#define ALTCF_IDECTL_IIDE	0x01	/* IDE interrupt enable */

/* Macros to read/write CF CONTROL registers. */
#define READ_CFCTL(altcf)	readl((altcf)->cfc_base + ALTCF_CFC_REG_CFCTL)
#define WRITE_CFCTL(altcf, d)	\
	writel((d), (altcf)->cfc_base + ALTCF_CFC_REG_CFCTL)
#define READ_IDECTL(altcf)	readl((altcf)->cfc_base + ALTCF_CFC_REG_IDECTL)
#define WRITE_IDECTL(altcf, d)	\
	writel((d), (altcf)->cfc_base + ALTCF_CFC_REG_IDECTL)

/*
 * This is the driver's private data for the device.  The struct device's
 * private data pointer is used by the libata core so we can't use it ourselves.
 * Instead, we use the devres functions to access it.  This is a lot slower
 * than using the device's private data pointer but we shouldn't have to do it
 * very often.
 */
struct altcf_private {
	struct device *dev;		/* point back to device */
	resource_size_t ide_raw_base;	/* unmapped IDE registers */
	unsigned char __iomem *ide_base;/* mapped IDE registers */
	unsigned char __iomem *cfc_base;/* mapped CF CONTROL registers */
	struct task_struct *cf_thread;	/* kthread to handle CF events */
	int cfc_irq;			/* IRQ for CF CONTROL */
	int ide_irq;			/* IRQ for IDE */
	unsigned long ide_irqflags;	/* IRQ flags for IDE */
	unsigned long event;		/* event/state bits */
	spinlock_t cfctl_lock;		/* spinlock to change CFCTL */
	unsigned char cfctl;		/* CFCTL register */
	void *ata_group_id;		/* devres group ID of ATA resources */
};

/* Event/state bits. */
enum altcf_event {
	EV_CHANGE,
	EV_HOLD,
	EV_DEAD
};

/* Update the CFCTL register. */
static void update_cfctl(struct altcf_private *altcf,
		unsigned char mask, unsigned char set)
{
	unsigned long flags;

	set &= mask;
	spin_lock_irqsave(&altcf->cfctl_lock, flags);
	altcf->cfctl = (altcf->cfctl & ~mask) | set;
	WRITE_CFCTL(altcf, altcf->cfctl);
	spin_unlock_irqrestore(&altcf->cfctl_lock, flags);
}

/* IRQ handler to handle CF detection state changes. */
static irqreturn_t altcf_cf_interrupt(int irq, void *context)
{
	struct altcf_private *altcf = context;

	READ_CFCTL(altcf);	/* Clear interrupt */
	/* Record the fact that something changed. */
	set_bit(EV_CHANGE, &altcf->event);
	smp_mb();
	/* Wake up the kthread to deal with it. */
	wake_up_process(altcf->cf_thread);
	return IRQ_HANDLED;
}

/* Provide our own set_mode(). */
static int altcf_set_mode(struct ata_link *link, struct ata_device **error)
{
	struct ata_device *dev;

	ata_for_each_dev(dev, link, ENABLED) {
		ata_dev_printk(dev, KERN_INFO, "configured for PIO0\n");
		dev->pio_mode = XFER_PIO_0;
		dev->xfer_mode = XFER_PIO_0;
		dev->xfer_shift = ATA_SHIFT_PIO;
		dev->flags |= ATA_DFLAG_PIO;
	}
	return 0;
}

static struct scsi_host_template altcf_sht = {
	ATA_PIO_SHT(DRV_NAME),
};

static struct ata_port_operations altcf_port_ops = {
	.inherits		= &ata_sff_port_ops,
	.sff_data_xfer		= ata_sff_data_xfer_noirq,
	.cable_detect		= ata_cable_40wire,
	.set_mode		= altcf_set_mode,
	.port_start		= ATA_OP_NULL,
};

/* Checks if CF present and sets up ATA host. */
static void altcf_detect_cf(struct altcf_private *altcf)
{
	struct ata_host *host;
	struct ata_port *ap;
	void *group_id;

	if ((READ_CFCTL(altcf) & ALTCF_CFCTL_DET) == 0)
		return;

	dev_dbg(altcf->dev, "CF detected\n");
	/* Hold in reset powered down for 0.5 seconds. */
	update_cfctl(altcf, ALTCF_CFCTL_RST | ALTCF_CFCTL_PWR, ALTCF_CFCTL_RST);
	msleep(500);
	if (test_bit(EV_CHANGE, &altcf->event)
			|| test_bit(EV_DEAD, &altcf->event)) {
		goto fail_reset_sleep;
	}
	/* Remove reset, power up and wait 0.5 seconds. */
	update_cfctl(altcf, ALTCF_CFCTL_RST | ALTCF_CFCTL_PWR, ALTCF_CFCTL_PWR);
	msleep(500);
	if (test_bit(EV_CHANGE, &altcf->event)
			|| test_bit(EV_DEAD, &altcf->event)) {
		goto fail_power_sleep;
	}

	/* Create devres group to manage ATA host resources. */
	group_id = devres_open_group(altcf->dev, NULL, GFP_KERNEL);
	if (group_id == NULL) {
		dev_warn(altcf->dev, "failed to allocate ATA group\n");
		goto fail_open_group;
	}

	/* Allocate ATA host with single port. */
	host = ata_host_alloc(altcf->dev, 1);
	if (host == NULL) {
		dev_warn(altcf->dev, "failed to allocate ATA host\n");
		goto fail_ata_host_alloc;
	}

	ap = host->ports[0];
	ap->ops = &altcf_port_ops;
	ap->pio_mask = 0x1f;	/* PIO4 */

	/* Use polling mode if there's no IRQ. */
	if (!altcf->ide_irq) {
		ap->flags |= ATA_FLAG_PIO_POLLING;
		ata_port_desc(ap, "no IRQ, using PIO polling");
	}

	/* Set up register addresses. */
	/* command section */
	ap->ioaddr.cmd_addr = altcf->ide_base;
	ap->ioaddr.data_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_DATA;
	ap->ioaddr.error_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_ERR;
	ap->ioaddr.feature_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_FEATURE;
	ap->ioaddr.nsect_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_NSECT;
	ap->ioaddr.lbal_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_LBAL;
	ap->ioaddr.lbam_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_LBAM;
	ap->ioaddr.lbah_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_LBAH;
	ap->ioaddr.device_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_DEVICE;
	ap->ioaddr.status_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_STATUS;
	ap->ioaddr.command_addr = ap->ioaddr.cmd_addr + ALTCF_IDE_REG_CMD;
	/* control section */
	ap->ioaddr.ctl_addr = altcf->ide_base + ALTCF_IDE_REG_CTL;
	ap->ioaddr.altstatus_addr = ap->ioaddr.ctl_addr;
	ata_port_desc(ap, "mmio cmd 0x%llx ctl 0x%llx",
			(unsigned long long)altcf->ide_raw_base,
			(unsigned long long)altcf->ide_raw_base
			+ ALTCF_IDE_REG_CTL);

	/* Enable the IDE IRQ if using it. */
	if (altcf->ide_irq)
		WRITE_IDECTL(altcf, ALTCF_IDECTL_IIDE);

	/* Activate the ATA host. */
	if (ata_host_activate(host, altcf->ide_irq,
				altcf->ide_irq ? ata_sff_interrupt : NULL,
				altcf->ide_irqflags, &altcf_sht)) {
		dev_warn(altcf->dev, "failed to activate ATA host\n");
		goto fail_ata_host_activate;
	}

	/* Success! */
	altcf->ata_group_id = group_id;
	devres_close_group(altcf->dev, group_id);
	return;

fail_ata_host_activate:

	/* Disable IDE IRQ. */
	WRITE_IDECTL(altcf, 0);
fail_ata_host_alloc:

	devres_release_group(altcf->dev, group_id);
fail_open_group:
fail_power_sleep:

	/* Hold in reset powered down. */
	update_cfctl(altcf, ALTCF_CFCTL_RST | ALTCF_CFCTL_PWR, ALTCF_CFCTL_RST);
fail_reset_sleep:

	dev_dbg(altcf->dev, "CF detection cancelled\n");
}

/* Get rid of old ATA host, if any. */
static void altcf_remove_ata(struct altcf_private *altcf)
{
	void *group_id;
	struct ata_host *host;

	/* Disable IDE IRQ. */
	WRITE_IDECTL(altcf, 0);
	/* Look for ATA resources. */
	group_id = altcf->ata_group_id;
	altcf->ata_group_id = NULL;
	if (group_id) {
		dev_dbg(altcf->dev, "removing old CF\n");
		/* Look for ATA host and detach it. */
		host = dev_get_drvdata(altcf->dev);
		if (host)
			ata_host_detach(host);
		/* Release ATA resources. */
		devres_release_group(altcf->dev, group_id);
	}
	/* Hold CF in reset powered down. */
	update_cfctl(altcf, ALTCF_CFCTL_RST | ALTCF_CFCTL_PWR, ALTCF_CFCTL_RST);
}

/*
 * Kthread function to handle CF detection state changes.
 * It is responsible for adding and removing the ATA host.
 */
static int altcf_thread(void *context)
{
	struct altcf_private *altcf = context;

	for (;;) {
		dev_dbg(altcf->dev, "altcf_thread loop\n");
		set_current_state(TASK_INTERRUPTIBLE);
		while (!kthread_should_stop()
				&& (test_bit(EV_HOLD, &altcf->event)
					|| (!test_and_clear_bit(EV_CHANGE,
							&altcf->event)
						&& !test_bit(EV_DEAD,
							&altcf->event)))) {
			dev_dbg(altcf->dev, "altcf_thread sleeping\n");
			schedule();
			dev_dbg(altcf->dev, "altcf_thread woken\n");
			set_current_state(TASK_INTERRUPTIBLE);
		}
		__set_current_state(TASK_RUNNING);
		if (kthread_should_stop()) {
			dev_dbg(altcf->dev, "altcf_thread stopping\n");
			break;
		}
		/* State changed, so remove ATA host if previously set up. */
		altcf_remove_ata(altcf);
		if (!test_bit(EV_DEAD, &altcf->event)) {
			/* Check if CF is present and set it up. */
			altcf_detect_cf(altcf);
		}
	}
	/* Finally, remove ATA host if set up. */
	altcf_remove_ata(altcf);
	dev_dbg(altcf->dev, "altcf_thread exit\n");
	return 0;
}

/* This is the devres release function for our private data. */
static void altcf_private_release(struct device *dev, void *res)
{
	/* no op */
}

/* Add a device. */
static int __devinit altcf_probe(struct device *dev,
		struct resource *ide_mem_res, struct resource *ide_irq_res,
		struct resource *cfc_mem_res, struct resource *cfc_irq_res)
{
	struct altcf_private *altcf;
	int rc = -ENXIO;

	/* Check resources */
	if (resource_type(ide_mem_res) != IORESOURCE_MEM) {
		dev_err(dev, "IDE region wrong type\n");
		return rc;
	}
	if (resource_size(ide_mem_res) < ALTCF_IDE_LEN) {
		dev_err(dev, "IDE region too small");
		return rc;
	}
	if (resource_type(ide_irq_res) != IORESOURCE_IRQ) {
		dev_err(dev, "IDE IRQ wrong type\n");
		return rc;
	}
	if (resource_type(cfc_mem_res) != IORESOURCE_MEM) {
		dev_err(dev, "CF CONTROL region wrong type\n");
		return rc;
	}
	if (resource_size(cfc_mem_res) < ALTCF_CFC_LEN) {
		dev_err(dev, "CF CONTROL region too small");
		return rc;
	}

	/* Allocate private data and add it to the device. */
	rc = -ENOMEM;
	altcf = devres_alloc(altcf_private_release,
			sizeof(struct altcf_private), GFP_KERNEL);
	if (!altcf) {
		dev_err(dev, "failed to alloc private data\n");
		return rc;
	}
	altcf->dev = dev;	/* point back to device */
	devres_add(dev, altcf);
	/* Some more initialization. */
	altcf->ide_irq = (int)ide_irq_res->start;
	altcf->ide_irqflags = ide_irq_res->flags & IORESOURCE_BITS;
	altcf->cfc_irq = (int)cfc_irq_res->start;
	spin_lock_init(&altcf->cfctl_lock);

	/* Map registers. */
	altcf->ide_raw_base = ide_mem_res->start;
	altcf->ide_base = devm_ioremap_nocache(dev, ide_mem_res->start,
			ALTCF_IDE_LEN);
	if (!altcf->ide_base) {
		dev_err(dev, "failed to remap IDE registers\n");
		return rc;
	}
	altcf->cfc_base = devm_ioremap_nocache(dev, cfc_mem_res->start,
			ALTCF_CFC_LEN);
	if (!altcf->cfc_base) {
		dev_err(dev, "failed to remap CF CONTROL registers\n");
		return rc;
	}

	/* Initialize CF CONTROL registers. */
	altcf->cfctl = ALTCF_CFCTL_RST;
	WRITE_CFCTL(altcf, altcf->cfctl);
	WRITE_IDECTL(altcf, 0);
	READ_CFCTL(altcf);	/* Clear "CF detect change" interrupt. */

	/* Set event to hold off processing of CF state changes. */
	set_bit(EV_HOLD, &altcf->event);
	smp_mb();

	/* Create kthread to handle CF state changes. */
	altcf->cf_thread = kthread_create(altcf_thread, altcf,
			DRV_NAME "/%s", dev_name(dev));
	if (IS_ERR(altcf->cf_thread)) {
		rc = PTR_ERR(altcf->cf_thread);
		altcf->cf_thread = NULL;
		dev_err(dev, "failed to create thread (%d)\n", rc);
		return rc;
	}

	/* Request IRQ handler to handle "CF detect change" interrupt. */
	rc = request_irq(altcf->cfc_irq, altcf_cf_interrupt,
			(cfc_irq_res->flags & IORESOURCE_BITS),
			DRV_NAME, altcf);
	if (rc) {
		dev_err(dev, "failed to request CF CONTROL IRQ %d flags 0x%lx"
				" (%d)\n",
				altcf->cfc_irq,
				(cfc_irq_res->flags & IORESOURCE_BITS), rc);
		goto fail_request_irq_cf;
	}

	/* Enable the "CF detect change" interrupt. */
	update_cfctl(altcf, ALTCF_CFCTL_IDET, ALTCF_CFCTL_IDET);

	/* Generate initial CF state change event. */
	set_bit(EV_CHANGE, &altcf->event);
	smp_mb__before_clear_bit();
	clear_bit(EV_HOLD, &altcf->event);
	smp_mb__after_clear_bit();
	wake_up_process(altcf->cf_thread);

	return 0;

#ifdef unused
	/* Disable the "CF detect change" interrupt and free the IRQ. */
	update_cfctl(altcf, ALTCF_CFCTL_IDET, 0);
	free_irq(altcf->cfc_irq, altcf);
#endif
fail_request_irq_cf:

	/* Stop the thread. */
	kthread_stop(altcf->cf_thread);
	/* Reinitialize CF CONTROL registers. */
	altcf->cfctl = ALTCF_CFCTL_RST;
	WRITE_CFCTL(altcf, altcf->cfctl);
	WRITE_IDECTL(altcf, 0);
	/* Everything else is removed by devres. */
	return rc;
}

/* Remove a device. */
static int __devexit altcf_remove(struct device *dev)
{
	struct altcf_private *altcf;

	/* Get pointer to our private data resource. */
	altcf = devres_find(dev, altcf_private_release, NULL, NULL);
	if (altcf == NULL)
		return -ENXIO;
	/* Hold off CF detect thread processing and mark device as dead. */
	set_bit(EV_HOLD, &altcf->event);
	set_bit(EV_DEAD, &altcf->event);
	smp_mb();
	/* Disable the "CF detect change" interrupt and free the IRQ. */
	update_cfctl(altcf, ALTCF_CFCTL_IDET, 0);
	free_irq(altcf->cfc_irq, altcf);
	/* Stop the thread. */
	if (altcf->cf_thread) {
		kthread_stop(altcf->cf_thread);
		altcf->cf_thread = NULL;
	}
	/* Reinitialize CF CONTROL registers. */
	altcf->cfctl = ALTCF_CFCTL_RST;
	WRITE_CFCTL(altcf, altcf->cfctl);
	WRITE_IDECTL(altcf, 0);
	/* Everything else is removed by devres. */
	return 0;
}

/* Add a platform device. */
static int __devinit altcf_platform_probe(struct platform_device *pdev)
{
	struct resource *ide_mem_res;
	struct resource *ide_irq_res;
	struct resource *cfc_mem_res;
	struct resource *cfc_irq_res;

	if (pdev->num_resources != 4) {
		dev_err(&pdev->dev, "invalid number of resources\n");
		return -EINVAL;
	}

	/* Get the IDE MMIO resource. */
	ide_mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (unlikely(ide_mem_res == NULL)) {
		dev_err(&pdev->dev, "no IDE mem resource\n");
		return -EINVAL;
	}

	/* Get the IDE IRQ resource. */
	ide_irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if ((unlikely(ide_irq_res == NULL))) {
		dev_err(&pdev->dev, "no IDE IRQ resource\n");
		return -EINVAL;
	}

	/* Get the CF CONTROL MMIO resource. */
	cfc_mem_res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (unlikely(ide_mem_res == NULL)) {
		dev_err(&pdev->dev, "no CF CONTROL mem resource\n");
		return -EINVAL;
	}

	/* Get the CF CONTROL IRQ resource. */
	cfc_irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, 1);
	if ((unlikely(cfc_irq_res == NULL))) {
		dev_err(&pdev->dev, "no CF CONTROL IRQ resource\n");
		return -EINVAL;
	}

	return altcf_probe(&pdev->dev, ide_mem_res, ide_irq_res,
			cfc_mem_res, cfc_irq_res);
}

/* Remove a platform device. */
static int __devexit altcf_platform_remove(struct platform_device *pdev)
{
	return altcf_remove(&pdev->dev);
}

#ifdef CONFIG_OF
static const struct of_device_id altera_cf_match[] = {
	{ .compatible = "ALTR,cf-1.0", },
	{},
}
MODULE_DEVICE_TABLE(of, altera_cf_match);
#endif /* CONFIG_OF */

/* We are a platform device driver. */
static struct platform_driver altcf_platform_driver = {
	.probe		= altcf_platform_probe,
	.remove		= __devexit_p(altcf_platform_remove),
	.driver = {
		.name		= DRV_NAME,
		.owner		= THIS_MODULE,
		.of_match_table = of_match_ptr(altera_cf_match),
	},
};

module_platform_driver(altcf_platform_driver);

MODULE_AUTHOR("Ian Abbott");
MODULE_DESCRIPTION("low-level driver for Altera SOPC Builder CompactFlash ATA");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
MODULE_ALIAS("platform:" DRV_NAME);
