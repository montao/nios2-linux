/*
 *  linux/drivers/mmc/nios_mmc.c - FPS-Tech NIOS_MMC Driver
 *
 *  Copyright (C) 2010 Jai Dhar / FPS-Tech, All Rights Reserved.
 *  Web: http://www.fps-tech.net
 *  Credits: This driver is partially derived from pxamci.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/mmc/host.h>
#include <linux/mmc/core.h>
#include <linux/pagemap.h>
#include <linux/of.h>

#include <asm/dma.h>
#include <asm/io.h>
#include <asm/scatterlist.h>
#include <linux/scatterlist.h>

#include "nios_mmc.h"

#define DRIVER_NAME	"nios_mmc"

#define SD_MAX_FREQ 25000000	/* Set the max frequency to 25MHz (std. speed) */
#define SD_MIN_FREQ 375000	/* Set the minimum frequency to 375Khz */

/********** Function prototypes ************/
static void nios_mmc_start_cmd(NIOS_MMC_HOST * host, struct mmc_command *cmd);
static void nios_mmc_end_request(struct mmc_host *mmc, struct mmc_request *mrq);
static int nios_mmc_procinit(NIOS_MMC_HOST * host);
static void nios_mmc_procclose(void);

unsigned int max_blk_count = 128;
module_param(max_blk_count, uint, S_IRUGO);

unsigned int max_req_size = 512 * 128;
module_param(max_req_size, uint, S_IRUGO);

unsigned int max_seg_size = 512 * 128;
module_param(max_seg_size, uint, S_IRUGO);

unsigned int dat_width = 1;
module_param(dat_width, uint, S_IRUGO);

unsigned int blk_prefetch = 1;
module_param(blk_prefetch, uint, S_IRUGO);

unsigned int fmax = SD_MAX_FREQ;
module_param(fmax, uint, S_IRUGO);

static unsigned int irq_count;
/**************************** Low-level register access ******************************/
static inline void nios_mmc_writel(unsigned int val, NIOS_MMC_HOST *host, unsigned char off)
{
	writel(val, host->base + off);
}
static inline unsigned int nios_mmc_readl(NIOS_MMC_HOST *host, unsigned char off)
{
	return readl(host->base + off);
}
/***************************** Start of main functions ********************************/

static void nios_mmc_end_cmd(NIOS_MMC_HOST * host, unsigned int stat)
{
	unsigned int ret;
	struct mmc_command *cmd = host->cmd;
	struct mmc_data *data = cmd->data;
	struct mmc_command *stop = data->stop;

	BUG_ON(cmd == NULL);

	/* assign stop only if data is assigned */
	if (data)
		stop = data->stop;
	else
		stop = NULL;

	/* Interrupt flags will be cleared in ISR routine, so we don't have to touch them */
	if (stat & NIOS_MMC_CTLSTAT_TIMEOUTERR_IF) {
		dev_dbg(mmc_dev(host->mmc), "Timeout error\n");
		ret = -ETIMEDOUT;
	} else if (stat & NIOS_MMC_CTLSTAT_FRMERR_IF) {
		dev_dbg(mmc_dev(host->mmc), "Framing error\n");
		ret = -EILSEQ;
	} else if (stat & NIOS_MMC_CTLSTAT_CRCERR_IF) {
		dev_dbg(mmc_dev(host->mmc), "CRC Error\n");
		ret = -EILSEQ;
	} else if (stat & NIOS_MMC_CTLSTAT_FIFO_UNDERRUN_IF) {
		dev_dbg(mmc_dev(host->mmc), "FIFO Underrun error\n");
		ret = -EINVAL;
	} else if (stat & NIOS_MMC_CTLSTAT_FIFO_OVERRUN_IF) {
		dev_dbg(mmc_dev(host->mmc), "FIFO Overrun error\n");
		ret = -EINVAL;
	} else {
		/* Response is good! */
		ret = 0;
	}
	if (ret) {
		dev_dbg(mmc_dev(host->mmc), "Error executing CMD%d\n", cmd->opcode);
		dev_dbg(mmc_dev(host->mmc), "Response argument: 0x%X\n",
			  nios_mmc_readl(host, NIOS_MMC_REG_CMD_ARG0));
	}
	/* Load response into command structure */
	cmd->error = ret;
	if (mmc_resp_type(cmd) == MMC_RSP_R2) {
		cmd->resp[0] = nios_mmc_readl(host, NIOS_MMC_REG_CMD_ARG3);
		cmd->resp[1] = nios_mmc_readl(host, NIOS_MMC_REG_CMD_ARG2);
		cmd->resp[2] = nios_mmc_readl(host, NIOS_MMC_REG_CMD_ARG1);
		cmd->resp[3] = nios_mmc_readl(host, NIOS_MMC_REG_CMD_ARG0);
	} else
		cmd->resp[0] = nios_mmc_readl(host, NIOS_MMC_REG_CMD_ARG0);
	/* Check if this was a data transaction */
	if (data) {
		if (cmd->error == 0)
			data->bytes_xfered = data->blksz * data->blocks;
		else
			data->bytes_xfered = 0;
	}
	if (stop) {
		/* Schedule the stop command */
		/* We will need to reassign the pointer in the structure since we are 
		 * switching commands now */
		nios_mmc_start_cmd(host, stop);
	} else {
		/* No other commands needed, finish off transaction */
		nios_mmc_end_request(host->mmc, cmd->mrq);
	}
}

