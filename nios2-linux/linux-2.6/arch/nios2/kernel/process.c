/*
 * Architecture-dependent parts of process handling.
 *
 * Copyright (C) 2010 Tobias Klauser <tklauser@distanz.ch>
 * Copyright (C) 2009 Wind River Systems Inc
 *   Implemented by fredrik.markstrom@gmail.com and ivarholmqvist@gmail.com
 * Copyright (C) 2004 Microtronix Datacom Ltd
 *
 * based on arch/m68knommu/kernel/process.c which is:
 *
 * Copyright (C) 2000-2002 David McCullough <davidm@snapgear.com>
 * Copyright (C) 1995 Hamish Macdonald
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/export.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/tick.h>
#include <linux/fs.h>

#include <asm/unistd.h>
#include <asm/traps.h>
#include <asm/cacheflush.h>
#include <asm/cpuinfo.h>

asmlinkage void ret_from_fork(void);

/*
 * The following aren't currently used.
 */
void (*pm_idle)(void) = NULL;
EXPORT_SYMBOL(pm_idle);

void (*pm_power_off)(void) = NULL;
EXPORT_SYMBOL(pm_power_off);

void default_idle(void)
{
	local_irq_disable();
	if (!need_resched()) {
		local_irq_enable();
		__asm__("nop");
	} else
		local_irq_enable();
}

void (*idle)(void) = default_idle;

/*
 * The idle thread. There's no useful work to be
 * done, so just try to conserve power and have a
 * low exit latency (ie sit in a loop waiting for
 * somebody to say that they'd like to reschedule)
 */
void cpu_idle(void)
{
	while (1) {
		tick_nohz_idle_enter();
		rcu_idle_enter();
		while (!need_resched())
			idle();
		rcu_idle_exit();
		tick_nohz_idle_exit();

		preempt_enable_no_resched();
		schedule();
		preempt_disable();
	}
}

/*
 * The development boards have no way to pull a board reset. Just jump to the
 * cpu reset address and let the boot loader or the code in head.S take care of
 * resetting peripherals.
 */
void machine_restart(char * __unused)
{
	printk(KERN_NOTICE "Machine restart (%08x)...\n", cpuinfo.reset_addr);
	local_irq_disable();
	__asm__ __volatile__ (
	"jmp	%0\n\t"
	:
	: "r" (cpuinfo.reset_addr)
	: "r4");
}

void machine_halt(void)
{
	printk(KERN_NOTICE "Machine halt...\n");
	local_irq_disable();
	for (;;);
}

/*
 * There is no way to power off the development boards. So just spin for now. If
 * we ever have a way of resetting a board using a GPIO we should add that here.
 */
void machine_power_off(void)
{
	printk(KERN_NOTICE "Machine power off...\n");
	local_irq_disable();
	for (;;);
}

void show_regs(struct pt_regs *regs)
{
	printk(KERN_NOTICE "\n");

	printk(KERN_NOTICE "r1:  %08lx r2:  %08lx r3:  %08lx r4:  %08lx\n",
	       regs->r1,  regs->r2,  regs->r3,  regs->r4);

	printk(KERN_NOTICE "r5:  %08lx r6:  %08lx r7:  %08lx r8:  %08lx\n",
	       regs->r5,  regs->r6,  regs->r7,  regs->r8);

	printk(KERN_NOTICE "r9:  %08lx r10: %08lx r11: %08lx r12: %08lx\n",
	       regs->r9,  regs->r10, regs->r11, regs->r12);

	printk(KERN_NOTICE "r13: %08lx r14: %08lx r15: %08lx\n",
	       regs->r13, regs->r14, regs->r15);

	printk(KERN_NOTICE "ra:  %08lx fp:  %08lx sp:  %08lx gp:  %08lx\n",
	       regs->ra,  regs->fp,  regs->sp,  regs->gp);

	printk(KERN_NOTICE "ea:  %08lx estatus:  %08lx\n",
	       regs->ea,  regs->estatus);
#ifndef CONFIG_MMU
	printk(KERN_NOTICE "status_extension: %08lx\n", regs->status_extension);
#endif
}

