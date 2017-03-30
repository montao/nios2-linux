/****************************************************************************
 * This application tries to cause one of every type of fault event that the
 * Blackfin processor can or can not handle, in order to test the kernel's
 * exception handler's robustness, and ability to recover from a userspace
 * fault condition properly.
 *
 * This is all bad bad code - you should not look at this as examples for
 * anything (unless you want to also test the kernel's robustness). If you
 * can find something that the kernel does not respond well to, please add
 * it to this list.
 *
 **********************************************************************
 * Copyright Analog Devices Inc 2007 - 2009
 * Released under the GPL 2 or later
 *
 ***************************************************************************/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <errno.h>
#include <sched.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <asm/ptrace.h>
#include <sys/klog.h>
#ifdef __x86_64__
#include <sys/reg.h>
#endif

#define NULL_PTR		0x00000000
/*
 * Hardware Error interrupt for "External Memory Address Error" is triggered
 * ahead of data misaligned exception(0x24) and data access CPLB miss
 * exception(0x26) on bf533 if:
 * 1) read from an odd address outside real memory region.
 * 2) rts to an odd address outside real memory region.
 *
 * Use POPULATED_ODD in real memory (<32M) for test against exception 0x24.
 * Because CPLB entry can't be easily invalidated from user space, tests
 * agait exception 0x26 has no walkaround for bf533. Just don't run these
 * tests on bf533.
 */
#define POPULATED_ODD		0x01654321
#define UNPOPULATED_EVEN	0x87654320
#define UNPOPULATED_ODD		0x87654321

#define SCRATCHPAD_COREA	0xFFB00000
#define L1_DATA_A_COREA		0xFF800000
#define L1_DATA_B_COREA		0xFF900000
#define L1_INSTRUCTION_COREA	0xFFA10000
#define L1_NON_EXISTANT_COREA	0xFFAFFF00

#define SCRATCHPAD_COREB	0xFF700000
#define L1_DATA_A_COREB		0xFF400000
#define L1_DATA_B_COREB		0xFF500000
#define L1_INSTRUCTION_COREB	0xFF610000
#define L1_NON_EXISTANT_COREB	0xFF6FFF00

#define SYSMMR_BASE		0xFFC00000
#define COREMMR_BASE		0xFFE00000

static const char *progname;

#define NR_CPUS 2
static unsigned cpu;
static unsigned scratch_pad[] = {SCRATCHPAD_COREA, SCRATCHPAD_COREB};
static unsigned l1_data_a[] = {L1_DATA_A_COREA, L1_DATA_A_COREB};
static unsigned l1_dta_b[] = {L1_DATA_B_COREA, L1_DATA_B_COREB};
static unsigned l1_instruction[] = {L1_INSTRUCTION_COREA, L1_INSTRUCTION_COREB};
static unsigned l1_non_existant[] = {L1_NON_EXISTANT_COREA, L1_NON_EXISTANT_COREB};

#define _stderr(pfx, fmt, args...)  fprintf(stderr, "%s: " pfx ": " fmt "\n", progname, ## args)
#define _stderrp(pfx, fmt, args...) _stderr(pfx, fmt ": %s", ## args, strerror(errno))
#define warn(fmt, args...)          _stderr("warning", fmt, ## args)
#define warnp(fmt, args...)         _stderrp("warning", fmt, ## args)
#define err(fmt, args...)           do { _stderr("error", fmt, ## args); exit(EXIT_FAILURE); } while (0)
#define errp(fmt, args...)          do { _stderrp("error", fmt, ## args); exit(EXIT_FAILURE); } while (0)

const char seqstat_path[] = "/sys/kernel/debug/blackfin/core_regs/last_seqstat";

#ifdef __FDPIC__
# define _get_func_ptr(addr) ({ unsigned long __addr[2] = { addr, 0 }; __addr; })
#else
# define _get_func_ptr(addr) (addr)
#endif
#define get_func_ptr(addr) (void *)(_get_func_ptr(addr))

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(*x))

void _bad_return_address(unsigned long rets);
asm("__bad_return_address: rets = R0; nop; nop; nop; nop; rts;\n");

void _bad_stack_set(unsigned long rets);
asm("__bad_stack_set: SP = R0; FP = R0; nop; nop; rts;\n");

#define flush(ptr)    asm volatile ("flush[%0];\n"    : : "p"(ptr));
#define flushinv(ptr) asm volatile ("flushinv[%0];\n" : : "p"(ptr));
#define iflush(ptr)   asm volatile ("iflush[%0];\n"   : : "p"(ptr));
#define prefetch(ptr) asm volatile ("prefetch[%0];\n" : : "p"(ptr));

#define bad_stack_push(addr)	asm volatile ("R1 = SP ; SP = %0; [SP] = %0; SP = R1" :  : "r"(addr) : "R1");

/*
 * These tests should test all things possible that can create an
 * exception. For details, look in arch/blackfin/mach-common/entry.S
 * in the kernel
 */