/* nios_mmc_execute_cmd(): Key function that interacts with MMC Host to carry out command */
static void nios_mmc_execute_cmd(NIOS_MMC_HOST * host, unsigned char cmd,
				 unsigned int arg_in, unsigned char resp_type,
				 unsigned char nocrc, unsigned int bytes,
				 unsigned int blocks, unsigned char rwn,
				 unsigned int buf)
{
	unsigned int xfer_ctl = 0;
	unsigned char cmdidx;

	/* Do a sanity check that the core isn't busy... why should it be since we haven't started a cmd?? */
	if (nios_mmc_readl(host, NIOS_MMC_REG_CTLSTAT) & NIOS_MMC_CTLSTAT_BUSY) {
		BUG();
	}

	xfer_ctl = (cmd & 0x3F) << NIOS_MMC_XFER_CTL_CMD_IDX_SHIFT;
	nios_mmc_writel(arg_in, host, NIOS_MMC_REG_CMD_ARG0);
	xfer_ctl |= (resp_type & 0x3) << NIOS_MMC_XFER_CTL_RESP_CODE_SHIFT;
	if (nocrc)
		xfer_ctl |= NIOS_MMC_XFER_CTL_RESP_NOCRC;
	if (rwn)
		xfer_ctl |= NIOS_MMC_XFER_CTL_DAT_RWn;
	xfer_ctl |= (bytes & 0x3FF) << NIOS_MMC_XFER_CTL_BYTE_COUNT_SHIFT;
	xfer_ctl |= (blocks & 0x3FF) << NIOS_MMC_XFER_CTL_BLOCK_COUNT_SHIFT;
	xfer_ctl |= NIOS_MMC_XFER_CTL_XFER_START;
	if (host->dat_width)
		xfer_ctl |= NIOS_MMC_XFER_CTL_DAT_WIDTH;
	cmdidx = (xfer_ctl >> NIOS_MMC_XFER_CTL_CMD_IDX_SHIFT) & 0x3F;
	if (bytes) {
		/* Setup DMA base */
		flush_dcache_range(buf, buf + bytes * blocks);
		dev_dbg(mmc_dev(host->mmc), "Flushed d-cache range 0x%X + %d bytes\n",
				buf, bytes * blocks);
		nios_mmc_writel(buf, host, NIOS_MMC_REG_DMA_BASE);
		dev_dbg(mmc_dev(host->mmc),
			  "XFER_CTL: 0x%X (CMD%d), DMA_BASE(%c): 0x%X, ARG_IN: 0x%X, %d/%db\n",
			  xfer_ctl, cmdidx,
			  (xfer_ctl & NIOS_MMC_XFER_CTL_DAT_RWn) ? 'R' : 'W',
			  buf, arg_in, blocks, bytes);
	} else {
		dev_dbg(mmc_dev(host->mmc), "XFER_CTL: 0x%X (CMD%d), ARG_IN: 0x%X\n",
			  xfer_ctl, cmdidx, arg_in);
	}
	/* Execute command */
	nios_mmc_writel(xfer_ctl, host, NIOS_MMC_REG_XFER_CTL);
}

