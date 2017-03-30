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

#include <bfin_sram.h>
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

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif
#ifndef EM_BLACKFIN
# define EM_BLACKFIN 106
#endif

//#define CMD_COREB_START _IO('b', 0)

#define ICC_TASKINIT_FUNC "_icc_task_init"
#define ICC_TASKEXIT_FUNC "_icc_task_exit"

static int open_coreb(void)
{
	int ret = open("/dev/coreb", O_RDWR);
	if (ret < 0) {
		perror("unable to open /dev/coreb");
		exit(10);
	}
	return ret;
}

static void start_coreb(void)
{
	int fd = open_coreb();
	if (ioctl(fd, CMD_COREB_START, NULL) != 0)
		perror("ioctl(CMD_COREB_START) failed");
	close(fd);
}

static unsigned long total_mem(void)
{
	struct sysinfo info;
	if (sysinfo(&info))
		perror("sysinfo() failed");
	return info.totalram;
}

#define ASYNC_BASE 0x20000000
#define ASYNC_LEN  0x10000000

/* The valid memory map of Core B ... sanity checking so we don't
 * do something bad (by accident?)
 */
struct {
	void *start, *end;
	int index;
} mem_regions[] = {
	{	/* L1 Data Bank A */
		.start = (void*)0xFF400000,
		.end   = (void*)0xFF400000 + 0x8000,
		.index = 3,
	},{	/* L1 Data Bank B */
		.start = (void*)0xFF500000,
		.end   = (void*)0xFF500000 + 0x8000,
		.index = 2,
	},{	/* L1 Instruction SRAM */
		.start = (void*)0xFF600000,
		.end   = (void*)0xFF600000 + 0x10000,
		.index = 0,
	},{ /* L1 Instruction SRAM/Cache */
		.start = (void*)0xFF610000,
		.end   = (void*)0xFF610000 + 0x4000,
		.index = 1,
	},{	/* L2 SRAM */
		.start = (void*)0xFEB00000,
		.end   = (void*)0xFEB00000 + 0x20000,
		.index = -1,
	},{
		.start = (void*)0xc8080000,
		.end   = (void*)0xc8080000 + 0x40000,
		.index = -1,
	},{	/* SDRAM - just assume from 0 to top of ASYNC bank is OK */
		.start = (void*)0x00000000,
		.end   = (void*)0x30000000,
		.index = -1,
	}
};

/* XXX: should a lot of this be tossed out in favor of using dma_memcpy ? */
static int put_region(void *dst, size_t dst_size, const void *src, size_t src_size)
{
	size_t i;
	int ret;
	void *new_src = NULL;
	unsigned int nbytes = 0;
	unsigned long ldst = (unsigned long)dst;
	char *cdst, *csrc;

	if ((unsigned long)dst == 0)
		return 0;

	/* figure out how to get this section into the memory map */
	for (i = 0; i < ARRAY_SIZE(mem_regions); ++i) {
		if (dst >= mem_regions[i].start && dst < mem_regions[i].end) {
			if (dst + dst_size > mem_regions[i].end) {
				fprintf(stderr, "section at 0x%p (length=%zi) overflows bound 0x%p!\n",
				        dst, dst_size, mem_regions[i].end);
				return -1;
			}
			break;
		}
	}
	if (i == ARRAY_SIZE(mem_regions)) {
		fprintf(stderr, "no valid memory region found for 0x%p\n", dst);
		return 1;
	}

	/* see if this is an alloced region */
	if (dst_size > src_size) {
		new_src = malloc(dst_size);
		if (!new_src) {
			fprintf(stderr, "out of memory (could not malloc(%zi))\n", dst_size);
			return 1;
		}
		/* if data and bss are merged into one load, copy over
		 * data first and zero fill the remaining section
		 */
		if (src_size)
			memcpy(new_src, src, src_size);
		memset(new_src + src_size, 0x00, dst_size - src_size);
		src = new_src;
	}

	/* move the memory into Core B -- L1 stuff needs kernel help */
#define MEM_ERR(fmt, args...) \
	fprintf(stderr, \
		"\nERROR: Your destination address looks wrong: %p\n" \
		fmt "; aborting.\n"\
		" (re-run with --force to skip this check)\n\n", dst, ## args)

	ret = 0;
	if (ldst >= 0xff800000) {
		/* should not load into Core A L1 */
		ret = 1;
		MEM_ERR("This seems to be Core A L1 memory");
	} else if (ldst >= ASYNC_BASE && ldst < ASYNC_BASE + ASYNC_LEN) {
		/* should not load into async memory */
		ret = 1;
		MEM_ERR("It doesn't really make sense to try and load into async");
	} else if (ldst <= total_mem()) {
		/* should not load into unavailable memory */
		ret = 1;
		MEM_ERR("The max mem available on your system seems to be 0x%08lx,\n"
		        "but the destination is above that", total_mem());
	} else if (ldst <= 0x4000) {
		/* should not load into start of memory */
		ret = 1;
		MEM_ERR("The start of memory is reserved (NULL/fixed_code/kernel)");
	}

	if (ret == 0) {
		if (ldst >= 0xff000000)
			dma_memcpy(dst, src, dst_size);
		else {
			memset(dst, 0, dst_size);
			memcpy(dst, src, dst_size);
			nbytes = dst_size;
			cdst = (char *)dst;
			csrc = (char *)src;
			for (i = 0; i < dst_size; i++) {
				if (cdst[i] != csrc[i])
					printf("check failed %d\n", nbytes);
			}

		}
	}


	free(new_src);

	printf("writing to 0x%08lx, 0x%-7zx bytes: %s\n", (unsigned long)dst, dst_size,
		ret ? "FAILED" : "OK");

	return ret;
}

#define IS_ELF(buff) \
	(buff[EI_MAG0] == ELFMAG0 && \
	 buff[EI_MAG1] == ELFMAG1 && \
	 buff[EI_MAG2] == ELFMAG2 && \
	 buff[EI_MAG3] == ELFMAG3)

void *map_elf(const char *coreb_elf, struct stat *stat)
{
	void *buf;
	int fd = open(coreb_elf, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "Unable to load %s: %s\n", coreb_elf, strerror(errno));
		return NULL;
	}

	if (fstat(fd, stat) < 0) {
		fprintf(stderr, "Unable to stat %s: %s\n", coreb_elf, strerror(errno));
		close(fd);
		return NULL;
	}

	if (stat->st_size < EI_NIDENT) {
		fprintf(stderr, "File is too small to be an ELF\n");
		close(fd);
		return NULL;
	}

	buf = mmap(0, stat->st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		fprintf(stderr, "Unable to mmap %s: %s\n", coreb_elf, strerror(errno));
		close(fd);
		return NULL;
	}
	close(fd);
	return buf;
}

