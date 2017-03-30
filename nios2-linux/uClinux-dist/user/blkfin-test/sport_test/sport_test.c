/******************************************************************************
 * Filename:	sport_test.c - test sport driver
 * Description:	This program write data to ad73311 audio card through
 * 		 interface /dev/sport.
 * Author:	Roy Huang <roy.huang@analog.com>
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

#include "bfin_sport.h"
#include "ad73311.h"

static int sport_fd, data_fd;

static const char *argv0;

#define GPIO_DIR_IN     0
#define GPIO_DIR_OUT    1

static int gpio_export(unsigned gpio)
{
        int fd, len;
        char buf[11];

        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd < 0) {
                perror("gpio/export");
                return fd;
        }

        len = snprintf(buf, sizeof(buf), "%d", gpio);
        write(fd, buf, len);
        close(fd);

        return 0;
}

static int gpio_unexport(unsigned gpio)
{
        int fd, len;
        char buf[11];

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd < 0) {
                perror("gpio/export");
                return fd;
        }

        len = snprintf(buf, sizeof(buf), "%d", gpio);
        write(fd, buf, len);
        close(fd);
        return 0;
}

static int gpio_dir(unsigned gpio, unsigned dir)
{
        int fd, len;
        char buf[60];

        len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

        fd = open(buf, O_WRONLY);
        if (fd < 0) {
                perror("gpio/direction");
                return fd;
        }

        if (dir == GPIO_DIR_OUT)
                write(fd, "out", 4);
        else
                write(fd, "in", 3);

        close(fd);
        return 0;
}

static int gpio_dir_out(unsigned gpio)
{
        return gpio_dir(gpio, GPIO_DIR_OUT);
}

static int gpio_value(unsigned gpio, unsigned value)
{
        int fd, len;
        char buf[60];

        len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

        fd = open(buf, O_WRONLY);
        if (fd < 0) {
                perror("gpio/value");
                return fd;
        }

        if (value)
                write(fd, "1", 2);
        else
                write(fd, "0", 2);

        close(fd);
        return 0;
}

static void ad73311_enable(unsigned gpio)
{
	gpio_export(gpio);
	gpio_dir_out(gpio);
	gpio_value(gpio,1);
}

#define warn(fmt, args...) \
	fprintf(stderr, "%s: " fmt "\n", argv0 , ## args)
#define warnf(fmt, args...) warn("%s(): " fmt, __func__ , ## args)
#define warnp(fmt, args...) warn(fmt ": %s" , ## args , strerror(errno))
#define _err(wfunc, fmt, args...) \
	do { \
		wfunc(fmt, ## args); \
		close(sport_fd); \
		close(data_fd); \
		exit(EXIT_FAILURE); \
	} while (0)
#define err(fmt, args...) _err(warn, fmt, ## args)
#define errf(fmt, args...) _err(warnf, fmt, ## args)
#define errp(fmt, args...) _err(warnp, fmt , ## args)

#define DEFAULT_SPORT "/dev/sport0"

/* Definitions for Microsoft WAVE format */
#define RIFF		0x46464952
#define WAVE		0x45564157
#define FMT		0x20746D66
#define DATA		0x61746164
#define PCM_CODE	1
#define WAVE_MONO	1
#define WAVE_STEREO	2

struct wave_header {
	u_long	main_chunk;	/* 'RIFF' */
	u_long	length;		/* filelen */
	u_long	chunk_type;	/* 'WAVE' */

	u_long	sub_chunk;	/* 'fmt ' */
	u_long	sc_len;		/* length of sub_chunk, =16 */
	u_short	format;		/* should be 1 for PCM-code */
	u_short	modus;		/* 1 Mono, 2 Stereo */
	u_long	sample_fq;	/* frequence of sample */
	u_long	byte_p_sec;
	u_short	byte_p_spl;	/* samplesize; 1 or 2 bytes */
	u_short	bit_p_spl;	/* 8, 12 or 16 bit */