static irqreturn_t nios_mmc_irq(int irq, void *dev_id)
{
	NIOS_MMC_HOST *host = dev_id;
	unsigned int stat;

	stat = nios_mmc_readl(host, NIOS_MMC_REG_CTLSTAT);
	/* Clear the interrupt */
	nios_mmc_writel(stat, host, NIOS_MMC_REG_CTLSTAT);
	dev_dbg(mmc_dev(host->mmc), "IRQ, ctlstat: 0x%X\n", stat);

	if (stat & NIOS_MMC_CTLSTAT_CD_IF) {
		/* Card-detect interrupt */
		dev_dbg(mmc_dev(host->mmc), "HOT-PLUG: Card %s\n",
				stat & NIOS_MMC_CTLSTAT_CD ? "inserted" : "removed");
		mmc_detect_change(host->mmc, 100);
	}
	if (stat & NIOS_MMC_CTLSTAT_XFER_IF) {
		dev_dbg(mmc_dev(host->mmc), "Detected XFER Interrupt\n");
		/* Transfer has completed */
		nios_mmc_end_cmd(host, stat);
	}
	irq_count++;
	return IRQ_HANDLED;
}

/* Function to start the CMD process */
static void nios_mmc_start_cmd(NIOS_MMC_HOST *host, struct mmc_command *cmd)
{
	unsigned char resp_type = 0, nocrc = 0, rwn = 0;
	struct mmc_data *data = cmd->data;
	struct scatterlist *sg;
	unsigned int current_address = 0, bytes = 0, blocks = 0;

	host->cmd = cmd;

	dev_dbg(mmc_dev(host->mmc), "Opcode: %d Arg: 0x%X ", cmd->opcode, cmd->arg);
	switch (mmc_resp_type(cmd)) {
	case MMC_RSP_R3:
		nocrc = 1;
	case MMC_RSP_R1:
	case MMC_RSP_R1B:
		resp_type = 1;
		break;
	case MMC_RSP_R2:
		resp_type = 2;
		nocrc = 1;
		break;
	case MMC_RSP_NONE:
		resp_type = 0;
		break;
	default:
		BUG();
		break;
	}

	cmd->error = 0;
	if (data) {
		sg = data->sg;
		current_address = (unsigned int)sg_phys(sg);
		dev_dbg(mmc_dev(host->mmc), "Completed sg_phys() mapping to 0x%X\n", current_address);
		BUG_ON(data->sg_len > 1);

		dev_dbg(mmc_dev(host->mmc), "Block size: %d Blocks: %d sg_len: %d\n",
			  data->blksz, data->blocks, data->sg_len);
		if (data->stop) {
			dev_dbg(mmc_dev(host->mmc), "Stop command present\n");
		}
		/* Setup byte count */
		bytes = data->blksz;
		blocks = data->blocks - 1;

		if (data->flags & MMC_DATA_READ)
			rwn = 1;
		else
			rwn = 0;

	}
	nios_mmc_execute_cmd(host, cmd->opcode, cmd->arg,
			     resp_type, nocrc, bytes, blocks, rwn,
			     current_address);

}

/* Profiling functions */
void nios_mmc_clear_prof(NIOS_MMC_HOST * host)
{
	unsigned int misc_reg;
	misc_reg = nios_mmc_readl(host, NIOS_MMC_REG_MISC);
	misc_reg |= NIOS_MMC_MISC_PROF_RESET;
	nios_mmc_writel(misc_reg, host, NIOS_MMC_REG_MISC);
}
unsigned long long nios_mmc_prof_cnt(NIOS_MMC_HOST * host, unsigned char cnt)
{
	unsigned long long tmp = 0;
	unsigned int misc_reg;
	/* Select the counter first */
	misc_reg = nios_mmc_readl(host, NIOS_MMC_REG_MISC);
	misc_reg &= ~(0x7 << NIOS_MMC_MISC_PROF_CNT_SEL_SHIFT);
	misc_reg |= (cnt << NIOS_MMC_MISC_PROF_CNT_SEL_SHIFT);
	nios_mmc_writel(misc_reg, host, NIOS_MMC_REG_MISC);
	tmp = nios_mmc_readl(host, NIOS_MMC_REG_PROF_CNT1);
	tmp <<= 32;
	tmp |= nios_mmc_readl(host, NIOS_MMC_REG_PROF_CNT0);
	return tmp;
}