void unmap_elf(void *buf, struct stat* stat)
{
	munmap(buf, stat->st_size);
}

unsigned int elf_sym_addr(void *buf, const char *symname)
{
	ElfW(Ehdr) *ehdr;
	ElfW(Phdr) *phdr;
	unsigned int ret = 0;
	uint16_t i;
	Elf32_Sym *symtab;
	unsigned int symnum;
	char *strtab;

	ehdr = (ElfW(Ehdr) *)buf;

	/* make sure we have a valid ELF */
	if (!IS_ELF(ehdr->e_ident) || ehdr->e_machine != EM_BLACKFIN) {
		fprintf(stderr, "file is not a Blackfin ELF file\n");
		return -1;
	}

	ElfW(Shdr) *shdr = (ElfW(Shdr) *)(buf + ehdr->e_shoff);
	for (i = 0; i < ehdr->e_shnum; ++i) {
		if (shdr[i].sh_type == SHT_SYMTAB) {
			symtab = (Elf32_Sym *)(buf + shdr[i].sh_offset);
			symnum = shdr[i].sh_size/shdr[i].sh_entsize;
			strtab = (char *)buf + shdr[shdr[i].sh_link].sh_offset;
		}
	}

	for (i = 0; i < symnum; i++) {
		if (strcmp(strtab + symtab[i].st_name, symname) == 0) {
			ret = symtab[i].st_value;
			return ret;
		}
	}

	return -1;
}


