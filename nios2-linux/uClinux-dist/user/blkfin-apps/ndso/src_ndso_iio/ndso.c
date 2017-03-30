/*
 * Copyright 2004-2011 Analog Devices Inc.
 *
 * Licensed under the GPL-2.
 */

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm/bfin_sport.h>

#ifdef TM_IN_SYS_TIME
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "cgivars.h"
#include "htmllib.h"
#include "ndso.h"

static s_info sinfo;

void display_on_framebuffer(s_info * info)
{
	if (!info->framebuffer)
		return;

	if (vfork() == 0) {
		char img[256];
		snprintf(img, sizeof(img), "/home/httpd/img%s.png",
			 info->pREMOTE_ADDR);
		execlp("pngview", "pngview", "-q", img, NULL);
		printf("<br>Hmm, could not run pngview, that's odd ...<br>\n");
		_exit(-1);
	}
}

char *itostr(u_int iNumber, u_char cDigits, u_char cMode, u_char cDec_mode)
{
	static char cBuffer[31];
	char c, cMod;
	u_int iDivisor = 10;

	if (cDigits > 30) {
		cBuffer[0] = '\0';
	} else {
		if (iNumber) {
			switch (cMode) {
			case OUT_BIN:
				iDivisor = 2;
				break;

			case OUT_DEC:
				iDivisor = 10;
				break;

			case OUT_HEX:
				iDivisor = 16;
				break;
			}

			for (c = cDigits; c > 0; c--) {
				cMod = iNumber % iDivisor;
				if (cMode == OUT_HEX) {
					if (cMod > 9)
						cBuffer[c - 1] = cMod + 55;
					else
						cBuffer[c - 1] = cMod + 48;
				} else {
					if (cMode == OUT_DEC) {
						if ((!iNumber) && (cDec_mode))
							cBuffer[c - 1] = ' ';
						else
							cBuffer[c - 1] =
							    cMod + 48;
					} else
						cBuffer[c - 1] = cMod + 48;
				}
				iNumber /= iDivisor;
			}

			cBuffer[cDigits] = '\0';
		} else {
			if ((cMode == OUT_DEC) && (cDec_mode)) {
				for (c = 0; c < cDigits; c++)
					cBuffer[(unsigned char)c] = ' ';
				cBuffer[cDigits - 1] = '0';
				cBuffer[cDigits] = '\0';
			} else {
				for (c = 0; c < cDigits; c++)
					cBuffer[(unsigned char)c] = '0';
				cBuffer[(unsigned char)c] = '\0';
			}
		}
	}

	return (cBuffer);
};

int getrand(int max)
{

	int j;
	struct timeval tv;

	if (gettimeofday(&tv, NULL) != 0) {
		printf("Error getting time\n");
	}

	srand(tv.tv_sec);
	j = 1 + (int)((float)max * rand() / (23457 + 1.0));

	return j;
};

/* str2num */
int str2num(char *str)
{
	int num = 0;
	int i = 0, ilen;

	if (str == NULL)
		return -1;
	ilen = strlen(str);

	if (str[0] == '*' && str[1] == 0)
		return 1;

	for (i = 0; i < ilen; i++) {
		if (str[i] == '.' || str[i] == '-')
			i++;	// ignore dot and sign

		if (str[i] > 57 || str[i] < 48)
			return -1;
		num = num * 10 + (str[i] - 48);
	}
	return (str[0] == '-' ? (-1) * num : num);
};

void make_session_files(s_info * info)
{
	char str[80];

/* Generate File Names Based on the REMOTE IP ADDR */
	info->pREMOTE_ADDR = strdup(getRemoteAddr());

	info->pGNUPLOT =
	    strdup(strcat(strcpy(str, CALL_GNUPLOT), info->pREMOTE_ADDR));
	info->pFILENAME_T_OUT =
	    strdup(strcat(strcpy(str, FILENAME_T_OUT), info->pREMOTE_ADDR));
	info->pFILENAME_GNUPLT =
	    strdup(strcat(strcpy(str, FILENAME_GNUPLT), info->pREMOTE_ADDR));

	return;
};

void free_session_files(s_info * info)
{
	free(info->pREMOTE_ADDR);
	free(info->pFILENAME_T_OUT);
	free(info->pFILENAME_GNUPLT);
	free(info->pGNUPLOT);

	return;
};