/* User Defined - Linux Syscall                        EXCAUSE 0x00 */
/* User Defined - Software breakpoint                  EXCAUSE 0x01 */
void expt_1(void)
{
	asm volatile("excpt 0x1;");
}
/* User Defined - Should fail                          EXCAUSE 0x02 */
void expt_2(void)
{
	asm volatile("excpt 0x2;");
}
/* User Defined - userspace stack overflow             EXCAUSE 0x03 */
void expt_3(void)
{
	asm volatile("excpt 0x3;");
}
/* User Defined - dump trace buffer                    EXCAUSE 0x04 */
void expt_4(void)
{
	asm volatile("excpt 0x4;");
}
/* User Defined - Should fail                          EXCAUSE 0x05 */
void expt_5(void)
{
	asm volatile("excpt 0x5;");
}
/* User Defined - Should fail                          EXCAUSE 0x06 */
void expt_6(void)
{
	asm volatile("excpt 0x6;");
}
/* User Defined - Should fail                          EXCAUSE 0x07 */
void expt_7(void)
{
	asm volatile("excpt 0x7;");
}
/* User Defined - Should fail                          EXCAUSE 0x08 */
void expt_8(void)
{
	asm volatile("excpt 0x8;");
}
/* User Defined - Should fail                          EXCAUSE 0x09 */
void expt_9(void)
{
	asm volatile("excpt 0x9;");
}
/* User Defined - Should fail                          EXCAUSE 0x0A */
void expt_A(void)
{
	asm volatile("excpt 0xA;");
}
/* User Defined - Should fail                          EXCAUSE 0x0B */
void expt_B(void)
{
	asm volatile("excpt 0xB;");
}
/* User Defined - Should fail                          EXCAUSE 0x0C */
void expt_C(void)
{
	asm volatile("excpt 0xC;");
}
/* User Defined - Should fail                          EXCAUSE 0x0D */
void expt_D(void)
{
	asm volatile("excpt 0xD;");
}
/* User Defined - Should fail                          EXCAUSE 0x0E */
void expt_E(void)
{
	asm volatile("excpt 0xE;");
}
/* User Defined - Should fail                          EXCAUSE 0x0F */
void expt_F(void)
{
	asm volatile("excpt 0xF;");
}

/* Single Step -                                       EXCAUSE 0x10 */
/* Can't do this in userspace */

/* Exception caused by a trace buffer full condition - EXCAUSE 0x11 */
/* Can't do this in userspace */

/* Undefined instruction -                             EXCAUSE 0x21 */
void unknown_instruction(void)
{
	asm volatile(".word 0x0001;");
}

/* Illegal instruction combination -                   EXCAUSE 0x22 */
void illegal_instruction(void)
{
	/* P0 = [P0++] */
	asm volatile(".dw 0x9040");
}

/* Data access CPLB protection violation -             EXCAUSE 0x23 */

/* Data access misaligned address violation -          EXCAUSE 0x24 */
void data_read_odd_address(void)
{
	int *i = (void *)POPULATED_ODD;
	printf("%i\n", *i);
}

void data_write_odd_address(void)
{
	int *i = (void *)POPULATED_ODD;
	*i = 0;
}

void stack_odd_address(void)
{
	_bad_stack_set(POPULATED_ODD);
}

void stack_push_odd_address(void)
{
	bad_stack_push(POPULATED_ODD);
}

/* Unrecoverable event -                               EXCAUSE 0x25 */
/* Can't do this in userspace (hopefully) */

/* Data access CPLB miss -                             EXCAUSE 0x26 */
void data_read_miss(void)
{
	int *i = (void *)UNPOPULATED_EVEN;
	printf("%i\n", *i);
}

void data_write_miss(void)
{
	int *i = (void *)UNPOPULATED_EVEN;
	*i = 0;
}

void stack_miss(void)
{
	_bad_stack_set(UNPOPULATED_EVEN);
}

void stack_push_miss(void)
{
	bad_stack_push(UNPOPULATED_EVEN);
}

/* Data access multiple CPLB hits -                    EXCAUSE 0x27 */
/* We use this to trap null pointers */
void null_pointer_write(void)
{
	int *i = NULL_PTR;
	*i = 0;
}

void null_pointer_read(void)
{
	int *i = NULL_PTR;
	printf("%i", *i);
}

void stack_zero(void)
{
	_bad_stack_set(NULL_PTR);
}

void stack_push_zero(void)
{
	bad_stack_push(NULL_PTR);
}

/* Exception caused by an emulation watchpoint match - EXCAUSE 0x28 */
/* Can't do this in userspace */

/* Instruction fetch misaligned address violation -    EXCAUSE 0x2A */
void instruction_fetch_odd_address(void)
{
	int (*foo)(void);
	foo = get_func_ptr((int)&instruction_fetch_odd_address + 1);
	(*foo)();
}

/* Instruction fetch CPLB protection violation -       EXCAUSE 0x2B
 * with mpu on, these return 2B, otherwise
 */
void bad_return_scratchpad(void)
{
	_bad_return_address(scratch_pad[cpu]);
}

void bad_return_l1dataA(void)
{
	_bad_return_address(l1_data_a[cpu]);
}

