/*
 * File:         drivers/char/bfin_timer_latency.c
 * Based on:
 * Author:       Luke Yang
 *
 * Created:
 * Description:  Simple driver for testing interrupt latencies.
 *
 * Modified:
 *               Copyright 2005-2006 Analog Devices Inc.
 *
 * Bugs:         Enter bugs at http://blackfin.uclinux.org/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see the file COPYING, or write
 * to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/blackfin.h>
#include <asm/irq.h>

#undef DEBUG

#ifdef DEBUG
# define DPRINTK(x...)	printk(KERN_DEBUG x)
#else
# define DPRINTK(x...)	do { } while (0)
#endif

#define WARM_CNT 0

struct timer_latency_data_t {
	char value;
	unsigned long worst_latency;
	unsigned long long total_latency;
	unsigned long test_number;
	unsigned int period_sclk;
	unsigned int period_ns;
};

struct proc_dir_entry *timer_latency_file;
struct timer_latency_data_t timer_latency_data;

static unsigned long long ns_start, ns_end;
static unsigned long warmup;

static int read_timer_latency(char *page, char **start,
			      off_t offset, int count, int *eof, void *data)
{
	unsigned long long average_latency = timer_latency_data.total_latency;

	if (likely(timer_latency_data.test_number > 0))
		do_div(average_latency, timer_latency_data.test_number);

	return sprintf(page,
		       "number,   worst latency(ns), average latency(ns)\n"
			"%lu,      %lu,          %llu\n",
			timer_latency_data.test_number,
			timer_latency_data.worst_latency, average_latency);
}

static int write_timer_latency(struct file *file, const char *buffer,
			       unsigned long count, void *data)
{
	unsigned long sclk;
	char user_value[8] = {'\0'};
	unsigned int wd_period_us;

	if (count > sizeof(user_value) - 1)
		return -EINVAL;

	if (copy_from_user(user_value, buffer, count) != 0)
		return -EFAULT;

	user_value[count] = '\0';

	wd_period_us = simple_strtoul(user_value, NULL, 0);

	if ((wd_period_us >= 100) && (timer_latency_data.value == 0)) {
		DPRINTK("start timer_latency: period: %d us\n", wd_period_us);
		timer_latency_data.value = 1;
		sclk = get_sclk() / 1000000;

		timer_latency_data.period_sclk = wd_period_us * sclk;
		timer_latency_data.period_ns = wd_period_us * 1000;

		timer_latency_data.worst_latency = 0;
		timer_latency_data.total_latency = 0;
		timer_latency_data.test_number = 0;
		warmup = WARM_CNT;
		
		/* set count timer cycles */
		bfin_write_WDOG_CNT(timer_latency_data.period_sclk);
		/* start WDOT timer */
		bfin_write_WDOG_CTL(0x4);
		ns_start = sched_clock();
		ns_end = 0;
	} else if ((wd_period_us == 0) && timer_latency_data.value == 1) {
		DPRINTK("stop timer_latency\n");
		timer_latency_data.value = 0;
	
		bfin_write_WDOG_CTL(0x8AD6);	/* close counter */
		bfin_write_WDOG_CTL(0x8AD6);	/* have to write it twice to disable the timer */
	}

	return count;		/* always write 1 byte */
}


static irqreturn_t timer_latency_irq(int irq, void *dev_id)
{
	struct timer_latency_data_t *data = dev_id;
	unsigned long latency = 0;

	ns_end = sched_clock();

	bfin_write_WDOG_CTL(0x8AD6);	/* close counter */
	bfin_write_WDOG_CTL(0x8AD6);	/* have to write it twice to disable the timer */

	latency = (ns_end - ns_start) - data->period_ns;	/* latency in ns */

	DPRINTK("latecy is %lu\n", latency);

	if (bfin_read_WDOG_STAT() != 0) {
		printk(KERN_ERR "timer_latency error!\n");
		return IRQ_HANDLED;
	}
	
	if (likely(warmup <= 0)) {
		if (latency > data->worst_latency)
			data->worst_latency = latency;
		data->test_number++;
		data->total_latency += latency;
	} else
		warmup--;

	/* restart watchdog timer again */
	bfin_write_WDOG_CNT(data->period_sclk);
	bfin_write_WDOG_CTL(0x4);
	ns_start = sched_clock();
	return IRQ_HANDLED;
}

static int __init timer_latency_init(void)
{
	DPRINTK("timer_latency start!\n");

	timer_latency_file = create_proc_entry("timer_latency", 0666, NULL);
	if (timer_latency_file == NULL)
		return -ENOMEM;

	/* default value is 0 (timer is stopped) */
	timer_latency_data.value = 0;
	timer_latency_data.worst_latency = 0;
	timer_latency_data.total_latency = 0;
	warmup = WARM_CNT;
	ns_start = ns_end = 0;

	timer_latency_file->data = &timer_latency_data;
	timer_latency_file->read_proc = &read_timer_latency;
	timer_latency_file->write_proc = &write_timer_latency;

	if (request_irq(IRQ_WATCH, timer_latency_irq, IRQF_DISABLED,
	                "timer_latency", &timer_latency_data)) {
		remove_proc_entry("timer_latency", NULL);
		return -EBUSY;
	}
	printk(KERN_INFO "timer_latency module loaded\n");

	return 0;		/* everything's OK */
}

static void __exit timer_latency_exit(void)
{
	remove_proc_entry("timer_latency", NULL);
	free_irq(IRQ_WATCH, NULL);
	printk(KERN_INFO "timer_latency module removed\n");
}

module_init(timer_latency_init);
module_exit(timer_latency_exit);

MODULE_AUTHOR("Luke Yang");
MODULE_DESCRIPTION("Timer Latency testing module");
MODULE_LICENSE("GPL");
