/*
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_IRQ_H
#define _ASM_NIOS2_IRQ_H

#define NIOS2_CPU_NR_IRQS	32
/* Reserve 32 additional interrupts for GPIO IRQs */
#define NR_IRQS			(NIOS2_CPU_NR_IRQS + 32)

#include <asm-generic/irq.h>
#include <linux/irqdomain.h>

#endif
