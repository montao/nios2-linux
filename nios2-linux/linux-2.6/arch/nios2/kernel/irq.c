/*
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 * Copyright (C) 2008 Thomas Chou <thomas@wytron.com.tw>
 *
 * based on irq.c from m68k which is:
 *
 * Copyright (C) 2007 Greg Ungerer <gerg@snapgear.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/linkage.h>
#include <linux/interrupt.h>

asmlinkage void do_IRQ(int irq, struct pt_regs *regs)
{
	struct pt_regs *oldregs = set_irq_regs(regs);

	irq_enter();
	generic_handle_irq(irq);
	irq_exit();

	set_irq_regs(oldregs);
}

static void chip_unmask(struct irq_data *d)
{
	unsigned ien;
	ien = RDCTL(CTL_IENABLE);
	ien |= (1 << d->irq);
	WRCTL(CTL_IENABLE, ien);
}

static void chip_mask(struct irq_data *d)
{
	unsigned ien;
	ien = RDCTL(CTL_IENABLE);
	ien &= ~(1 << d->irq);
	WRCTL(CTL_IENABLE, ien);
}

static struct irq_chip m_irq_chip = {
	.name		= "NIOS2-INTC",
	.irq_unmask	= chip_unmask,
	.irq_mask	= chip_mask,
};

void __init init_IRQ(void)
{
	int irq;

	for (irq = 0; irq < NIOS2_CPU_NR_IRQS; irq++)
		irq_set_chip_and_handler(irq, &m_irq_chip, handle_level_irq);
}