void do_files(s_info * info)
{
	printf("<hr>\n<menu>\n");

	info->pFile_samples = fopen(info->pFILENAME_T_OUT, "r");
	if (info->pFile_samples) {
		fclose(info->pFile_samples);
		printf
		    ("  <li><font face=\"Arial Black\"><a href=\"t_samples.txt_%s\">Time Samples</a></font></li>\n",
		     info->pREMOTE_ADDR);
	}

	info->pFile_init = fopen(info->pFILENAME_GNUPLT, "r");
	if (info->pFile_init) {
		fclose(info->pFile_init);
		printf
		    ("  <li><font face=\"Arial Black\"><a href=\"gnu.plt_%s\">Gnuplot File</a></font></li>\n",
		     info->pREMOTE_ADDR);
	}

	if ((info->pFile_samples == NULL) && (info->pFile_init == NULL))
		printf
		    ("  <li><font face=\"Arial Black\">No Files available from %s</font></li>\n",
		     info->pREMOTE_ADDR);

	printf("</menu>\n<hr>\n");

	return;
};

int do_html(int form_method, char **getvars, char **postvars, s_info * info)
{

	switch (info->run) {
	case ACQUIRE:
		htmlHeader("NDSO Demo Web Page");
		htmlBody();
		printf
		    ("\n<img border=\"0\" src=\"/img%s.png?id=%s\" align=\"left\">\n",
		     info->pREMOTE_ADDR, itostr(getrand(6), 6, 1, 1));
		break;
	case SHOWDEVATTR:
		htmlHeader("NDSO Demo Web Page");
		htmlBody();
		iio_read_device_files(postvars[info->sinput.device], 1);
		break;
	case GNUPLOT_FILES:
		htmlHeader("NDSO Demo Web Page");
		htmlBody();
		do_files(info);
		break;
	default:

		break;
	}
	htmlFooter();
	cleanUp(form_method, getvars, postvars);

	fflush(stdout);

	return 0;
}

int do_error(int errnum, int form_method, char **getvars, char **postvars,
	   s_info * info)
{
	htmlHeader("NDSO Demo Web Page");
	htmlBody();

	switch (errnum) {

	case IIO_OPEN:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     IIO_OPEN);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Can't open %s IIO Device.\n</font></p>",
		    postvars[info->sinput.device]);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">- Try again later -\n</font></p>");
		break;
	case FILE_OPEN:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     FILE_OPEN);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Can't open FILE.\n</font></p>");
		break;
	case SAMPLE_RATE:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     SAMPLE_RATE);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Sample Rate outside specified range: [%d] < Rate < [%d] \n</font></p>",
		     MINSAMPLERATE, MAXSAMPLERATE);
		break;
	case SAMPLE_DEPTH:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     SAMPLE_DEPTH);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Sample Depth outside specified range: [%d] < Depth < [%d] \n</font></p>",
		     MINNUMSAMPLES, MAXNUMSAMPLES);
		break;
	case SIZE_RATIO:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     SIZE_RATIO);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Size Ratio contains invalid characters r exceeds maximum Size Ratio < [%d]\n</font></p>",
		     MAXSIZERATIO);
		break;
	case RANGE:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     RANGE);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Specified Range is invalid or out of range.\n</font></p>");
		break;
	case TIME_OUT:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[%d]:\n</font></p>",
		     TIME_OUT);
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">Ratio between Sample Depth and Sample Rate will exceed Timeout criteria [%d sec].\n</font></p>",
		     TIMEOUT);
		break;
	default:
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">ERROR[UNDEF]:\n</font></p>");
		printf
		    ("<p><font face=\"Tahoma\" size=\"7\">undefined ERROR: \n</font></p>");
		break;
	}

	htmlFooter();
	cleanUp(form_method, getvars, postvars);
	free_session_files(info);
	fflush(stdout);

	exit(1);
};

