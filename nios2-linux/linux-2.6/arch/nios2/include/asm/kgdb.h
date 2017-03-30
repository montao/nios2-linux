/*
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 *
 * Based on the code posted by Kazuyasu on the Altera Forum at:
 * http://www.alteraforum.com/forum/showpost.php?p=77003&postcount=20
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_KGDB_H
#define _ASM_NIOS2_KGDB_H

#define CACHE_FLUSH_IS_SAFE	1
#define BUFMAX			2048

enum regnames {
	GDB_R0 = 0,
	GDB_AT,
	GDB_R2,
	GDB_R3,
	GDB_R4,
	GDB_R5,
	GDB_R6,
	GDB_R7,
	GDB_R8,
	GDB_R9,
	GDB_R10,
	GDB_R11,
	GDB_R12,
	GDB_R13,
	GDB_R14,
	GDB_R15,
	GDB_R16,
	GDB_R17,
	GDB_R18,
	GDB_R19,
	GDB_R20,
	GDB_R21,
	GDB_R22,
	GDB_R23,
	GDB_ET,
	GDB_BT,
	GDB_GP,
	GDB_SP,
	GDB_FP,
	GDB_EA,
	GDB_BA,
	GDB_RA,
	GDB_PC,
	GDB_STATUS,
	GDB_ESTATUS,
	GDB_BSTATUS,
	GDB_IENABLE,
	GDB_IPENDING,
	GDB_CPUID,
	GDB_CTL6,
	GDB_EXCEPTION,
	GDB_PTEADDR,
	GDB_TLBACC,
	GDB_TLBMISC,
	GDB_CTL11,
	GDB_BADADDR,
	GDB_CONFIG,
	/* do not change the last entry or anything below! */
	GDB_NUMREGBYTES		/* number of registers */
};

#define NUMREGBYTES		(GDB_NUMREGBYTES * 4)

#define BREAK_INSTR_SIZE	4
static inline void arch_kgdb_breakpoint(void)
{
	/*
	 * we cannot use 'trap 30' here as the nios2 assembler is bugged and
	 * does consider any argument an error, so we just encode it directly.
	 */
	__asm__ __volatile__ (".word 0x003b6fba");
}

#endif /* _ASM_NIOS2_KGDB_H */