#ifdef CONFIG_MMU
static void kernel_thread_helper(void *arg, int (*fn)(void *))
{
	do_exit(fn(arg));
}
#endif

/*
 * Create a kernel thread
 */
int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
#ifdef CONFIG_MMU
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));
	regs.r4 = (unsigned long) arg;
	regs.r5 = (unsigned long) fn;
	regs.ea = (unsigned long) kernel_thread_helper;
	regs.estatus = STATUS_PIE;

	return do_fork(flags | CLONE_VM | CLONE_UNTRACED, 0, &regs, 0, NULL, NULL);
#else /* !CONFIG_MMU */
	long retval;
	long clone_arg = flags | CLONE_VM;
	mm_segment_t fs;

	fs = get_fs();
	set_fs(KERNEL_DS);

	__asm__ __volatile(
		"movi    r2,%6\n\t"		/* TRAP_ID_SYSCALL          */
		"movi    r3,%1\n\t"		/* __NR_clone               */
		"mov     r4,%5\n\t"		/* (clone_arg               */
						/*   (flags | CLONE_VM))    */
		"movia   r5,-1\n\t"		/* usp: -1                  */
		"trap\n\t"			/* sys_clone                */
		"\n\t"
		"cmpeq   r4,r3,zero\n\t"	/* 2nd return value in r3   */
		"bne     r4,zero,1f\n\t"	/* 0: parent, just return.  */
						/* See copy_thread, called  */
						/*  by do_fork, called by   */
						/*  nios2_clone, called by  */
						/*  sys_clone, called by    */
						/*  syscall trap handler.   */

		"mov     r4,%4\n\t"		/* fn's parameter (arg)     */
		"\n\t"
		"callr   %3\n\t"		/* Call function (fn)       */
		"\n\t"
		"mov     r4,r2\n\t"		/* fn's rtn code//;dgt2;tmp;*/
		"movi    r2,%6\n\t"		/* TRAP_ID_SYSCALL          */
		"movi    r3,%2\n\t"		/* __NR_exit                */
		"trap\n\t"			/* sys_exit()               */

		/* Not reached by child */
		"1:\n\t"
		"mov     %0,r2\n\t"		/* error rtn code (retval)  */

		:   "=r" (retval)               /* %0                       */

		:   "i" (__NR_clone)		/* %1                       */
		  , "i" (__NR_exit)		/* %2                       */
		  , "r" (fn)			/* %3                       */
		  , "r" (arg)			/* %4                       */
		  , "r" (clone_arg)		/* %5  (flags | CLONE_VM)   */
		  , "i" (TRAP_ID_SYSCALL)	/* %6                       */

		:   "r2", "r3", "r4", "r5", "ra"/* Clobbered                */
	);

	set_fs(fs);
	return retval;
#endif /* CONFIG_MMU */
}
EXPORT_SYMBOL(kernel_thread);

void flush_thread(void)
{
	set_fs(USER_DS);
}

/*
 * "nios2_fork()".. By the time we get here, the non-volatile registers have
 * also been saved on the stack. We do some ugly pointer stuff here.. (see also
 * copy_thread)
 */
asmlinkage int nios2_fork(struct pt_regs *regs)
{
#ifdef CONFIG_MMU
	return do_fork(SIGCHLD, regs->sp, regs, 0, NULL, NULL);
#else
	/* fork almost works, enough to trick you into looking elsewhere :-( */
	return -EINVAL;
#endif /* CONFIG_MMU */
}

/*
 * nios2_execve() executes a new program.
 */
asmlinkage int nios2_execve(struct pt_regs *regs)
{
	int error;
	char *filename;

	filename = getname((char *) regs->r4);
	error = PTR_ERR(filename);
	if (IS_ERR(filename))
		return error;
	error = do_execve(filename,
			  (const char __user *const __user *) regs->r5,
			  (const char __user *const __user *) regs->r6,
			  regs);
	putname(filename);
	return error;
}

