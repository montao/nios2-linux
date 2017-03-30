#include "interrupt_handler.h"

void ps2_port_isr()
{
	//we simply read the data when there is an interrupt at the PS2 port
	alt_u32 data_reg = 0;
	alt_u16 num = 0;
	do {
		data_reg = read_data_reg();
		num = get_num_bytes_available(data_reg);
	} while (num > 0);
}

void disable_interrupt()
{
	NIOS2_WRITE_IENABLE(0x00);
	NIOS2_WRITE_STATUS(0x00);
	set_ctrl_reg(0x00);
}

void interrupt_init()
{
	//enable the interrupt enable register inside NIOS II
	NIOS2_WRITE_IENABLE(PS2_INTERRUPT_MASK | JTAG_UART_INTERRUPT_MASK);
	NIOS2_WRITE_STATUS(0x01);
	//enable read enable register for the PS2 port
	set_ctrl_reg(CTRL_RE_REG_MASK);
}

void interrupt_handler(void)
/*****************************************************************************/
/* Interrupt Service Routine                                                 */
/*   Determines what caused the interrupt and calls the appropriate          */
/*  subroutine.                                                              */
/*                                                                           */
/* ipending - Control register 4 which has the pending external interrupts   */
/*****************************************************************************/
{
	int ipending;

	NIOS2_READ_IPENDING(ipending);

	if ((ipending & PS2_INTERRUPT_MASK) == PS2_INTERRUPT_MASK)
	{
		ps2_port_isr();
	}

	return;
}




void the_isr (void) __attribute__ ((section ("isr")));
void the_isr (void)
/*****************************************************************************/
/* Interrupt Service Routine                                                 */
/*   Calls the interrupt handler and performs return from exception.         */
/*****************************************************************************/
{
asm (".set		noat");
asm (".set		nobreak");
asm (	"subi	sp,  sp, 128");
asm (	"subi	sp,  sp, 128");

asm (	"stw	et,  96(sp)");

asm (	"rdctl	et,  ctl4");
asm (	"beq	et,  r0, SKIP_EA_DEC");	/* Interrupt is not external         */

asm (	"subi	ea,  ea, 4");		/* Must decrement ea by one instruction  */
									/*   for external interupts, so that the */
									/*   interrupted instruction will be run */

asm ("SKIP_EA_DEC:");
asm (	"stw	r1,  4(sp)");		/* Save all registers on the Stack       */
asm (	"stw	r2,  8(sp)");
asm (	"stw	r3,  12(sp)");
asm (	"stw	r4,  16(sp)");
asm (	"stw	r5,  20(sp)");
asm (	"stw	r6,  24(sp)");
asm (	"stw	r7,  28(sp)");
asm (	"stw	r8,  32(sp)");
asm (	"stw	r9,  36(sp)");
asm (	"stw	r10, 40(sp)");
asm (	"stw	r11, 44(sp)");
asm (	"stw	r12, 48(sp)");
asm (	"stw	r13, 52(sp)");
asm (	"stw	r14, 56(sp)");
asm (	"stw	r15, 60(sp)");
asm (	"stw	r16, 64(sp)");
asm (	"stw	r17, 68(sp)");
asm (	"stw	r18, 72(sp)");
asm (	"stw	r19, 76(sp)");
asm (	"stw	r20, 80(sp)");
asm (	"stw	r21, 84(sp)");
asm (	"stw	r22, 88(sp)");
asm (	"stw	r23, 92(sp)");
//asm (	"stw	r24, 96(sp)");		/* et (r24) is already on the stack      */
asm (	"stw	r25, 100(sp)");
asm (	"stw	r26, 104(sp)");
asm (	"stw	r27, 108(sp)");
asm (	"stw	r28, 112(sp)");
asm (	"stw	r29, 116(sp)");
asm (	"stw	r30, 120(sp)");
asm (	"stw	r31, 124(sp)");
asm (	"addi	fp,  sp, 128");

asm (	"call	interrupt_handler");/* Call the C language interrupt handler */

asm (	"ldw	r1,  4(sp)");		/* Restore all register                  */
asm (	"ldw	r2,  8(sp)");
asm (	"ldw	r3,  12(sp)");
asm (	"ldw	r4,  16(sp)");
asm (	"ldw	r5,  20(sp)");
asm (	"ldw	r6,  24(sp)");
asm (	"ldw	r7,  28(sp)");
asm (	"ldw	r8,  32(sp)");
asm (	"ldw	r9,  36(sp)");
asm (	"ldw	r10, 40(sp)");
asm (	"ldw	r11, 44(sp)");
asm (	"ldw	r12, 48(sp)");
asm (	"ldw	r13, 52(sp)");
asm (	"ldw	r14, 56(sp)");
asm (	"ldw	r15, 60(sp)");
asm (	"ldw	r16, 64(sp)");
asm (	"ldw	r17, 68(sp)");
asm (	"ldw	r18, 72(sp)");
asm (	"ldw	r19, 76(sp)");
asm (	"ldw	r20, 80(sp)");
asm (	"ldw	r21, 84(sp)");
asm (	"ldw	r22, 88(sp)");
asm (	"ldw	r23, 92(sp)");
asm (	"ldw	r24, 96(sp)");
asm (	"ldw	r25, 100(sp)");
asm (	"ldw	r26, 104(sp)");
asm (	"ldw	r27, 108(sp)");
asm (	"ldw	r28, 112(sp)");
asm (	"ldw	r29, 116(sp)");
asm (	"ldw	r30, 120(sp)");
asm (	"ldw	r31, 124(sp)");

asm (	"addi	sp,  sp, 128");

asm (	"eret");

}