/****************** Driver-level interface *****************/

/* This function is called from the driver level above */
/* nios_mmc_request() initiates the MMC request as setup in the mrq structure */
static void nios_mmc_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	NIOS_MMC_HOST *host = mmc_priv(mmc);

	if (host->cmd != NULL) {
		dev_dbg(mmc_dev(mmc), "HOST_CMD Not null!\n");
	}

	dev_dbg(mmc_dev(mmc), "Start req\n");
	nios_mmc_start_cmd(host, mrq->cmd);
}

/* Function to cleanup previous call */
static void nios_mmc_end_request(struct mmc_host *mmc, struct mmc_request *mrq)
{
	NIOS_MMC_HOST *host = mmc_priv(mmc);
	host->cmd = NULL;
	mmc_request_done(host->mmc, mrq);
}
static int nios_mmc_get_ro(struct mmc_host *mmc)
{
	int ctlstat;
	NIOS_MMC_HOST *host = mmc_priv(mmc);
	dev_dbg(mmc_dev(mmc), "Get RO\n");
	ctlstat = nios_mmc_readl(host, NIOS_MMC_REG_CTLSTAT);
	if (ctlstat & NIOS_MMC_CTLSTAT_WP)
		return 1;
	return 0;
}
static void nios_mmc_set_ios(struct mmc_host *mmc, struct mmc_ios *ios)
{
	NIOS_MMC_HOST *host = mmc_priv(mmc);
	int div;

	if (ios->clock) {
		/* FIXME: Look at divider calculation! */
		dev_dbg(mmc_dev(mmc), "Requesting clock: %d\n", ios->clock);
		div = (host->clock_freq / (2 * ios->clock)) - 1;
		/* Check if div is less than 1 */
		if (div < 1)
			div = 1;
		nios_mmc_writel((div & 0xFFFF) | NIOS_MMC_CLK_CTL_CLK_EN,
		       host, NIOS_MMC_REG_CLK_CTL);
	} else {
		/* Stop the clock */
		dev_dbg(mmc_dev(mmc), "Request stop clock\n");
		nios_mmc_writel(0, host, NIOS_MMC_REG_CLK_CTL);
	}

	if (ios->bus_width)
		host->dat_width = 1;
	else
		host->dat_width = 0;
}

static struct mmc_host_ops nios_mmc_ops = {
	.request = nios_mmc_request,
	.get_ro = nios_mmc_get_ro,
	.set_ios = nios_mmc_set_ios,
};

