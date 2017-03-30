/*
 * Driver for the Altera Watchdog Timer
 *
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 * Copyright (C) 2005 Walter Goossens
 *
 * Originally based on wdt.c which is
 *
 * Copyright (C) 1995-1997 Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 * Software timeout heartbeat code based on pika_wdt.c which is
 *
 * Copyright (c) 2008 PIKA Technologies
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/of_platform.h>

#define WATCHDOG_NAME	"altera_wdt"

/* Register offsets */
#define ALTERA_WDT_STATUS	0x00
#define ALTERA_WDT_CONTROL	0x04
#define ALTERA_WDT_PERIODL	0x08
#define ALTERA_WDT_PERIODH	0x0C

#define ALTERA_WDT_RUN_BIT	0x04

/* User land timeout */
#define WDT_HEARTBEAT 15
static int heartbeat = WDT_HEARTBEAT;
module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeats in seconds. "
	"(default = " __MODULE_STRING(WDT_HEARTBEAT) ")");

static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

static struct {
	void __iomem *base;
	unsigned long wdt_timeout;	/* timeout of the hardware timer */

	unsigned long next_heartbeat;	/* the next_heartbeat for the timer */
	unsigned long is_open;
	char expect_close;
	struct timer_list timer;	/* software timer that pings the watchdog */
} altera_wdt_priv;

/*
 * Start the watchdog. Once it has been started, it cannot be stopped anymore.
 */
static void altera_wdt_setup(void)
{
	u32 control = readl(altera_wdt_priv.base + ALTERA_WDT_CONTROL);

	writel(control | ALTERA_WDT_RUN_BIT, altera_wdt_priv.base + ALTERA_WDT_CONTROL);
}

/*
 * Tickle the watchdog (reset the watchdog timer)
 */
static void altera_wdt_reset(void)
{
	/* It doesn't matter what value we write */
	writel(1, altera_wdt_priv.base + ALTERA_WDT_PERIODL);
}

/*
 * Software timer tick
 */
static void altera_wdt_ping(unsigned long data)
{
	if (time_before(jiffies, altera_wdt_priv.next_heartbeat) ||
			(!nowayout && !altera_wdt_priv.is_open)) {
		altera_wdt_reset();
		mod_timer(&altera_wdt_priv.timer, jiffies + altera_wdt_priv.wdt_timeout);
	} else
		pr_crit(WATCHDOG_NAME ": I will reset your machine!\n");
}

static void altera_wdt_keepalive(void)
{
	altera_wdt_priv.next_heartbeat = jiffies + heartbeat * HZ;
}

static void altera_wdt_start(void)
{
	altera_wdt_keepalive();
	mod_timer(&altera_wdt_priv.timer, jiffies + altera_wdt_priv.wdt_timeout);
}

static int altera_wdt_open(struct inode *inode, struct file *file)
{
	/* /dev/watchdog can only be opened once */
	if (test_and_set_bit(0, &altera_wdt_priv.is_open))
		return -EBUSY;

	altera_wdt_start();

	return nonseekable_open(inode, file);
}

static int altera_wdt_release(struct inode *inode, struct file *file)
{
	/* stop internal ping */
	if (!altera_wdt_priv.expect_close)
		del_timer(&altera_wdt_priv.timer);

	clear_bit(0, &altera_wdt_priv.is_open);
	altera_wdt_priv.expect_close = 0;

	return 0;
}

static ssize_t altera_wdt_write(struct file *file, const char __user *data,
		size_t len, loff_t *ppos)
{
	if (!len)
		return 0;

	/* Scan for magic character */
	if (!nowayout) {
		size_t i;

		altera_wdt_priv.expect_close = 0;

		for (i = 0; i < len; i++) {
			char c;
			if (get_user(c, data + i))
				return -EFAULT;
			if (c == 'V') {
				altera_wdt_priv.expect_close = 42;
				break;
			}
		}
	}

	altera_wdt_keepalive();

	return len;
}

static const struct watchdog_info altera_wdt_info = {
	.identity		= "Altera Watchdog",
	.options		= WDIOF_SETTIMEOUT |
				  WDIOF_KEEPALIVEPING |
				  WDIOF_MAGICCLOSE,
	.firmware_version	= 1,
};

