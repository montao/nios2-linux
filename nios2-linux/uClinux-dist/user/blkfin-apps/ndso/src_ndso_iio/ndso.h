/*
 * Copyright 2004-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2.
 */

/* ------------ Defines ------------ */

#define DEBUG 				0

#define CALL_GNUPLOT "/bin/gnuplot /home/httpd/cgi-bin/gnu.plt_"
#define FILENAME_T_OUT "/home/httpd/cgi-bin/t_samples.txt_"
#define FILENAME_F_OUT "/home/httpd/cgi-bin/f_samples.txt_"
#define FILENAME_GNUPLT "/home/httpd/cgi-bin/gnu.plt_"

#define VALUE_FRAME "\n<html>\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1252\">\n<title></title></head><body> <p><font face=\"Tahoma\" size=\"10\">%4.3f Volt</font></p>\n"

#define MINSAMPLERATE 		1
#define MAXSAMPLERATE 		10001
#define MAXNUMSAMPLES 		10000
#define MINNUMSAMPLES 		1
#define MAXSIZERATIO		4
#define TIMEOUT			10

#define OUT_DEC 1
#define OUT_BIN 2
#define OUT_HEX 3

#ifndef fixed
#define fixed short
#endif

/* ------------ Structs ------------ */

typedef struct {
	unsigned short vdiv;
} vertical;

typedef struct {
	unsigned short set_grid;
	unsigned short axis;
	unsigned short style;
	unsigned short linestyle;
	unsigned short color;
	unsigned short logscale;
	unsigned short size_ratio;
	unsigned short xrange;
	unsigned short xrange1;
	unsigned short smooth;
	unsigned short tdom;
} display;

typedef struct {
	unsigned short device;
	unsigned short trigger;

} input;

typedef struct {
	unsigned int sps;
	unsigned int samples;
	unsigned int fsamples;
} time_set;

typedef struct {
	display sdisplay;
	vertical svertical;
	input sinput;
	time_set stime_s;
	unsigned short num_channels;
	unsigned long channel_en_mask;
	unsigned short run;
	int fd0;
	int framebuffer;
	FILE *pFile_samples;
	FILE *pFile_init;
	char *pFILENAME_T_OUT;
	char *pFILENAME_GNUPLT;
	char *pGNUPLOT;
	char *pREMOTE_ADDR;
} s_info;

/* ------------ Enums ------------ */

enum {
	ACQUIRE, SHOWDEVATTR, GNUPLOT_FILES
};				/* what program we want to run */

enum {
	IIO_OPEN, FILE_OPEN, SAMPLE_RATE, SAMPLE_DEPTH, SIZE_RATIO, RANGE, TIME_OUT
};

/* ------------ function prototypes ------------ */

extern int gettimeofday(struct timeval *, void *);

int iio_sample(int form_method, char **getvars, char **postvars, s_info * info,
	       char *device_name, char *trigger_name);
int iio_read_device_files(char *device_name, unsigned out);