void bad_return_l1dataB(void)
{
	_bad_return_address(l1_dta_b[cpu]);
}

/* Instruction fetch CPLB miss -                       EXCAUSE 0x2C */
void instruction_fetch_miss(void)
{
	int (*foo)(void);
	foo = get_func_ptr(UNPOPULATED_EVEN);
	(*foo)();
}

void bad_return_bad_location(void)
{
	_bad_return_address(UNPOPULATED_EVEN);
}

void mmr_jump(void)
{
	int (*foo)(void);
	foo = get_func_ptr(SYSMMR_BASE);
	(*foo)();
}

/* Instruction fetch multiple CPLB hits -              EXCAUSE 0x2D */
void jump_to_zero(void)
{
	int (*foo)(void);
	foo = get_func_ptr(0);
	(*foo)();
}

void bad_return_zero(void)
{
	_bad_return_address(0x0);
}

/* Illegal use of supervisor resource -                EXCAUSE 0x2E */
void supervisor_instruction(void)
{
	asm volatile("cli R0;");
}

void supervisor_resource_mmr_read(void)
{
	int *i = (void *)SYSMMR_BASE;
	printf("chip id = %x", *i);

}

void supervisor_resource_mmr_write(void)
{
	int *i = (void *)SYSMMR_BASE;
	*i = 0;
}

sigjmp_buf supervisor_brute_buf;
void supervisor_brute_sighdl(int sig)
{
	switch (sig) {
		case SIGBUS:
		case SIGILL:
			siglongjmp(supervisor_brute_buf, 1);
		default:
			warn("signal %i is not what we wanted", sig);
			_exit(10);
	}
}
void supervisor_brute_doit(bool read, int size)
{
	int ret;
	unsigned long mmr = SYSMMR_BASE;

	signal(SIGBUS, supervisor_brute_sighdl);
	signal(SIGILL, supervisor_brute_sighdl);
	setbuf(stdout, NULL);

	switch (size) {
		case 8:	/* bits */
		case 16:
		case 32: size /= 8;
		case 1:	/* bytes */
		case 2:
		case 4: break;
		default:
			err("invalid size %i", size);
	}

 jmp_again:
	ret = sigsetjmp(supervisor_brute_buf, 1);
	if (ret) {
		/* just been restored */
		mmr += size;
		goto jmp_again;
	}
	if (mmr == 0)
		return;

	ret = mmr - SYSMMR_BASE;
	if (ret % 0x1000 == 0)
		printf("%i ", ret / 0x1000);

	if (read) {
		switch (size) {
			case 1: ret = *(volatile uint8_t  *)mmr; break;
			case 2: ret = *(volatile uint16_t *)mmr; break;
			case 4: ret = *(volatile uint32_t *)mmr; break;
		}
	} else {
		switch (size) {
			case 1: *(volatile uint8_t  *)mmr = ret; break;
			case 2: *(volatile uint16_t *)mmr = ret; break;
			case 4: *(volatile uint32_t *)mmr = ret; break;
		}
	}
	/* should never be reached ... */
	err("brute test failed (ret = %i)", ret);
}
void supervisor_brute_force(const char *test)
{
	switch (*test) {
		case 'r':
		case 'w': break;
		default:  err("invalid test '%c' !~ '[rw]'", *test);
	}

	supervisor_brute_doit(*test == 'r', atoi(test + 1));

	exit(EXIT_SUCCESS);
}

/* Things that cause Hardware errors (IRQ5), not exceptions (IRQ3) */
/* System MMR Error                                    HWERRCAUSE 0x02 */
/* Can't do this in userspace */

/* External Memory Addressing Error -                  HWERRCAUSE 0x03 */
//__attribute__ ((l1_text))
void l1_instruction_read(void)
{
	int *i = (void *)l1_instruction[cpu];
	printf("%i\n", *i);
}

void l1_instruction_write(void)
{
	int *i = (void *)l1_instruction[cpu];
	*i = 0;
}

void stack_instruction(void)
{
	_bad_stack_set(l1_instruction[cpu]);
}

void l1_dataA_jump(void)
{
	int (*foo)(void);
	foo = get_func_ptr(l1_data_a[cpu]);
	(*foo)();
}

void l1_dataB_jump(void)
{
	int (*foo)(void);
	foo = get_func_ptr(l1_dta_b[cpu]);
	(*foo)();
}

void l1_scratchpad_jump(void)
{
	int (*foo)(void);
	foo = get_func_ptr(scratch_pad[cpu]);
	(*foo)();
}

void l1_non_existant_jump(void)
{
	int (*foo)(void);
	foo = get_func_ptr(l1_non_existant[cpu]);
	(*foo)();
}

void l1_non_existant_read(void)
{
	int *i = (void *)l1_non_existant[cpu];
	printf("%i\n", *i);
}

void l1_non_existant_write(void)
{
	int *i = (void *)l1_non_existant[cpu];
	*i = 0;
}

void l1_non_existant_write_syscall(void)
{
	int *i = (void *)l1_non_existant[cpu];
	*i = 0;
	sync();
}

