/*
 * test anomaly 05000310 workaround
 *
 * anomaly 05000310: Fetches at the boundary of either reserved memory or L1 Instruction
 * cache memory (if instruction cache enabled) which is covered by a valid CPLB cause a
 * false Hardware Error (External Memory Addressing Error).
 *
 * Copyright 2007-2009 Analog Devices Inc.
 * Licensed under the GPL-2 or later.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <mach/mem_map.h>
#include <asm/dma.h>

static void boundary_anomaly_test_func(void)
{
	volatile int i;
	volatile val;
	/* some branch instructions */
	for (i = 0; i < 10; i++)
	{
		val = i;
		switch (val)
		{
		case 1:
			val = 2;
			break;
		case 2:
			val = 1;
			break;
		default:
			val = 0;
			break;
		}
	}

	printk("boundary_anomaly_test: running at the boundary\n");
	val = 3;
}

static int __init boundary_anomaly_test_init(void)
{
	unsigned long boundary_addr = L1_CODE_START + L1_CODE_LENGTH - 120;
	void (*func)(void) = boundary_addr;
	
	/* 
	 * instruction fetch at the boundary: work normally 
	 */
	dma_memcpy(boundary_addr, boundary_anomaly_test_func, 120);
	
	printk("-----test 1: fetch instructions at the boundary-------\n");
	
	printk("boundary_anomaly_test: jump to boundary %p\n", func);
	func();
	printk("boundary_anomaly_test: exit from boundary %p\n", func);

	printk("-----test 1: pass-------\n\n");
	
	/* 
	 * real external hardware error: panic and oops expected 
	 */
	printk("-----test 2: trigger a real external hardware error-------\n");
	*(unsigned int *)boundary_addr = 0;
	printk("-----test 2: fail-------\n");
	
	return 0;
}
module_init(boundary_anomaly_test_init);

static void __exit boundary_anomaly_test_cleanup(void)
{
}
module_exit(boundary_anomaly_test_cleanup);

MODULE_DESCRIPTION("boundary anomaly test module");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Barry Song <21cnbao@gmail.com>");