static int nios_mmc_probe(struct platform_device *pdev)
{
	struct mmc_host *mmc;
	struct resource *r;
	NIOS_MMC_HOST *host = NULL;
	int ret, irq;
	u32 clk;

	pr_debug("Starting NIOS_MMC Probe\n");
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	irq = platform_get_irq(pdev, 0);
	if (!r || irq < 0)
		return -ENXIO;
	r = request_mem_region(r->start, 16 * 4, DRIVER_NAME);
	if (!r) {
		pr_debug("Error allocating mem. region\n");
		return -EBUSY;
	}
	mmc = mmc_alloc_host(sizeof(NIOS_MMC_HOST), &pdev->dev);
	if (!mmc) {
		pr_debug("Error allocating MMC Host\n");
		ret = -ENOMEM;
		goto out;
	}
	mmc->ops = &nios_mmc_ops;
	dev_dbg(mmc_dev(mmc), "Done initial probe\n");
	/* SG DMA Caps */
	/* Setup block-related parameters on host */
	mmc->max_segs = 1;
	mmc->max_blk_size = 512;
	mmc->max_blk_count = max_blk_count;
	if (max_seg_size)
		mmc->max_seg_size = max_seg_size;
	if (max_req_size)
		mmc->max_req_size = max_req_size;

	host = mmc_priv(mmc);
	host->mmc = mmc;
	host->dat_width = 0;
	host->cmd = NULL;
	mmc->ocr_avail = MMC_VDD_32_33 | MMC_VDD_33_34;

	spin_lock_init(&host->lock);
	host->res = r;
	host->irq = irq;
	host->base = ioremap(r->start, 16 * 4);
	if (!host->base) {
		ret = -ENOMEM;
		dev_dbg(mmc_dev(mmc), "Error in IO Remap\n");
		goto out;
	}
	dev_dbg(mmc_dev(mmc), "Setup host with Base: 0x%X IRQ: %d\n",
		  (unsigned int)host->base, host->irq);

	/* Check that SD/MMC Core is present */
	ret = 0;
	ret = nios_mmc_readl(host, NIOS_MMC_REG_VERSION_INFO);
	if ((ret & 0xFFFF) != 0xBEEF) {
		dev_dbg(mmc_dev(mmc), "Core not present\n");
		ret = -ENXIO;
		goto out;
	}

	/* Setup clock frequency support */
	if (of_property_read_u32(pdev->dev.of_node, "clock-frequency", &clk)) {
		ret = -ENODEV;
		goto out;
	}

	host->clock_freq = clk;

	mmc->f_max = host->clock_freq / 4;
	/* Assign FMAX to be minimum of cpu_clk/4 and 'fmax' variable */
	if (mmc->f_max > fmax) {
		mmc->f_max = fmax;
	}
	mmc->f_min = SD_MIN_FREQ;
	dev_info(mmc_dev(mmc), "FPS-Tech SD/SDIO/MMC Host, IP version %d.%d\n",
		 ret >> 24, (ret >> 16) & 0xff);
	dev_info(mmc_dev(mmc), "F_MAX: %d KHz, F_MIN: %d KHz\n",
		 mmc->f_max / 1000, mmc->f_min / 1000);
	ret = nios_mmc_readl(host, NIOS_MMC_REG_CTLSTAT);
	dev_info(mmc_dev(mmc), "Host built with %s DAT driver\n",
	       (ret & NIOS_MMC_CTLSTAT_HOST_4BIT) ? "4-bit" : "1-bit");
	if (ret & NIOS_MMC_CTLSTAT_PROF_EN) {
		dev_dbg(mmc_dev(mmc),
			"NIOS_MMC: Host built with profiling capabilities\n");
		host->prof_en = 1;
	} else
		host->prof_en = 0;
	if (!dat_width) {
		/* Force dat_width to 1-bit */
		mmc->caps = 0;
		dev_info(mmc_dev(mmc), "NIOS_MMC: Forcing 1-bit DAT width\n");
	} else {
		/* Set dat_width based on host capabilities */
		mmc->caps =
		    (ret & NIOS_MMC_CTLSTAT_HOST_4BIT) ? MMC_CAP_4_BIT_DATA : 0;
	}
	/* Execute soft-reset on core */
	nios_mmc_writel(NIOS_MMC_CTLSTAT_SOFT_RST, host, NIOS_MMC_REG_CTLSTAT);

	/* Enable interrupts on CD and XFER_IF only */
	/* Use BLK_PREFETCH for linux unless disabled */
	/* This section sets up CTLSTAT for the rest of the driver.
	 * Make sure all further writes to CTLSTAT are using bitwise OR!!! */
	ret = NIOS_MMC_CTLSTAT_CD_IE | NIOS_MMC_CTLSTAT_XFER_IE;
	if (blk_prefetch)
		ret |= NIOS_MMC_CTLSTAT_BLK_PREFETCH;
	/* Execute write to CTLSTAT here */
	nios_mmc_writel(ret, host, NIOS_MMC_REG_CTLSTAT);
	if (ret & NIOS_MMC_CTLSTAT_BLK_PREFETCH) {
		dev_dbg(mmc_dev(mmc), "NIOS_MMC: Using block-prefetching\n");
	} else {
		dev_dbg(mmc_dev(mmc), "NIOS_MMC: Block-prefetching disabled!\n");
	}

	ret =
	    request_irq(host->irq, nios_mmc_irq, IRQF_SHARED, DRIVER_NAME, (void *)host);
	if (ret) {
		dev_dbg(mmc_dev(mmc), "Error allocating interrupt\n");
		goto out;
	}
	platform_set_drvdata(pdev, mmc);
	mmc_add_host(mmc);
	dev_dbg(mmc_dev(mmc),
		  "NIOS_MMC: max_blk_cnt: %d max_seg_size: %d max_req_size: %d\n",
		  mmc->max_blk_count, mmc->max_seg_size, mmc->max_req_size);
#ifdef CONFIG_PROC_FS
	/* Setup Proc file system */
	nios_mmc_procinit(host);
#endif

	dev_dbg(mmc_dev(mmc), "Completed full probe successfully\n");
	return 0;

out:
	if (host) {
	}
	if (mmc)
		mmc_free_host(mmc);
	release_resource(r);
	return ret;
}

