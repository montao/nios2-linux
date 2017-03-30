/* Define the machine-dependent type `jmp_buf'.  Nios II version.
   Copyright (C) 1992,1993,1995,1997,2000,2002,2003,2004,2005,2006,2008
	Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _NIOS2_BITS_SETJMP_H
#define _NIOS2_BITS_SETJMP_H 1

#if !defined(_SETJMP_H) && !defined(_PTHREAD_H)
# error "Never include <bits/setjmp.h> directly; use <setjmp.h> instead."
#endif

typedef struct
  {
    /* r16,r17,r18,r19,r20,r21,r22,r23,sp,fp,ra */
    int __regs[11];

  } __jmp_buf[1];

#endif /* _NIOS2_BITS_SETJMP_H */
