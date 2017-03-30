/*
 * NiosII low-level thread information
 *
 * Copyright (C) 2011 Tobias Klauser <tklauser@distanz.ch>
 * Copyright (C) 2004 Microtronix Datacom Ltd.
 *
 * Based on asm/thread_info_no.h from m68k which is:
 *
 * Copyright (C) 2002 David Howells <dhowells@redhat.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_NIOS2_THREAD_INFO_H
#define _ASM_NIOS2_THREAD_INFO_H

#ifdef __KERNEL__

/*
 * Size of the kernel stack for each process.
 */
#define THREAD_SIZE_ORDER	1
#define THREAD_SIZE		8192 /* 2 * PAGE_SIZE */

#ifndef __ASSEMBLY__

typedef struct {
	unsigned long seg;
} mm_segment_t;

/*
 * low level task data that entry.S needs immediate access to
 * - this struct should fit entirely inside of one cache line
 * - this struct shares the supervisor stack pages
 * - if the contents of this structure are changed, the assembly constants
 *   must also be changed
 */
struct thread_info {
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
	unsigned long		flags;		/* low level flags */
	__u32			cpu;		/* current CPU */
	int			preempt_count;	/* 0 => preemptable, <0 => BUG */
	mm_segment_t		addr_limit;	/* thread address space:
						   0-0x7FFFFFFF for user-thead
						   0-0xFFFFFFFF for kernel-thread
						*/
	struct restart_block	restart_block;
#ifdef CONFIG_MMU
	struct pt_regs		*regs;
#endif
};

/*
 * macros/functions for gaining access to the thread information structure
 *
 * preempt_count needs to be 1 initially, until the scheduler is functional.
 */
#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
	.addr_limit	= KERNEL_DS,		\
	.restart_block	= {			\
		.fn = do_no_restart_syscall,	\
	},					\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

/* how to get the thread information struct from C */
static inline struct thread_info *current_thread_info(void)
{
	struct thread_info *ti;
	__asm__ __volatile__(
		"mov	%0, sp\n"
		"and	%0, %0, %1\n"
		: "=&r" (ti)
		: "r" (~(THREAD_SIZE-1))
		);
	return ti;
}
#endif /* !__ASSEMBLY__ */

#define PREEMPT_ACTIVE		0x10000000

/*
 * thread information flags
 * - these are process state flags that various assembly files may need to
 *   access
 * - pending work-to-be-done flags are in LSW
 * - other flags in MSW
 */
#ifdef CONFIG_MMU
# define TIF_SIGPENDING		2	/* signal pending */
# define TIF_NEED_RESCHED	3	/* rescheduling necessary */
# define TIF_SYSCALL_AUDIT	4	/* syscall auditing active */
# define TIF_SECCOMP		5	/* secure computing */
# define TIF_RESTORE_SIGMASK	9	/* restore signal mask in do_signal() */
# define TIF_USEDFPU		16	/* FPU was used by this task this quantum (SMP) */
# define TIF_POLLING_NRFLAG	17	/* true if poll_idle() is polling TIF_NEED_RESCHED */
# define TIF_MEMDIE		18
# define TIF_FREEZE		19
# define TIF_SYSCALL_TRACE	31	/* syscall trace active */
#else
# define TIF_SYSCALL_TRACE	0	/* syscall trace active */
# define TIF_SIGPENDING		1	/* signal pending */
# define TIF_NEED_RESCHED	2	/* rescheduling necessary */
# define TIF_POLLING_NRFLAG	3	/* true if poll_idle() is polling
					   TIF_NEED_RESCHED */
# define TIF_MEMDIE		4
# define TIF_RESTORE_SIGMASK	9	/* restore signal mask in do_signal() */
# define TIF_FREEZE		16	/* is freezing for suspend */
#endif /* CONFIG_MMU */

#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
#ifdef CONFIG_MMU
# define _TIF_SYSCALL_AUDIT	(1<<TIF_SYSCALL_AUDIT)
# define _TIF_SECCOMP		(1<<TIF_SECCOMP)
# define _TIF_USEDFPU		(1<<TIF_USEDFPU)
#endif /* CONFIG_MMU */
#define _TIF_RESTORE_SIGMASK	(1<<TIF_RESTORE_SIGMASK)
#define _TIF_POLLING_NRFLAG	(1<<TIF_POLLING_NRFLAG)
#define _TIF_FREEZE		(1<<TIF_FREEZE)

/* work to do on interrupt/exception return */
#ifdef CONFIG_MMU
# define _TIF_WORK_MASK		(0x0000ffef & ~_TIF_SECCOMP)
/* work to do on any return to u-space */
# define _TIF_ALLWORK_MASK	(0x8000ffff & ~_TIF_SECCOMP)
#else
# define _TIF_WORK_MASK		0x0000FFFE	/* work to do on interrupt/exception return */
#endif /* CONFIG_MMU */

#endif /* __KERNEL__ */

#endif /* _ASM_NIOS2_THREAD_INFO_H */
