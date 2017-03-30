/*
 * Analog Devices V4L2 test application
 *
 * Copyright (c) 2012 Analog Devices Inc.
 *
 * Author: Scott Jiang <Scott.Jiang.Linux@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include "videodev2.h"

#if __GNUC__ < 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ < 6))
/* Sadly gcc versions older than 4.6 have a bug in how they initialize
   anonymous unions where they require additional curly brackets.
   This violates the C1x standard. This workaround adds the curly brackets
   if needed. */
#define V4L2_INIT_BT_TIMINGS(_width, args...) \
	{ .bt = { _width , ## args } }
#else
#define V4L2_INIT_BT_TIMINGS(_width, args...) \
	.bt = { _width , ## args }
#endif

#define V4L2_DV_BT_CEA_1280X720P50 { \
	.type = V4L2_DV_BT_656_1120, \
	V4L2_INIT_BT_TIMINGS(1280, 720, 0, \
		V4L2_DV_HSYNC_POS_POL | V4L2_DV_VSYNC_POS_POL, \
		74250000, 440, 40, 220, 5, 5, 20, 0, 0, 0, \
		V4L2_DV_BT_STD_CEA861, 0) \
}

#define V4L2_DV_BT_CEA_1280X720P60 { \
	.type = V4L2_DV_BT_656_1120, \
	V4L2_INIT_BT_TIMINGS(1280, 720, 0, \
		V4L2_DV_HSYNC_POS_POL | V4L2_DV_VSYNC_POS_POL, \
		74250000, 110, 40, 220, 5, 5, 20, 0, 0, 0, \
		V4L2_DV_BT_STD_CEA861, V4L2_DV_FL_CAN_REDUCE_FPS) \
}

struct cap_buffer {
	void *start;
	size_t length;
};

static struct cap_buffer *pbuffer;

static const char *device = "/dev/video0";
static const char *file = "/1.yuv";
static const char *standard = "pal";
static const char *dv = "720p60";
static int output = 0;
static int width = 640;
static int height = 480;
static int image_size;

static int std_cap = 0;
static int dv_timings_cap = 0;

static void usage(const char *prog)
{
	printf("Usage: %s [-DOFsdwh]\n", prog);
	puts("  -D --device   device to use (default /dev/video0)\n"
	     "  -O --output   select output (default 0)\n"
	     "  -F --file     file name (default /1.yuv)\n"
	     "  -s --std      video standard pal/ntsc (default pal)\n"
	     "  -d --dv       dv timings 720p60/720p50 (default 720p60)\n"
	     "  -w --width    image width (default 640)\n"
	     "  -h --height   image height (default 480)\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "output",  1, 0, 'O' },
			{ "file",    1, 0, 'F' },
			{ "std",     1, 0, 's' },
			{ "dv",      1, 0, 'd' },
			{ "width",   1, 0, 'w' },
			{ "height",  1, 0, 'h' },
			{ NULL,      0, 0, 0   },
		};
		int c;

		c = getopt_long(argc, argv, "D:O:F:s:d:w:h:", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 'O':
			output = atoi(optarg);
			break;
		case 'F':
			file = optarg;
			break;
		case 's':
			standard = optarg;
			break;
		case 'd':
			dv = optarg;
			break;
		case 'w':
			width = atoi(optarg);
			break;
		case 'h':
			height = atoi(optarg);
			break;
		default:
			usage(argv[0]);
			break;
		}
	}
}

static int query_cap(int fd)
{
	struct v4l2_capability cap;
	int ret;

	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		printf("ioctl VIDIOC_QUERYCAP error\n");
		return ret;
	}
	if (!(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)) {
		printf("%s is not video output device\n", device);
		return ret;
	}
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("%s does not support streaming i/o\n", device);
		return ret;
	}
	return 0;
}