asmlinkage int nios2_vfork(struct pt_regs *regs)
{
	return do_fork(CLONE_VFORK | CLONE_VM | SIGCHLD, regs->sp, regs, 0, NULL, NULL);
}

asmlinkage int nios2_clone(struct pt_regs *regs)
{
	unsigned long clone_flags;
	unsigned long newsp;
	int __user *parent_tidptr, *child_tidptr;

	clone_flags = regs->r4;
	newsp = regs->r5;
	if (newsp == 0)
		newsp = regs->sp;
#ifdef CONFIG_MMU
	parent_tidptr = (int __user *) regs->r6;
	child_tidptr = (int __user *) regs->r8;
#else
	parent_tidptr = NULL;
	child_tidptr = NULL;
#endif

	return do_fork(clone_flags, newsp, regs, 0, 
	               parent_tidptr, child_tidptr);
}

int copy_thread(unsigned long clone_flags,
		unsigned long usp, unsigned long topstk,
		struct task_struct *p, struct pt_regs *regs)
{
	struct pt_regs *childregs;
	struct switch_stack *childstack, *stack;

	childregs = task_pt_regs(p);

	/* Save pointer to registers in thread_struct */
	p->thread.kregs = childregs;

	/* Copy registers */
	*childregs = *regs;
#ifndef CONFIG_MMU
	childregs->r2 = 0;	/* Redundant? See return values below */
#endif

	/* Copy stacktop and copy the top entrys from parent to child */
	stack = ((struct switch_stack *) regs) - 1;
	childstack = ((struct switch_stack *) childregs) - 1;
	*childstack = *stack;
	childstack->ra = (unsigned long) ret_from_fork;

#ifdef CONFIG_MMU
	if (childregs->estatus & ESTATUS_EU)
		childregs->sp = usp;
	else
		childregs->sp = (unsigned long) childstack;
#else
	if (usp == -1)
		p->thread.kregs->sp = (unsigned long) childstack;
	else
		p->thread.kregs->sp = usp;
#endif /* CONFIG_MMU */

	/* Store the kernel stack in thread_struct */
	p->thread.ksp = (unsigned long) childstack;

#ifdef CONFIG_MMU
	/* Initialize tls register. */
	if (clone_flags & CLONE_SETTLS)
		childstack->r23 = regs->r7;
#endif

	/* Set the return value for the child. */
	childregs->r2 = 0;
#ifdef CONFIG_MMU
	childregs->r7 = 0;
#else
	childregs->r3 = 1;	/* kernel_thread parent test */
#endif

	/* Set the return value for the parent. */
	regs->r2 = p->pid;
#ifdef CONFIG_MMU
	regs->r7 = 0;	/* No error */
#else
	regs->r3 = 0;	/* kernel_thread parent test */
#endif

	return 0;
}

/*
 *	Generic dumping code. Used for panic and debug.
 */
