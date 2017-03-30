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
#include <linux/videodev2.h>

struct cap_buffer {
	void *start;
	size_t length;
};

static struct cap_buffer *pbuffer;

static const char *device = "/dev/video0";
static const char *file = "/1.yuv";
static int input = 0;
static int rate = 0;
static int number = 1;
static int width = 640;
static int height = 480;
static int image_size;

static int std_cap = 0;
static int dv_timings_cap = 0;

static void usage(const char *prog)
{
	printf("Usage: %s [-DIrFNwh]\n", prog);
	puts("  -D --device   device to use (default /dev/video0)\n"
	     "  -I --input    select input (default 0)\n"
	     "  -r --rate     frame rate (default 0)\n"
	     "  -F --file     file name (default /1.yuv)\n"
	     "  -N --number   number of frames to capture (default 1)\n"
	     "  -w --width    image width (default 640)\n"
	     "  -h --height   image height (default 480)\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "input",   1, 0, 'I' },
			{ "rate",    1, 0, 'r' },
			{ "file",    1, 0, 'F' },
			{ "number",  1, 0, 'N' },
			{ "width",   1, 0, 'w' },
			{ "height",  1, 0, 'h' },
			{ NULL,      0, 0, 0   },
		};
		int c;

		c = getopt_long(argc, argv, "D:I:r:F:N:w:h:", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 'I':
			input = atoi(optarg);
			break;
		case 'r':
			rate = atoi(optarg);
			break;
		case 'F':
			file = optarg;
			break;
		case 'N':
			number = atoi(optarg);
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
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		printf("%s is not video capture device\n", device);
		return ret;
	}
	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		printf("%s does not support streaming i/o\n", device);
		return ret;
	}
	return 0;
}

static int set_control(int fd)
{
	struct v4l2_control contrast;

	contrast.id = V4L2_CID_CONTRAST;
	contrast.value = 0x5A;
	return ioctl(fd, VIDIOC_S_CTRL, &contrast);
}

