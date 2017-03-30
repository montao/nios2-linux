/*
 * cycles.c - example module for playing with the cycle counters
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Copyright 2006-2009 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#define pr_fmt(fmt) "cycles: " fmt

#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mike Frysinger");
MODULE_DESCRIPTION("Example Blackfin cycles code");

/*
 * Return the 64bit cycle counter -- better to use the get_cycles()
 * from the common Blackfin header (timex.h) rather than defining your
 * own function.  But we show you how here for info.
 */
#if 1
# define cycles_get() (unsigned long long)get_cycles()
#else
static inline unsigned long long cycles_get(void)
{
	unsigned long ret_high, ret_low;
	__asm__ __volatile__(
		"%0 = CYCLES;"
		"%1 = CYCLES2;"
		: "=d"(ret_low), "=d"(ret_high)
	);
	return ((unsigned long long)ret_high << 32) + ret_low;
}
#endif

/*
 * Reset the 64bit cycle counter to 0
 */
static inline void cycles_clear(void)
{
	__asm__ __volatile__(
		"CYCLES = %0;"
		"CYCLES2 = %0;"
		: : "d"(0)
	);
}

/*
 * Turn off the cycle counter completely
 */
static inline void cycles_turn_off(void)
{
	unsigned long scratch;
	__asm__ __volatile__(
		"%0 = SYSCFG;"
		"BITCLR(%0, 1);"
		"SYSCFG = %0;"
		: "=d"(scratch)
	);
	CSYNC();
}

/*
 * Turn on the cycle counter
 */
static inline void cycles_turn_on(void)
{
	unsigned long scratch;
	__asm__ __volatile__(
		"%0 = SYSCFG;"
		"BITSET(%0, 1);"
		"SYSCFG = %0;"
		: "=d"(scratch)
	);
	CSYNC();
}

/*
 * Some cheesy example code
 */
static int __init cycles_module_init(void)
{
	int cnt;

	pr_info("turned off:\n");
	cycles_turn_off();
	cycles_clear();
	for (cnt = 0; cnt < 10; ++cnt) {
		mdelay(100);
		pr_info(" %llu\n", cycles_get());
	}

	pr_info("turned on:\n");
	cycles_turn_on();
	for (cnt = 0; cnt < 10; ++cnt) {
		mdelay(100);
		pr_info(" %llu\n", cycles_get());
	}

	return 0;
}
module_init(cycles_module_init);

/*
 * Need this in order to unload the module
 */
static void __exit cycles_module_cleanup(void)
{
	return;
}
module_exit(cycles_module_cleanup);
