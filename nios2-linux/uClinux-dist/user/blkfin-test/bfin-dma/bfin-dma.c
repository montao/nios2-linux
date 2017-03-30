#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

#define passert(expr) ({ if (!(expr)) { perror(#expr); exit(1); } })

static int mmr32_style_dma;

struct dmasg16 {
	void *next_desc_addr;
	void *start_addr;
	unsigned short cfg;
	unsigned short x_count;
	short x_modify;
	unsigned short y_count;
	short y_modify;
} __attribute__((packed));

struct dma16_state {
	unsigned int channel;
	volatile int done;
	struct dmasg16 dsc_src, dsc_dst;
};

#define BF_DMA16_REQUEST _IOW('D', 0x00, struct dma16_state)
#define BF_DMA16_FREE    _IOW('D', 0x01, struct dma16_state)
#define BF_DMA16_RUN     _IOW('D', 0x02, struct dma16_state)
#define BF_DMA16_ARUN    _IOW('D', 0x03, struct dma16_state)

struct dmasg32 {
	void *next_desc_addr;
	void *start_addr;
	unsigned long cfg;
	unsigned long x_count;
	long x_modify;
	unsigned long y_count;
	long y_modify;
} __attribute__((packed));

struct dma32_state {
	unsigned int channel;
	volatile int done;
	struct dmasg32 dsc_src, dsc_dst;
};


#define BF_DMA32_REQUEST _IOW('D', 0x00, struct dma32_state)
#define BF_DMA32_FREE    _IOW('D', 0x01, struct dma32_state)
#define BF_DMA32_RUN     _IOW('D', 0x02, struct dma32_state)
#define BF_DMA32_ARUN    _IOW('D', 0x03, struct dma32_state)

#define BF_DMA_REQUEST mmr32_style_dma?BF_DMA32_REQUEST:BF_DMA16_REQUEST
#define BF_DMA_FREE    mmr32_style_dma?BF_DMA32_FREE:BF_DMA16_FREE
#define BF_DMA_RUN     mmr32_style_dma?BF_DMA32_RUN:BF_DMA16_RUN
#define BF_DMA_ARUN    mmr32_style_dma?BF_DMA32_ARUN:BF_DMA16_ARUN

#define BF_DMA_DI_EN		(mmr32_style_dma?0x100000:0x80)
#define BF_DMA_EN		0x1
#define BF_DMA_WNR		0x2
#define BF_DMA_MSIZE		(mmr32_style_dma?0x700:0xC)
#define BF_DMA_MSIZE_OFFSET	(mmr32_style_dma?8:2)
#define BF_DMA_MSIZE_8		0
#define BF_DMA_MSIZE_16		1
#define BF_DMA_MSIZE_32		2

#define SIZE 1024
char src[SIZE], dst[SIZE];

#define ioctl(fd, cmd, arg) \
({ \
	int ret; \
	printf("ioctl(%i, %i, %p) = ", fd, cmd, arg); \
	ret = ioctl(fd, cmd, arg); \
	printf("%i", ret); \
	if (ret) \
		printf(" (%s)", strerror(errno)); \
	printf("\n"); \
})

static void dump(char *c, size_t l)
{
	size_t i;
	for (i = 0; i < l; ++i)
		printf("%x ", c[i]);
	printf("\n");
}

static unsigned int scan_file(const char *prefix, const char *reg)
{
	unsigned int ret;
	char *file;
	FILE *fp;
	asprintf(&file, "%s%s", prefix, reg);
	fp = fopen(file, "r");
	if (!fp) {
		perror(file);
		return 0;
	}
	fscanf(fp, "%x", &ret);
	fclose(fp);
	return ret;
}

