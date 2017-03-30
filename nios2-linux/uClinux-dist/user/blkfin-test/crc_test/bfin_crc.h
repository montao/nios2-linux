/*
 * bfin_crc.h - interface to Blackfin CRC controllers
 *
 * Copyright 2012 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BFIN_CRC_H__
#define __BFIN_CRC_H__

/* Function driver which use hardware crc must initialize the structure */
struct crc_info {
	/* Input data address */
	unsigned char *in_addr;
	/* Output data address */
	unsigned char *out_addr;
	/* Input or output bytes */
	unsigned long datasize;
	union {
	/* CRC to compare with that of input buffer */
	unsigned long crc_compare;
	/* Value to compare with input data */
	unsigned long val_verify;
	/* Value to fill */
	unsigned long val_fill;
	};
	/* Value to program the 32b CRC Polynomial */
	unsigned long crc_poly;
	union {
	/* CRC calculated from the input data */
	unsigned long crc_result;
	/* First failed position to verify input data */
	unsigned long pos_verify;
	};
	/* CRC mirror flags */
	unsigned int bitmirr:1;
	unsigned int bytmirr:1;
	unsigned int w16swp:1;
	unsigned int fdsel:1;
	unsigned int rsltmirr:1;
	unsigned int polymirr:1;
	unsigned int cmpmirr:1;
};

/* Userspace interface */
#define CRC_IOC_MAGIC		'C'
#define CRC_IOC_CALC_CRC	_IOWR('C', 0x01, unsigned int)
#define CRC_IOC_MEMCPY_CRC	_IOWR('C', 0x02, unsigned int)
#define CRC_IOC_VERIFY_VAL	_IOWR('C', 0x03, unsigned int)
#define CRC_IOC_FILL_VAL	_IOWR('C', 0x04, unsigned int)


#endif