int parse_request(int form_method, char **getvars, char **postvars, s_info * info)
{
	int i;

	/*Preset checkbox settings */
	info->sdisplay.set_grid = 0;
	info->sdisplay.axis = 0;
	info->framebuffer = 0;

	if (form_method == POST) {
		/* Parse Request */
		for (i = 0; postvars[i]; i += 2) {
			if (strncmp(postvars[i], "D5", 2) == 0) {
				info->svertical.vdiv = i + 1;
			} else if (strncmp(postvars[i], "T2", 2) == 0) {
				info->stime_s.sps = str2num(postvars[i + 1]);
			} else if (strncmp(postvars[i], "T3", 2) == 0) {
				info->stime_s.samples =
				    str2num(postvars[i + 1]);
			} else if (strncmp(postvars[i], "set_grid", 8) == 0) {
				info->sdisplay.set_grid = 1;
			} else if (strncmp(postvars[i], "axis", 4) == 0) {
				info->sdisplay.axis = 1;
			} else if (strncmp(postvars[i], "linestyle", 9) == 0) {
				info->sdisplay.style = i + 1;
			} else if (strncmp(postvars[i], "color", 5) == 0) {
				info->sdisplay.color = i + 1;
			} else if (strncmp(postvars[i], "xrangeS", 7) == 0) {
				info->sdisplay.xrange = i + 1;
			} else if (strncmp(postvars[i], "xrangeE", 7) == 0) {
				info->sdisplay.xrange1 = i + 1;
			} else if (strncmp(postvars[i], "logscale", 8) == 0) {
				info->sdisplay.logscale = i + 1;
			} else if (strncmp(postvars[i], "size_ratio", 10) == 0) {
				info->sdisplay.size_ratio = i + 1;
			} else if (strncmp(postvars[i], "smooth", 6) == 0) {
				info->sdisplay.smooth = i + 1;
			} else if (strncmp(postvars[i], "device", 6) == 0) {
				info->sinput.device = i + 1;
			} else if (strncmp(postvars[i], "trigger", 7) == 0) {
				info->sinput.trigger = i + 1;
			} else if (strncmp(postvars[i], "C10", 3) == 0) {
				info->channel_en_mask |= (1 << 0);
			} else if (strncmp(postvars[i], "C11", 3) == 0) {
				info->channel_en_mask |= (1 << 1);
			} else if (strncmp(postvars[i], "C12", 3) == 0) {
				info->channel_en_mask |= (1 << 2);
			} else if (strncmp(postvars[i], "C13", 3) == 0) {
				info->channel_en_mask |= (1 << 3);
			} else if (strncmp(postvars[i], "C14", 3) == 0) {
				info->channel_en_mask |= (1 << 4);
			} else if (strncmp(postvars[i], "C15", 3) == 0) {
				info->channel_en_mask |= (1 << 5);
			} else if (strncmp(postvars[i], "C16", 3) == 0) {
				info->channel_en_mask |= (1 << 6);
			} else if (strncmp(postvars[i], "C17", 3) == 0) {
				info->channel_en_mask |= (1 << 7);
			} else if (strncmp(postvars[i], "R3", 2) == 0) {
				info->sdisplay.tdom = str2num(postvars[i + 1]);
			} else if (strncmp(postvars[i], "B1", 2) == 0) {
				info->run = ACQUIRE;
			} else if (strncmp(postvars[i], "B5", 2) == 0) {
				info->run = SHOWDEVATTR;
			} else if (strncmp(postvars[i], "B6", 2) == 0) {
				info->run = GNUPLOT_FILES;
			} else if (strncmp(postvars[i], "FB", 2) == 0) {
				info->framebuffer = str2num(postvars[i + 1]);
			}
		}
	}

	return 0;
};

int check_request(int form_method, char **getvars, char **postvars, s_info * info)
{

	if (info->stime_s.sps > (MAXSAMPLERATE)
	    || (info->stime_s.sps <= MINSAMPLERATE))
		do_error(SAMPLE_RATE, form_method, getvars, postvars, info);

	if (info->stime_s.samples > MAXNUMSAMPLES
	    || info->stime_s.samples <= MINNUMSAMPLES)
		do_error(SAMPLE_DEPTH, form_method, getvars, postvars, info);

	if ((info->stime_s.samples / info->stime_s.sps) > TIMEOUT)
		do_error(TIME_OUT, form_method, getvars, postvars, info);

	if (atof(postvars[info->sdisplay.size_ratio]) <= 0
	    || atof(postvars[info->sdisplay.size_ratio]) >= MAXSIZERATIO)
		do_error(SIZE_RATIO, form_method, getvars, postvars, info);

	if (str2num(postvars[info->sdisplay.xrange]) < 0)
		do_error(RANGE, form_method, getvars, postvars, info);

	if (str2num(postvars[info->sdisplay.xrange1]) < 0)
		do_error(RANGE, form_method, getvars, postvars, info);

	if (!(postvars[info->sdisplay.xrange][0] == '*' &&
	      postvars[info->sdisplay.xrange1][0] == '*'))
		if (atof(postvars[info->sdisplay.xrange1]) <=
		    atof(postvars[info->sdisplay.xrange]))
			do_error(RANGE, form_method, getvars, postvars, info);

	return 0;
}

