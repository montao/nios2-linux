/*
 * example code for playing with kernel timers
 *
 * Copyright 2007-2009 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#define DEBUG
#define pr_fmt(fmt) "timer_test: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/types.h>

static struct timer_list timer_test_1, timer_test_2;

static ulong delay = 5;
module_param(delay, ulong, 0);
MODULE_PARM_DESC(delay, "number of seconds to delay before firing; default = 5 seconds");

static void timer_test_func(unsigned long data)
{
	pr_debug("timer_test_func: here i am with my data '%li'!\n", data);
}

static int __init timer_test_init(void)
{
	int ret;
	pr_debug("timer module init\n");

	/* These two methods for setting up a timer are equivalent.
	 * Depending on your code, it may be easier to do this in
	 * steps or all at once.
	 */

	pr_debug("arming timer 1 to fire %lu seconds from now\n", delay);
	setup_timer(&timer_test_1, timer_test_func, 1234);
	ret = mod_timer(&timer_test_1, jiffies + msecs_to_jiffies(delay * 1000));
	if (ret)
		pr_debug("mod_timer() returned %i!  that's not good!\n", ret);

	pr_debug("arming timer 2 to fire %lu seconds from now\n", delay*2);
	init_timer(&timer_test_2);
	timer_test_2.function = timer_test_func;
	timer_test_2.data = 9876;
	timer_test_2.expires = jiffies + msecs_to_jiffies(delay * 2 * 1000);
	add_timer(&timer_test_2);

	return 0;
}
module_init(timer_test_init);

static void __exit timer_test_cleanup(void)
{
	pr_debug("timer module cleanup\n");
	if (del_timer(&timer_test_1))
		pr_debug("timer 1 is still in use!\n");
	if (del_timer(&timer_test_2))
		pr_debug("timer 2 is still in use!\n");
}
module_exit(timer_test_cleanup);

MODULE_DESCRIPTION("example kernel timer driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mike Frysinger <vapier@gentoo.org>");
