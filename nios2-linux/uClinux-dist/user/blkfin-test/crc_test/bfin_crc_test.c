/*
 * bfin_crc_test.c - Blackfin CRC test app
 *
 * Copyright 2012 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <errno.h>
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "bfin_crc.h"

static int sport_fd, data_fd;

#define DEFAULT_CRC	"/dev/bfin_crc0"

static void usage(int status)
{
	fprintf(status ? stderr : stdout,
		"Usage: bfin_crc_test [options] <crc device path>\n"
		"\n"
		"Options:\n"
		"  -m <mode>   Test crc device in <mode>.\n"
		"	0 - test all\n"
		"	1 - calculate crc\n"
		"	2 - memory copy with crc\n"
		"	3 - verify value\n"
		"	4 - fill value\n"
		"  -s <words>  buffer size to run the test\n"
		"  -c <crc>    expected CRC\n"
		"  -v <val>    fill val\n"
		"  -b <val>    set buffer to val before test\n"
		"  -h          This help.\n"
	);
	exit(status);
}

int main(int argc, char *argv[])
{
	char *crc_device;
	char c, c_val;
	unsigned long *data;
	struct crc_info info;
	int crc_fd;
	int ret = 0;
	unsigned long expected_crc = 0x12345678;
	unsigned long fill_val = 0x12345678;
	unsigned long buf_len = 1024;
	int test_mode = 0;

	crc_device = DEFAULT_CRC;
	c_val = 'C';

	while ((c = getopt(argc, argv, "m:s:c:v:b:")) != EOF)
		switch (c) {
		case 'm':
			test_mode = atoi(optarg);
			break;
		case 's':
			buf_len = strtoul(optarg, NULL, 10) << 2;
			break;
		case 'c':
			expected_crc = strtoul(optarg, NULL, 16);
			break;
		case 'v':
			fill_val = strtoul(optarg, NULL, 16);
			break;
		case 'b':
			c_val = *optarg;
			break;
		case 'h':
			usage(0);
		default:
			usage(1);
		}

	if (optind + 1 != argc)
		usage(1);
	crc_device = argv[optind];

	crc_fd = open(crc_device, O_RDWR, 0);
	if (crc_fd < 0)
		printf("Failed to open %s\n", crc_device);

	memset(&info, 0, sizeof(info));
	if ((info.in_addr = malloc(buf_len*2)) == NULL)
		printf("malloc() failed\n");

	info.out_addr = info.in_addr + buf_len;
	info.datasize = buf_len;
	info.crc_poly = 0x10101010;

	if (test_mode == 0 || test_mode == 1) {
		printf("Start calculate CRC: set buffer by '%c'\n", c_val);
		memset(info.in_addr, c_val, buf_len);
		ret = ioctl(crc_fd, CRC_IOC_CALC_CRC, &info);
		if (ret < 0) {
			printf("crc: ioctl fail with %d\n", ret);
			goto out;
		}
		printf("End Calculate CRC: pass, crc=0x%x\n", info.crc_result);

		printf("\n");

		printf("Start compare CRC(0x%x):\n", expected_crc);
		info.crc_compare = expected_crc;
		info.crc_result = 0;
		ret = ioctl(crc_fd, CRC_IOC_CALC_CRC, &info);
		if (ret < 0) {
			printf("crc: ioctl fail with %d\n", ret);
			goto out;
		}
		if (info.crc_compare == info.crc_result)
			printf("End compare CRC(0x%x): pass\n", info.crc_result);
		else
			printf("End compare CRC(0x%x): fail\n", info.crc_result);

		printf("\n");
	}

	if (test_mode == 0 || test_mode == 2) {
		printf("Start memory copy with CRC: set buffer by '%c'\n", c_val);
		memset(info.in_addr, c_val, buf_len);
		memset(info.out_addr, 'R', buf_len);
		ret = ioctl(crc_fd, CRC_IOC_MEMCPY_CRC, &info);
		if (ret < 0) {
			printf("crc: ioctl fail with %d\n", ret);
			goto out;
		}
		if ((ret = memcmp(info.in_addr, info.out_addr, buf_len)) == 0)
			printf("End memory copy with CRC: pass, crc=0x%x\n", info.crc_result);
		else
			printf("End memory copy with CRC: fail at %d, crc=0x%x\n", ret, info.crc_result);

		printf("\n");
	}

	if (test_mode == 0 || test_mode == 3) {
		printf("Start verify with right value:\n");
		info.crc_compare = 0x55555555; 
		memset(info.in_addr, 0x55, buf_len);
		ret = ioctl(crc_fd, CRC_IOC_VERIFY_VAL, &info);
		if (ret < 0) {
			printf("crc: ioctl fail with %d\n", ret);
			goto out;
		}
		if (info.pos_verify)
			printf("End verify value: fail at position %d\n", info.pos_verify);
		else
			printf("End verify value: pass\n");
		
		printf("\n");

		printf("Start verify with wrong value:\n");
		*(char *)(info.in_addr + (buf_len >> 1)) = 0xaa;
		ret = ioctl(crc_fd, CRC_IOC_VERIFY_VAL, &info);
		if (ret < 0) {
			printf("crc: ioctl fail with %d\n", ret);
			goto out;
		}
		if (info.pos_verify)
			printf("End verify value: detect wrong value at position %d, pass\n", info.pos_verify);
		else
			printf("End verify value: fail\n");

		printf("\n");
	}

	if (test_mode == 0 || test_mode == 4) {
		printf("Start fill value(0x%x):\n", fill_val);
		info.val_fill = fill_val;
		ret = ioctl(crc_fd, CRC_IOC_FILL_VAL, &info);
		if (ret < 0) {
			printf("crc: ioctl fail with %d\n", ret);
			goto out;
		}
		for (data = (unsigned long *)info.out_addr; data < (unsigned long*)(info.out_addr + buf_len); data++)
			if (*data != fill_val)
				break;
		if (data == (unsigned long *)(info.out_addr + buf_len))
			printf("End fill value: pass\n");
		else
			printf("End fill value: fail at position 0x%x\n", ((unsigned long*)info.out_addr - data) >> 2);

		printf("\n");
	}

out:
	close(crc_fd);
	free(info.in_addr);
	
	return 0;
}
