/*
 * Copyright (C) 2010 Tobias Klauser <tklauser@distanz.ch>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_TLBFLUSH_H
#define _ASM_NIOS2_TLBFLUSH_H

#ifndef CONFIG_MMU
# include <asm-generic/tlbflush.h>
#else

struct mm_struct;

/*
 * TLB flushing:
 *
 *  - flush_tlb_all() flushes all processes TLB entries
 *  - flush_tlb_mm(mm) flushes the specified mm context TLB entries
 *  - flush_tlb_page(vma, vmaddr) flushes one page
 *  - flush_tlb_range(vma, start, end) flushes a range of pages
 *  - flush_tlb_kernel_range(start, end) flushes a range of kernel pages
 */
extern void flush_tlb_all(void);
extern void flush_tlb_mm(struct mm_struct *mm);
extern void flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
			    unsigned long end);
extern void flush_tlb_kernel_range(unsigned long start, unsigned long end);
extern void flush_tlb_one(unsigned long vaddr);

static inline void flush_tlb_page(struct vm_area_struct *vma, unsigned long addr)
{
	flush_tlb_one(addr);
}

#endif /* CONFIG_MMU */

#endif /* _ASM_NIOS2_TLBFLUSH_H */