	u_long	data_chunk;	/* 'data' */
	u_long	data_length;	/* samplecount */
};

int test_wavefile(void *buffer)
{
	struct wave_header *wp = buffer;
	if (wp->main_chunk == RIFF && wp->chunk_type == WAVE &&
			wp->sub_chunk == FMT && wp->data_chunk == DATA) {
		if (wp->format != PCM_CODE) {
			fprintf(stderr, "Can't play non-pcm-coded wave\n");
			return -1;
		}

		if (wp->modus != WAVE_MONO) {
			fprintf(stderr, "Can only play mono wave file\n");
			return -1;
		}
	}
	return 0;
}

#define BUF_LEN		0x1000

static void fill_waveheader(int fd, int cnt)
{
	struct wave_header wh;

	wh.main_chunk = RIFF;
	wh.length     = cnt + sizeof(wh) - 8;
	wh.chunk_type = WAVE;
	wh.sub_chunk  = FMT;
	wh.sc_len     = 16;
	wh.format     = PCM_CODE;
	wh.modus      = 1;
	wh.sample_fq  = 8000;
	wh.byte_p_spl = 2;
	wh.byte_p_sec = 8000 * 1 * 2;
	wh.bit_p_spl  = 16;
	wh.data_chunk = DATA;
	wh.data_length= cnt;
	write(fd, &wh, sizeof(wh));
}

static void usage(int status)
{
	fprintf(status ? stderr : stdout,
		"Usage: sport_test [options] <filename>\n"
		"\n"
		"Options:\n"
		"  -d          Use TDM mode\n"
		"  -n <ch_nu>  Use <ch_nu> channels for TDM mode, default is 8\n"
		"  -r          Read from SPORT and write to <filename>\n"
		"  -t          Write to sport and read from <filename> (default)\n"
		"  -s <sport>  Use <sport> rather than default %s\n"
		"  -g <gpio>   Use gpio<gpio> rather than gpio4 \n"
		"              (for enabling AD73311)\n",
		DEFAULT_SPORT
	);
	exit(status);
}

