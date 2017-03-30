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

static int open_icc(void)
{
	int ret = open("/dev/icc", O_RDWR);
	if (ret < 0) {
		perror("unable to open /dev/icc");
		exit(10);
	}
	return ret;
}

static void send_recv_test(int fd, const char *optarg)
{
	struct sm_packet pkt;
	uint32_t scalar0, scalar1;
	int ret = 0;
	pkt.local_ep = 9;
	if( strcmp("audio", optarg) == 0)
		pkt.remote_ep = 6;
	else
		pkt.remote_ep = 5;
	pkt.type = SP_SCALAR;
	printf("sp packet %d\n", pkt.type);

	printf("begin create ep\n");
	ioctl(fd, CMD_SM_CREATE, &pkt);
	printf("finish create ep session index = %d\n", pkt.session_idx);

	pkt.dst_cpu = 1;
	scalar0 = 0xAB;
	scalar1 = 0xCD;
	pkt.buf_len = scalar1;
	pkt.buf = scalar0;


	pkt.type = SM_SCALAR_READY_64;
	ioctl(fd, CMD_SM_SEND, &pkt);

	pkt.type = SM_SCALAR_READY_32;
	ret = ioctl(fd, CMD_SM_RECV, &pkt);
	scalar0 = pkt.buf;
	printf("ret = %x scalar received %x\n", ret, scalar0);

	sleep(4);
	ioctl(fd, CMD_SM_SHUTDOWN, &pkt);

}

#define GETOPT_FLAGS "rs:fhV"
#define a_argument required_argument
static struct option const long_opts[] = {
	{"receive",	no_argument, NULL, 'r'},
	{"send",	no_argument, NULL, 's'},
	{"help",	no_argument, NULL, 'h'},
	{"version",	no_argument, NULL, 'V'},
	{NULL,		no_argument, NULL, 0x0}
};

__attribute__ ((noreturn))
static void show_version(void)
{
	exit(EXIT_SUCCESS);
}

__attribute__ ((noreturn))
static void show_usage(int exit_status)
{
	printf(
		"\nUsage: packet_test [options] \n"
		"\n"
		"Options:\n"
	);
	exit(exit_status);
}

int main(int argc, char *argv[])
{
	int i;
	struct stat stat;
	void *buf;
	int fd = open_icc();
	unsigned int task_init_addr, task_exit_addr;
	struct sm_packet pkt;

	while ((i=getopt_long(argc, argv, GETOPT_FLAGS, long_opts, NULL)) != -1) {
		switch (i) {
		case 's':
			send_recv_test(fd, optarg);
			break;
		case 'h': show_usage(EXIT_SUCCESS);
		case 'V': show_version();
		case ':':
			fprintf(stderr, "Option '%c' is missing parameter", optopt);
			show_usage(EXIT_FAILURE);
		case '?':
			fprintf(stderr, "Unknown option '%c' or argument missing", optopt);
			show_usage(EXIT_FAILURE);
		default:
			fprintf(stderr, "Unhandled option '%c'; please report this", i);
			return EXIT_FAILURE;
		}
	}

	close(fd);

	return 0;
}
