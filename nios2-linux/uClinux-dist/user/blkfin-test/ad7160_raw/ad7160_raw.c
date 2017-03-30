/*
 * AD7160 raw interface test
 *
 * Copyright 2010 Analog Devices Inc.
 * Michael Hennerich (hennerich@blackfin.uclinux.org)
 *
 * Licensed under the GPL-2 or later
 */

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <strings.h>
#include <linux/types.h>

#if 1
struct ad7160_iocreg_access {
	__u32 reg;
	__u32 data;
} __attribute__ ((packed));

#define AD7160_RAW_IOCSREG	_IOW('o', 1, struct ad7160_iocreg_access)
#define AD7160_RAW_IOCGREG	_IOR('o', 2, struct ad7160_iocreg_access)
#define AD7160_RAW_IOCGJUNKSIZE	_IOR('o', 3, unsigned int)
#else
#include <linux/input/ad7160.h>
#endif

void usage(FILE *fp, int rc)
{
	fprintf(fp,
		"Usage: ad7160_raw [-h?vt]\n"
		"        -h?            this help\n"
		"        -v             print version info\n"
		"        -p             print data\n"
		"        -u             write data to USB ttyGSO\n"
	);
	exit(rc);
}

#define AD7160_RAW_DEVICE		"/dev/ad7160_raw"
#define USB_GADGET_SERAIL_DEVICE	"/dev/ttyGS0"
#define VERSION 			"0.0.0.0"

#define AD7160_REG_DEVICE_ID		0x40051700
#define AD7160_REG_FINGER_ACT_CTRL	0x40051728
#define FINGER_ACT_CTRL_VAL		0x61

unsigned ad7160_reg_read(int fd, unsigned reg)
{
	struct ad7160_iocreg_access ioc;
	int ret;

	ioc.reg = reg;
	ioc.data = 0;

	ret = ioctl(fd, AD7160_RAW_IOCGREG, &ioc);
	if (ret)
		perror("ioctl");

	return ioc.data;
}

unsigned ad7160_reg_write(int fd, unsigned reg, unsigned val)
{
	struct ad7160_iocreg_access ioc;
	int ret;

	ioc.reg = reg;
	ioc.data = val;

	ret = ioctl(fd, AD7160_RAW_IOCSREG, &ioc);
	if (ret)
		perror("ioctl");

	return ret;
}

int main(int argc, char *argv[])
{
	int fd, fd_gs, i, ret;
	unsigned *buffer;
	unsigned val, junksize, print = 0, usb = 0;

	while ((i = getopt(argc, argv, "vth?pu")) > 0) {
		switch (i) {
		case 'v':
			printf("%s: version %s\n", argv[0], VERSION);
			exit(0);
		case 'h':
		case '?':
			usage(stdout, 0);
			break;
		case 'p':
			print++;
			break;
		case 'u':
			usb++;
			break;
		default:
			fprintf(stderr, "ERROR: unkown option '%c'\n", i);
			usage(stderr, 1);
			break;
		}
	}

	/* Open /dev/ad7160_raw */
	fd = open(AD7160_RAW_DEVICE, O_RDONLY, 0);
	if (fd == -1) {
		perror("open");
		exit(1);
	}

	if (usb) {
		/* Open /dev/ttyGS0 */
		fd_gs = open(USB_GADGET_SERAIL_DEVICE, O_RDWR | O_NONBLOCK, 0);
		if (fd_gs == -1) {
			perror("ttyGS0 open");
			exit(1);
		}
	}

	/* -------- READ Test -------- */

	ret = ad7160_reg_read(fd, AD7160_REG_DEVICE_ID);
	printf("AD7160_REG_DEVICE_ID = %x\n", ret);

	/* -------- WRITE Test -------- */

	ret = ad7160_reg_write(fd, AD7160_REG_FINGER_ACT_CTRL,
				FINGER_ACT_CTRL_VAL);
	if (ret)
		printf("ad7160_reg_write returned %d\n", ret);

	/* -------- READ Back Test -------- */

	val = ad7160_reg_read(fd, AD7160_REG_FINGER_ACT_CTRL);

	if (val != FINGER_ACT_CTRL_VAL)
		printf("ioc.data = %x (should be %x)\n", val,
			FINGER_ACT_CTRL_VAL);
	else
		printf("Write Modify Read: PASSED\n");

	/* -------- Char Test -------- */

	ret = ioctl(fd, AD7160_RAW_IOCGJUNKSIZE, &junksize);
	if (ret)
		perror("ioctl");

	buffer = malloc(junksize);
	if (buffer == NULL) {
		perror("malloc");
		exit(1);
	}

	while (1) {
		ret = read(fd, buffer, junksize);

		if (ret != junksize)
			printf("read() returned %d\n", ret);

		if (usb) {
			/* Write Binary */
			ret = write(fd_gs, buffer, junksize);

			if (ret != junksize)
				printf("usb write() returned %d\n", ret);
		}
		if (print)
			for (i = 0; i < (junksize / sizeof(*buffer)); i++) {
				printf("[%d] = %d\n", i, buffer[i]);
			}
	}
	close(fd);
	if (usb)
		close(fd_gs);
	free(buffer);
	exit(0);
}
