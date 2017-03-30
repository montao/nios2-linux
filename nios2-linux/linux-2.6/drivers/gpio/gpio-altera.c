/*
 * Altera GPIO driver
 *
 * Copyright (C) 2012 Tobias Klauser <tklauser@distanz.ch>
 * Copyright (C) 2011 Thomas Chou <thomas@wytron.com.tw>
 *
 * Based on Xilinx gpio driver, which is
 * Copyright 2008 Xilinx, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#define DRV_NAME "altera_gpio"

/* Register Offset Definitions */
#define ALTERA_GPIO_DATA_OFFSET		0x0	/* Data register  */
#define ALTERA_GPIO_DIR_OFFSET		0x4	/* I/O direction register  */
#define ALTERA_GPIO_IRQ_MASK		0x8
#define ALTERA_GPIO_EDGE_CAP		0xc

struct altera_gpio_instance {
	struct of_mm_gpio_chip mmchip;
	u32 gpio_state;		/* GPIO state shadow register */
	u32 gpio_dir;		/* GPIO direction shadow register */
	int irq;		/* GPIO controller IRQ number */
	int irq_base;		/* base number for the "virtual" GPIO IRQs */
	u32 irq_mask;		/* IRQ mask */
	spinlock_t gpio_lock;	/* Lock used for synchronization */
};

static inline struct altera_gpio_instance *to_altera_gpio(
	struct of_mm_gpio_chip *mm_gc)
{
	return container_of(mm_gc, struct altera_gpio_instance, mmchip);
}

/*
 * altera_gpio_get - Read the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * This function reads the specified signal of the GPIO device. It returns 0 if
 * the signal clear, 1 if signal is set or negative value on error.
 */
static int altera_gpio_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct altera_gpio_instance *chip = to_altera_gpio(mm_gc);

	if (chip->irq >= 0) {
		if (readl(mm_gc->regs + ALTERA_GPIO_EDGE_CAP) & (1 << gpio))
			writel(1 << gpio, mm_gc->regs + ALTERA_GPIO_EDGE_CAP);
	}
	return (readl(mm_gc->regs + ALTERA_GPIO_DATA_OFFSET) >> gpio) & 1;
}

/*
 * altera_gpio_set - Write the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function writes the specified value in to the specified signal of the
 * GPIO device.
 */
static void altera_gpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct altera_gpio_instance *chip = to_altera_gpio(mm_gc);

	spin_lock_irqsave(&chip->gpio_lock, flags);

	/* Write to shadow register and output */
	if (val)
		chip->gpio_state |= 1 << gpio;
	else
		chip->gpio_state &= ~(1 << gpio);
	writel(chip->gpio_state, mm_gc->regs + ALTERA_GPIO_DATA_OFFSET);

	spin_unlock_irqrestore(&chip->gpio_lock, flags);
}

/*
 * altera_gpio_dir_in - Set the direction of the specified GPIO signal as input.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * This function sets the direction of specified GPIO signal as input.
 * It returns 0 if direction of GPIO signals is set as input otherwise it
 * returns negative error value.
 */
static int altera_gpio_dir_in(struct gpio_chip *gc, unsigned int gpio)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct altera_gpio_instance *chip = to_altera_gpio(mm_gc);

	spin_lock_irqsave(&chip->gpio_lock, flags);

	/* Clear the GPIO bit in shadow register and set direction as input */
	chip->gpio_dir &= ~(1 << gpio);
	writel(chip->gpio_dir, mm_gc->regs + ALTERA_GPIO_DIR_OFFSET);

	spin_unlock_irqrestore(&chip->gpio_lock, flags);

	return 0;
}

/*
 * altera_gpio_dir_out - Set the direction of the specified GPIO as output.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function sets the direction of specified GPIO signal as output. If all
 * GPIO signals of GPIO chip is configured as input then it returns
 * error otherwise it returns 0.
 */
static int altera_gpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	unsigned long flags;
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct altera_gpio_instance *chip = to_altera_gpio(mm_gc);

	spin_lock_irqsave(&chip->gpio_lock, flags);

	/* Write state of GPIO signal */
	if (val)
		chip->gpio_state |= 1 << gpio;
	else
		chip->gpio_state &= ~(1 << gpio);
	writel(chip->gpio_state, mm_gc->regs + ALTERA_GPIO_DATA_OFFSET);

	/* Set the GPIO bit in shadow register and set direction as output */
	chip->gpio_dir |= (1 << gpio);
	writel(chip->gpio_dir, mm_gc->regs + ALTERA_GPIO_DIR_OFFSET);

	spin_unlock_irqrestore(&chip->gpio_lock, flags);

	return 0;
}

/*
 * altera_gpio_save_regs - Set initial values of GPIO pins
 * @mm_gc: pointer to memory mapped GPIO chip structure
 */
static void altera_gpio_save_regs(struct of_mm_gpio_chip *mm_gc)
{
	struct altera_gpio_instance *chip = to_altera_gpio(mm_gc);

	writel(chip->gpio_state, mm_gc->regs + ALTERA_GPIO_DATA_OFFSET);
	writel(chip->gpio_dir, mm_gc->regs + ALTERA_GPIO_DIR_OFFSET);
}

static int altera_gpio_to_irq(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct altera_gpio_instance *chip = to_altera_gpio(mm_gc);

	if (chip->irq_base < 0)
		return -EINVAL;

	return chip->irq_base + gpio;
}

/*
 * GPIO IRQ
 */

static void altera_gpio_irq_mask(struct irq_data *d)
{
	struct altera_gpio_instance *chip = irq_data_get_irq_chip_data(d);
	int offset = d->irq - chip->irq_base;
	unsigned long flags;

	spin_lock_irqsave(&chip->gpio_lock, flags);
	chip->irq_mask &= ~(1 << offset);
	writel(chip->irq_mask, chip->mmchip.regs + ALTERA_GPIO_IRQ_MASK);
	spin_unlock_irqrestore(&chip->gpio_lock, flags);
}