void stack_l1_non_existant(void)
{
	_bad_stack_set(l1_non_existant[cpu]);
}

void stack_push_l1_non_existant(void)
{
	bad_stack_push(l1_non_existant[cpu]);
}

void bad_return_l1_non_existant(void)
{
	_bad_return_address(l1_non_existant[cpu]);
}

void bad_return_mmr(void)
{
	_bad_return_address(SYSMMR_BASE);
}

void flush_null(void)
{
	flush(NULL_PTR);
}

void flush_upop(void)
{
	flush(UNPOPULATED_EVEN);
}

void flush_upop_odd(void)
{
	flush(UNPOPULATED_ODD);
}

void flush_scratch(void)
{
	flush(scratch_pad[cpu]);
}

void flush_l1_dataA(void)
{
	flush(l1_data_a[cpu]);
}

void flush_l1_dataB(void)
{
	flush(l1_dta_b[cpu]);
}

void flush_l1_instruction(void)
{
	flush(l1_instruction[cpu]);
	}

void flush_l1_non(void)
{
	flush(l1_non_existant[cpu]);
}

void flush_sysmmr(void)
{
	flush(SYSMMR_BASE);
}

void flush_coremmr(void)
{
	flush(COREMMR_BASE);
}
/* -------- */
void prefetch_null(void)
{
	prefetch(NULL_PTR);
}

void prefetch_upop(void)
{
	prefetch(UNPOPULATED_EVEN);
}

void prefetch_upop_odd(void)
{
	prefetch(UNPOPULATED_ODD);
}

void prefetch_scratch(void)
{
	prefetch(scratch_pad[cpu]);
}

void prefetch_l1_dataA(void)
{
	prefetch(l1_data_a[cpu]);
}

void prefetch_l1_dataB(void)
{
	prefetch(l1_dta_b[cpu]);
}

void prefetch_l1_instruction(void)
{
	prefetch(l1_instruction[cpu]);
}

void prefetch_l1_non(void)
{
	prefetch(l1_non_existant[cpu]);
}

void prefetch_sysmmr(void)
{
	prefetch(SYSMMR_BASE);
}

void prefetch_coremmr(void)
{
	prefetch(COREMMR_BASE);
}

/*-------------*/
void flushinv_null(void)
{
	flushinv(NULL_PTR);
}

void flushinv_upop(void)
{
	flushinv(UNPOPULATED_EVEN);
}

void flushinv_upop_odd(void)
{
	flushinv(UNPOPULATED_ODD);
}

void flushinv_scratch(void)
{
	flushinv(scratch_pad[cpu]);
}

void flushinv_l1_dataA(void)
{
	flushinv(l1_data_a[cpu]);
}

void flushinv_l1_dataB(void)
{
	flushinv(l1_dta_b[cpu]);
}

void flushinv_l1_instruction(void)
{
	flushinv(l1_instruction[cpu]);
}

void flushinv_l1_non(void)
{
	flushinv(l1_non_existant[cpu]);
}

void flushinv_sysmmr(void)
{
	flushinv(SYSMMR_BASE);
}

void flushinv_coremmr(void)
{
	flushinv(COREMMR_BASE);
}

/*--------- */
void iflush_null(void)
{
	iflush(NULL_PTR);
}

void iflush_upop(void)
{
	iflush(UNPOPULATED_EVEN);
}

void iflush_upop_odd(void)
{
	iflush(UNPOPULATED_ODD);
}

void iflush_scratch(void)
{
	iflush(scratch_pad[cpu]);
}

void iflush_l1_dataA(void)
{
	iflush(l1_data_a[cpu]);
}

void iflush_l1_dataB(void)
{
	iflush(l1_dta_b[cpu]);
}

void iflush_l1_instruction(void)
{
	iflush(l1_instruction[cpu]);
}

void iflush_l1_non(void)
{
	iflush(l1_non_existant[cpu]);
}

void iflush_sysmmr(void)
{
	iflush(SYSMMR_BASE);
}

void iflush_coremmr(void)
{
	iflush(COREMMR_BASE);
}

/* Performance Monitor Overflow                        HWERRCAUSE 0x012*/
/* Can't do this in userspace */

/* RAISE 5 instruction                                 HWERRCAUSE 0x18 */
/* Can't do this in userspace - since this is a supervisor instruction */


/* List of the tests  - if the tests is not added to the list - it will not be run */

