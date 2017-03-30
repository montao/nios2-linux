/*
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_REGISTERS_H
#define _ASM_NIOS2_REGISTERS_H

/* control register numbers */
#define CTL_STATUS	0
#define CTL_ESTATUS	1
#define CTL_BSTATUS	2
#define CTL_IENABLE	3
#define CTL_IPENDING	4
#define CTL_CPUID	5
#define CTL_RSV1	6
#define CTL_EXCEPTION	7
#define CTL_PTEADDR	8
#define CTL_TLBACC	9
#define CTL_TLBMISC	10
#define CTL_RSV2	11
#define CTL_BADADDR	12
#define CTL_CONFIG	13
#define CTL_MPUBASE	14
#define CTL_MPUACC	15

/* access control registers using GCC builtins */
#define RDCTL(r)	__builtin_rdctl(r)
#define WRCTL(r, v)	__builtin_wrctl(r, v)

/* status register bits */
#define STATUS_PIE	(1<<0)	/* processor interrupt enable */
#define STATUS_U	(1<<1)	/* user mode */

/* estatus register bits */
#define ESTATUS_EPIE	(1<<0)	/* processor interrupt enable */
#define ESTATUS_EU	(1<<1)	/* user mode */

#ifdef CONFIG_MMU

/* tlbmisc register bits */
#define TLBMISC_WE	(1<<18)	/* TLB write enable */
#define TLBMISC_RD	(1<<19)	/* TLB read */

#endif /* CONFIG_MMU */

#endif /* _ASM_NIOS2_REGISTERS_H */
