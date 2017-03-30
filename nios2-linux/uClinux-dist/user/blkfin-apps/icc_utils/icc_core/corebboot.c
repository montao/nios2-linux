/*
 * IPI management based on arch/arm/kernel/smp.c (Copyright 2002 ARM Limited)
 *
 * Copyright 2007-2009 Analog Devices Inc.
 *                         Philippe Gerum <rpm@xenomai.org>
 *
 * Licensed under the GPL-2.
 */

#include <generated/autoconf.h>
#include <linux/linkage.h>
#include <linux/types.h>

#include <asm/icc.h>

#include <asm/cplb.h>
#include <asm/irqflags.h>

#include <asm/blackfin.h>

#include <protocol.h>
#include <debug.h>

extern uint16_t pending;
extern struct coreb_icc_node coreb_info;

static inline void coreb_idle(void)
{
	__asm__ __volatile__( \
			".align 8;" \
			"nop;"  \
			"nop;"  \
			"idle;" \
			: \
			:  \
			);
}

bool get_mem16(unsigned short *val, unsigned short *address)
{
	unsigned long addr = (unsigned long)address;

	*val = *address;
	return true;
}

bool get_instruction(unsigned int *val, unsigned short *address)
{
	unsigned long addr = (unsigned long)address;
	unsigned short opcode0, opcode1;

	/* Check for odd addresses */
	if (addr & 0x1)
		return false; 

	/* MMR region will never have instructions */
	if (addr >= SYSMMR_BASE) {
		return false;
	}

	if (addr < COREB_TASK_START) {
		return false;
	}

	if ((addr >= COREB_L1_CODE_START) && (addr <= COREB_L1_CODE_START + L1_CODE_LENGTH))
		return false;

	if (!get_mem16(&opcode0, address))
		return false;

	/* was this a 32-bit instruction? If so, get the next 16 bits */
	if ((opcode0 & 0xc000) == 0xc000) {
		if (!get_mem16(&opcode1, address + 1))
			return false;
		*val = (opcode0 << 16) + opcode1;
	} else
		*val = opcode0;

	return true;
}

static bool is_bfin_call(unsigned short *addr)
{
	unsigned int opcode;

	if (!get_instruction(&opcode, addr))
		return false;

	if ((opcode >= 0x0060 && opcode <= 0x0067) ||
			(opcode >= 0x0070 && opcode <= 0x0077) ||
			(opcode >= 0xE3000000 && opcode <= 0xE3FFFFFF))
		return true;

	return false;
}

static inline void *get_sp(void)
{
	void *sp;
	__asm__("%0 = sp;" : "=da"(sp));
	return sp;
}

#define INITIAL_STACK   (COREB_L1_SCRATCH_START + L1_SCRATCH_LENGTH - 12)
void coreb_dump_stack(unsigned int errno, unsigned int addr)
{
	void *stack, *stackend;
	unsigned int *p, *fp = 0;
	unsigned short *ins, *ret_addr;
	int frame_no = 0;

	coreb_msg("execption %x addr %x, ipend=0x%x\n", errno, addr, readipend());

	stack = get_sp();
	stackend = INITIAL_STACK;
	coreb_msg("coreb dump stack\n");
	for (p = stack; p <= stackend; p++) {
		if (*p & 0x1)
			continue;
		ins = (unsigned short *)*p;
		ins--;
		if (is_bfin_call(ins)) {
			fp = p - 1;
			coreb_msg("found fp: %08x\n", fp);
			while (fp < stackend && fp) {
				fp = (unsigned int *)*fp;
			}
			if (fp == stackend) {
				fp = p - 1;
				break;
			}

		}

	}

	if (!fp) {
		coreb_msg("no frame found!!\n");
		return;
	}

	ret_addr = 0;
	for (p = stack; p <= stackend; p++) {
		if (*p & 0x1)
			continue;
		if (frame_no > 5)
			while(1)
				continue;

		ins = (unsigned short *)*p;
		ins--;
		if (is_bfin_call(ins)) {
			coreb_msg("call function [%08x]\n", ins);
			coreb_msg("call ret address %08x\n", *p);
		} else if (p == fp){
			if (fp == stackend) {
				fp = p - 1;
				break;
			}
			frame_no++;
			coreb_msg("call frame %d\n", frame_no);
			fp = *fp;
		} else
			coreb_msg(" call frame %d -%d %08x\n", frame_no, (fp - p), *p);

	}
}

