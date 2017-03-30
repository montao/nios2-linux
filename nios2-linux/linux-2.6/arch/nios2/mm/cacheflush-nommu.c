/*
 * Copyright (C) 2004 Microtronix Datacom Ltd.
 *
 * based on arch/m68k/mm/memory.c which is:
 *
 * Copyright (C) 1999-2002 Greg Ungerer <gerg@snapgear.com>
 * Copyright (C) 1998 Kenneth Albanowski <kjahds@kjahds.com>
 * Copyright (C) 1995 Hamish Macdonald
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <asm/setup.h>
#include <asm/segment.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/traps.h>
#include <asm/io.h>
#include <asm/cpuinfo.h>

/*
 *	Define cache invalidate functions. The instruction and data cache
 *	will need to be flushed. Write back the dirty data cache and invalidate
 *	the instruction cache for the range.
 *
 */
static __inline__ void cache_invalidate_inst(unsigned long paddr, int len)
{
	if (len >= cpuinfo.icache_size) {
		__asm__ __volatile__("1:\n\t"
				     "flushi	%0\n\t"
				     "sub	%0,%0,%1\n\t"
				     "bgt	%0,r0,1b\n\t"
				     "flushp\n\t"
				     :
				     : "r" (cpuinfo.icache_size),
				       "r" (cpuinfo.icache_line_size));
	} else {
		unsigned long sset, eset;

		sset =
		    (paddr & (cpuinfo.icache_size - 1)) &
		    (~(cpuinfo.icache_line_size - 1));
		eset =
		    (((paddr & (cpuinfo.icache_size - 1)) + len) &
		     (~(cpuinfo.icache_line_size - 1))) + cpuinfo.icache_line_size;

		__asm__ __volatile__("1:\n\t"
				     "flushi	%0\n\t"
				     "add	%0,%0,%2\n\t"
				     "blt	%0,%1,1b\n\t"
				     "flushp\n\t"
				     :
				     : "r" (sset),
				       "r" (eset),
				       "r" (cpuinfo.icache_line_size));
	}
}

static __inline__ void cache_invalidate_data(unsigned long paddr, int len)
{
	if (len >= cpuinfo.dcache_size * 2) {
		__asm__ __volatile__("1:\n\t"
				     "initd	0(%0)\n\t"
				     "sub	%0,%0,%1\n\t"
				     "bgt	%0,r0,1b\n\t"
				     :
				     : "r" (cpuinfo.dcache_size),
				       "r" (cpuinfo.dcache_line_size));

	} else {
		unsigned long sset, eset;

		sset = paddr & (~(cpuinfo.dcache_line_size - 1));
		eset =
		    (paddr + len + cpuinfo.dcache_line_size - 1) &
		    (~(cpuinfo.dcache_line_size - 1));

		__asm__ __volatile__("1:\n\t"
				     "initda	0(%0)\n\t"
				     "add	%0,%0,%2\n\t"
				     "blt	%0,%1,1b\n\t"
				     :
				     : "r" (sset),
				       "r" (eset),
				       "r"(cpuinfo.dcache_line_size));
	}
}

static __inline__ void cache_push_invalidate_data(unsigned long paddr, int len)
{
	if (len >= cpuinfo.dcache_size * 2) {
		__asm__ __volatile__("1:\n\t"
				     "flushd	0(%0)\n\t"
				     "sub	%0,%0,%1\n\t"
				     "bgt	%0,r0,1b\n\t"
				     :
				     : "r" (cpuinfo.dcache_size),
				       "r" (cpuinfo.dcache_line_size));

	} else {
		unsigned long sset, eset;

		sset = paddr & (~(cpuinfo.dcache_line_size - 1));
		eset =
		    (paddr + len + cpuinfo.dcache_line_size - 1) &
		    (~(cpuinfo.dcache_line_size - 1));

		__asm__ __volatile__("1:\n\t"
				     "flushda	0(%0)\n\t"
				     "add	%0,%0,%2\n\t"
				     "blt	%0,%1,1b\n\t"
				     :
				     : "r" (sset),
				       "r" (eset),
				       "r"(cpuinfo.dcache_line_size));
	}
}

/*
 * cache_push() semantics: Write back any dirty cache data in the given area,
 * and invalidate the range in the instruction cache. It needs not (but may)
 * invalidate those entries also in the data cache. The range is defined by a
 * _physical_ address.
 */

void cache_push(unsigned long paddr, int len)
{
	cache_push_invalidate_data(paddr, len);
	cache_invalidate_inst(paddr, len);
}

void dcache_push_all(void)
{
	__asm__ __volatile__("1:\n\t"
			     "flushd	0(%0)\n\t"
			     "sub	%0,%0,%1\n\t"
			     "bgt	%0,r0,1b\n\t"
			     :
			     : "r" (cpuinfo.dcache_size),
			       "r" (cpuinfo.dcache_line_size));
}

void icache_push_all(void)
{
	__asm__ __volatile__("1:\n\t"
			     "flushi	%0\n\t"
			     "sub	%0,%0,%1\n\t"
			     "bgt	%0,r0,1b\n\t"
			     "flushp\n\t"
			     :
			     : "r" (cpuinfo.icache_size),
			       "r" (cpuinfo.icache_line_size));
}

/*
 * dcache_push() semantics: Write back and dirty data cache and invalidate
 * the range.
 */
void dcache_push(unsigned long vaddr, int len)
{
	cache_push_invalidate_data(vaddr, len);
}
EXPORT_SYMBOL(dcache_push);

/*
 * icache_push() semantics: Invalidate instruction cache in the range.
 * Need to write back dirty data cache lines first.  As a side-effect,
 * this also invalidates the affected data lines.
 */
void icache_push(unsigned long vaddr, int len)
{
	cache_push_invalidate_data(vaddr, len);
	cache_invalidate_inst(vaddr, len);
}