static int set_output(int fd)
{
	struct v4l2_output outputs;
	int ret, i;

	printf("enum outputs\n");
	for (i = 0; ; i++) {
		outputs.index = i;
		ret = ioctl(fd, VIDIOC_ENUMOUTPUT, &outputs);
		if (ret < 0)
			break;
		printf("output[%d]: %s\n", outputs.index, outputs.name);
	}
	ret = ioctl(fd, VIDIOC_S_OUTPUT, &output);
	if (ret < 0) {
		printf("ioctl VIDIOC_S_OUTPUT error\n");
		return ret;
	}
	printf("set output to %d: support ", output);
	outputs.index = output;
	ioctl(fd, VIDIOC_ENUMOUTPUT, &outputs);
	if (outputs.capabilities & V4L2_OUT_CAP_STD) {
		std_cap = 1;
		printf("std ");
	}
	if (outputs.capabilities & V4L2_OUT_CAP_CUSTOM_TIMINGS) {
		dv_timings_cap = 1;
		printf("dv_timings ");
	}
	printf("\n");
	sleep(1);

	return 0;
}

static int set_std(int fd)
{
	v4l2_std_id std;
	int ret = -1;

	printf("set video standard to ");
	if (!strcmp(standard, "pal")) {
		std = V4L2_STD_PAL;
		printf("PAL\n");
	} else if (!strcmp(standard, "ntsc")) {
		std = V4L2_STD_NTSC;
		printf("NTSC\n");
	} else {
		std = V4L2_STD_UNKNOWN;
		printf("Unknown\n");
	}

	if (std != V4L2_STD_UNKNOWN) {
		ret = ioctl(fd, VIDIOC_S_STD, &std);
		if (ret < 0)
			printf("ioctl VIDIOC_S_STD error: %llx\n", std);
	}
	return ret;
}

static int set_dv_timings(int fd)
{
	struct v4l2_dv_timings dv_timings;
	struct v4l2_bt_timings bt;
	int ret;

	if (!strcmp(dv, "720p60"))
		dv_timings = (struct v4l2_dv_timings)V4L2_DV_BT_CEA_1280X720P60;
	else if (!strcmp(dv, "720p50"))
		dv_timings = (struct v4l2_dv_timings)V4L2_DV_BT_CEA_1280X720P50;
	else
		return -1;

	ret = ioctl(fd, VIDIOC_S_DV_TIMINGS, &dv_timings);
	if (ret) {
		printf("ioctl VIDIOC_S_DV_TIMINGS error\n");
		return ret;
	}
	bt = dv_timings.bt;
	printf("dv_timings: %s,width=%d,height=%d\n"
			"hfrontporch=%d,hsync=%d,hbackporch=%d,"
			"vfrontporch=%d,vsync=%d,vbackporch=%d\n",
			bt.interlaced ? "interlaced" : "progressive",
			bt.width, bt.height,
			bt.hfrontporch, bt.hsync, bt.hbackporch,
			bt.vfrontporch, bt.vsync, bt.vbackporch);
	if (bt.interlaced)
		printf("il_vfrontporch=%d,il_vsync=%d,il_vbackporch=%d\n",
				bt.il_vfrontporch, bt.il_vsync,
				bt.il_vbackporch);
	return 0;
}