int main(int argc, char *argv[])
{
	int transmit, c;
	int tdm = 0;
	int gpio_en = 4;
	int ch_nu = 8;
	char *sport_path, *data_path;
	unsigned short ctrl_regs[6];
	struct sport_config config;
	unsigned char *buffer;
	int count = 10 * 2 * 8000; /* Only record 10 second */

	argv0 = argv[0];

	sport_path = DEFAULT_SPORT;
	transmit = 1;	/* default to writing data to sport */

	while ((c = getopt(argc, argv, "dn:hrts:g:")) != EOF)
		switch (c) {
		case 'd':
			tdm = 1;
			break;
		case 'n':
			ch_nu = atoi(optarg);
			break;
		case 'r':
			transmit = 0;
			break;
		case 't':
			transmit = 1;
			break;
		case 's':
			sport_path = optarg;
			break;
		case 'g':
			gpio_en = atoi(optarg);
			break;
		case 'h':
			usage(0);
		default:
			usage(1);
		}

	if (optind + 1 != argc)
		usage(1);
	data_path = argv[optind];

	sport_fd = open(sport_path, O_RDWR, 0);
	if (sport_fd < 0)
		errp("failed to open %s\n", sport_path);

	if ((buffer = malloc(BUF_LEN)) == NULL)
		errp("malloc() failed");

	if (transmit == 1) { /* Test and read wave data file */
		if ((data_fd = open(data_path, O_RDONLY, 0)) < 0)
			errp("opening data file '%s' failed", data_path);
		if (read(data_fd, buffer, sizeof(struct wave_header)) < 0)
			errp("reading wave_header from '%s' failed", data_path);
		if (test_wavefile(buffer) < 0)
			err("file '%s' doesn't seem to contain a wave file", data_path);
	} else {
		/* Open the file for write data */
		if ((data_fd = open(data_path, O_WRONLY | O_CREAT, O_TRUNC)) < 0)
			errp("opening data file '%s' failed", data_path);
		/* Write the head of the wave file */
		fill_waveheader(data_fd, count);
	}
	if(!tdm){	
		ad73311_enable(gpio_en);
	}

	/* Set registers on AD73311L through SPORT.  */
#if 0
	/* DMCLK = MCLK/4 = 16.384/4 = 4.096 MHz
	 * SCLK = DMCLK/8 = 512 KHz
	 * Sample Rate = DMCLK/512 = 8 KHz */
	ctrl_regs[0] = AD_CONTROL | AD_WRITE | CTRL_REG_B | MCDIV(0x3) | \
								DIRATE(0x2) ;
#else
	/* DMCLK = MCLK = 16.384 MHz
	 * SCLK = DMCLK/8 = 2.048 MHz
	 * Sample Rate = DMCLK/2048  = 8 KHz */
	ctrl_regs[0] = AD_CONTROL | AD_WRITE | CTRL_REG_B | MCDIV(0) | \
							SCDIV(0) | DIRATE(0);

#endif
	ctrl_regs[1] = AD_CONTROL | AD_WRITE | CTRL_REG_C | PUDEV | PUADC | \
				PUDAC | PUREF | REFUSE ;/* Register C */
	ctrl_regs[2] = AD_CONTROL | AD_WRITE | CTRL_REG_D | OGS(0) | IGS(5);
	ctrl_regs[3] = AD_CONTROL | AD_WRITE | CTRL_REG_E | DA(0x1f);
	ctrl_regs[4] = AD_CONTROL | AD_WRITE | CTRL_REG_F | SEEN ;
//	ctrl_regs[4] = AD_CONTROL | AD_WRITE | CTRL_REG_F | ALB;
//	ctrl_regs[4] = AD_CONTROL | AD_WRITE | CTRL_REG_F | 0;
	/* Put AD73311L to data mode */
	ctrl_regs[5] = AD_CONTROL | AD_WRITE | CTRL_REG_A | MODE_DATA;
//	ctrl_regs[5] = AD_CONTROL | AD_WRITE | CTRL_REG_A | SLB | MODE_DATA;

#if 0
	fprintf(stderr, "0x%04x 0x%04x 0x%04x 0x%04x 0x%4x 0x%4x\n",
			ctrl_regs[0], ctrl_regs[1], ctrl_regs[2],
			ctrl_regs[3], ctrl_regs[4], ctrl_regs[5]);
#endif

	memset(&config, 0, sizeof(struct sport_config));
	config.fsync = 1;
	config.word_len = 16;
	config.dma_enabled = 1;
	if(tdm){
		config.mode = TDM_MODE;
		config.channels = ch_nu;
		config.int_clk = 1;
		config.serial_clk = 5000000;
	}
	/* Configure sport controller by ioctl */
	if (ioctl(sport_fd, SPORT_IOC_CONFIG, &config) < 0)
		errp("ioctl('%s', SPORT_IOC_CONFIG) failed", sport_path);

	/* Write control data to ad73311's control register by write operation*/
	if(!tdm){
		if (write(sport_fd, (char *)ctrl_regs, 12) < 0)
			errp("setting up sport ctrl regs failed");
	}

	if (transmit == 1) {
		/* Write data into sport device through write operation */
		while (read(data_fd, buffer, BUF_LEN) > 0)
			if (write(sport_fd, buffer, BUF_LEN) != BUF_LEN)
				errp("writing to sport failed");
	} else {
		int left = count, temp1, temp2;
		/* Read data from sport and write it into file */
		while (left > 0) {
			temp1 = left > BUF_LEN ? BUF_LEN : left;
			if ((temp2 = read(sport_fd, buffer, temp1)) < 0)
				errp("reading from sport failed");
			write(data_fd, buffer, temp1);
			left -= temp2;
		}
	}

	close(sport_fd);
	close(data_fd);
	free(buffer);
	
	gpio_unexport(gpio_en);	

	return 0;
}
