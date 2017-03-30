/*
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 *
 * Based on asm/cpuinfo.h from microblaze
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License. See the file COPYING in the main directory of this
 * archive for more details.
 */

#ifndef _ASM_NIOS2_CPUINFO_H
#define _ASM_NIOS2_CPUINFO_H

#include <linux/types.h>
#include <asm/prom.h>

struct cpuinfo {
	/* Core CPU configuration */
	char cpu_impl[12];
	u32 cpu_clock_freq;
	u32 mmu;
	u32 has_div;
	u32 has_mul;
	u32 has_mulx;

	/* CPU caches */
	u32 icache_line_size;
	u32 icache_size;
	u32 dcache_line_size;
	u32 dcache_size;

	/* TLB */
	u32 tlb_pid_num_bits;	/* number of bits used for the PID in TLBMISC */
	u32 tlb_num_ways;
	u32 tlb_num_ways_log2;
	u32 tlb_num_entries;
	u32 tlb_num_lines;
	u32 tlb_ptr_sz;

	/* Addresses */
	u32 reset_addr;
	u32 exception_addr;
	u32 fast_tlb_miss_exc_addr;
};

extern struct cpuinfo cpuinfo;

extern void setup_cpuinfo(void);

#endif /* _ASM_NIOS2_CPUINFO_H */
