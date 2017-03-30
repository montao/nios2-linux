#ifndef _ASM_NIOS2_USER_H
#define _ASM_NIOS2_USER_H

/*--------------------------------------------------------------------
 *
 * include/asm-nios2/user.h
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


#include <asm/page.h>

/* Core file format: The core file is written in such a way that gdb
   can understand it and provide useful information to the user (under
   linux we use the 'trad-core' bfd).  There are quite a number of
   obstacles to being able to view the contents of the floating point
   registers, and until these are solved you will not be able to view the
   contents of them.  Actually, you can read in the core file and look at
   the contents of the user struct to find out what the floating point
   registers contain.
   The actual file contents are as follows:
   UPAGE: 1 page consisting of a user struct that tells gdb what is present
   in the file.  Directly after this is a copy of the task_struct, which
   is currently not used by gdb, but it may come in useful at some point.
   All of the registers are stored as part of the upage.  The upage should
   always be only one page.
   DATA: The data area is stored.  We use current->end_text to
   current->brk to pick up all of the user variables, plus any memory
   that may have been malloced.  No attempt is made to determine if a page
   is demand-zero or if a page is totally unused, we just cover the entire
   range.  All of the addresses are rounded in such a way that an integral
   number of pages is written.
   STACK: We need the stack information in order to get a meaningful
   backtrace.  We need to write the data from (esp) to
   current->start_stack, so we round each of these off in order to be able
   to write an integer number of pages.
   The minimum core file size is 3 pages, or 12288 bytes.
*/

struct user_m68kfp_struct {
	unsigned long  fpregs[8*3];	/* fp0-fp7 registers */
	unsigned long  fpcntl[3];	/* fp control regs */
};

/* This is needs more work, probably should look like gdb useage */
struct user_regs_struct {
	unsigned long  r8;		/* r8-r15 Caller-saved GP registers */
	unsigned long  r9;
	unsigned long  r10;
	unsigned long  r11;
	unsigned long  r12;
	unsigned long  r13;
	unsigned long  r14;
	unsigned long  r15;
	unsigned long  r1;		/* Assembler temporary */
	unsigned long  r2;		/* Retval LS 32bits */
	unsigned long  r3;		/* Retval MS 32bits */
	unsigned long  r4;		/* r4-r7 Register arguments */
	unsigned long  r5;
	unsigned long  r6;
	unsigned long  r7;
	unsigned long  orig_r2;		/* Copy of r2 ?? */
	unsigned long  ra;		/* Return address */
	unsigned long  fp;		/* Frame pointer */
	unsigned long  sp;		/* Stack pointer */
	unsigned long  gp;		/* Global pointer */
	unsigned long  estatus;
	unsigned long  ea;		/* Exception return address (pc) */
	unsigned long  orig_r7;

	unsigned long  r16;		/* r16-r23 Callee-saved GP registers */
	unsigned long  r17;
	unsigned long  r18;
	unsigned long  r19;
	unsigned long  r20;
	unsigned long  r21;
	unsigned long  r22;
	unsigned long  r23;
	unsigned long  sw_fp;
	unsigned long  sw_gp;
	unsigned long  sw_ra;
};

	
/* When the kernel dumps core, it starts by dumping the user struct -
   this will be used by gdb to figure out where the data and stack segments
   are within the file, and what virtual addresses to use. */
struct user{
/* We start with the registers, to mimic the way that "memory" is returned
   from the ptrace(3,...) function.  */
  struct user_regs_struct regs;	/* Where the registers are actually stored */
/* ptrace does not yet supply these.  Someday.... */
  int u_fpvalid;		/* True if math co-processor being used. */
                                /* for this mess. Not yet used. */
  struct user_m68kfp_struct m68kfp; /* Math Co-processor registers. */
/* The rest of this junk is to help gdb figure out what goes where */
  unsigned long int u_tsize;	/* Text segment size (pages). */
  unsigned long int u_dsize;	/* Data segment size (pages). */
  unsigned long int u_ssize;	/* Stack segment size (pages). */
  unsigned long start_code;     /* Starting virtual address of text. */
  unsigned long start_stack;	/* Starting virtual address of stack area.
				   This is actually the bottom of the stack,
				   the top of the stack is always found in the
				   esp register.  */
  long int signal;     		/* Signal that caused the core dump. */
  int reserved;			/* No longer used */
  struct user_regs_struct *u_ar0;
				/* Used by gdb to help find the values for */
				/* the registers. */
  struct user_m68kfp_struct* u_fpstate;	/* Math Co-processor pointer. */
  unsigned long magic;		/* To uniquely identify a core file */
  char u_comm[32];		/* User command that was responsible */
};
#define NBPG PAGE_SIZE
#define UPAGES 1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR (u.start_stack + u.u_ssize * NBPG)

#endif  /* _ASM_NIOS2_USER_H */
