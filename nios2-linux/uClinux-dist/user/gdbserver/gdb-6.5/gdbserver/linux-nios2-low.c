/* GNU/Linux/Nios II specific low level interface, for the remote server for
   GDB.
   Copyright (C) 1995, 1996, 1998, 1999, 2000, 2001, 2002, 2005, 2006, 2007,
   2008
   Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

#include "server.h"
#include "linux-low.h"

#include <sys/ptrace.h>
#include <endian.h>

#include "gdb_proc_service.h"

#ifndef PTRACE_GET_THREAD_AREA
#define PTRACE_GET_THREAD_AREA 25
#endif

#define nios2_num_regs 49

#include <asm/ptrace.h>

union nios2_register
{
  unsigned char buf[4];

  int reg32;
};

/* Return the ptrace ``address'' of register REGNO. */

static int nios2_regmap[] = {
  -1,  1,  2,  3,  4,  5,  6,  7,
  8,  9,  10, 11, 12, 13, 14, 15,
  16, 17, 18, 19, 20, 21, 22, 23,
  24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47,
  48,

  0
};

/* Pseudo registers can not be read.  ptrace does not provide a way to
   read (or set) PS_REGNUM, and there's no point in reading or setting
   ZERO_REGNUM.  We also can not set BADVADDR, CAUSE, or FCRIR via
   ptrace().  */

static int
nios2_cannot_fetch_register (int regno)
{
  if (nios2_regmap[regno] == -1)
    return 1;

  if (find_regno ("r0") == regno)
    return 1;

  return 0;
}

static int
nios2_cannot_store_register (int regno)
{
  if (nios2_regmap[regno] == -1)
    return 1;

  if (find_regno ("r0") == regno)
    return 1;

  return 0;
}

static CORE_ADDR
nios2_get_pc ()
{
  union nios2_register pc;
  collect_register_by_name ("pc", pc.buf);
  return pc.reg32;
}

static void
nios2_set_pc (CORE_ADDR pc)
{
  union nios2_register newpc;
  newpc.reg32 = pc;

  supply_register_by_name ("pc", newpc.buf);
}

static const unsigned int nios2_breakpoint = 0x003b6ffa;
#define nios2_breakpoint_len 4

/* We only place breakpoints in empty marker functions, and thread locking
   is outside of the function.  So rather than importing software single-step,
   we can just run until exit.  */
static CORE_ADDR
nios2_reinsert_addr ()
{
  union nios2_register ra;
  collect_register_by_name ("r31", ra.buf);
  return ra.reg32;
}

static int
nios2_breakpoint_at (CORE_ADDR where)
{
  unsigned int insn;

  (*the_target->read_memory) (where, (unsigned char *) &insn, 4);
  if (insn == nios2_breakpoint)
    return 1;

  /* If necessary, recognize more trap instructions here.  GDB only uses the
     one.  */
  return 0;
}

/* Fetch the thread-local storage pointer for libthread_db.  */

ps_err_e
ps_get_thread_area (const struct ps_prochandle *ph,
                    lwpid_t lwpid, int idx, void **base)
{
  if (ptrace (PTRACE_GET_THREAD_AREA, lwpid, NULL, base) != 0)
    return PS_ERR;

  /* IDX is the bias from the thread pointer to the beginning of the
     thread descriptor.  It has to be subtracted due to implementation
     quirks in libthread_db.  */
  *base = (void *) ((char *)*base - idx);

  return PS_OK;
}

#ifdef HAVE_PTRACE_GETREGS

static void
nios2_collect_register (int regno, union nios2_register *reg)
{
  union nios2_register tmp_reg;

  collect_register (regno, &tmp_reg.reg32);
  reg->reg32 = tmp_reg.reg32;
}

static void
nios2_supply_register (int regno,
		       const union nios2_register *reg)
{
  int offset = 0;

  supply_register (regno, reg->buf + offset);
}

static void
nios2_fill_gregset (void *buf)
{
  union nios2_register *regset = buf;
  int i;

  for (i = 1; i < 49; i++)
    nios2_collect_register (i, regset + i);
}

static void
nios2_store_gregset (const void *buf)
{
  const union nios2_register *regset = buf;
  int i;

  for (i = 0; i < 49; i++)
    nios2_supply_register (i, regset + i);
}
#endif /* HAVE_PTRACE_GETREGS */

struct regset_info target_regsets[] = {
#ifdef HAVE_PTRACE_GETREGS
  { PTRACE_GETREGS, PTRACE_SETREGS, 49 * 4, GENERAL_REGS,
    nios2_fill_gregset, nios2_store_gregset },
#endif /* HAVE_PTRACE_GETREGS */
  { 0, 0, -1, -1, NULL, NULL }
};

struct linux_target_ops the_low_target = {
  nios2_num_regs,
  nios2_regmap,
  nios2_cannot_fetch_register,
  nios2_cannot_store_register,
  nios2_get_pc,
  nios2_set_pc,
  (const unsigned char *) &nios2_breakpoint,
  nios2_breakpoint_len,
  nios2_reinsert_addr,
  0,
  nios2_breakpoint_at,
};