static int set_fmt(int fd)
{
	struct v4l2_format fmt;
	struct v4l2_fmtdesc fmtdesc;
	int ret, i;

	printf("enum formats\n");
	for (i = 0; ; i++) {
		fmtdesc.index = i;
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
		if (ret < 0)
			break;
		printf("format[%d]: %s\n", fmtdesc.index, fmtdesc.description);
	}

	fmt.type                = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	fmt.fmt.pix.width       = width;
	fmt.fmt.pix.height      = height;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
	fmt.fmt.pix.field       = V4L2_FIELD_ANY;

	ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
	if (ret < 0) {
		printf("ioctl VIDIOC_S_FMT error\n");
		return ret;
	}
	printf("negotiated formats: %lux%lu\n",
			fmt.fmt.pix.width, fmt.fmt.pix.height);
	printf("field order: ");
	switch (fmt.fmt.pix.field) {
	case V4L2_FIELD_NONE:
		printf("progressive\n");
		break;
	case V4L2_FIELD_INTERLACED:
		printf("interlaced\n");
		break;
	case V4L2_FIELD_SEQ_TB:
		printf("sequential TB\n");
		break;
	case V4L2_FIELD_SEQ_BT:
		printf("sequential BT\n");
		break;
	default:
		printf("unknown field order\n");
		break;
	}
	printf("size = %lu\n", fmt.fmt.pix.sizeimage);
	printf("bytesperline = %lu\n", fmt.fmt.pix.bytesperline);
	printf("pixelformat: ");
	switch (fmt.fmt.pix.pixelformat) {
	case V4L2_PIX_FMT_UYVY:
		printf("UYVY\n");
		break;
	case V4L2_PIX_FMT_YUYV:
		printf("YUYV\n");
		break;
	case V4L2_PIX_FMT_RGB565:
		printf("RGB565\n");
		break;
	case V4L2_PIX_FMT_RGB444:
		printf("RGB444\n");
		break;
	default:
		printf("unknown pixel format\n");
		break;
	}
	image_size = fmt.fmt.pix.sizeimage;

	return 0;
}

int main(int argc, char *argv[])
{
	int fd, ret, index, i;
	struct v4l2_requestbuffers req;
	struct v4l2_buffer buf;
	enum v4l2_buf_type type;
	FILE *fp;

	parse_opts(argc, argv);

	fd = open(device, O_RDWR);
	if (fd < 0) {
		printf("cannot open '%s': %s\n", device, strerror(errno));
		return -1;
	}

	ret = query_cap(fd);
	if (ret)
		goto err;

	ret = set_output(fd);
	if (ret)
		goto err;

	if (std_cap) {
		ret = set_std(fd);
		if (ret)
			goto err;
	}
	if (dv_timings_cap) {
		ret = set_dv_timings(fd);
		if (ret)
			goto err;
	}

	ret = set_fmt(fd);
	if (ret)
		goto err;

	req.count       = 3;
	req.type        = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	req.memory      = V4L2_MEMORY_MMAP;
	ret = ioctl(fd, VIDIOC_REQBUFS, &req);
	if (ret < 0) {
		if (EINVAL == errno)
			printf ("%s does not support memory mapping\n",
					device);
		else
			printf("ioctl VIDIOC_REQBUFS error\n");
		goto err;
	}
	printf("request %lu buffers\n", req.count);

	fp = fopen(file, "rb");
	if (!fp) {
		printf("cannot open '%s': %s\n", file, strerror(errno));
		goto err3;
	}
	printf("open image file '%s' success\n", file);

	pbuffer = calloc(req.count, sizeof(*pbuffer));
	if (!pbuffer) {
		printf("no memory\n");
		goto err1;
	}

	for (i = 0; i< req.count; i++) {
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));
		buf.type        = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_QUERYBUF error\n");
			goto err2;
		}
		pbuffer[i].length = buf.length;
		pbuffer[i].start = mmap(NULL, buf.length,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fd, buf.m.offset);

		if (MAP_FAILED == pbuffer[i].start) {
			printf("mmap error: %s\n", strerror(errno));
			goto err2;
		}
		rewind(fp);
		fread(pbuffer[i].start, 1, image_size, fp);
		ret = ioctl(fd, VIDIOC_QBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_QBUF error\n");
			goto err2;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if (ret < 0) {
		printf("ioctl VIDIOC_STREAMON error\n");
		goto err3;
	}
	printf("stream on success\n");

	for (i = 0;; i++) {
		memset(&buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
		buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_DQBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_DQBUF error\n");
			goto err4;
		}

		//memset(pbuffer[buf.index].start,0x65,image_size);

		ret = ioctl(fd, VIDIOC_QBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_QBUF error\n");
			goto err4;
		}
	}
err4:
	ioctl(fd, VIDIOC_STREAMOFF, &type);
err3:
	for (i = 0; i< req.count; i++)
		munmap(pbuffer[i].start, pbuffer[i].length);
err2:
	free(pbuffer);
err1:
	fclose(fp);
err:
	close(fd);
	return ret;
}