void decode_state(const char *prefix)
{
	unsigned int NEXT_DESC_PTR = scan_file(prefix, "NEXT_DESC_PTR");
	unsigned int START_ADDR    = scan_file(prefix, "START_ADDR");
	unsigned int CONFIG        = scan_file(prefix, "CONFIG");
	unsigned int X_COUNT       = scan_file(prefix, "X_COUNT");
	unsigned int X_MODIFY      = scan_file(prefix, "X_MODIFY");
	unsigned int Y_COUNT       = scan_file(prefix, "Y_COUNT");
	unsigned int Y_MODIFY      = scan_file(prefix, "Y_MODIFY");
	unsigned int CURR_DESC_PTR = scan_file(prefix, "CURR_DESC_PTR");
	unsigned int CURR_ADDR     = scan_file(prefix, "CURR_ADDR");
	unsigned int IRQ_STATUS    = scan_file(prefix, "IRQ_STATUS");
	unsigned int CURR_X_COUNT  = scan_file(prefix, "CURR_X_COUNT");
	unsigned int CURR_Y_COUNT  = scan_file(prefix, "CURR_Y_COUNT");

	printf("  --- %s ---\n", prefix);
	printf("desc: curr: 0x%08x  next: 0x%08x\n", CURR_DESC_PTR, NEXT_DESC_PTR);
	printf("addr: curr: 0x%08x start: 0x%08x\n", CURR_ADDR, START_ADDR);
	printf("X: curr: 0x%04x count: 0x%04x mod: 0x%04x (%i)\n", CURR_X_COUNT, X_COUNT, X_MODIFY, (short)X_MODIFY);
	printf("Y: curr: 0x%04x count: 0x%04x mod: 0x%04x (%i)\n", CURR_Y_COUNT, Y_COUNT, Y_MODIFY, (short)Y_MODIFY);

	printf("dma config: 0x%04x (%sabled %s ", CONFIG,
		(CONFIG & (1 << 0)) ? "en" : "dis",
		(CONFIG & (0x1 << 1)) ? "write" : "read");
	switch (CONFIG & (0x3 << 2)) {
		case 0x3: printf("WDSIZE:INVALID "); break;
		case 0x2: printf("32-bit "); break;
		case 0x1: printf("16-bit "); break;
		case 0x0: printf("8-bit "); break;
	}
	printf("%s %s%s%s",
		(CONFIG & (0x1 << 4)) ? "2D" : "1D",
		(CONFIG & (0x1 << 5)) ? "sync " : "",
		(CONFIG & (0x1 << 6)) ? "di_sel " : "",
		(CONFIG & (0x1 << 7)) ? "interrupt " : "");
	unsigned int NDSIZE = CONFIG & (0xF << 8);
	if (NDSIZE > 0 && NDSIZE < 10)
		printf("NDSIZE_%i ", NDSIZE);
	else if (NDSIZE >= 10)
		printf("NDSIZE:INVALID:%i ", NDSIZE);
	unsigned int FLOW = CONFIG & (0x7 << 12);
	switch (FLOW) {
		case 0: printf("stop"); break;
		case 1: printf("autobuffer"); break;
		case 4: printf("descriptor_array"); break;
		case 6: printf("descriptor_list_small"); break;
		case 7: printf("descriptor_list_large"); break;
		default: printf("FLOW:INVALID:%i", FLOW);
	}
	printf(")\n");
	printf("irq status: 0x%04x (%s%s%s%s)\n", IRQ_STATUS,
		(IRQ_STATUS & (0x1 << 0)) ? "done " : "",
		(IRQ_STATUS & (0x1 << 1)) ? "err " : "",
		(IRQ_STATUS & (0x1 << 2)) ? "dfetch " : "",
		(IRQ_STATUS & (0x1 << 3)) ? "run " : "");
}
#if 0
#define ds_s1() decode_state("/sys/kernel/debug/blackfin/MDMA Source 1/MDMA_S1_")
#define ds_d1() decode_state("/sys/kernel/debug/blackfin/MDMA Destination 1/MDMA_D1_")
#define ds_1() ds_s1(), ds_d1()
#else
#define ds_1()
#endif