static long altera_wdt_ioctl(struct file *file, unsigned int cmd,
		unsigned long arg)
{
	void __user *argp = (void __user *) arg;
	int __user *p = argp;
	int new_value;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &altera_wdt_info, sizeof(altera_wdt_info));

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);

	case WDIOC_KEEPALIVE:
		altera_wdt_keepalive();
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_value, p))
			return -EFAULT;

		heartbeat = new_value;
		altera_wdt_keepalive();

		return put_user(new_value, p);  /* return current value */

	case WDIOC_GETTIMEOUT:
		return put_user(heartbeat, p);

	default:
		return -ENOTTY;
	}
}

static const struct file_operations altera_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.open		= altera_wdt_open,
	.release	= altera_wdt_release,
	.write		= altera_wdt_write,
	.unlocked_ioctl	= altera_wdt_ioctl,
};

static struct miscdevice altera_wdt_miscdev = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &altera_wdt_fops,
};

static int __devinit altera_wdt_probe(struct platform_device *pdev)
{
	struct resource *res, *mem;
	const __be32 *freq_prop, *timeout_prop;
	unsigned long timeout;
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENOENT;

	mem = devm_request_mem_region(&pdev->dev, res->start,
				      resource_size(res), pdev->name);
	if (!mem)
		return -EBUSY;

	altera_wdt_priv.base = devm_ioremap_nocache(&pdev->dev, mem->start,
						    resource_size(mem));
	if (!altera_wdt_priv.base)
		return -ENOMEM;

	freq_prop = of_get_property(pdev->dev.of_node, "clock-frequency", NULL);
	if (!freq_prop)
		return -ENODEV;

	timeout_prop = of_get_property(pdev->dev.of_node, "timeout", NULL);
	if (!timeout_prop)
		return -ENODEV;

	/* Add 1 as the timeout property actually holds the load value */
	timeout = be32_to_cpup(timeout_prop) + 1;
	/* Convert timeout to msecs */
	timeout = timeout / (be32_to_cpup(freq_prop) / MSEC_PER_SEC);
	/* Tickle the watchdog twice per timeout period */
	altera_wdt_priv.wdt_timeout = msecs_to_jiffies(timeout / 2);

	/* TODO: Should we do a lower bounds check on timeout here? */

	ret = misc_register(&altera_wdt_miscdev);
	if (ret)
		return ret;

	altera_wdt_setup();
	altera_wdt_priv.next_heartbeat = jiffies + heartbeat * HZ;
	setup_timer(&altera_wdt_priv.timer, altera_wdt_ping, 0);
	mod_timer(&altera_wdt_priv.timer, jiffies + altera_wdt_priv.wdt_timeout);

	pr_info(WATCHDOG_NAME " enabled (heartbeat=%d sec, nowayout=%d)\n",
		heartbeat, nowayout);

	return 0;
}

static int __devexit altera_wdt_remove(struct platform_device *pdev)
{
	misc_deregister(&altera_wdt_miscdev);
	return 0;
}

static struct of_device_id altera_wdt_match[] = {
	{ .compatible = "ALTR,wdt-1.0", },
	{},
};
MODULE_DEVICE_TABLE(of, altera_wdt_match);

static struct platform_driver altera_wdt_driver = {
	.probe		= altera_wdt_probe,
	.remove		= __devexit_p(altera_wdt_remove),
	.driver		= {
		.owner		= THIS_MODULE,
		.name		= WATCHDOG_NAME,
		.of_match_table	= altera_wdt_match,
	},
};

static int __init altera_wdt_init(void)
{
	return platform_driver_register(&altera_wdt_driver);
}

static void __exit altera_wdt_exit(void)
{
	platform_driver_unregister(&altera_wdt_driver);
}

module_init(altera_wdt_init);
module_exit(altera_wdt_exit);

MODULE_AUTHOR("Walter Goossens, Tobias Klauser <tklauser@distanz.ch>");
MODULE_DESCRIPTION("Driver for Altera Watchdog Timer");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_ALIAS("platform:" WATCHDOG_NAME);