static int nios_mmc_remove(struct platform_device *pdev)
{
	struct mmc_host *mmc = platform_get_drvdata(pdev);

	if (mmc) {
		NIOS_MMC_HOST *host = mmc_priv(mmc);
		mmc_remove_host(mmc);
		free_irq(host->irq, (void *)host);
		iounmap(host->base);
		release_resource(host->res);
		mmc_free_host(mmc);
#ifdef CONFIG_PROC_FS
		nios_mmc_procclose();
#endif
	}
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id nios_mmc_dt_match[] = {
	{ .compatible = "fps,mmc-1.1", },
	{}
};
MODULE_DEVICE_TABLE(of, nios_mmc_dt_match);
#endif

static struct platform_driver nios_mmc_driver = {
	.probe	= nios_mmc_probe,
	.remove	= nios_mmc_remove,
	.driver	= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(nios_mmc_dt_match),
	},
};
module_platform_driver(nios_mmc_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("NIOS MMC Host Driver");
MODULE_ALIAS("platform:" DRIVER_NAME);

/********** PROC FS Stuff **************/
#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#define procfs_name "mmc_stats"

static struct proc_dir_entry *mmc_proc_file;
static struct proc_dir_entry *mmc_proc_root = NULL;

int
procfile_read(char *buffer, char **buffer_location, off_t offset,
	      int buffer_length, int *eof, void *data)
{
	int ret;
	unsigned long long xfer_len, busy_wait_len, bus_wait_len;

	NIOS_MMC_HOST *host = (NIOS_MMC_HOST *) data;
	if (offset > 0)
		ret = 0;
	else if (!host->prof_en) {
		ret = sprintf(buffer, "Host does not have profiling enabled\n");
	} else {
		xfer_len = nios_mmc_prof_cnt(host, 0);
		busy_wait_len = nios_mmc_prof_cnt(host, 1);
		bus_wait_len = nios_mmc_prof_cnt(host, 2);
		ret =
		    sprintf(buffer,
			    "Profiling Counters:\nInterrupts: %d\nXFER_LEN: %llds\nBUSY_WAIT_LEN: %llds\nBUS_WAIT_LEN: %llds\n",
			    irq_count,
			    xfer_len / host->clock_freq,
			    busy_wait_len / host->clock_freq,
			    bus_wait_len / host->clock_freq);
		nios_mmc_clear_prof(host);
		irq_count = 0;
	}
	return ret;
}

static int nios_mmc_procinit(NIOS_MMC_HOST * host)
{
	mmc_proc_root = proc_mkdir("nios_mmc", NULL);
	mmc_proc_file = create_proc_entry(procfs_name, 0644, mmc_proc_root);
	if (!mmc_proc_file || !mmc_proc_root) {
		remove_proc_entry(procfs_name, mmc_proc_root);
		dev_err(mmc_dev(host->mmc), "NIOS_MMC: Could not init. /proc/%s\n", procfs_name);
		return -ENOMEM;
	}
	dev_dbg(mmc_dev(host->mmc), "/proc/%s added\n", procfs_name);
	mmc_proc_file->read_proc = procfile_read;
	mmc_proc_file->mode = S_IFREG | S_IRUGO;
	mmc_proc_file->uid = 0;
	mmc_proc_file->gid = 0;
	mmc_proc_file->data = (void *)host;
	irq_count = 0;

	return 0;
}

static void nios_mmc_procclose()
{
	remove_proc_entry(procfs_name, mmc_proc_root);
	remove_proc_entry("nios_mmc", NULL);
}
#endif
