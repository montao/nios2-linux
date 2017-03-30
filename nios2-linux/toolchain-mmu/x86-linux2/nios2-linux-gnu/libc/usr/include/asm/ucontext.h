#ifndef _ASM_NIOS2_UCONTEXT_H
#define _ASM_NIOS2_UCONTEXT_H

/*--------------------------------------------------------------------
 *
 * include/asm-nios2/ucontext.h
 *
 * Derived from M68knommu
 *
 * Copyright (C) 2004   Microtronix Datacom Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * Jan/20/2004		dgt	    NiosII
 *
 ---------------------------------------------------------------------*/


typedef int greg_t;
#define NGREG 32
typedef greg_t gregset_t[NGREG];

struct mcontext {
	int version;
	gregset_t gregs;
};

#define MCONTEXT_VERSION 2

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	struct mcontext	  uc_mcontext;
	sigset_t	  uc_sigmask;	/* mask last for extensibility */
};

#endif
