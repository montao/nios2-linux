/*
 * OpenCores bitwise GPIO driver
 *
 * http://opencores.org/project,bit_gpio
 *
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
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#define DRV_NAME "oc_bit_gpio"

#define OC_BIT_GPIO_OFFSET 4
#define OC_BIT_GPIO_OE 2

struct oc_bit_gpio_instance {
	struct of_mm_gpio_chip mmchip;
	int bidir_width;
	int total_width;
};

/*
 * oc_bit_gpio_get - Read the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * This function reads the specified signal of the GPIO device. It returns 0 if
 * the signal clear, 1 if signal is set or negative value on error.
 */
static int oc_bit_gpio_get(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);

	return readl(mm_gc->regs + (gpio * OC_BIT_GPIO_OFFSET)) & 1;
}

/*
 * oc_bit_gpio_set - Write the specified signal of the GPIO device.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function writes the specified value in to the specified signal of the
 * GPIO device.
 */
static void oc_bit_gpio_set(struct gpio_chip *gc, unsigned int gpio, int val)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	unsigned d = OC_BIT_GPIO_OE | (val ? 1 : 0);

	writel(d, mm_gc->regs + (gpio * OC_BIT_GPIO_OFFSET));
}

/*
 * oc_bit_gpio_dir_in - Set the direction of the specified GPIO signal as input.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * This function sets the direction of specified GPIO signal as input.
 * It returns 0 if direction of GPIO signals is set as input otherwise it
 * returns negative error value.
 */
static int oc_bit_gpio_dir_in(struct gpio_chip *gc, unsigned int gpio)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);

	writel(0, mm_gc->regs + (gpio * OC_BIT_GPIO_OFFSET));
	return 0;
}


/*
 * oc_bit_gpio_dir_out - Set the direction of the specified GPIO as output.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * This function sets the direction of specified GPIO signal as output. If all
 * GPIO signals of GPIO chip is configured as input then it returns
 * error otherwise it returns 0.
 */
static int oc_bit_gpio_dir_out(struct gpio_chip *gc, unsigned int gpio, int val)
{
	struct of_mm_gpio_chip *mm_gc = to_of_mm_gpio_chip(gc);
	struct oc_bit_gpio_instance *chip =
	    container_of(mm_gc, struct oc_bit_gpio_instance, mmchip);

	if (gpio < chip->bidir_width) {
		unsigned d = OC_BIT_GPIO_OE | (val ? 1 : 0);

		writel(d, mm_gc->regs + (gpio * OC_BIT_GPIO_OFFSET));
		return 0;
	} else
		return -EINVAL;
}

/*
 * oc_bit_gpio_of_probe - Probe method for the GPIO device.
 * @np: pointer to device tree node
 *
 * This function probes the GPIO device in the device tree. It initializes the
 * driver data structure. It returns 0, if the driver is bound to the GPIO
 * device, or a negative value if there is an error.
 */
static int __devinit oc_bit_gpio_of_probe(struct device_node *np)
{
	struct oc_bit_gpio_instance *chip;
	int status = 0;
	const u32 *tree_info;
	int bidir_width = 0;
	int total_width = 0;

	/* Check device node for device width */
	tree_info = of_get_property(np, "input-width", NULL);
	if (tree_info)
		total_width = be32_to_cpup(tree_info);
	tree_info = of_get_property(np, "bidir-width", NULL);
	if (tree_info)
		bidir_width = be32_to_cpup(tree_info);
	total_width += bidir_width;
	if (total_width <= 0) {
		pr_err("%s: error in probe function with width %d\n",
		       np->full_name, total_width);
		return -EINVAL;
	}

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip)
		return -ENOMEM;

	chip->mmchip.gc.direction_input = oc_bit_gpio_dir_in;
	chip->mmchip.gc.direction_output = oc_bit_gpio_dir_out;
	chip->mmchip.gc.get = oc_bit_gpio_get;
	chip->mmchip.gc.set = oc_bit_gpio_set;
	chip->mmchip.gc.ngpio = total_width;
	chip->bidir_width = bidir_width;

	/* Call the OF gpio helper to setup and register the GPIO device */
	status = of_mm_gpiochip_add(np, &chip->mmchip);
	if (status) {
		kfree(chip);
		pr_err("%s: error in probe function with status %d\n",
		       np->full_name, status);
		return status;
	}
	pr_info(DRV_NAME ": %s: registered\n", np->full_name);
	return 0;
}

static struct of_device_id oc_bit_gpio_of_match[] __devinitdata = {
	{ .compatible = "opencores,bit-gpio-rtlsvn2", },
	{},
};

/* Make sure we get initialized before anyone else tries to use us */
void __init oc_bit_gpio_init(void)
{
	struct device_node *np;

	for_each_matching_node(np, oc_bit_gpio_of_match)
		oc_bit_gpio_of_probe(np);
}

MODULE_DESCRIPTION("OpenCores bitwise GPIO driver");
MODULE_AUTHOR("Thomas Chou <thomas@wytron.com.tw>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:" DRV_NAME);
