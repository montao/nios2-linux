#ifndef _NIOS_MMC_H
#define _NIOS_MMC_H

typedef volatile struct {
	void __iomem *base;
	struct mmc_host *mmc;
	spinlock_t lock;
	struct resource *res;
	int irq;
	unsigned char dat_width;	/* 1=4-bit mode, 0=1-bit */
	unsigned char prof_en;
	/* This should point to current cmd we are processing */
	struct mmc_command *cmd;
	unsigned int clock_freq;
} NIOS_MMC_HOST;

#define NIOS_MMC_REG_CTLSTAT 0*4
#define NIOS_MMC_REG_CLK_CTL 1*4
#define NIOS_MMC_REG_CMD_ARG0 2*4
#define NIOS_MMC_REG_CMD_ARG1 3*4
#define NIOS_MMC_REG_CMD_ARG2 4*4
#define NIOS_MMC_REG_CMD_ARG3 5*4
#define NIOS_MMC_REG_DMA_BASE 6*4
#define NIOS_MMC_REG_XFER_CTL 7*4
#define NIOS_MMC_REG_MISC 8*4
#define NIOS_MMC_REG_PROF_CNT0 9*4
#define NIOS_MMC_REG_PROF_CNT1 10*4
#define NIOS_MMC_REG_VERSION_INFO 15*4

#define NIOS_MMC_CTLSTAT_BUSY (1<<0)
#define NIOS_MMC_CTLSTAT_CD (1<<1)
#define NIOS_MMC_CTLSTAT_WP (1<<2)
#define NIOS_MMC_CTLSTAT_CD_IF (1<<3)
#define NIOS_MMC_CTLSTAT_DEV_IF (1<<4)
#define NIOS_MMC_CTLSTAT_XFER_IF (1<<5)
#define NIOS_MMC_CTLSTAT_FRMERR_IF (1<<6)
#define NIOS_MMC_CTLSTAT_CRCERR_IF (1<<7)
#define NIOS_MMC_CTLSTAT_TIMEOUTERR_IF (1<<8)
#define NIOS_MMC_CTLSTAT_FIFO_OVERRUN_IF (1<<9)
#define NIOS_MMC_CTLSTAT_FIFO_UNDERRUN_IF (1<<10)
#define NIOS_MMC_CTLSTAT_PROF_EN (1<<13)
#define NIOS_MMC_CTLSTAT_HOST_4BIT (1<<14)
#define NIOS_MMC_CTLSTAT_CD_IE (1<<16)
#define NIOS_MMC_CTLSTAT_XFER_IE (1<<18)
#define NIOS_MMC_CTLSTAT_BLK_PREFETCH (1<<20)
#define NIOS_MMC_CTLSTAT_SOFT_RST (1<<31)

#define NIOS_MMC_CLK_CTL_CLK_EN (1<<31)
#define NIOS_MMC_CLK_CTL_DLY_SHIFT 29
#define NIOS_MMC_CLK_CTL_INV (1<<28)

#define NIOS_MMC_XFER_CTL_XFER_START (1<<0)
#define NIOS_MMC_XFER_CTL_CMD_IDX_SHIFT (1)
#define NIOS_MMC_XFER_CTL_DAT_WIDTH (1<<7)
#define NIOS_MMC_XFER_CTL_RESP_CODE_SHIFT (8)
#define NIOS_MMC_XFER_CTL_DAT_RWn (1<<10)
#define NIOS_MMC_XFER_CTL_RESP_NOCRC (1<<11)
#define NIOS_MMC_XFER_CTL_BYTE_COUNT_SHIFT (12)
#define NIOS_MMC_XFER_CTL_BLOCK_COUNT_SHIFT (22)

#define NIOS_MMC_MISC_PROF_RESET (1<<0)
#define NIOS_MMC_MISC_PROF_CNT_SEL_SHIFT (1)

#endif /* _NIOS_MMC_H */
