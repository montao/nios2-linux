/*
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_SETUP_H
#define _ASM_NIOS2_SETUP_H

#include <asm-generic/setup.h>

#ifndef __ASSEMBLY__
# ifdef __KERNEL__

extern char cmd_line[COMMAND_LINE_SIZE];

extern char exception_handler_hook[];
extern char fast_handler[];
extern char fast_handler_end[];

extern void pagetable_init(void);

extern void setup_early_printk(void);

# endif/* __KERNEL__ */
#endif /* __ASSEMBLY__ */

#endif /* _ASM_NIOS2_SETUP_H */
