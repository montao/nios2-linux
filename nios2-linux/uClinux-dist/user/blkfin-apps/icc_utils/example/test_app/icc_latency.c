/*
 * User space application to load a standalone Blackfin ELF
 * into the second core of a dual core Blackfin (like BF561).
 *
 * Copyright 2005-2009 Analog Devices Inc.
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <link.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/ioctl.h>

#include <icc.h>
#include <asm/bfin_simple_timer.h>

int timer_fd;

static int open_icc(void)
{
	int ret = open("/dev/icc", O_RDWR);
	if (ret < 0) {
		perror("unable to open /dev/icc");
		exit(10);
	}
	return ret;
}

static int enable_timer(void)
{
	unsigned long period = 100000000;
	unsigned long width = 50000000;
	unsigned long mode;

	char *timer_dev = "/dev/timer1";
	mode = BFIN_SIMPLE_TIMER_MODE_PWMOUT_CONT_NOIRQ;

	timer_fd = open(timer_dev, O_RDWR);
	if (timer_fd < 0) {
		perror("unable to open timer dev\n");
		exit(10);
	}

	ioctl(timer_fd, BFIN_SIMPLE_TIMER_SET_MODE, mode);

	ioctl(timer_fd, BFIN_SIMPLE_TIMER_SET_PERIOD, period);

	ioctl(timer_fd, BFIN_SIMPLE_TIMER_SET_WIDTH, width);

	ioctl(timer_fd, BFIN_SIMPLE_TIMER_START, 0);

}

static unsigned long get_timer(void)
{
	unsigned long count;
	ioctl(timer_fd, BFIN_SIMPLE_TIMER_READ_COUNTER, &count);
	return count;
}

static void send_recv_test(int fd, const char *optarg)
{
	struct sm_packet pkt;
	uint32_t scalar0, scalar1;
	uint32_t stamp_start;
	int ret = 0;
	pkt.local_ep = 9;
	pkt.remote_ep = 5;
	pkt.type = SP_SCALAR;
	printf("sp packet %d\n", pkt.type);

	printf("begin create ep\n");
	ioctl(fd, CMD_SM_CREATE, &pkt);
	printf("finish create ep session index = %d\n", pkt.session_idx);

	pkt.dst_cpu = 1;
	enable_timer();

	scalar0 = 0xBAD;
	scalar1 = get_timer();
	stamp_start = scalar1;
	pkt.buf_len = scalar1;
	pkt.buf = scalar0;


	pkt.type = SM_SCALAR_READY_64;
	ioctl(fd, CMD_SM_SEND, &pkt);

	ret = ioctl(fd, CMD_SM_RECV, &pkt);
	scalar0 = pkt.buf;
	scalar1 = pkt.buf_len;
	printf("ret = %x scalar received %d %d\n", ret, scalar0, scalar1);

	printf("icc latency core0 -> core1 %d us\n", (scalar1 - stamp_start) / 100);
	printf("icc latency core1 -> core0 %d us\n", (get_timer() - scalar1) / 100);

	sleep(4);
	ioctl(fd, CMD_SM_SHUTDOWN, &pkt);

	close(timer_fd);
}

int main(int argc, char *argv[])
{
	int i;
	struct stat stat;
	void *buf;
	int fd = open_icc();
	unsigned int task_init_addr, task_exit_addr;
	struct sm_packet pkt;
	int timer_fd;

	send_recv_test(fd, optarg);

	close(fd);

	return 0;
}
