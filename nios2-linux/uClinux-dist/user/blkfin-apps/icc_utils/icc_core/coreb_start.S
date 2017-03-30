/*
 * BF561 coreB bootstrap file
 *
 * Copyright 2007-2009 Analog Devices Inc.
 *               Philippe Gerum <rpm@xenomai.org>
 *
 * Licensed under the GPL-2 or later.
 */

#define __ASSEMBLY__
#include <linux/linkage.h>
#include <asm/blackfin.h>
#include "context.S"

#define SM_TASK_NONE 0
#define SM_TASK_RESET 1
#define SM_TASK_INIT 2
#define SM_TASK_RUNNING 3

.section .l1.text

/* Lay the initial stack into the L1 scratch area of Core B */
#define INITIAL_STACK	(COREB_L1_SCRATCH_START + L1_SCRATCH_LENGTH - 12)


ENTRY(_coreb_trampoline_start)
	/* Set the SYSCFG register */
	R0 = 0x36;
	SYSCFG = R0; /*Enable Cycle Counter and Nesting Of Interrupts(3rd Bit)*/
	R0 = 0;

	/*Clear Out All the data and pointer  Registers*/
	R1 = R0;
	R2 = R0;
	R3 = R0;
	R4 = R0;
	R5 = R0;
	R6 = R0;
	R7 = R0;

	P0 = R0;
	P1 = R0;
	P2 = R0;
	P3 = R0;
	P4 = R0;
	P5 = R0;

	LC0 = r0;
	LC1 = r0;
	L0 = r0;
	L1 = r0;
	L2 = r0;
	L3 = r0;

	/* Clear Out All the DAG Registers*/
	B0 = r0;
	B1 = r0;
	B2 = r0;
	B3 = r0;

	I0 = r0;
	I1 = r0;
	I2 = r0;
	I3 = r0;

	M0 = r0;
	M1 = r0;
	M2 = r0;
	M3 = r0;

	/* Turn off the icache */
	p0.l = LO(IMEM_CONTROL);
	p0.h = HI(IMEM_CONTROL);
	R1 = [p0];
	R0 = ~ENICPLB;
	R0 = R0 & R1;

	/* Disabling of CPLBs should be proceeded by a CSYNC */
	CSYNC;
	[p0] = R0;
	SSYNC;

	/* Turn off the dcache */
	p0.l = LO(DMEM_CONTROL);
	p0.h = HI(DMEM_CONTROL);
	R1 = [p0];
	R0 = ~ENDCPLB;
	R0 = R0 & R1;

	/* Disabling of CPLBs should be proceeded by a CSYNC */
	CSYNC;
	[p0] = R0;
	SSYNC;
	/* Initialize stack pointer */
	sp.l = lo(INITIAL_STACK);
	sp.h = hi(INITIAL_STACK);
	fp = sp;
	usp = sp;

	/* This section keeps the processor in supervisor mode
	 * during core B startup.  Branches to the idle task.
	 */

	/* EVT15 = _real_start */

	p0.l = lo(EVT15);
	p0.h = hi(EVT15);
	p1.l = _coreb_start;
	p1.h = _coreb_start;
	[p0] = p1;
	csync;

	p0.l = lo(IMASK);
	p0.h = hi(IMASK);
	p1.l = IMASK_IVG15;
	p1.h = 0x0;
	[p0] = p1;
	csync;

	raise 15;
	p0.l = .LWAIT_HERE;
	p0.h = .LWAIT_HERE;
	reti = p0;
	nop; nop; nop;
	rti;

.LWAIT_HERE:
	jump .LWAIT_HERE;
ENDPROC(_coreb_trampoline_start)
ENTRY(_coreb_trampoline_end)

.section ".text"
ENTRY(_coreb_start)

	[--sp] = reti;

#ifdef CONIG_BF561
	p0.l = LO(WDOGB_CTL);
	p0.h = HI(WDOGB_CTL);
	r0 = 0xAD6(z);
	w[p0] = r0;	/* Clear the watchdog. */
	ssync;
#endif

	/*
	 * switch to IDLE stack.
	 */
	sp += -12;
	call _secondary_start_kernel;
	sp += 12;
.L_exit:
	jump.s	.L_exit;
ENDPROC(_coreb_start)

ENTRY(_evt_evt6)
	save_context_no_interrupts

	sp += -12;
	call _timer_handle;
	sp += 12;

	restore_context_no_interrupts
	csync;
	rti;
ENDPROC(_evt_evt6)

ENTRY(_evt_evt7)
	save_context_no_interrupts

	sp += -12;
	call _ipi_handler_int0;
	sp += 12;

	p0.l = _sm_task1_status;
	p0.h = _sm_task1_status;
	r0 = [p0];
	cc = r0 == SM_TASK_INIT;
	if cc jump .L_reload_task;

	restore_context_no_interrupts
	csync;
	rti;
.L_reload_task:
	/* Initialize stack pointer */
	sp.l = lo(INITIAL_STACK);
	sp.h = hi(INITIAL_STACK);
	fp = sp;
	usp = sp;
	p0.l = _coreb_icc_dispatcher;
	p0.h = _coreb_icc_dispatcher;
	reti = p0;
	rti;
ENDPROC(_evt_evt7)

ENTRY(_evt_evt11)
	save_context_no_interrupts

	sp += -12;
	call _ipi_handler_int0;
	sp += 12;
	p0.l = _sm_task1_status;
	p0.h = _sm_task1_status;
	r0 = [p0];
	cc = r0 == SM_TASK_RESET;
	if cc jump .L_reload_task1;

	restore_context_no_interrupts
	csync;
	rti;
.L_reload_task1:
	r0.l = SM_TASK_INIT;
	r0.h = 0;
	[p0] = r0;
	/* Initialize stack pointer */
	sp.l = lo(INITIAL_STACK);
	sp.h = hi(INITIAL_STACK);
	fp = sp;
	usp = sp;
	p0.l = _coreb_icc_dispatcher;
	p0.h = _coreb_icc_dispatcher;
	reti = p0;
	rti;
ENDPROC(_evt_evt11)

ENTRY(_evt_evt2)
	save_context_no_interrupts

	restore_context_no_interrupts
	csync;
	rtn;
ENDPROC(_evt_evt2)

ENTRY(_evt_evt3)
	save_context_no_interrupts

	r0 = SEQSTAT;
	r0 <<= 26;
	r0 >>= 26;
	r1 = retx;
	sp += -12;
	call _dump_execption;
	sp += 12;

	restore_context_no_interrupts
	csync;
	rtx;
ENDPROC(_evt_evt3)

ENTRY(_evt_evt5)
	save_context_no_interrupts

	restore_context_no_interrupts
	csync;
	rti;
ENDPROC(_evt_evt5)
