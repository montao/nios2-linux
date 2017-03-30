/*
 * Based on arch/mips/include/asm/prom.h which is:
 *
 * Copyright (C) 2010 Cisco Systems Inc. <dediao@cisco.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_PROM_H
#define _ASM_NIOS2_PROM_H

extern unsigned long early_altera_uart_or_juart_console(void);

extern void device_tree_init(void);

#endif /* _ASM_NIOS2_PROM_H */
