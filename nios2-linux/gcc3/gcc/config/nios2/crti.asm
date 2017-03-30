/*
  Copyright (C) 2003 
 by Jonah Graham (jgraham@altera.com)

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

   As a special exception, if you link this library with files
   compiled with GCC to produce an executable, this does not cause
   the resulting executable to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.


This file just make a stack frame for the contents of the .fini and
.init sections.  Users may put any desired instructions in those
sections.


While technically any code can be put in the init and fini sections
most stuff will not work other than stuff which obeys the call frame
and ABI. All the call-preserved registers are saved, the call clobbered
registers should have been saved by the code calling init and fini.

See crtstuff.c for an example of code that inserts itself in the 
init and fini sections. 

See crt0.s for the code that calls init and fini.
*/

	.file	"crti.asm"

	.section	".init"
	.align 2
	.global	_init
_init:
	addi	sp, sp, -48
	stw	ra, 44(sp)
	stw	r23, 40(sp)
	stw	r22, 36(sp)
	stw	r21, 32(sp)
	stw	r20, 28(sp)
	stw	r19, 24(sp)
	stw	r18, 20(sp)
	stw	r17, 16(sp)
	stw	r16, 12(sp)
	stw	fp, 8(sp)
	addi	fp, sp, 8
	
	
	.section	".fini"
	.align	2
	.global	_fini
_fini:
	addi	sp, sp, -48
	stw	ra, 44(sp)
	stw	r23, 40(sp)
	stw	r22, 36(sp)
	stw	r21, 32(sp)
	stw	r20, 28(sp)
	stw	r19, 24(sp)
	stw	r18, 20(sp)
	stw	r17, 16(sp)
	stw	r16, 12(sp)
	stw	fp, 8(sp)
	addi	fp, sp, 8
	