extern void evt_evt7(void );
extern void evt_evt11(void );
extern void evt_evt6(void );
extern void evt_evt2(void );
extern void evt_evt3(void );
extern void evt_evt5(void );

#define blackfin_core_id() (bfin_dspid() & 0xff)

enum irqreturn {
	IRQ_NONE=0,
	IRQ_HANDLED,
	IRQ_WAKE_THREAD,
};

typedef enum irqreturn irqreturn_t;


extern unsigned long mcc_arg;
extern int iccq_should_stop;

uint16_t intcnt;

#define BFIN_IPI_RESCHEDULE   0
#define BFIN_IPI_CALL_FUNC    1
#define BFIN_IPI_CPU_STOP     2

#if !CONFIG_BFIN_EXTMEM_WRITETHROUGH
# error need CONFIG_BFIN_EXTMEM_WRITETHROUGH
#endif
extern int vsprintf(char *buf, const char *fmt, va_list args);


void udelay(uint32_t count)
{
	while(count--);
}

void delay(uint32_t count)
{
	uint32_t ncount = 30 * count;
	while(ncount--)
		udelay(10000);
}

void *memcpy(void *dest, const void *src, size_t count)
{
	char *tmp = dest;
	const char *s = src;
	if (!dest || !src) {
		coreb_msg("BUG dst %08x src %08x\n", dest, src);
		return NULL;
	}

	while (count--)
		*tmp++ = *s++;
	return dest;
}

void *memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}

char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s;
}


void bfin_coretmr_init(void)
{
	/* power up the timer, but don't enable it just yet */
	bfin_write_TCNTL(TMPWR);
	CSYNC();

	/* the TSCALE prescaler counter. */
	bfin_write_TSCALE(0);
	bfin_write_TPERIOD(0);
	bfin_write_TCOUNT(0);

	CSYNC();
}

int bfin_coretmr_set_next_event(unsigned long cycles)
{
	bfin_write_TCNTL(TMPWR);
	CSYNC();
	bfin_write_TCOUNT(cycles);
	CSYNC();
	bfin_write_TCNTL(TMPWR | TMREN);
	return 0;
}

#ifdef DEBUG
int coreb_debug_level = 2;
static unsigned char debug_buf[DEBUG_MSG_BUF_SIZE];
void coreb_msg(char *fmt, ...)
{
	va_list args;
	int i;
	uint16_t sent, received, pending;
	unsigned long flags = bfin_cli();
	char buf[DEBUG_MSG_LINE] = "COREB: ";
	va_start(args, fmt);
	i = vsprintf(buf + 7, fmt, args);
	va_end(args);
	struct sm_message_queue *queue = (struct sm_message_queue *)MSGQ_START_ADDR;
	struct sm_msg *msg = &queue->messages[0];
	sent = sm_atomic_read(&queue->sent);
	received = sm_atomic_read(&queue->received);
	void *p = (void *)debug_buf + (sent % SM_MSGQ_LEN) * DEBUG_MSG_LINE;

	pending = sent - received;
	if (pending < 0)
		pending += USHRT_MAX;
	while(pending >= (SM_MSGQ_LEN - 1)) {
		delay(1);
		sent = sm_atomic_read(&queue->sent);
		received = sm_atomic_read(&queue->received);
		pending = sent - received;
		if (pending < 0)
			pending += USHRT_MAX;
	}

	memset(p, 0, DEBUG_MSG_LINE);
	SSYNC();
	strcpy(p, buf);
	memset(&msg[sent%SM_MSGQ_LEN], 0, sizeof(struct sm_msg));
	msg[(sent % SM_MSGQ_LEN)].type = SM_BAD_MSG;
	msg[(sent % SM_MSGQ_LEN)].dst_ep = received;
	msg[(sent % SM_MSGQ_LEN)].src_ep = (sent + 1);
	msg[(sent % SM_MSGQ_LEN)].payload = p;
	sent++;
	sm_atomic_write(&queue->sent, sent);
	SSYNC();
	platform_send_ipi_cpu(0, COREB_ICC_LOW_SEND);
	bfin_sti(flags);
}
#endif