static void altera_gpio_irq_unmask(struct irq_data *d)
{
	struct altera_gpio_instance *chip = irq_data_get_irq_chip_data(d);
	int offset = d->irq - chip->irq_base;
	unsigned long flags;

	spin_lock_irqsave(&chip->gpio_lock, flags);
	chip->irq_mask |= 1 << offset;
	writel(chip->irq_mask, chip->mmchip.regs + ALTERA_GPIO_IRQ_MASK);
	spin_unlock_irqrestore(&chip->gpio_lock, flags);
}

static int altera_gpio_irq_set_type(struct irq_data *d, unsigned type)
{
	/* TODO: Do we need to check type here? */
	return 0;
}

static struct irq_chip altera_gpio_irq_chip = {
	.name		= "altera_gpio",
	.irq_mask	= altera_gpio_irq_mask,
	.irq_unmask	= altera_gpio_irq_unmask,
	.irq_set_type	= altera_gpio_irq_set_type,
};

static void altera_gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	struct altera_gpio_instance *chip = irq_get_handler_data(irq);
	unsigned long edgecap;
	unsigned int offset;

	/* disable the IRQ temporarily */
	writel(0, chip->mmchip.regs + ALTERA_GPIO_IRQ_MASK);

	/* Reset edge capture register */
	edgecap = readl(chip->mmchip.regs + ALTERA_GPIO_EDGE_CAP);
	writel(edgecap, chip->mmchip.regs + ALTERA_GPIO_EDGE_CAP);

	for_each_set_bit(offset, &edgecap, chip->mmchip.gc.ngpio)
		generic_handle_irq(altera_gpio_to_irq(&chip->mmchip.gc, offset));

	/* reenable the IRQ */
	writel(chip->irq_mask, chip->mmchip.regs + ALTERA_GPIO_IRQ_MASK);
}

/*
 * altera_gpio_of_probe - Probe method for the GPIO device.
 * @np: pointer to device tree node
 *
 * This function probes the GPIO device in the device tree. It initializes the
 * driver data structure. It returns 0, if the driver is bound to the GPIO
 * device, or a negative value if there is an error.
 */
static int __devinit altera_gpio_of_probe(struct device_node *np)
{
	struct altera_gpio_instance *chip;
	int status = 0;
	u32 reg;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	/* Update GPIO state shadow register with default value */
	if (of_property_read_u32(np, "resetvalue", &reg) == 0)
		chip->gpio_state = reg;

	/* Update GPIO direction shadow register with default value */
	chip->gpio_dir = 0; /* By default, all pins are inputs */

	/* Check device node for device width */
	if (of_property_read_u32(np, "width", &reg) == 0)
		chip->mmchip.gc.ngpio = reg;
	else
		chip->mmchip.gc.ngpio = 32; /* By default assume full GPIO controller */

	/* Check device node for interrupt */
	chip->irq = of_irq_count(np) ? of_irq_to_resource(np, 0, NULL) : -ENXIO;
	if (of_irq_count(np)) {
		chip->irq_base = irq_alloc_descs(-1, 0, chip->mmchip.gc.ngpio, 0);
		if (chip->irq_base < 0) {
			kfree(chip);
			pr_err("%s: couldn't allocate IRQ numbers\n", np->full_name);
			return -ENODEV;
		}

		chip->irq = of_irq_to_resource(np, 0, NULL);
		chip->mmchip.gc.to_irq = altera_gpio_to_irq;
	} else {
		chip->irq = -1;
		chip->irq_base = -1;
		chip->mmchip.gc.to_irq = NULL;
	}

	spin_lock_init(&chip->gpio_lock);

	chip->mmchip.gc.direction_input = altera_gpio_dir_in;
	chip->mmchip.gc.direction_output = altera_gpio_dir_out;
	chip->mmchip.gc.get = altera_gpio_get;
	chip->mmchip.gc.set = altera_gpio_set;

	chip->mmchip.save_regs = altera_gpio_save_regs;

	/* Call the OF gpio helper to setup and register the GPIO device */
	status = of_mm_gpiochip_add(np, &chip->mmchip);
	if (status) {
		kfree(chip);
		pr_err("%s: error in probe function with status %d\n",
		       np->full_name, status);
		return status;
	}
	if (chip->irq >= 0) {
		unsigned int i;

		/* clear edge and disable all IRQs */
		writel(0, chip->mmchip.regs + ALTERA_GPIO_EDGE_CAP);
		writel(0, chip->mmchip.regs + ALTERA_GPIO_IRQ_MASK);

		for (i = 0; i < chip->mmchip.gc.ngpio; i++) {
			irq_set_chip_and_handler_name(chip->irq_base + i,
					&altera_gpio_irq_chip, handle_simple_irq, "demux");
			irq_set_chip_data(chip->irq_base + i, chip);
		}

		irq_set_handler_data(chip->irq, chip);
		irq_set_chained_handler(chip->irq, altera_gpio_irq_handler);
	}

	return 0;
}

static struct of_device_id altera_gpio_of_match[] __devinitdata = {
	{ .compatible = "ALTR,pio-1.0", },
	{},
};

/* Make sure we get initialized before anyone else tries to use us */
void __init altera_gpio_init(void)
{
	struct device_node *np;

	for_each_matching_node(np, altera_gpio_of_match)
		altera_gpio_of_probe(np);
}

MODULE_DESCRIPTION("Altera GPIO driver");
MODULE_AUTHOR("Thomas Chou <thomas@wytron.com.tw>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
