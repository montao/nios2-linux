/*
 *    Rev:          $Id$
 *    Created:      06.07.2005 18:16
 *    Author:       Michael Hennerich
 *    mail:         hennerich@blackfin.uclinux.org
 *    Description:  Simple I2C Routines
 *
 *   Copyright (C) 2005 Michael Hennerich
 *
 *   Licensed under the GPL-2 or later.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "i2c-dev.h"
#include "i2c.h"

#define I2C_DEVICE "/dev/i2c-0"
#define I2C_SLAVE_ADDR 0x38	/* Randomly picked */
#define I2C_DEVID (0xB8>>1)

#define pwarn(fmt, args...) fprintf(stderr, "%s: error: " fmt ": %s\n", __func__, ## args, strerror(errno))
#define perr(fmt, args...)  do { pwarn(fmt, ## args); exit(1); } while (0)

static int i2c_open_slave(const char *device, int addr)
{
	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0)
		perr("could not open %s", device);

	if (ioctl(fd, I2C_SLAVE, addr) < 0)
		perr("could not bind address %x", addr);

	return fd;
}

int i2c_write_register(const char *device, unsigned char client,
		unsigned char reg, unsigned short value)
{
	int addr = I2C_SLAVE_ADDR;
	unsigned char msg_data[32];
	struct i2c_msg msg = { addr, 0, 0, msg_data };
	struct i2c_rdwr_ioctl_data rdwr = { &msg, 1 };
	int fd;

	fd = i2c_open_slave(device, addr);

	msg.len = 3;
	msg.flags = 0;
	msg_data[0] = reg;
	msg_data[2] = (0xFF & value);
	msg_data[1] = (value >> 8);
	msg.addr = client;

	if (ioctl(fd, I2C_RDWR, &rdwr) < 0) {
		pwarn("could not write msg");
		return -1;
	}

	close(fd);
	return 0;
}

int i2c_read_register(const char *device, unsigned char client, unsigned char reg)
{
	int addr = I2C_SLAVE_ADDR;
	unsigned char msg_data[32];
	struct i2c_msg msg = { addr, 0, 0, msg_data };
	struct i2c_rdwr_ioctl_data rdwr = { &msg, 1 };
	int fd;

	fd = i2c_open_slave(device, addr);

	msg_data[0] = reg;
	msg.addr = client;
	msg.len = 1;
	msg.flags = 0;

	if (ioctl(fd, I2C_RDWR, &rdwr) < 0) {
		pwarn("could not write msg");
		close(fd);
		return -1;
	}

	msg.len = 2;
	msg_data[0] = 0;
	msg_data[1] = 0;
	msg.flags = I2C_M_RD;

	if (ioctl(fd, I2C_RDWR, &rdwr) < 0) {
		pwarn("could not read msg");
		close(fd);
		return -1;
	}

	close(fd);
	return (msg_data[0] << 8) | msg_data[1];
}

void i2c_dump_register(const char *device, unsigned char client,
		unsigned short start, unsigned short end)
{
	int addr = I2C_SLAVE_ADDR;
	unsigned char msg_data[32];
	struct i2c_msg msg = { addr, 0, 0, msg_data };
	struct i2c_rdwr_ioctl_data rdwr = { &msg, 1 };
	int fd, i;

	fd = i2c_open_slave(device, addr);

	for (i = start; i < end; i++) {

		msg_data[0] = i;
		msg.addr = client;
		msg.len = 1;
		msg.flags = 0;

		if (ioctl(fd, I2C_RDWR, &rdwr) < 0) {
			pwarn("could not write to %i\n", i);
			continue;
		}

		msg.len = 2;
		msg_data[0] = 0;
		msg_data[1] = 0;
		msg.flags = I2C_M_RD;

		if (ioctl(fd, I2C_RDWR, &rdwr) < 0)
			pwarn("could not read back %i\n", i);
		else
			fprintf(stderr, "Register %02x : %02x%02x\n", i,
				msg_data[0], msg_data[1]);
	}

	close(fd);
}

void i2c_scan_bus(const char *device)
{
	int addr = I2C_SLAVE_ADDR;
	unsigned char msg_data[32];
	struct i2c_msg msg = { addr, 0, 0, msg_data };
	struct i2c_rdwr_ioctl_data rdwr = { &msg, 1 };
	int fd, i;

	fd = i2c_open_slave(device, addr);

	msg.len = 1;
	msg.flags = 0;
	msg_data[0] = 0;
	msg_data[1] = 0;

	for (i = 0; i < 128; ++i) {

		msg.addr = i;

		if (ioctl(fd, I2C_RDWR, &rdwr) < 0) {
			//fprintf(stderr, "Error: could not write \n");
		} else
			fprintf(stderr,
				"FOUND I2C device at 0x%X (8-bit Adrress 0x%X) \n",
				msg.addr, msg.addr << 1);
	}

	close(fd);
}

#ifdef main
int main()
{
	i2c_scan_bus(I2C_DEVICE);
	i2c_write_register(I2C_DEVICE, I2C_DEVID, 9, 0x0248);
	i2c_dump_register(I2C_DEVICE, I2C_DEVID, 0, 255);
	printf("Read Register 9 = 0x%X \n",
	       i2c_read_register(I2C_DEVICE, I2C_DEVID, 9));

	exit(0);
}
#endif