struct {
	int excause;
	void (*func)(void);
	int kill_sig;
	const char *name;
} bad_funcs[] = {
	{ 0x00, flush_scratch, 0, "flush scratch pad"},
	{ 0x00, flush_l1_dataA, 0, "l1_dataA"},
	{ 0x00, flush_l1_dataB, 0, "l1_dataB"},
	{ 0x00, flush_l1_instruction, 0, "l1_instruction"},
	{ 0x00, flush_l1_non, 0, "flush_l1_non"},
	{ 0x00, flush_sysmmr, 0, "flush_sysmmr"},
	{ 0x00, flush_coremmr, 0, "flush_coremmr"},
	{ 0x00, iflush_null, 0 , "iflush null pointer"},
	{ 0x00, iflush_upop, 0, "iflush unpopulated address"},
	{ 0x00, iflush_upop_odd, 0, "iflush unpopulated odd address"},
	{ 0x00, iflush_scratch, 0, "iflush scratch pad"},
	{ 0x00, iflush_l1_dataA, 0, "iflush l1_dataA"},
	{ 0x00, iflush_l1_dataB, 0, "iflush l1_dataB"},
	{ 0x00, iflush_l1_instruction, 0, "iflush l1_instruction"},
	{ 0x00, iflush_l1_non, 0, "iflush_l1_non"},
	{ 0x00, iflush_sysmmr, 0, "iflush_sysmmr"},
	{ 0x00, iflush_coremmr, 0, "iflush_coremmr"},
#if 1 /* hardware is broken :( */
	{ 0x00, prefetch_scratch,  0, "prefetch scratch pad"},
	{ 0x00, prefetch_l1_dataA, 0, "prefetch l1_dataA"},
	{ 0x00, prefetch_l1_dataB, 0, "prefetch l1_dataB"},
	{ 0x00, prefetch_l1_instruction, SIGILL, "prefetch l1_instruction"},
	{ 0x00, prefetch_l1_non,   0, "prefetch _l1_non"},
	{ 0x00, prefetch_sysmmr,   0, "prefetch _sysmmr"},
	{ 0x00, prefetch_coremmr,  0, "prefetch _coremmr"},
#endif
	{ 0x00, flushinv_scratch, 0, "flushinv scratch pad"},
	{ 0x00, flushinv_l1_dataA, 0, "flushinv l1_dataA"},
	{ 0x00, flushinv_l1_dataB, 0, "flushinv l1_dataB"},
	{ 0x00, flushinv_l1_instruction, 0, "flushinv l1_instruction"},
	{ 0x00, flushinv_l1_non, 0, "flushinv _l1_non"},
	{ 0x00, flushinv_sysmmr, 0, "flushinv _sysmmr"},
	{ 0x00, flushinv_coremmr, 0, "flushinv _coremmr"},

	{ 0x01, expt_1, SIGTRAP, "EXCPT 0x01" },
	{ 0x02, expt_2, SIGILL, "EXCPT 0x02" },
	{ 0x03, expt_3, SIGSEGV, "EXCPT 0x03" },
	{ 0x04, expt_4, SIGILL, "EXCPT 0x04" },
	{ 0x05, expt_5, SIGILL, "EXCPT 0x05" },
	{ 0x06, expt_6, SIGILL, "EXCPT 0x06" },
	{ 0x07, expt_7, SIGILL, "EXCPT 0x07" },
	{ 0x08, expt_8, SIGILL, "EXCPT 0x08" },
	{ 0x09, expt_9, SIGILL, "EXCPT 0x09" },
	{ 0x0A, expt_A, SIGILL, "EXCPT 0x0A" },
	{ 0x0B, expt_B, SIGILL, "EXCPT 0x0B" },
	{ 0x0C, expt_C, SIGILL, "EXCPT 0x0C" },
	{ 0x0D, expt_D, SIGILL, "EXCPT 0x0D" },
	{ 0x0E, expt_E, SIGILL, "EXCPT 0x0E" },
	{ 0x0F, expt_F, SIGILL, "EXCPT 0x0F" },
	{ 0x21, unknown_instruction, SIGILL, "Invalid Opcode" },
	{ 0x22, illegal_instruction, SIGILL, "Illegal Instruction" },
	{ 0x23, supervisor_resource_mmr_read, SIGSEGV, "Illegal use of supervisor resource - MMR Read" },
	{ 0x23, supervisor_resource_mmr_write, SIGSEGV, "Illegal use of supervisor resource - MMR Write" },
	{ 0x24, data_read_odd_address, SIGBUS, "Data read misaligned address violation" },
	{ 0x24, data_write_odd_address, SIGBUS, "Data write misaligned address violation" },
	{ 0x24, stack_odd_address, SIGBUS, "Stack set to odd address - misaligned address violation" },
	{ 0x24, stack_push_odd_address, SIGBUS, "Stack push to odd address" },
	{ 0x26, data_read_miss, SIGBUS, "Data Read CPLB miss" },
	{ 0x26, data_write_miss, SIGBUS, "Data Write CPLB miss" },
	{ 0x26, stack_miss, SIGBUS, "Stack CPLB miss" },
	{ 0x26, stack_push_miss, SIGBUS, "Stack push to miss" },
	{ 0x26, flush_upop, SIGBUS, "flush unpopulated address"},
	{ 0x26, flush_upop_odd, SIGBUS, "flush unpopulated odd address"},
	{ 0x26, prefetch_upop,     SIGBUS, "prefetch unpopulated address"},
	{ 0x26, prefetch_upop_odd, SIGBUS, "prefetch unpopulated odd address"},
	{ 0x26, flushinv_upop, SIGBUS, "flushinv unpopulated address"},
	{ 0x26, flushinv_upop_odd, SIGBUS, "flushinv unpopulated odd address"},
	{ 0x27, null_pointer_read, SIGSEGV, "Data access multiple CPLB hits/Null Pointer Read" },
	{ 0x27, null_pointer_write, SIGSEGV, "Data access multiple CPLB hits/Null Pointer Write" },
	{ 0x27, stack_zero, SIGSEGV, "Stack set to zero" },
	{ 0x27, stack_push_zero, SIGSEGV, "Stack, push while SP is zero" },
	{ 0x27, flush_null, SIGSEGV, "flush null pointer"},
	{ 0x27, prefetch_null,     SIGSEGV, "prefetch null pointer"},
	{ 0x27, flushinv_null, SIGSEGV, "flushinv null pointer"},
	{ 0x2a, instruction_fetch_odd_address, SIGBUS, "Instruction fetch misaligned address violation"  },
	{ 0x2b, l1_dataA_jump,  SIGBUS, "Jump to L1 Data A" },
	{ 0x2b, bad_return_l1dataA, SIGBUS, "Return to L1 Data A" },
	{ 0x2b, l1_dataB_jump,  SIGBUS, "Jump to L1 Data B" },
	{ 0x2b, bad_return_l1dataB, SIGBUS, "Return to L1 Data B" },
	{ 0x2b, l1_scratchpad_jump, SIGBUS, "Jump to L1 scratchpad" },
	{ 0x2b, bad_return_scratchpad, SIGBUS, "Return to scratchpad" },
	{ 0x2c, instruction_fetch_miss, SIGBUS, "Instruction fetch CPLB miss"  },
	{ 0x2c, mmr_jump, SIGBUS, "Jump to MMR Space" },
	{ 0x2c, bad_return_bad_location, SIGBUS, "Return to non-existant L3" },
	{ 0x2c, bad_return_mmr, SIGBUS, "Return to an MMR address" },
	{ 0x2d, jump_to_zero, SIGSEGV, "Instruction fetch multiple CPLB hits - Jump to zero" },
	{ 0x2d, bad_return_zero, SIGSEGV, "Return to zero" },
	{ 0x2e, supervisor_instruction, SIGILL, "Illegal use of supervisor resource - Instruction" },
	{ 0x3f, l1_instruction_read, SIGBUS, "Read of L1 instruction" },
	{ 0x3f, l1_instruction_write, SIGBUS, "Write of L1 instruction" },
	{ 0x3f, l1_non_existant_jump, SIGBUS, "Jump to non-existant L1" },
	{ 0x3f, l1_non_existant_read, SIGBUS, "Read non-existant L1" },
	{ 0x3f, l1_non_existant_write, SIGBUS, "Write non-existant L1" },
	{ 0x3f, l1_non_existant_write_syscall, SIGBUS, "Write non-existant L1, then system call" },
	{ 0x3f, bad_return_l1_non_existant, SIGBUS, "Return to non-existant L1" },
	{ 0x3f, stack_instruction, SIGBUS, "Stack set to L1 instruction" },
	{ 0x3f, stack_l1_non_existant, SIGBUS, "Stack set to non-existant L1" },
	{ 0x3f, stack_push_l1_non_existant, SIGBUS, "Stack push to non-existant L1" },
};