#define INIT_USER_DMA(type, name, msize)\
	unsigned long name##_cfg = BF_DMA_DI_EN | BF_DMA_EN |	\
				(msize << BF_DMA_MSIZE_OFFSET);	\
	struct type name = {		\
		.channel = 1,		\
		.dsc_src = {		\
			.next_desc_addr = NULL,			\
			.start_addr = src,			\
			.cfg = name##_cfg,			\
			.x_count = sizeof(src) >> msize,	\
			.x_modify = 1 << msize,			\
		},			\
		.dsc_dst = {		\
			.next_desc_addr = NULL,			\
			.start_addr = dst,			\
			.cfg = name##_cfg | BF_DMA_WNR,		\
			.x_count = sizeof(dst) >> msize,	\
			.x_modify = 1 << msize,			\
		},			\
	};

#define BF_DMA_IOCTL(fd, cmd, name)		\
	if (mmr32_style_dma)			\
		ioctl(fd, cmd, &name##_32);	\
	else					\
		ioctl(fd, cmd, &name##_16);

int main(int argc, char *argv[])
{
	int i, fd;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	if (system("cat /proc/cpuinfo | grep 0x27fe") == 0) {
		mmr32_style_dma = 1;
		printf("Test 32-bit MMR DMA copy.\n");
	} else {
		mmr32_style_dma = 0;
		printf("Test 16-bit MMR DMA copy.\n");
	}

	fd = open("/dev/bfin-dma", O_WRONLY);
	passert(fd != -1);
	printf("open(/dev/bfin-dma) = %i\n", fd);

	if (argc == 2) {
		if (!strcmp(argv[1], "-d")) {
			/* -d = decode dma state */
			ds_1();
			return 0;
		} else if (!strcmp(argv[1], "-f")) {
			/* -f = force free dma channel */
			struct dma16_state state_16 = { .channel = 1 };
			struct dma32_state state_32 = { .channel = 1 };
			BF_DMA_IOCTL(fd, BF_DMA_FREE, state);
			return 0;
		}
	}

	i = mmr32_style_dma?8:2;

	/* Do a synchronous transfer (8bit) first */
	INIT_USER_DMA(dma16_state, state_16, BF_DMA_MSIZE_8)
	INIT_USER_DMA(dma32_state, state_32, BF_DMA_MSIZE_8)
	BF_DMA_IOCTL(fd, BF_DMA_REQUEST, state);

	ds_1();

	memset(src, 's', sizeof(src));
	memset(dst, 'd', sizeof(dst));
	BF_DMA_IOCTL(fd, BF_DMA_RUN, state);
	i = memcmp(src, dst, sizeof(src));
	printf("memcmp = %i\n", i);
	if (i) {
		dump(src, sizeof(src));
		dump(dst, sizeof(dst));
	}

	BF_DMA_IOCTL(fd, BF_DMA_FREE, state);

	/* Do an asynchronous transfer (16bit) next */
	INIT_USER_DMA(dma16_state, state2_16, BF_DMA_MSIZE_16)
	INIT_USER_DMA(dma32_state, state2_32, BF_DMA_MSIZE_16)
	BF_DMA_IOCTL(fd, BF_DMA_REQUEST, state2);

	memset(src, 's', sizeof(src));
	memset(dst, 'd', sizeof(dst));
	BF_DMA_IOCTL(fd, BF_DMA_ARUN, state2);
	i = 0;
	if (mmr32_style_dma)			\
		while (!state2_32.done)
			++i;
	else
		while (!state2_16.done)
			++i;
	printf("slept for %i loads\n", i);
	i = memcmp(src, dst, sizeof(src));
	printf("memcmp = %i\n", i);
	if (i) {
		dump(src, sizeof(src));
		dump(dst, sizeof(dst));
	}

	BF_DMA_IOCTL(fd, BF_DMA_FREE, state2);

	return 0;
}