void dump(struct pt_regs *fp)
{
	unsigned long	*sp;
	unsigned char	*tp;
	int		i;

	printk(KERN_EMERG "\nCURRENT PROCESS:\n\n");
	printk(KERN_EMERG "COMM=%s PID=%d\n", current->comm, current->pid);

	if (current->mm) {
		printk(KERN_EMERG "TEXT=%08x-%08x DATA=%08x-%08x BSS=%08x-%08x\n",
			(int) current->mm->start_code,
			(int) current->mm->end_code,
			(int) current->mm->start_data,
			(int) current->mm->end_data,
			(int) current->mm->end_data,
			(int) current->mm->brk);
		printk(KERN_EMERG "USER-STACK=%08x  KERNEL-STACK=%08x\n\n",
			(int) current->mm->start_stack,
			(int)(((unsigned long) current) + THREAD_SIZE));
	}

	printk(KERN_EMERG "PC: %08lx\n", fp->ea);
	printk(KERN_EMERG "SR: %08lx    SP: %08lx\n", (long) fp->estatus, (long) fp);

	printk(KERN_EMERG "r1: %08lx    r2: %08lx    r3: %08lx\n",
		fp->r1, fp->r2, fp->r3);

	printk(KERN_EMERG "r4: %08lx    r5: %08lx    r6: %08lx    r7: %08lx\n",
		fp->r4, fp->r5, fp->r6, fp->r7);
	printk(KERN_EMERG "r8: %08lx    r9: %08lx    r10: %08lx    r11: %08lx\n",
		fp->r8, fp->r9, fp->r10, fp->r11);
	printk(KERN_EMERG "r12: %08lx  r13: %08lx    r14: %08lx    r15: %08lx\n",
		fp->r12, fp->r13, fp->r14, fp->r15);
	printk(KERN_EMERG "or2: %08lx   ra: %08lx     fp: %08lx    sp: %08lx\n",
		fp->orig_r2, fp->ra, fp->fp, fp->sp);
	printk(KERN_EMERG "\nUSP: %08x   TRAPFRAME: %08x\n", (unsigned int) fp->sp,
		(unsigned int) fp);

	printk(KERN_EMERG "\nCODE:");
	tp = ((unsigned char *) fp->ea) - 0x20;
	for (sp = (unsigned long *) tp, i = 0; (i < 0x40);  i += 4) {
		if ((i % 0x10) == 0)
			printk(KERN_EMERG "\n%08x: ", (int) (tp + i));
		printk(KERN_EMERG "%08x ", (int) *sp++);
	}
	printk(KERN_EMERG "\n");

	printk(KERN_EMERG "\nKERNEL STACK:");
	tp = ((unsigned char *) fp) - 0x40;
	for (sp = (unsigned long *) tp, i = 0; (i < 0xc0); i += 4) {
		if ((i % 0x10) == 0)
			printk(KERN_EMERG "\n%08x: ", (int) (tp + i));
		printk(KERN_EMERG "%08x ", (int) *sp++);
	}
	printk(KERN_EMERG "\n");
	printk(KERN_EMERG "\n");

	printk(KERN_EMERG "\nUSER STACK:");
	tp = (unsigned char *) (fp->sp - 0x10);
	for (sp = (unsigned long *) tp, i = 0; (i < 0x80); i += 4) {
		if ((i % 0x10) == 0)
			printk(KERN_EMERG "\n%08x: ", (int) (tp + i));
		printk(KERN_EMERG "%08x ", (int) *sp++);
	}
	printk(KERN_EMERG "\n\n");
}

unsigned long get_wchan(struct task_struct *p)
{
	unsigned long fp, pc;
	unsigned long stack_page;
	int count = 0;
	if (!p || p == current || p->state == TASK_RUNNING)
		return 0;

	stack_page = (unsigned long)p;
	fp = ((struct switch_stack *)p->thread.ksp)->fp;        //;dgt2
	do {
		if (fp < stack_page+sizeof(struct task_struct) ||
		    fp >= 8184+stack_page)                          //;dgt2;tmp
			return 0;
		pc = ((unsigned long *)fp)[1];
		if (!in_sched_functions(pc))
			return pc;
		fp = *(unsigned long *) fp;
	} while (count++ < 16);                                 //;dgt2;tmp
	return 0;
}

/*
 * Do necessary setup to start up a newly executed thread.
 * Will startup in user mode (status_extension = 0).
 */
void start_thread(struct pt_regs *regs, unsigned long pc, unsigned long sp)
{
	memset((void *) regs, 0, sizeof(struct pt_regs));
#ifdef CONFIG_MMU
	regs->estatus = ESTATUS_EPIE | ESTATUS_EU;
#else
	/* No user mode setting on NOMMU, at least for now */
	regs->estatus = ESTATUS_EPIE;
#endif /* CONFIG_MMU */
	regs->ea = pc;
	regs->sp = sp;
}

#ifdef CONFIG_MMU
#include <linux/elfcore.h>

/* Fill in the FPU structure for a core dump. */
int dump_fpu(struct pt_regs *regs, elf_fpregset_t *r)
{
	return 0; /* Nios2 has no FPU and thus no FPU registers */
}
#endif /* CONFIG_MMU */