void dump_execption(unsigned int errno, unsigned int addr)
{
	if (errno == 0x26) {
		unsigned long fault_addr = bfin_read_DCPLB_FAULT_ADDR();

		if (fault_addr >= 0x8000000)
			coreb_dump_stack(errno, addr);
		_disable_dcplb();
		fault_addr &= ~(0x400000 - 1);
		bfin_write32(DCPLB_ADDR1 + 4 * ((fault_addr/ 0x1000000) % 15), fault_addr);
		bfin_write32(DCPLB_DATA1 + 4 * ((fault_addr/ 0x1000000) % 15), (CPLB_COMMON | PAGE_SIZE_4MB));
		_enable_dcplb();
	} else if (errno == 0x2c) {
		unsigned long fault_addr = bfin_read_ICPLB_FAULT_ADDR();

		if (fault_addr >= 0x8000000)
			coreb_dump_stack(errno, addr);
		_disable_icplb();
		fault_addr &= ~(0x400000 - 1);
		bfin_write32(ICPLB_ADDR1 + 4 * ((fault_addr/ 0x1000000) % 15), fault_addr);
		bfin_write32(ICPLB_DATA1 + 4 * ((fault_addr/ 0x1000000) % 15), ((SDRAM_IGENERIC & ~CPLB_L1_CHBL) | PAGE_SIZE_4MB));
		_enable_icplb();
	} else {
		coreb_dump_stack(errno, addr);
	}
}

void init_exception_vectors(void)
{
        /* cannot program in software:
         * evt0 - emulation (jtag)
         * evt1 - reset
         */
	/* ipi evt */
	bfin_write_EVT7(evt_evt7);
	bfin_write_EVT11(evt_evt11);
	bfin_write_EVT6(evt_evt6);
	bfin_write_EVT2(evt_evt2);
	bfin_write_EVT3(evt_evt3);
	bfin_write_EVT5(evt_evt5);
	CSYNC();

}

void platform_secondary_init(void)
{
#ifdef CONFIG_BF561
        bfin_write_SICB_IMASK0(bfin_read_SIC_IMASK0());
        bfin_write_SICB_IMASK1(bfin_read_SIC_IMASK1());
        SSYNC();

        /* Clone setup for IARs from CoreA. */
        bfin_write_SICB_IAR0(bfin_read_SIC_IAR0());
        bfin_write_SICB_IAR1(bfin_read_SIC_IAR1());
        bfin_write_SICB_IAR2(bfin_read_SIC_IAR2());
        bfin_write_SICB_IAR3(bfin_read_SIC_IAR3());
        bfin_write_SICB_IAR4(bfin_read_SIC_IAR4());
        bfin_write_SICB_IAR5(bfin_read_SIC_IAR5());
        bfin_write_SICB_IAR6(bfin_read_SIC_IAR6());
        bfin_write_SICB_IAR7(bfin_read_SIC_IAR7());
        bfin_write_SICB_IWR0(IWR_DISABLE_ALL);
        bfin_write_SICB_IWR1(0xC0000000);
        SSYNC();
#endif
}


/* Use IRQ_SUPPLE_0 to request reschedule.
 * When returning from interrupt to user space,
 * there is chance to reschedule */
irqreturn_t ipi_handler_int0(int irq, void *dev_instance)
{
	uint32_t cpu = blackfin_core_id();
	++intcnt;

	platform_clear_ipi(cpu, COREB_ICC_LOW_RECV);
	pending = iccqueue_getpending();
	sm_handle_control_message();
	platform_unmask_ipi(cpu, COREB_ICC_LOW_RECV);
	return IRQ_HANDLED;
}

irqreturn_t timer_handle(int irq, void *dev_instance)
{
	pending = iccqueue_getpending();
	return IRQ_HANDLED;
}