/* XXX: should we sanity check and make sure the ELF contains no relocations ? */
int elf_load(void *buf)
{
	ElfW(Ehdr) *ehdr;
	ElfW(Phdr) *phdr;
	int ret = 0;
	uint16_t i;

	ehdr = (ElfW(Ehdr) *)buf;

	/* make sure we have a valid ELF */
	if (!IS_ELF(ehdr->e_ident) || ehdr->e_machine != EM_BLACKFIN) {
		fprintf(stderr, "file is not a Blackfin ELF file\n");
		return 1;
	}

	/* make sure we have no unhandled program headers */
	phdr = (ElfW(Phdr) *)(buf + ehdr->e_phoff);
	for (i = 0; i < ehdr->e_phnum; ++i) {
		switch (phdr->p_type) {
			case PT_LOAD: break;
			default:
				fprintf(stderr, "unhandled program header %i (%X): did you link this application properly?\n",
				        i, phdr->p_type);
				return 2;
		}
		++phdr;
	}

	/* now load all the program headers */
	phdr = (ElfW(Phdr) *)(buf + ehdr->e_phoff);
	for (i = 0; i < ehdr->e_phnum; ++i) {
		ret |= put_region((void*)phdr->p_vaddr, phdr->p_memsz, buf + phdr->p_offset, phdr->p_filesz);
		++phdr;
	}

	/* VisualDSP fails to fully populate the program headers for
	 * bss sections so we need to walk the section headers.  weak.
	 */
	if (ehdr->e_flags == 0x4) { /* gcc doesnt use this flag */
		ElfW(Shdr) *shdr = (ElfW(Shdr) *)(buf + ehdr->e_shoff);
		printf("hacking around broken VDSP program header table\n");
		for (i = 0; i < ehdr->e_shnum; ++i) {
			/* assume NOBITS sections == bss */
			if (shdr->sh_type == SHT_NOBITS)
				ret |= put_region((void*)shdr->sh_addr, shdr->sh_size, NULL, 0);
			++shdr;
		}
	}

	return ret;
}

static int open_icc(void)
{
	int ret = open("/dev/icc", O_RDWR);
	if (ret < 0) {
		perror("unable to open /dev/icc");
		exit(10);
	}
	return ret;
}

static void exec_task(int fd, unsigned int task_init_addr, unsigned int task_exit_addr)
{
	struct sm_packet pkt;
	struct sm_task *task1 = NULL;
	int taskargs = 2;
	char taskargv0[] = "task1";
	char taskargv1[] = "icc";
	int packetsize = sizeof(struct sm_task) + MAX_TASK_NAME * taskargs;

	task1 = malloc(packetsize);
	if (!task1) {
		printf("malloc failed\n");
	}

	memset(task1, 0, packetsize);
	task1->task_init = task_init_addr;
	task1->task_exit = task_exit_addr;
	task1->task_argc = 2;
	strcpy(task1->task_argv[0], taskargv0);
	strcpy(task1->task_argv[1], taskargv1);

	memset(&pkt, 0, sizeof(struct sm_packet));
	pkt.local_ep = 0;
	pkt.remote_ep = 5;
	pkt.type = SP_TASK_MANAGER;
	pkt.dst_cpu = 1;
	pkt.buf_len = packetsize;
	pkt.buf = task1;

	ioctl(fd, CMD_SM_CREATE, &pkt);

	pkt.type = SM_TASK_RUN;
	ioctl(fd, CMD_SM_SEND, &pkt);

	sleep(5);
	ioctl(fd, CMD_SM_SHUTDOWN, &pkt);

	free(task1);

}
#define GETOPT_FLAGS "l:e:kfhV"
#define a_argument required_argument
static struct option const long_opts[] = {
	{"load",	no_argument, NULL, 'l'},
	{"exec",	no_argument, NULL, 'e'},
	{"kill",	no_argument, NULL, 'k'},
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
		"\nUsage: icc_loader [options] \n"
		"\n"
		"Options:\n"
			"-l load icc core\n"
			"-e exec icc task\n"
			"-k kill icc task\n"
			"-h help\n"
			"-V version\n"
	);
	exit(exit_status);
}

int main(int argc, char *argv[])
{
	int i;
	struct stat stat;
	void *buf = NULL;
	int fd = open_icc();
	unsigned int task_init_addr, task_exit_addr;
	struct sm_packet pkt;

	while ((i=getopt_long(argc, argv, GETOPT_FLAGS, long_opts, NULL)) != -1) {
		switch (i) {
		case 'l':
			buf = map_elf(optarg, &stat);
			if (!buf)
				return EXIT_FAILURE;
			elf_load(buf);
			unmap_elf(buf, &stat);
			start_coreb();
			break;
		case 'e':
			buf = map_elf(optarg, &stat);
			if (!buf)
				return EXIT_FAILURE;
			elf_load(buf);
			task_init_addr = elf_sym_addr(buf, ICC_TASKINIT_FUNC);
			if (task_init_addr == 0xffffffff)
				return EXIT_FAILURE;
			fprintf(stderr, "task_init_addr %x\n", task_init_addr);
			task_exit_addr = elf_sym_addr(buf, ICC_TASKEXIT_FUNC);
			fprintf(stderr, "task_exit_addr %x\n", task_exit_addr);
			if (task_exit_addr == 0xffffffff)
				task_exit_addr = 0;
			exec_task(fd, task_init_addr, task_exit_addr);
			unmap_elf(buf, &stat);
			break;
		case 'k':
			memset(&pkt, 0, sizeof(struct sm_packet));
			ioctl(fd, 8, &pkt);
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

	return i;
}
