#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_BUS		"/dev/i2c-0"
#define I2C_SLAVE_FORCE	0x0706	/* Change slave address */

#define warn(fmt, args...) printf("%s: " fmt "\n", __func__, ## args)
#define warnp(fmt, args...) warn(fmt ": %s", ## args, strerror(errno))
#define err(fmt, args...) do { warn("error: " fmt, ## args); exit(1); } while (0)

int
local_cam_setget_register(int filehandle, int iDataWidth, int oDataWidth,
                          int slave_addr, int reg_addr, int *val, int newval)
{
	unsigned char buf[10], out[10];
	struct i2c_rdwr_ioctl_data i2c_data;
	struct i2c_msg msgs[2];

	out[0] = out[1] = 0;

	buf[0] = (unsigned char)reg_addr;

	if (iDataWidth == 1) {
		buf[1] = newval & 0xFF;
	} else {
		buf[1] = (newval >> 8) & 0xFF;
		buf[2] = newval & 0xFF;
	}

	msgs[0].addr = slave_addr;
	msgs[0].flags = 0;
	msgs[0].len = iDataWidth + 1;
	msgs[0].buf = buf;
	msgs[1].addr = slave_addr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = oDataWidth;
	msgs[1].buf = out;

	i2c_data.nmsgs = 2;
	i2c_data.msgs = msgs;

	if (ioctl(filehandle, I2C_RDWR, &i2c_data) != -1) {
		printf("setget done\n");
		if (iDataWidth == 1)
			warn("0x%x 0x%x written", buf[0], buf[1]);
		else
			printf("0x%x 0x%x 0x%x written", buf[0], buf[1], buf[2]);

		if (oDataWidth == 1) {
			*val = out[0];
		} else {
			*val = out[0] << 8;
			*val |= out[1];
		}
		return 0;
	}

	if (iDataWidth == 1)
		warnp("Error sending 0x%02x at 0x%02x", newval, reg_addr);
	else
		warnp("Error sending 0x%04x at 0x%02x", newval, reg_addr);

	return -1;
}

int
local_cam_set_register(int filehandle, int iDataWidth, int reg_addr, int val)
{
	unsigned char buf[10];

	buf[0] = (unsigned char)reg_addr;

	if (iDataWidth == 1) {
		buf[1] = val & 0xFF;
	} else {
		buf[1] = (val >> 8) & 0xFF;
		buf[2] = val & 0xFF;
	}

	if (write(filehandle, buf, iDataWidth + 1) == iDataWidth + 1) {
		if (iDataWidth == 1)
			warn("0x%x 0x%x written", buf[0], buf[1]);
		else
			printf("0x%x 0x%x 0x%x written", buf[0], buf[1], buf[2]);
		return 0;
	}

	if (iDataWidth == 1)
		warnp("Error sending 0x%02x at 0x%02x", val, reg_addr);
	else
		warnp("Error sending 0x%04x at 0x%02x", val, reg_addr);

	return -1;
}

int
local_cam_get_register(int filehandle, int bus_width, int reg_addr, int *val)
{
	unsigned char regval = reg_addr;	/* Device register to access */
	unsigned char buf[10];

	buf[0] = regval;

	if (write(filehandle, buf, 1) != 1) {
		warnp("error sending register address 0x%02x", regval);
		return -1;
	}

	if (read(filehandle, buf, bus_width) == bus_width) {
		if (bus_width == 1) {
			*val = buf[0];
		} else {
			*val = buf[0] << 8;
			*val |= buf[1];
		}

		return 0;
	}

	warnp("error reading value @ 0x%02x", regval);

	return -1;
}

static int Case = 0;
static void print_case(const char *banner)
{
	printf("Case %i: %s\n-----------------------------------------\n", ++Case, banner);
}

int main(int argc, char *argv[])
{
	int regval, regaddr, rc = 0;
	int i2c_fd;
	int slave_addr, wrong_addr;
	int lastregval;

	if (argc < 3) {
		printf(
			"Usage: twi_test 0x<correct slave addr> 0x<wrong slave addr>\n"
			"Example: 0x5e for ad5280 i2c chip on bf537-lq035 board.\n"
		);
		return 0;
	}

	slave_addr = strtol(argv[1], NULL, 16);
	wrong_addr = strtol(argv[2], NULL, 16);

	printf("slave addr: 0x%x\n", slave_addr);

	slave_addr >>= 1;

	i2c_fd = open(I2C_BUS, O_RDWR);

	print_case("Program with correct I2C_SLAVE_FORCE address");
	if (ioctl(i2c_fd, I2C_SLAVE_FORCE, slave_addr) < 0) /* Correct address */
		err("Fail to set SLAVE address\n");

	regaddr = 2;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 1;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 0;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 0x06;
	regval = 0x7f;
	rc = local_cam_set_register(i2c_fd, 1, regaddr, regval);
	if (rc)
		err("Fail to set data to register %d\n", regaddr);

	sleep(1);
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Data set to register %d is wrong\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	print_case("Program with incorrect I2C_SLAVE_FORCE address");
	ioctl(i2c_fd, I2C_SLAVE_FORCE, wrong_addr);	/* Incorrect address */

	sleep(1);
	regaddr = 2;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (!rc)
		err("Wrong addr is incorrect, read %d from reg %d\n", regval, regaddr);

	sleep(1);
	regaddr = 0x06;
	regval = 0x20;
	rc = local_cam_set_register(i2c_fd, 1, regaddr, regval);
	if (!rc)
		err("Wrong addr is incorrect, write %d to reg %d\n", regval, regaddr);

	print_case("Program with correct I2C_SLAVE_FORCE address");
	ioctl(i2c_fd, I2C_SLAVE_FORCE, slave_addr);	/* Correct address */

	sleep(1);
	regaddr = 2;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 1;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 0;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 0x09;
	regval = 0x30;
	rc = local_cam_set_register(i2c_fd, 1, regaddr, regval);
	if (rc)
		err("Fail to set data to register %d\n", regaddr);

	sleep(1);
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Data set to register %d is wrong\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	print_case("test twi repeat");
	regaddr = 2;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	sleep(1);
	regaddr = 2;
	regval = 0;
	lastregval = 2;
	rc = local_cam_setget_register(i2c_fd, 1, 1, slave_addr, regaddr, &regval, lastregval);
	if (rc)
		err("Fail to setget data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", 2, regval);
	if (lastregval != regval)
		err("Fail to run twi repeat test\n");

	sleep(1);
	regaddr = 2;
	rc = local_cam_get_register(i2c_fd, 1, regaddr, &regval);
	if (rc)
		err("Fail to get data from register %d\n", regaddr);

	printf("reg %d = 0x%x\n", regaddr, regval);

	close(i2c_fd);

	printf("TWI I2C test .... [PASS]\n");

	return 0;
}
