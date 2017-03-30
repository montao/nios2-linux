#include <asm/blackfin.h>

#ifdef CONFIG_BF609
# define SIC_SYSIRQ(irq)        ((irq) - 15)

void bfin_sec_raise_irq(unsigned int sid)
{
	bfin_write32(SEC_RAISE, sid);
}

static void bfin_sec_mask_ack_irq(unsigned int cpu, unsigned int irq)
{
	unsigned int sid = SIC_SYSIRQ(irq);

	bfin_write_SEC_SCI(cpu, SEC_CSID, sid);

}

static void bfin_sec_unmask_irq(unsigned int irq)
{
	unsigned int sid = SIC_SYSIRQ(irq);

	bfin_write32(SEC_END, sid);
}

void platform_send_ipi_cpu(unsigned int cpu, int irq)
{
	unsigned int sid = SIC_SYSIRQ(irq);

	bfin_sec_raise_irq(sid);
}

void platform_unmask_ipi(unsigned int cpu, int irq)
{
	bfin_sec_unmask_irq(irq);
}

void platform_clear_ipi(unsigned int cpu, int irq)
{
	bfin_sec_mask_ack_irq(cpu, irq);
}
#endif


#ifdef CONFIG_BF561
#include <mach/irq.h>
void platform_send_ipi_cpu(unsigned int cpu, int irq)
{
	int offset = (irq == IRQ_SUPPLE_0) ? 6 : 8;
	SSYNC();
	bfin_write_SICB_SYSCR(bfin_read_SICB_SYSCR() | (1 << (offset + cpu)));
	SSYNC();
}

void platform_unmask_ipi(unsigned int cpu, int irq)
{
}

void platform_clear_ipi(unsigned int cpu, int irq)
{
	int offset = (irq == IRQ_SUPPLE_0) ? 10 : 12;
	SSYNC();
	bfin_write_SICB_SYSCR(bfin_read_SICB_SYSCR() | (1 << (offset + cpu)));
	SSYNC();
}
#endif