static void setup_secondary()
{
	unsigned long ilat;
	unsigned long bfin_irq_flags;

	bfin_write_IMASK(0);
	CSYNC();
	ilat = bfin_read_ILAT();
	CSYNC();
	bfin_write_ILAT(ilat);
	CSYNC();

	/* Enable interrupt levels IVG7. IARs have been already
	 * programmed by the boot CPU.  */
#ifdef CONFIG_BF561
	bfin_irq_flags = IMASK_IVG7| IMASK_IVGTMR;
#else
	bfin_irq_flags = IMASK_IVG11| IMASK_IVGTMR;
#endif
	bfin_sti(bfin_irq_flags);
	SSYNC();
}

inline int readipend(void)
{
	int _tmp;
	__asm__ __volatile__(
		"P1.H = ((0xffe02108>>16) & 0xFFFF);"
		"P1.L = (0xffe02108 & 0xFFFF);"
		"%0 = [P1];"
		: "=r" (_tmp) ::"P1"
	);
	return _tmp;
}


void bfin_setup_caches(unsigned int cpu)
{
	unsigned long addr;
	int i;

	addr = 4 * 1024 * 1024;
	i = 0;


	bfin_write32(ICPLB_ADDR0 + i * 4, L2_START);
	bfin_write32(ICPLB_DATA0 + i * 4, (CPLB_COMMON | PAGE_SIZE_1MB));
	bfin_write32(DCPLB_ADDR0 + i * 4, L2_START);
	bfin_write32(DCPLB_DATA0 + i * 4, (CPLB_COMMON | PAGE_SIZE_1MB));
	i++;

	for(i = 1; i < 16; i++) {
		bfin_write32(ICPLB_ADDR0 + i * 4, addr + (i - 1) * 4 * 1024 * 1024);
		bfin_write32(ICPLB_DATA0 + i * 4 ,((SDRAM_IGENERIC & ~CPLB_L1_CHBL) | PAGE_SIZE_4MB));
		bfin_write32(DCPLB_ADDR0 + i * 4, addr + (i - 1) * 4 * 1024 * 1024);
		bfin_write32(DCPLB_DATA0 + i * 4, (CPLB_COMMON | PAGE_SIZE_4MB));
	}
	_enable_cplb(IMEM_CONTROL, (IMC | ENICPLB));

	delay(1);

//	coreb_msg("IMEM %X \n", (IMC | ENICPLB));

	_enable_cplb(DMEM_CONTROL, (DMEM_CNTR | PORT_PREF0 | PORT_PREF1 ));

}

void icc_run_task(void);
void coreb_icc_dispatcher(void)
{
	while (1) {
		while (iccq_should_stop) {
			/*to do drop no control messages*/
			coreb_idle();
		}
		icc_wait(0);
		pending = iccqueue_getpending();
		sm_handle_control_message();
	}
}

void icc_init(void)
{
	struct gen_pool *pool;

	memset(COREB_MEMPOOL_START , 0, 0x100000 * 2);
	pool = gen_pool_create(12);
	if (!pool)
		coreb_msg("@@@ create 4k pool failed\n");
	coreb_info.pool = pool;
	if (gen_pool_add(pool, COREB_MEMPOOL_START, (1 << 12) * 256))
		coreb_msg("@@@add chunk fail\n");

	pool = gen_pool_create(6);
	if (!pool)
		coreb_msg("@@@ create msg pool failed\n");
	coreb_info.msg_pool = pool;
	if (gen_pool_add(pool, COREB_MEMPOOL_START + (1 << 12) * 256 , (1 << 6) * 64))
		coreb_msg("@@@add chunk fail\n");


	coreb_info.icc_info.icc_queue = (struct sm_message_queue *)MSGQ_START_ADDR +
				(blackfin_core_id() - 1) * MSGQ_SIZE;
	coreb_info.icc_info.icc_high_queue = coreb_info.icc_info.icc_queue + 2;
	coreb_info.icc_info.peer_cpu = 0;
	init_sm_session_table();
	register_sm_proto();
}

void secondary_start_kernel(void)
{
	init_exception_vectors();
	SSYNC();

	setup_secondary();

	platform_secondary_init();

	bfin_setup_caches(blackfin_core_id());

	bfin_coretmr_init();

	icc_init();

	coreb_icc_dispatcher();
}