int
make_file_samples(int form_method, char **getvars, char **postvars, s_info * info)
{
	int ret = iio_sample(form_method, getvars, postvars, info,
			     postvars[info->sinput.device],
			     postvars[info->sinput.trigger]);

	if (ret < 0) {
		do_error(IIO_OPEN, form_method, getvars, postvars, info);
	}

	return ret;
}

int
make_file_init(int form_method, char **getvars, char **postvars, s_info * info)
{
	int i, j;
	/* open file for write */

	info->pFile_init = fopen(info->pFILENAME_GNUPLT, "w");

	if (info->pFile_init == NULL) {
		do_error(FILE_OPEN, form_method, getvars, postvars, info);
	}

	/* print header information */

	fprintf(info->pFile_init, "#GNUPLOT File generated by NDSO\n");
	fprintf(info->pFile_init, "set term png\nset output \"../img%s.png\"\n",
		info->pREMOTE_ADDR);

	/* print commands */

	if (info->sdisplay.set_grid)
		fprintf(info->pFile_init, "set grid\n");

	if (info->sdisplay.axis)
		fprintf(info->pFile_init, "set xzeroaxis lt 2 lw 4\n");

	if (info->sdisplay.logscale)
		fprintf(info->pFile_init, "set %s\n",
			postvars[info->sdisplay.logscale]);

	if (info->sdisplay.style)
		fprintf(info->pFile_init, "set data style %s\n",
			postvars[info->sdisplay.style]);

	if (info->sdisplay.size_ratio)
		fprintf(info->pFile_init, "set size %s\n",
			postvars[info->sdisplay.size_ratio]);

	fprintf(info->pFile_init, "set xrange [%s:%s]\n",
		postvars[info->sdisplay.xrange],
		postvars[info->sdisplay.xrange1]);

	if (postvars[info->svertical.vdiv][0] != 'X')
		fprintf(info->pFile_init, "set ytics %s\n",
			postvars[info->svertical.vdiv]);

	fprintf(info->pFile_init,
		"set xlabel \"%d Samples @ %d Samples/s                t/ns->\"\n",
		info->stime_s.samples, info->stime_s.sps);
	fprintf(info->pFile_init, "set ylabel \"mVolt\" \n");

	fprintf(info->pFile_init, "plot \"%s\" ", info->pFILENAME_T_OUT);

	/* Last channel is the timestamp */

	for (i = 0, j = 0; i < info->num_channels - 1; i++) {
		while (!((info->channel_en_mask >> j++) & 1)) ;
		if (i == (info->num_channels - 2)) {
			if (str2num(postvars[info->sdisplay.smooth]))
				fprintf(info->pFile_init,
					"using %d:%d smooth %s title \"ch%d\"",
					info->num_channels, i + 1, postvars[info->sdisplay.smooth],
					j - 1);
			else
				fprintf(info->pFile_init,
					"using %d:%d title \"ch%d\"",info->num_channels, i + 1,
					j - 1);
		} else {
			if (str2num(postvars[info->sdisplay.smooth]))
				fprintf(info->pFile_init,
					"using %d:%d smooth %s title \"ch%d\", '' ",
					info->num_channels, i + 1, postvars[info->sdisplay.smooth],
					j - 1);
			else
				fprintf(info->pFile_init,
					"using %d:%d title \"ch%d\", '' ",info->num_channels, i + 1,
					j - 1);
		}
	}
	fprintf(info->pFile_init, "\nexit\n");

	/* close file */

	fclose(info->pFile_init);

	return 0;
};

int main()
{
	char **postvars = NULL;	/* POST request data repository */
	char **getvars = NULL;	/* GET request data repository */
	int form_method;	/* POST = 1, GET = 0 */

	s_info *info = &sinfo;

	form_method = getRequestMethod();

	if (form_method == POST) {
		getvars = getGETvars();
		postvars = getPOSTvars();
	} else if (form_method == GET) {
		getvars = getGETvars();
	}

	make_session_files(info);

	parse_request(form_method, getvars, postvars, info);

	check_request(form_method, getvars, postvars, info);

	switch (info->run) {

	case ACQUIRE:
		info->num_channels =
		    make_file_samples(form_method, getvars, postvars, info);
		make_file_init(form_method, getvars, postvars, info);
		system(info->pGNUPLOT);
		do_html(form_method, getvars, postvars, info);
		display_on_framebuffer(info);
		break;

	case SHOWDEVATTR:
	case GNUPLOT_FILES:
		do_html(form_method, getvars, postvars, info);
		break;

	default:

		break;
	}

	free_session_files(info);
	exit(0);
}