/* helper functions needed for tracing */

static long _xptrace(int request, const char *srequest, pid_t pid, void *addr, void *data)
{
	int ret;

	/* some requests have -1 as a valid return */
	errno = 0;
	ret = ptrace(request, pid, addr, data);
	if (errno && ret == -1)
		warnp("ptrace(%i (%s), %i, %p, %p) failed\n",
			request, srequest, pid, addr, data);
	return ret;
}

#define xptrace(request, pid, addr, data) _xptrace(request, #request, pid, addr, data)

static long sysnum(pid_t pid)
{
	long offset;
#if defined(__bfin__)
	offset = PT_ORIG_P0;
#elif defined(__x86_64__)
	offset = 8 * ORIG_RAX;
#endif
	return xptrace(PTRACE_PEEKUSER, pid, (void *)offset, NULL);
}

/* Standard helper functions */

__attribute__((noreturn))
void list_tests(void)
{
	long test_num;

	printf("#\texcause\ttest %s\n", progname);
	for (test_num = 0; test_num < ARRAY_SIZE(bad_funcs); ++test_num)
		printf("%li\t0x%02x\t%s\n", test_num, bad_funcs[test_num].excause, bad_funcs[test_num].name);

	exit(EXIT_SUCCESS);
}

__attribute__((noreturn))
void usage(const char *errmsg)
{
	printf(
		"Usage: %s [-c count] [-d milliseconds] [-e] [-q] [-l] [-p] [-t] [-v] [-b <test>] [starting test number] [ending test number]\n"
		"\n"
		"-b test\t\tRun brute force test: <r|w><1|2|4> (read/write 1/2/4 bytes)\n"
		"-c count\tRepeat the test(s) count times before stopping\n"
		"-d seconds\tThe number of milliseconds to delay between flushing stdout, and\n"
		"\t\trunning the test (default is 1)\n"
		"-e\t\texclude the checking of excause is as expected\n"
		"-l\t\tList tests, then quit\n"
		"-q\t\tQuiet (don't print out test info)\n"
		"-p\t\tRun the test in the parent process, otherwise fork a child process.\n\t\tOnly valid for a single test.\n"
		"-t\t\tTrace (single step) through the failing tests, to simulate gdb\n"
		"-v\t\tVerbose. equivilent to 'dmesg -n 7', so that all kernel messages print out\n"
		"\t\totherwise do 'dmesg -n 3', so things are a little quieter\n\n"
		"If no test number is specified, the number of tests available will be shown.\n\n"
		"If a single test number is specified (0 <= n < # of tests), that test will be run.\n\n"
		"If two tests numbers are specified (0 <= start < end < # tests), those tests will be run\n\n"
		"If you specify -1, then all tests will be run in order.\n\n", progname
	);

	if (errmsg)
		err("%s", errmsg);
	else
		exit(EXIT_SUCCESS);
}

