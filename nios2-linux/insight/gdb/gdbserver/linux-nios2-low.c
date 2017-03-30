/* GNU/Linux/Nios2 specific low level interface for the remote server for GDB */

#include "server.h"
#include "linux-low.h"

#ifdef HAVE_SYS_REG_H
#include <sys/reg.h>
#endif

#include <asm/ptrace.h>

static int nios2_regmap[] =
{
           -1, PTR_R1  * 4, PTR_R2  * 4, PTR_R3  * 4,
  PTR_R4  * 4, PTR_R5  * 4, PTR_R6  * 4, PTR_R7  * 4,
  PTR_R8  * 4, PTR_R9  * 4, PTR_R10 * 4, PTR_R11 * 4,
  PTR_R12 * 4, PTR_R13 * 4, PTR_R14 * 4, PTR_R15 * 4,  /* reg 15 */
  PTR_R16 * 4, PTR_R17 * 4, PTR_R18 * 4, PTR_R19 * 4,
  PTR_R20 * 4, PTR_R21 * 4, PTR_R22 * 4, PTR_R23 * 4,
           -1,          -1, PTR_GP  * 4, PTR_SP  * 4, 
  PTR_FP  * 4,          -1,          -1, PTR_RA  * 4,  /* reg 31 */
  PTR_PC  * 4,          -1,          -1,          -1,
	   -1,          -1,          -1,          -1,
	   -1,          -1,          -1,          -1
};

#define nios2_num_regs (sizeof(nios2_regmap) / sizeof(nios2_regmap[0]))

static int
nios2_cannot_store_register (int regno)
{
  return (regno >= 33);
}

static int
nios2_cannot_fetch_register (int regno)
{
  return (regno >= 33);
}

static CORE_ADDR
nios2_get_pc ()
{
  unsigned long pc;
  collect_register_by_name ("pc", &pc);
  return pc;
}

static void
nios2_set_pc (CORE_ADDR pc)
{
  unsigned long newpc = pc;
  supply_register_by_name ("pc", &newpc);
}

#if 0
static const unsigned long nios2_breakpoint = 0x003da03a;
#else
static const unsigned long nios2_breakpoint = 0x003b687a;	/* Trap instr. w/imm=0x01 */
#endif
#define nios2_breakpoint_len 4

static int
nios2_breakpoint_at (CORE_ADDR where)
{
  unsigned long insn;

  (*the_target->read_memory) (where, (char *) &insn, nios2_breakpoint_len);
  if (insn == nios2_breakpoint)
    return 1;

  /* If necessary, recognize more trap instructions here.  GDB only uses the
     one.  */
  return 0;
}

struct linux_target_ops the_low_target = {
  nios2_num_regs,
  nios2_regmap,
  nios2_cannot_fetch_register,
  nios2_cannot_store_register,
  nios2_get_pc,
  nios2_set_pc,
  (const char *) &nios2_breakpoint,
  nios2_breakpoint_len,
  NULL,
  0,
  nios2_breakpoint_at,
};