static int set_input(int fd)
{
	struct v4l2_input inputs;
	int ret, i;

	printf("enum inputs\n");
	for (i = 0; ; i++) {
		inputs.index = i;
		ret = ioctl(fd, VIDIOC_ENUMINPUT, &inputs);
		if (ret < 0)
			break;
		printf("input[%d]: %s\n", inputs.index, inputs.name);
	}
	ret = ioctl(fd, VIDIOC_S_INPUT, &input);
	if (ret < 0) {
		printf("ioctl VIDIOC_S_INPUT error\n");
		return ret;
	}
	printf("set input to %d: support ", input);
	inputs.index = input;
	ioctl(fd, VIDIOC_ENUMINPUT, &inputs);
	if (inputs.capabilities & V4L2_IN_CAP_STD) {
		std_cap = 1;
		printf("std ");
	}
	if (inputs.capabilities & V4L2_IN_CAP_CUSTOM_TIMINGS) {
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
	int ret;

	ret = ioctl(fd, VIDIOC_QUERYSTD, &std);
	if (!ret) {
		printf("sensed video standard is ");
		if (std == V4L2_STD_PAL_Nc)
			printf("PAL Combination N\n");
		else if (std == V4L2_STD_PAL_60)
			printf("PAL 60\n");
		else if (std == V4L2_STD_NTSC_443)
			printf("NTSC 443\n");
		else if (std == V4L2_STD_PAL_N)
			printf("PAL N\n");
		else if (std == V4L2_STD_PAL_M)
			printf("PAL M\n");
		else if(std & V4L2_STD_NTSC)
			printf("NTSC\n");
		else if (std & V4L2_STD_PAL)
			printf("PAL\n");
		else if (std & V4L2_STD_SECAM)
			printf("SECAM\n");
		else
			printf("unknown\n");
		ret = ioctl(fd, VIDIOC_S_STD, &std);
		if (ret < 0)
			printf("ioctl VIDIOC_S_STD error: %llx\n", std);
	}
	return ret;
}

static int get_dv_timings(int fd)
{
	struct v4l2_dv_timings dv_timings;
	struct v4l2_bt_timings bt;
	int ret;

	ret = ioctl(fd, VIDIOC_G_DV_TIMINGS, &dv_timings);
	if (ret) {
		printf("ioctl VIDIOC_G_DV_TIMINGS error\n");
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
		fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc);
		if (ret < 0)
			break;
		printf("format[%d]: %s\n", fmtdesc.index, fmtdesc.description);
	}

	fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
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

static int set_rate(int fd)
{
	struct v4l2_streamparm param;
	struct v4l2_fract *tpf = &param.parm.capture.timeperframe;
	int ret;

	param.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_G_PARM, &param);
	if (ret < 0) {
		printf("frame rate setting is not supported\n");
	} else if (rate) {
		printf("default frame rate = %d\n",
				tpf->denominator / tpf->numerator);
		tpf->numerator = 1;
		tpf->denominator = rate;
		ret = ioctl(fd, VIDIOC_S_PARM, &param);
		if (ret < 0) {
			printf("ioctl VIDIOC_S_PARM error\n");
			return ret;
		}
		printf("set frame rate to %d\n",
				tpf->denominator / tpf->numerator);
	}
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

	ret = set_input(fd);
	if (ret)
		goto err;

	if (std_cap) {
		ret = set_std(fd);
		if (ret)
			goto err;
	}
	if (dv_timings_cap) {
		ret = get_dv_timings(fd);
		if (ret)
			goto err;
	}

	ret = set_fmt(fd);
	if (ret)
		goto err;

	ret = set_rate(fd);
	if (ret)
		goto err;
	
	req.count       = 3;
	req.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
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

	pbuffer = calloc(req.count, sizeof(*pbuffer));
	if (!pbuffer) {
		printf("no memory\n");
		goto err;
	}

	for (i = 0; i< req.count; i++) {
		struct v4l2_buffer buf;
		memset(&buf, 0, sizeof(buf));
		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.index       = i;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_QUERYBUF error\n");
			goto err1;
		}
		printf("bufffer[%d]: offset = %lu, lengeth = %lu\n",
				i, buf.m.offset, buf.length);
		pbuffer[i].length = buf.length;
		pbuffer[i].start = mmap(NULL, buf.length,
					PROT_READ | PROT_WRITE,
					MAP_SHARED,
					fd, buf.m.offset);

		if (MAP_FAILED == pbuffer[i].start) {
			printf("mmap error: %s\n", strerror(errno));
			goto err1;
		}
		ret = ioctl(fd, VIDIOC_QBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_QBUF error\n");
			goto err1;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if (ret < 0) {
		printf("ioctl VIDIOC_STREAMON error\n");
		goto err2;
	}
	printf("stream on success\n");

	fp = fopen(file, "wb");
	if (!fp) {
		printf("cannot open '%s': %s\n", file, strerror(errno));
		goto err3;
	}
	printf("open image file '%s' success\n", file);

	for (i = 0; i < number; i++) {
		printf("processing frame %d\n", i);
		memset(&buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_DQBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_DQBUF error\n");
			goto err4;
		}

		fwrite(pbuffer[buf.index].start, 1, image_size, fp);

		ret = ioctl(fd, VIDIOC_QBUF, &buf);
		if (ret < 0) {
			printf("ioctl VIDIOC_QBUF error\n");
			goto err4;
		}
	}
	printf("%d frames captured\n", i);
err4:
	fclose(fp);
err3:
	ioctl(fd, VIDIOC_STREAMOFF, &type);
err2:
	for (i = 0; i< req.count; i++)
		munmap(pbuffer[i].start, pbuffer[i].length);
err1:
	free(pbuffer);
err:
	close(fd);
	return ret;
}