/* Now for the main code */

int main(int argc, char *argv[])
{
	char *endptr;
	long start_test = 0, end_test = 0, test, pass_tests = 0;
	int c, repeat = 1, del, parent = 0;
	int quiet = 0, trace = 0, verbose = 0, verify = 1;
	struct timespec delay;
	struct stat last_seqstat;
	FILE *seqstat_file;
	cpu_set_t set;

	if (sched_getaffinity(0, sizeof(cpu_set_t), &set) < 0 ){
		err("sched_getaffinity failed\n");
	}
	for (cpu = 0; cpu < NR_CPUS; cpu++)
		if (CPU_ISSET(cpu, &set)) break;

	progname = argv[0];

	delay.tv_sec = 1;
	delay.tv_nsec = 0;
	del = 1000;

	seqstat_file = NULL;

	if (argc == 1) {
		printf("%li\n", ARRAY_SIZE(bad_funcs) - 1);
		return EXIT_SUCCESS;
	}

	while ((c = getopt (argc, argv, "1b:c:d:ehlpqtv")) != -1)
		switch (c) {
		case '1':
			start_test = -1;
			break;
		case 'b':
			supervisor_brute_force(optarg);
			break;
		case 'c':
			repeat = strtol(optarg, &endptr, 10);
			if (optarg == endptr || endptr[0] || repeat <= 0)
				usage("did not understand count");
			break;
		case 'h':
			usage(NULL);
			break;
		case 'd':
			del = strtol(optarg, &endptr, 10);
			if (optarg == endptr || endptr[0])
				usage("did not understand delay");
			/* get seconds & nanoseconds */
			delay.tv_sec = del / 1000;
			delay.tv_nsec = (del - (delay.tv_sec * 1000)) * 1000000;
			break;
		case 'e':
			verify = 0;
			break;
		case 'l':
			list_tests();
			break;
		case 'p':
			parent = 1;
			break;
		case 'q':
			quiet = 1;
			break;
		case 't':
			trace = 1;
			break;
		case 'v':
			/* turn the equivilent to "dmesg -n 7" */
			klogctl(8, NULL, 7);
			verbose = 1;
			break;
		case '?':
		default:
			usage("unknown option");
			break;
		}

	if (verify && stat(seqstat_path, &last_seqstat)) {
		verify = 0;
		warnp("can't verify excause - could not open '%s'", seqstat_path);
	}

	if (verify) {
		seqstat_file = fopen(seqstat_path, "r");
		if (seqstat_file == NULL)
			errp("couldn't open '%s' for reading", seqstat_path);
		fclose(seqstat_file);
	}

	if ((optind == argc || argc - optind >= 3) && start_test != -1)
		usage(NULL);

	if (start_test == -1) {
		start_test = 0;
		end_test = ARRAY_SIZE(bad_funcs) - 1;
	} else {
		start_test = strtol(argv[optind], &endptr, 10);
		if (argv[optind] == endptr || endptr[0])
			usage("Specified start test is not a number");
		if (start_test >= ARRAY_SIZE(bad_funcs))
			usage("Start Test number out of range");

		if (optind + 1 >= argc)
			end_test = start_test;
		else {
			end_test = strtol(argv[optind + 1], &endptr, 10);
			if (argv[optind + 1] == endptr || endptr[0])
				usage("Specified end test is not a number");

			if (start_test >= end_test)
				usage("Specified end test must be larger than start test");
			if (end_test >= ARRAY_SIZE(bad_funcs))
				usage("End Test number out of range");
		}
	}

	if (parent && start_test != end_test) {
		warn("ignoring '-p' option, since running more than one test");
		parent = 0;
	}

	if (parent && repeat > 1) {
		warn("ignoring '-p' option, since count > 1");
		parent = 0;
	}

	/* By default, only KERN_ERR or more serious */
	if (!verbose && !parent)
		klogctl(8, NULL, 3);

	/* try and get output asap */
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	
	sleep(1);	

	for (test = start_test; test <= end_test ; ++test) {
		unsigned int ex_actual = 0, sig_actual=0, count, pass_count = 0;
		char *str_actual;
		char test_num[10];
		int _ret = 0;
		int sig_expect = bad_funcs[test].kill_sig;
		int ex_expect = bad_funcs[test].excause;

		if (!quiet)
			printf("\nRunning test %li for exception 0x%02x: %s\n... ", test, ex_expect, bad_funcs[test].name);
		nanosleep(&delay, NULL);

		/* should get killed ... */
		if (repeat == 1 && start_test == end_test && parent) {
			(*bad_funcs[test].func)();
			return EXIT_FAILURE;
		}

		sprintf(test_num, "%li", test);
		count = repeat;
		while (count) {
			pid_t pid;
			int status;
			siginfo_t info;

			count--;

			pid = vfork();
			if (pid == -1) {
				errp("vfork() failed");
			} else if (pid == 0) {
				if (trace && sig_expect != SIGTRAP)
					xptrace(PTRACE_TRACEME, 0, NULL, NULL);
				_ret = execlp(argv[0], argv[0], "-d", "0", "-q", "-p", test_num, NULL);

				warnp("execution of '%s' failed (%i)", argv[0], _ret);
				_exit(_ret);
			}

			/* since we wait for the specific child pid below - let's make sure it is valid */
			errno = 0;
			_ret = waitid(P_PID, pid, &info, WEXITED | WSTOPPED | WCONTINUED | WNOHANG | WNOWAIT);
			if (errno || _ret == -1) {
				warn("pid (%d) of child process didn't seem to start", pid);
				_exit(_ret);
			}

			if (trace && sig_expect != SIGTRAP) {
				/* wait until the child actually starts executing.  we could
				 * have the child execute an uncommon syscall and do PTRACE_SYSCALL
				 * until that point so as to speed the test up ...
				 */
				long nr = 0;

				while (nr != SYS_execve) {
					if (waitpid(pid, &status, 0) == -1)
						errp("wait() failed");
					if (WIFEXITED(status)) {
						printf("child exited with %i", WEXITSTATUS(status));
						if (WIFSIGNALED(status))
							printf(" with signal %i (%s)", sig_actual, strsignal(sig_actual));
						printf("\n");
						exit(EXIT_FAILURE);
					}
					nr = sysnum(pid);
					xptrace(PTRACE_SYSCALL, pid, NULL, NULL);
				}

				/* Single step the main test code */
				while (1) {
					if (waitpid(pid, &status, 0) == -1)
						errp("wait() failed");
					if (WIFEXITED(status)) {
						if (WIFSIGNALED(status))
							sig_actual = WTERMSIG(status);
						else
							sig_actual = 0;
						break;
					} else if (WIFSTOPPED(status)) {
						/* The signal is SIGTRAP when tracing normally */
						if (WSTOPSIG(status) != SIGTRAP) {
							sig_actual = WSTOPSIG(status);
							/* make sure any zombie child processes aren't left hanging */
							xptrace(PTRACE_CONT, pid, NULL, (void *)sig_actual);
							wait(NULL);
							break;
						}
					}

					/* last argument is if we want to pass a signal on to the
					 * child, but since we don't do any tests with signals, no
					 * need for that.
					 */
					xptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
				}
			} else {
				waitpid(pid, &status, 0);
				sig_actual = WTERMSIG(status);
			}
			if (verify) {
				seqstat_file = fopen(seqstat_path, "r");
				if (fscanf(seqstat_file, "%x", &ex_actual)) {
					if (ex_expect && (ex_actual & 0x3F) != ex_expect)
						printf("FAIL (test failed with wrong EXCAUSE, expected %x, but got %x)\n",
							ex_expect, (ex_actual & 0x3F));
				}
				fclose(seqstat_file);
			}

			if (sig_expect == sig_actual) {
				++pass_count;
				if (verify && sig_actual) {
					seqstat_file = fopen(seqstat_path, "r");
					if (fscanf(seqstat_file, "%x", &ex_actual)) {
						if ((ex_actual & 0x3F) != ex_expect) {
							printf("FAIL (test failed with wrong EXCAUSE, expected 0x%x, but got 0x%x)\n",
								ex_expect, (ex_actual & 0x3F));
							--pass_count;
						}
					}
					fclose(seqstat_file);
				}
			} else {
				char *str_expect = strsignal(sig_expect);
				str_actual = strsignal(sig_actual);
				if (sig_actual)
					printf("FAIL (test failed, but not with the right signal)\n"
						"\t(We expected %i '%s' but instead we got %i '%s')\n",
						sig_expect, str_expect, sig_actual, str_actual);
				else
					printf("FAIL (test exited sucessfully, and we expected %i %s failure\n",
						sig_expect, str_expect);
			}
		}
		str_actual = strsignal(sig_actual);

		if (pass_count == repeat) {
			++pass_tests;
			printf("PASS (test completed %i/%i times, as expected by signal %i: %s)\n",
				pass_count, repeat, sig_actual, str_actual);
		} else {
			printf("FAIL (test completed properly %i/%i times)\n",
				pass_count, repeat);
		}
	}

	test = end_test - start_test + 1;
	printf("\n%li/%li tests passed\n", pass_tests, test);

	exit(pass_tests == test ? EXIT_SUCCESS : EXIT_FAILURE);
}
