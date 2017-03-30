/* Subroutines for assembler code output for Altera NIOS 2G NIOS2 version.
   Copyright (C) 2005 Altera
   Contributed by Jonah Graham (jgraham@altera.com), Will Reece (wreece@altera.com),
   and Jeff DaSilva (jdasilva@altera.com).

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


#include <stdio.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "tm_p.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "recog.h"
#include "expr.h"
#include "toplev.h"
#include "basic-block.h"
#include "function.h"
#include "ggc.h"
#include "reload.h"
#include "debug.h"
#include "optabs.h"
#include "target.h"
#include "target-def.h"
#include "c-pragma.h"           /* for c_register_pragma */
#include "cpplib.h"             /* for CPP_NUMBER */

/* local prototypes */
static bool nios2_rtx_costs (rtx, int, int, int *);

static void nios2_asm_function_prologue (FILE *, HOST_WIDE_INT);
static int nios2_use_dfa_pipeline_interface (void);
static int nios2_issue_rate (void);
static struct machine_function *nios2_init_machine_status (void);
static bool nios2_in_small_data_p (tree);
static rtx save_reg (int, HOST_WIDE_INT, rtx);
static rtx restore_reg (int, HOST_WIDE_INT);
static unsigned int nios2_section_type_flags (tree, const char *, int);

/* 0 --> no #pragma seen
   1 --> in scope of #pragma reverse_bitfields
   -1 --> in scope of #pragma no_reverse_bitfields */
static int nios2_pragma_reverse_bitfields_flag = 0;
static void nios2_pragma_reverse_bitfields (struct cpp_reader *);
static void nios2_pragma_no_reverse_bitfields (struct cpp_reader *);
static tree nios2_handle_struct_attribute (tree *, tree, tree, int, bool *);
static void nios2_insert_attributes (tree, tree *);
static bool nios2_reverse_bitfield_layout_p (tree record_type);
static void nios2_init_builtins (void);
static rtx nios2_expand_builtin (tree, rtx, rtx, enum machine_mode, int);
static bool nios2_function_ok_for_sibcall (tree, tree);
static void nios2_encode_section_info (tree, rtx, int);

/* Initialize the GCC target structure.  */
#undef TARGET_ASM_FUNCTION_PROLOGUE
#define TARGET_ASM_FUNCTION_PROLOGUE nios2_asm_function_prologue

#undef TARGET_SCHED_USE_DFA_PIPELINE_INTERFACE
#define TARGET_SCHED_USE_DFA_PIPELINE_INTERFACE \
 nios2_use_dfa_pipeline_interface
#undef TARGET_SCHED_ISSUE_RATE
#define TARGET_SCHED_ISSUE_RATE nios2_issue_rate
#undef TARGET_IN_SMALL_DATA_P
#define TARGET_IN_SMALL_DATA_P nios2_in_small_data_p
#undef  TARGET_ENCODE_SECTION_INFO
#define TARGET_ENCODE_SECTION_INFO nios2_encode_section_info
#undef  TARGET_SECTION_TYPE_FLAGS
#define TARGET_SECTION_TYPE_FLAGS  nios2_section_type_flags

#undef TARGET_REVERSE_BITFIELD_LAYOUT_P
#define TARGET_REVERSE_BITFIELD_LAYOUT_P nios2_reverse_bitfield_layout_p

#undef TARGET_INIT_BUILTINS
#define TARGET_INIT_BUILTINS nios2_init_builtins
#undef TARGET_EXPAND_BUILTIN
#define TARGET_EXPAND_BUILTIN nios2_expand_builtin

#undef TARGET_FUNCTION_OK_FOR_SIBCALL
#define TARGET_FUNCTION_OK_FOR_SIBCALL nios2_function_ok_for_sibcall

#undef TARGET_RTX_COSTS
#define TARGET_RTX_COSTS nios2_rtx_costs

const struct attribute_spec nios2_attribute_table[] =
{
  /* { name, min_len, max_len, decl_req, type_req, fn_type_req, handler } */
  { "reverse_bitfields",    0, 0, false, false,  false, nios2_handle_struct_attribute },
  { "no_reverse_bitfields", 0, 0, false, false,  false, nios2_handle_struct_attribute },
  { "pragma_reverse_bitfields",    0, 0, false, false,  false, NULL },
  { "pragma_no_reverse_bitfields", 0, 0, false, false,  false, NULL },
  { NULL,        0, 0, false, false, false, NULL }
};

#undef TARGET_ATTRIBUTE_TABLE
#define TARGET_ATTRIBUTE_TABLE nios2_attribute_table

#undef  TARGET_INSERT_ATTRIBUTES
#define TARGET_INSERT_ATTRIBUTES nios2_insert_attributes

/* ??? Might want to redefine TARGET_RETURN_IN_MSB here to handle
   big-endian case; depends on what ABI we choose. */

struct gcc_target targetm = TARGET_INITIALIZER;



/* Threshold for data being put into the small data/bss area, instead
   of the normal data area (references to the small data/bss area take
   1 instruction, and use the global pointer, references to the normal
   data area takes 2 instructions).  */
unsigned HOST_WIDE_INT nios2_section_threshold = NIOS2_DEFAULT_GVALUE;


/* Structure to be filled in by compute_frame_size with register
   save masks, and offsets for the current function.  */

struct nios2_frame_info
GTY (())
{
  long total_size;		/* # bytes that the entire frame takes up */
  long var_size;		/* # bytes that variables take up */
  long args_size;		/* # bytes that outgoing arguments take up */
  int save_reg_size;		/* # bytes needed to store gp regs */
  int save_reg_rounded;		/* # bytes needed to store gp regs */
  long save_regs_offset;	/* offset from new sp to store gp registers */
  int initialized;		/* != 0 if frame size already calculated */
  int num_regs;			/* number of gp registers saved */
};

struct machine_function
GTY (())
{

  /* Current frame information, calculated by compute_frame_size.  */
  struct nios2_frame_info frame;
};


/***************************************
 * Register Classes
 ***************************************/

enum reg_class 
reg_class_from_constraint (char chr, const char *str)
{
  if (chr == 'D' && ISDIGIT (str[1]) && ISDIGIT (str[2]))
    {
      int regno;
      int ones = str[2] - '0';
      int tens = str[1] - '0';
      
      regno = ones + (10 * tens);
      if (regno < 0 || regno > 31)
        return NO_REGS;

      return D00_REG + regno;
    }

  return NO_REGS;
}


/***************************************
 * Stack Layout and Calling Conventions
 ***************************************/


#define TOO_BIG_OFFSET(X) ((X) > ((1 << 15) - 1))
#define TEMP_REG_NUM 8

static void
nios2_asm_function_prologue (FILE *file, HOST_WIDE_INT size ATTRIBUTE_UNUSED)
{
  if (flag_verbose_asm || flag_debug_asm)
    {
      compute_frame_size ();
      dump_frame_size (file);
    }
}

static rtx
save_reg (int regno, HOST_WIDE_INT offset, rtx cfa_store_reg)
{
  rtx insn, stack_slot;

  stack_slot = gen_rtx_PLUS (SImode,
			     cfa_store_reg,
			     GEN_INT (offset));

  insn = emit_insn (gen_rtx_SET (SImode,
				 gen_rtx_MEM (SImode, stack_slot),
				 gen_rtx_REG (SImode, regno)));

  RTX_FRAME_RELATED_P (insn) = 1;

  return insn;
}

static rtx
restore_reg (int regno, HOST_WIDE_INT offset)
{
  rtx insn, stack_slot;

  if (TOO_BIG_OFFSET (offset))
    {
      stack_slot = gen_rtx_REG (SImode, TEMP_REG_NUM);
      insn = emit_insn (gen_rtx_SET (SImode,
				     stack_slot,
				     GEN_INT (offset)));

      insn = emit_insn (gen_rtx_SET (SImode,
				     stack_slot,
                                     gen_rtx_PLUS (SImode,
				                   stack_slot,
				                   stack_pointer_rtx)));
    }
  else
    {
      stack_slot = gen_rtx_PLUS (SImode,
			         stack_pointer_rtx,
				 GEN_INT (offset));
    }

  stack_slot = gen_rtx_MEM (SImode, stack_slot);

  insn = emit_move_insn (gen_rtx_REG (SImode, regno), stack_slot);

  return insn;
}


/* There are two possible paths for prologue expansion,
- the first is if the total frame size is < 2^15-1. In that
case all the immediates will fit into the 16-bit immediate
fields.
- the second is when the frame size is too big, in that
case an additional temporary register is used, first 
as a cfa_temp to offset the sp, second as the cfa_store
register.

See the comment above dwarf2out_frame_debug_expr in 
dwarf2out.c for more explanation of the "rules."


Case 1:
Rule #  Example Insn                       Effect
2  	addi	sp, sp, -total_frame_size  cfa.reg=sp, cfa.offset=total_frame_size
                                           cfa_store.reg=sp, cfa_store.offset=total_frame_size
12  	stw	ra, offset(sp)		   
12  	stw	r16, offset(sp)

12      stw     fp, fp2sp-offset(sp)
1       addi    fp, sp, -fp2sp-offset
  
Case 2: 
Rule #  Example Insn                       Effect
6 	movi	r8, total_frame_size       cfa_temp.reg=r8, cfa_temp.offset=total_frame_size
2  	sub	sp, sp, r8                 cfa.reg=sp, cfa.offset=total_frame_size
                                           cfa_store.reg=sp, cfa_store.offset=total_frame_size
5   	add	r8, r8, sp                 cfa_store.reg=r8, cfa_store.offset=0
12  	stw	ra, offset(r8)
12  	stw	r16, offset(r8)


12       stw     fp, fp2r8-offset(r8)
         addi    fp, r8, -fp2r8-offset

*/

void
expand_prologue ()
{
  int i;
  HOST_WIDE_INT total_frame_size;
  int cfa_store_offset = 0;
  HOST_WIDE_INT sp_fp_offset = 0;
  rtx insn;
  rtx cfa_store_reg = 0;

  total_frame_size = compute_frame_size ();

  if (total_frame_size)
    {

      if (TOO_BIG_OFFSET (total_frame_size)) 
	{
	    /* cfa_temp and cfa_store_reg are the same register,
	       cfa_store_reg overwrites cfa_temp */
	    cfa_store_reg = gen_rtx_REG (SImode, TEMP_REG_NUM);
	    insn = emit_insn (gen_rtx_SET (SImode,
					   cfa_store_reg,
					   GEN_INT (total_frame_size)));

	    RTX_FRAME_RELATED_P (insn) = 1;


	    insn = gen_rtx_SET (SImode,
				stack_pointer_rtx,
				gen_rtx_MINUS (SImode,
					       stack_pointer_rtx,
					       cfa_store_reg));

	    insn = emit_insn (insn);
	    RTX_FRAME_RELATED_P (insn) = 1;


	    /* if there are no registers to save, I don't need to
	       create a cfa_store */
	    if (cfun->machine->frame.save_reg_size) 
	      {
		insn = gen_rtx_SET (SImode,
				    cfa_store_reg,
				    gen_rtx_PLUS (SImode,
						  cfa_store_reg,
						  stack_pointer_rtx));

		insn = emit_insn (insn);
		RTX_FRAME_RELATED_P (insn) = 1;
	      }

	    cfa_store_offset 
	      = total_frame_size 
		- (cfun->machine->frame.save_regs_offset
		   + cfun->machine->frame.save_reg_rounded);
	}
      else
	{
	    insn = gen_rtx_SET (SImode,
				stack_pointer_rtx,
				gen_rtx_PLUS (SImode,
					      stack_pointer_rtx,
					      GEN_INT (-total_frame_size)));
	    insn = emit_insn (insn);
	    RTX_FRAME_RELATED_P (insn) = 1;

	    cfa_store_reg = stack_pointer_rtx;
	    cfa_store_offset 
	      = cfun->machine->frame.save_regs_offset
		+ cfun->machine->frame.save_reg_rounded;
	}

      if (current_function_limit_stack)
	{
	  emit_insn (gen_stack_overflow_detect_and_trap ());
	}
    }

  if (MUST_SAVE_REGISTER (RA_REGNO))
    {
      cfa_store_offset -= 4;
      save_reg (RA_REGNO, cfa_store_offset, cfa_store_reg);
    }
  if (MUST_SAVE_REGISTER (FP_REGNO))
    {
      cfa_store_offset -= 4;
      save_reg (FP_REGNO, cfa_store_offset, cfa_store_reg);

      sp_fp_offset = cfa_store_offset;
    }

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (MUST_SAVE_REGISTER (i) && i != FP_REGNO && i != RA_REGNO)
	{
	  cfa_store_offset -= 4;
	  save_reg (i, cfa_store_offset, cfa_store_reg);
	}
    }

  if (frame_pointer_needed)
    {
      insn = gen_rtx_SET (SImode,
				     gen_rtx_REG (SImode, FP_REGNO),
                          gen_rtx_PLUS (SImode,
                                        cfa_store_reg,
                                        GEN_INT (sp_fp_offset)));

      insn = emit_insn (insn);

      if (cfa_store_reg == stack_pointer_rtx) 
        {
      RTX_FRAME_RELATED_P (insn) = 1;
    }
    }

  /* If we are profiling, make sure no instructions are scheduled before
     the call to mcount.  */
  if (current_function_profile)
    emit_insn (gen_blockage ());
}

void
expand_epilogue (bool sibcall_p)
{
  rtx insn;
  int i;
  HOST_WIDE_INT total_frame_size;
  int register_store_offset;

  total_frame_size = compute_frame_size ();

  if (!sibcall_p && nios2_can_use_return_insn ())
    {
      insn = emit_jump_insn (gen_return ());
      return;
    }

  emit_insn (gen_blockage ());

  register_store_offset =
    cfun->machine->frame.save_regs_offset +
    cfun->machine->frame.save_reg_rounded;

  if (MUST_SAVE_REGISTER (RA_REGNO))
    {
      register_store_offset -= 4;
      restore_reg (RA_REGNO, register_store_offset);
    }

  if (MUST_SAVE_REGISTER (FP_REGNO))
    {
      register_store_offset -= 4;
      restore_reg (FP_REGNO, register_store_offset);
    }

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    {
      if (MUST_SAVE_REGISTER (i) && i != FP_REGNO && i != RA_REGNO)
	{
	  register_store_offset -= 4;
	  restore_reg (i, register_store_offset);
	}
    }

  if (total_frame_size)
    {

      rtx sp_adjust;

      if (TOO_BIG_OFFSET (total_frame_size))
        {
	  sp_adjust = gen_rtx_REG (SImode, TEMP_REG_NUM);
	  insn = emit_insn (gen_rtx_SET (SImode,
					 sp_adjust,
					 GEN_INT (total_frame_size)));

      	}
      else
        {
	  sp_adjust = GEN_INT (total_frame_size);
	}

      insn = gen_rtx_SET (SImode,
			  stack_pointer_rtx,
			  gen_rtx_PLUS (SImode,
					stack_pointer_rtx,
					sp_adjust));
      insn = emit_insn (insn);
    }


  if (!sibcall_p)
    {
      insn = emit_jump_insn (gen_return_from_epilogue (gen_rtx (REG, Pmode,
								RA_REGNO)));
    }
}


bool
nios2_function_ok_for_sibcall (tree a ATTRIBUTE_UNUSED, tree b ATTRIBUTE_UNUSED)
{
  return true;
}





/* ----------------------- *
 * Profiling
 * ----------------------- */

void
function_profiler (FILE *file, int labelno ATTRIBUTE_UNUSED)
{
  fprintf (file, "\tmov\tr8, ra\n");
  fprintf (file, "\tcall\tmcount\n");
  fprintf (file, "\tmov\tra, r8\n");
}


/***************************************
 * Stack Layout
 ***************************************/


void
dump_frame_size (FILE *file)
{
  fprintf (file, "\t%s Current Frame Info\n", ASM_COMMENT_START);

  fprintf (file, "\t%s total_size = %ld\n", ASM_COMMENT_START,
	   cfun->machine->frame.total_size);
  fprintf (file, "\t%s var_size = %ld\n", ASM_COMMENT_START,
	   cfun->machine->frame.var_size);
  fprintf (file, "\t%s args_size = %ld\n", ASM_COMMENT_START,
	   cfun->machine->frame.args_size);
  fprintf (file, "\t%s save_reg_size = %d\n", ASM_COMMENT_START,
	   cfun->machine->frame.save_reg_size);
  fprintf (file, "\t%s save_reg_rounded = %d\n", ASM_COMMENT_START,
	   cfun->machine->frame.save_reg_rounded);
  fprintf (file, "\t%s initialized = %d\n", ASM_COMMENT_START,
	   cfun->machine->frame.initialized);
  fprintf (file, "\t%s num_regs = %d\n", ASM_COMMENT_START,
	   cfun->machine->frame.num_regs);
  fprintf (file, "\t%s save_regs_offset = %ld\n", ASM_COMMENT_START,
	   cfun->machine->frame.save_regs_offset);
  fprintf (file, "\t%s current_function_is_leaf = %d\n", ASM_COMMENT_START,
	   current_function_is_leaf);
  fprintf (file, "\t%s frame_pointer_needed = %d\n", ASM_COMMENT_START,
	   frame_pointer_needed);
  fprintf (file, "\t%s pretend_args_size = %d\n", ASM_COMMENT_START,
	   current_function_pretend_args_size);

}


/* Return the bytes needed to compute the frame pointer from the current
   stack pointer.
*/

HOST_WIDE_INT
compute_frame_size ()
{
  unsigned int regno;
  HOST_WIDE_INT var_size;	/* # of var. bytes allocated */
  HOST_WIDE_INT total_size;	/* # bytes that the entire frame takes up */
  HOST_WIDE_INT save_reg_size;	/* # bytes needed to store callee save regs */
  HOST_WIDE_INT save_reg_rounded;	
    /* # bytes needed to store callee save regs (rounded) */
  HOST_WIDE_INT out_args_size;	/* # bytes needed for outgoing args */

  save_reg_size = 0;
  var_size = STACK_ALIGN (get_frame_size ());
  out_args_size = STACK_ALIGN (current_function_outgoing_args_size);

  total_size = var_size + out_args_size;

  /* Calculate space needed for gp registers.  */
  for (regno = 0; regno <= FIRST_PSEUDO_REGISTER; regno++)
    {
      if (MUST_SAVE_REGISTER (regno))
	{
	  save_reg_size += 4;
	}
    }

  save_reg_rounded = STACK_ALIGN (save_reg_size);
  total_size += save_reg_rounded;

  total_size += STACK_ALIGN (current_function_pretend_args_size);

  /* Save other computed information.  */
  cfun->machine->frame.total_size = total_size;
  cfun->machine->frame.var_size = var_size;
  cfun->machine->frame.args_size = current_function_outgoing_args_size;
  cfun->machine->frame.save_reg_size = save_reg_size;
  cfun->machine->frame.save_reg_rounded = save_reg_rounded;
  cfun->machine->frame.initialized = reload_completed;
  cfun->machine->frame.num_regs = save_reg_size / UNITS_PER_WORD;

  cfun->machine->frame.save_regs_offset
    = save_reg_rounded ? current_function_outgoing_args_size + var_size : 0;

  return total_size;
}


int
nios2_initial_elimination_offset (int from, int to)
{
  int offset;

    compute_frame_size ();

  /* Set OFFSET to the offset from the stack pointer.  */
  switch (from)
    {
    case FRAME_POINTER_REGNUM:

      offset = 0;

      break;

    case ARG_POINTER_REGNUM:

      offset = cfun->machine->frame.total_size;
      offset -= current_function_pretend_args_size;

      break;

    case RETURN_ADDRESS_POINTER_REGNUM:
      /* since the return address is always the first of the
         saved registers, return the offset to the beginning
         of the saved registers block */

      offset = cfun->machine->frame.save_regs_offset;


      break;

    default:
      abort ();
    }

    /* If we are asked for the frame pointer offset, then adjust OFFSET
       by the offset from the frame pointer to the stack pointer.  */
    if( to == HARD_FRAME_POINTER_REGNUM )
    {
        int fp_to_sp_offset;

        // Get The FP to SP offset
        fp_to_sp_offset = (cfun->machine->frame.save_regs_offset
                        + cfun->machine->frame.save_reg_rounded - 4) * (-1);

        if (MUST_SAVE_REGISTER (RA_REGNO))
          {
            fp_to_sp_offset += 4;
          }

        offset += fp_to_sp_offset;

    } /*else to == STACK_POINTER_REGNUM */


  return offset;
}

/* Return nonzero if this function is known to have a null epilogue.
   This allows the optimizer to omit jumps to jumps if no stack
   was created.  */
int
nios2_can_use_return_insn ()
{
  if (!reload_completed)
    return 0;

  if (regs_ever_live[RA_REGNO] || current_function_profile)
    return 0;

  if (cfun->machine->frame.initialized)
    return cfun->machine->frame.total_size == 0;

  return compute_frame_size () == 0;
}





/***************************************
 *
 ***************************************/

/*
 * Try to take a bit of tedium out of the __builtin_custom_<blah>
 * builtin functions, too.
 */

#define NIOS2_FOR_ALL_CUSTOM_BUILTINS \
  NIOS2_DO_BUILTIN (N,    n,    n    ) \
  NIOS2_DO_BUILTIN (NI,   ni,   nX   ) \
  NIOS2_DO_BUILTIN (NF,   nf,   nX   ) \
  NIOS2_DO_BUILTIN (NP,   np,   nX   ) \
  NIOS2_DO_BUILTIN (NII,  nii,  nXX  ) \
  NIOS2_DO_BUILTIN (NIF,  nif,  nXX  ) \
  NIOS2_DO_BUILTIN (NIP,  nip,  nXX  ) \
  NIOS2_DO_BUILTIN (NFI,  nfi,  nXX  ) \
  NIOS2_DO_BUILTIN (NFF,  nff,  nXX  ) \
  NIOS2_DO_BUILTIN (NFP,  nfp,  nXX  ) \
  NIOS2_DO_BUILTIN (NPI,  npi,  nXX  ) \
  NIOS2_DO_BUILTIN (NPF,  npf,  nXX  ) \
  NIOS2_DO_BUILTIN (NPP,  npp,  nXX  ) \
  NIOS2_DO_BUILTIN (IN,   in,   Xn   ) \
  NIOS2_DO_BUILTIN (INI,  ini,  XnX  ) \
  NIOS2_DO_BUILTIN (INF,  inf,  XnX  ) \
  NIOS2_DO_BUILTIN (INP,  inp,  XnX  ) \
  NIOS2_DO_BUILTIN (INII, inii, XnXX ) \
  NIOS2_DO_BUILTIN (INIF, inif, XnXX ) \
  NIOS2_DO_BUILTIN (INIP, inip, XnXX ) \
  NIOS2_DO_BUILTIN (INFI, infi, XnXX ) \
  NIOS2_DO_BUILTIN (INFF, inff, XnXX ) \
  NIOS2_DO_BUILTIN (INFP, infp, XnXX ) \
  NIOS2_DO_BUILTIN (INPI, inpi, XnXX ) \
  NIOS2_DO_BUILTIN (INPF, inpf, XnXX ) \
  NIOS2_DO_BUILTIN (INPP, inpp, XnXX ) \
  NIOS2_DO_BUILTIN (FN,   fn,   Xn   ) \
  NIOS2_DO_BUILTIN (FNI,  fni,  XnX  ) \
  NIOS2_DO_BUILTIN (FNF,  fnf,  XnX  ) \
  NIOS2_DO_BUILTIN (FNP,  fnp,  XnX  ) \
  NIOS2_DO_BUILTIN (FNII, fnii, XnXX ) \
  NIOS2_DO_BUILTIN (FNIF, fnif, XnXX ) \
  NIOS2_DO_BUILTIN (FNIP, fnip, XnXX ) \
  NIOS2_DO_BUILTIN (FNFI, fnfi, XnXX ) \
  NIOS2_DO_BUILTIN (FNFF, fnff, XnXX ) \
  NIOS2_DO_BUILTIN (FNFP, fnfp, XnXX ) \
  NIOS2_DO_BUILTIN (FNPI, fnpi, XnXX ) \
  NIOS2_DO_BUILTIN (FNPF, fnpf, XnXX ) \
  NIOS2_DO_BUILTIN (FNPP, fnpp, XnXX ) \
  NIOS2_DO_BUILTIN (PN,   pn,   Xn   ) \
  NIOS2_DO_BUILTIN (PNI,  pni,  XnX  ) \
  NIOS2_DO_BUILTIN (PNF,  pnf,  XnX  ) \
  NIOS2_DO_BUILTIN (PNP,  pnp,  XnX  ) \
  NIOS2_DO_BUILTIN (PNII, pnii, XnXX ) \
  NIOS2_DO_BUILTIN (PNIF, pnif, XnXX ) \
  NIOS2_DO_BUILTIN (PNIP, pnip, XnXX ) \
  NIOS2_DO_BUILTIN (PNFI, pnfi, XnXX ) \
  NIOS2_DO_BUILTIN (PNFF, pnff, XnXX ) \
  NIOS2_DO_BUILTIN (PNFP, pnfp, XnXX ) \
  NIOS2_DO_BUILTIN (PNPI, pnpi, XnXX ) \
  NIOS2_DO_BUILTIN (PNPF, pnpf, XnXX ) \
  NIOS2_DO_BUILTIN (PNPP, pnpp, XnXX )

const char *nios2_sys_nosys_string;    /* for -msys=nosys */
const char *nios2_sys_lib_string;    /* for -msys-lib= */
const char *nios2_sys_crt0_string;    /* for -msys-crt0= */

#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
static const char *NIOS2_CONCAT (nios2_output_fpu_insn_, insn) (rtx); \
static void NIOS2_CONCAT (nios2_pragma_, insn) (struct cpp_reader *); \
static void NIOS2_CONCAT (nios2_pragma_no_, insn) (struct cpp_reader *);
NIOS2_FOR_ALL_FPU_INSNS

nios2_fpu_info nios2_fpu_insns[nios2_fpu_max_insn] = {
#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
  { NIOS2_STRINGIFY (opt), \
    NIOS2_STRINGIFY (insn), \
    NIOS2_STRINGIFY (args), \
    0, \
    -1, \
    NIOS2_CONCAT (nios2_output_fpu_insn_, insn), \
    "custom_" NIOS2_STRINGIFY (opt), \
    NIOS2_CONCAT (nios2_pragma_, insn), \
    "no_custom_" NIOS2_STRINGIFY (opt), \
    NIOS2_CONCAT (nios2_pragma_no_, insn), \
    0, \
    0, \
    0, \
    0, \
    0 },
  NIOS2_FOR_ALL_FPU_INSNS
};

const char *nios2_custom_fpu_cfg_string;

static const char *builtin_custom_seen[256];

static void
nios2_custom_switch (const char *parameter, int *value, const char *opt)
{
  /*
   * We only document values from 0-255, but we secretly allow -1 so
   * that the -mno-custom-<opt> switches work.
   */
  if (parameter && *parameter)
    {
      char *endptr;
      long v = strtol (parameter, &endptr, 0);
      if (*endptr)
        {
          error ("switch `-mcustom-%s' value `%s' must be a number between 0 and 255",
                 opt, parameter);
        }
      if (v < -1 || v > 255)
        {
          error ("switch `-mcustom-%s' value %ld must be between 0 and 255",
                 opt, v);
        }
      *value = (int)v;
    }
}

static void
nios2_custom_check_insns (int is_pragma)
{
  int i;
  int has_double = 0;
  int errors = 0;
  const char *ns[256];
  int ps[256];

  for (i = 0; i < nios2_fpu_max_insn; i++)
    {
      if (nios2_fpu_insns[i].is_double && nios2_fpu_insns[i].N >= 0)
        {
          has_double = 1;
        }
    }

  if (has_double)
    {
      for (i = 0; i < nios2_fpu_max_insn; i++)
        {
          if (nios2_fpu_insns[i].needed_by_double
              && nios2_fpu_insns[i].N < 0)
            {
              if (is_pragma)
                {
                  error ("either switch `-mcustom-%s' or `#pragma custom_%s' is required for double precision floating point",
                         nios2_fpu_insns[i].option,
                         nios2_fpu_insns[i].option);
                }
              else
                {
                  error ("switch `-mcustom-%s' is required for double precision floating point",
                         nios2_fpu_insns[i].option);
                }
              errors = 1;
            }
        }
    }

  /*
   * Warn if the user has certain exotic operations that won't get used
   * without -funsafe-math-optimizations, See expand_builtin () in
   * bulitins.c.
   */
  if (!flag_unsafe_math_optimizations)
    {
      for (i = 0; i < nios2_fpu_max_insn; i++)
        {
          if (nios2_fpu_insns[i].needs_unsafe && nios2_fpu_insns[i].N >= 0)
            {
              warning ("%s%s' has no effect unless -funsafe-math-optimizations is specified",
                       is_pragma ? "`#pragma custom_" : "switch `-mcustom-",
                       nios2_fpu_insns[i].option);
              /* Just one warning per function per compilation unit, please. */
              nios2_fpu_insns[i].needs_unsafe = 0;
            }
        }
    }

  /*
   * Warn if the user is trying to use -mcustom-fmins et. al, that won't
   * get used without -ffinite-math-only.  See fold in fold () in
   * fold-const.c
   */
  if (!flag_finite_math_only)
    {
      for (i = 0; i < nios2_fpu_max_insn; i++)
        {
          if (nios2_fpu_insns[i].needs_finite && nios2_fpu_insns[i].N >= 0)
            {
              warning ("%s%s' has no effect unless -ffinite-math-only is specified",
                       is_pragma ? "`#pragma custom_" : "switch `-mcustom-",
                       nios2_fpu_insns[i].option);
              /* Just one warning per function per compilation unit, please. */
              nios2_fpu_insns[i].needs_finite = 0;
            }
        }
    }

  /*
   * Warn the user about double precision divide braindamage until we
   * can fix it properly.  See the RDIV_EXPR case of expand_expr_real in
   * expr.c.
   */
  {
    static int warned = 0;
    if (flag_unsafe_math_optimizations
        && !optimize_size
        && nios2_fpu_insns[nios2_fpu_divdf3].N >= 0
        && !warned)
      {
        warning ("%s%s' behaves poorly without -Os",
                 is_pragma ? "`#pragma custom_" : "switch `-mcustom-",
                 nios2_fpu_insns[nios2_fpu_divdf3].option);
        warned = 1;
      }
  }

  /*
   * The following bit of voodoo is lifted from the generated file
   * insn-opinit.c: to allow #pragmas to work properly, we have to tweak
   * the optab_table manually -- it only gets initialized once after the
   * switches are handled and before any #pragmas are seen.
   */
  if (is_pragma)
    {
      /* Only do this if the optabs have already been defined, not
         when we're handling command line switches. */
      addv_optab->handlers[SFmode].insn_code =
      add_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      addv_optab->handlers[DFmode].insn_code =
      add_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      subv_optab->handlers[SFmode].insn_code =
      sub_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      subv_optab->handlers[DFmode].insn_code =
      sub_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      smulv_optab->handlers[SFmode].insn_code =
      smul_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      smulv_optab->handlers[DFmode].insn_code =
      smul_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      sdiv_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      sdiv_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      negv_optab->handlers[SFmode].insn_code =
      neg_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      negv_optab->handlers[DFmode].insn_code =
      neg_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      smin_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      smin_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      smax_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      smax_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      absv_optab->handlers[SFmode].insn_code =
      abs_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      absv_optab->handlers[DFmode].insn_code =
      abs_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      sqrt_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      sqrt_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      cos_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      cos_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      sin_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      sin_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      tan_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      tan_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      atan_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      atan_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      exp_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      exp_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      log_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      log_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;
      sfloat_optab->handlers[SFmode][SImode].insn_code = CODE_FOR_nothing;
      sfloat_optab->handlers[DFmode][SImode].insn_code = CODE_FOR_nothing;
      ufloat_optab->handlers[SFmode][SImode].insn_code = CODE_FOR_nothing;
      ufloat_optab->handlers[DFmode][SImode].insn_code = CODE_FOR_nothing;
      sfix_optab->handlers[SImode][SFmode].insn_code = CODE_FOR_nothing;
      sfix_optab->handlers[SImode][DFmode].insn_code = CODE_FOR_nothing;
      ufix_optab->handlers[SImode][SFmode].insn_code = CODE_FOR_nothing;
      ufix_optab->handlers[SImode][DFmode].insn_code = CODE_FOR_nothing;
      sext_optab->handlers[DFmode][SFmode].insn_code = CODE_FOR_nothing;
      trunc_optab->handlers[SFmode][DFmode].insn_code = CODE_FOR_nothing;
      cmp_optab->handlers[SFmode].insn_code = CODE_FOR_nothing;
      cmp_optab->handlers[DFmode].insn_code = CODE_FOR_nothing;

      if (HAVE_addsf3)
        addv_optab->handlers[SFmode].insn_code =
        add_optab->handlers[SFmode].insn_code = CODE_FOR_addsf3;
      if (HAVE_adddf3)
        addv_optab->handlers[DFmode].insn_code =
        add_optab->handlers[DFmode].insn_code = CODE_FOR_adddf3;
      if (HAVE_subsf3)
        subv_optab->handlers[SFmode].insn_code =
        sub_optab->handlers[SFmode].insn_code = CODE_FOR_subsf3;
      if (HAVE_subdf3)
        subv_optab->handlers[DFmode].insn_code =
        sub_optab->handlers[DFmode].insn_code = CODE_FOR_subdf3;
      if (HAVE_mulsf3)
        smulv_optab->handlers[SFmode].insn_code =
        smul_optab->handlers[SFmode].insn_code = CODE_FOR_mulsf3;
      if (HAVE_muldf3)
        smulv_optab->handlers[DFmode].insn_code =
        smul_optab->handlers[DFmode].insn_code = CODE_FOR_muldf3;
      if (HAVE_divsf3)
        sdiv_optab->handlers[SFmode].insn_code = CODE_FOR_divsf3;
      if (HAVE_divdf3)
        sdiv_optab->handlers[DFmode].insn_code = CODE_FOR_divdf3;
      if (HAVE_negsf2)
        negv_optab->handlers[SFmode].insn_code =
        neg_optab->handlers[SFmode].insn_code = CODE_FOR_negsf2;
      if (HAVE_negdf2)
        negv_optab->handlers[DFmode].insn_code =
        neg_optab->handlers[DFmode].insn_code = CODE_FOR_negdf2;
      if (HAVE_minsf3)
        smin_optab->handlers[SFmode].insn_code = CODE_FOR_minsf3;
      if (HAVE_mindf3)
        smin_optab->handlers[DFmode].insn_code = CODE_FOR_mindf3;
      if (HAVE_maxsf3)
        smax_optab->handlers[SFmode].insn_code = CODE_FOR_maxsf3;
      if (HAVE_maxdf3)
        smax_optab->handlers[DFmode].insn_code = CODE_FOR_maxdf3;
      if (HAVE_abssf2)
        absv_optab->handlers[SFmode].insn_code =
        abs_optab->handlers[SFmode].insn_code = CODE_FOR_abssf2;
      if (HAVE_absdf2)
        absv_optab->handlers[DFmode].insn_code =
        abs_optab->handlers[DFmode].insn_code = CODE_FOR_absdf2;
      if (HAVE_sqrtsf2)
        sqrt_optab->handlers[SFmode].insn_code = CODE_FOR_sqrtsf2;
      if (HAVE_sqrtdf2)
        sqrt_optab->handlers[DFmode].insn_code = CODE_FOR_sqrtdf2;
      if (HAVE_cossf2)
        cos_optab->handlers[SFmode].insn_code = CODE_FOR_cossf2;
      if (HAVE_cosdf2)
        cos_optab->handlers[DFmode].insn_code = CODE_FOR_cosdf2;
      if (HAVE_sinsf2)
        sin_optab->handlers[SFmode].insn_code = CODE_FOR_sinsf2;
      if (HAVE_sindf2)
        sin_optab->handlers[DFmode].insn_code = CODE_FOR_sindf2;
      if (HAVE_tansf2)
        tan_optab->handlers[SFmode].insn_code = CODE_FOR_tansf2;
      if (HAVE_tandf2)
        tan_optab->handlers[DFmode].insn_code = CODE_FOR_tandf2;
      if (HAVE_atansf2)
        atan_optab->handlers[SFmode].insn_code = CODE_FOR_atansf2;
      if (HAVE_atandf2)
        atan_optab->handlers[DFmode].insn_code = CODE_FOR_atandf2;
      if (HAVE_expsf2)
        exp_optab->handlers[SFmode].insn_code = CODE_FOR_expsf2;
      if (HAVE_expdf2)
        exp_optab->handlers[DFmode].insn_code = CODE_FOR_expdf2;
      if (HAVE_logsf2)
        log_optab->handlers[SFmode].insn_code = CODE_FOR_logsf2;
      if (HAVE_logdf2)
        log_optab->handlers[DFmode].insn_code = CODE_FOR_logdf2;
      if (HAVE_floatsisf2)
        sfloat_optab->handlers[SFmode][SImode].insn_code = CODE_FOR_floatsisf2;
      if (HAVE_floatsidf2)
        sfloat_optab->handlers[DFmode][SImode].insn_code = CODE_FOR_floatsidf2;
      if (HAVE_floatunssisf2)
        ufloat_optab->handlers[SFmode][SImode].insn_code = CODE_FOR_floatunssisf2;
      if (HAVE_floatunssidf2)
        ufloat_optab->handlers[DFmode][SImode].insn_code = CODE_FOR_floatunssidf2;
      if (HAVE_fixsfsi2)
        sfix_optab->handlers[SImode][SFmode].insn_code = CODE_FOR_fixsfsi2;
      if (HAVE_fixdfsi2)
        sfix_optab->handlers[SImode][DFmode].insn_code = CODE_FOR_fixdfsi2;
      if (HAVE_fixunssfsi2)
        ufix_optab->handlers[SImode][SFmode].insn_code = CODE_FOR_fixunssfsi2;
      if (HAVE_fixunsdfsi2)
        ufix_optab->handlers[SImode][DFmode].insn_code = CODE_FOR_fixunsdfsi2;
      if (HAVE_extendsfdf2)
        sext_optab->handlers[DFmode][SFmode].insn_code = CODE_FOR_extendsfdf2;
      if (HAVE_truncdfsf2)
        trunc_optab->handlers[SFmode][DFmode].insn_code = CODE_FOR_truncdfsf2;
      if (HAVE_cmpsf)
        cmp_optab->handlers[SFmode].insn_code = CODE_FOR_cmpsf;
      if (HAVE_cmpdf)
        cmp_optab->handlers[DFmode].insn_code = CODE_FOR_cmpdf;
    }

  /* Check for duplicate values of N */
  for (i = 0; i < 256; i++)
    {
      ns[i] = 0;
      ps[i] = 0;
    }

  for (i = 0; i < nios2_fpu_max_insn; i++)
    {
      int N = nios2_fpu_insns[i].N;
      if (N >= 0)
        {
          if (ns[N])
            {
              error ("%s%s' conflicts with %s%s'",
                     is_pragma ? "`#pragma custom_" : "switch `-mcustom-",
                     nios2_fpu_insns[i].option,
                     ps[N] ? "`#pragma custom_" : "switch `-mcustom-",
                     ns[N]);
              errors = 1;
            }
          else if (builtin_custom_seen[N])
            {
              error ("call to `%s' conflicts with %s%s'",
                     builtin_custom_seen[N],
                     (nios2_fpu_insns[i].pragma_seen
                      ? "`#pragma custom_" : "switch `-mcustom-"),
                     nios2_fpu_insns[i].option);
              errors = 1;
            }
          else
            {
              ns[N] = nios2_fpu_insns[i].option;
              ps[N] = nios2_fpu_insns[i].pragma_seen;
            }
        }
    }

  if (errors)
    {
      fatal_error ("conflicting use of -mcustom switches, #pragmas, and/or __builtin_custom_ functions");
    }
}

static void
nios2_handle_custom_fpu_cfg (const char *cfg, int is_pragma)
{
#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
  int opt = nios2_fpu_insns[NIOS2_CONCAT (nios2_fpu_, insn)].N;
NIOS2_FOR_ALL_FPU_INSNS

  /*
   * ??? These are just some sample possibilities.  We'll change these
   * at the last minute to match the capabilities of the actual fpu.
   */
  if (!strcasecmp (cfg, "60-1"))
    {
      fmuls = 252;
      fadds = 253;
      fsubs = 254;
      flag_single_precision_constant = 1;
    }
  else if (!strcasecmp (cfg, "60-2"))
    {
      fmuls = 252;
      fadds = 253;
      fsubs = 254;
      fdivs = 255;
      flag_single_precision_constant = 1;
    }
  else if (!strcasecmp (cfg, "72-3"))
    {
      floatus = 243;
      fixsi   = 244;
      floatis = 245;
      fcmpgts = 246;
      fcmples = 249;
      fcmpeqs = 250;
      fcmpnes = 251;
      fmuls   = 252;
      fadds   = 253;
      fsubs   = 254;
      fdivs   = 255;
      flag_single_precision_constant = 1;
    }
  else
    {
      warning ("ignoring unrecognized %sfpu-cfg' value `%s'",
               is_pragma ? "`#pragma custom_" : "switch -mcustom-", cfg);
    }

#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
  nios2_fpu_insns[NIOS2_CONCAT (nios2_fpu_, insn)].N = opt;
NIOS2_FOR_ALL_FPU_INSNS

  /* Guard against errors in the standard configurations. */
  nios2_custom_check_insns (is_pragma);
}

void
override_options ()
{
  int i;

  /* Function to allocate machine-dependent function status.  */
  init_machine_status = &nios2_init_machine_status;

  nios2_section_threshold 
    = g_switch_set ? g_switch_value : NIOS2_DEFAULT_GVALUE;

  if (nios2_sys_nosys_string && *nios2_sys_nosys_string)
    {
      error ("invalid option '-msys=nosys%s'", nios2_sys_nosys_string);
    }

  /* If we don't have mul, we don't have mulx either! */
  if (!TARGET_HAS_MUL && TARGET_HAS_MULX) 
    {
      target_flags &= ~HAS_MULX_FLAG;
    }

  /* Set up for stack limit checking */
  if (TARGET_STACK_CHECK)
    {
      stack_limit_rtx = gen_rtx_REG(SImode, ET_REGNO);
    }
  
  for (i = 0; i < nios2_fpu_max_insn; i++)
    {
      nios2_fpu_insns[i].is_double = (nios2_fpu_insns[i].args[0] == 'd'
                                      || nios2_fpu_insns[i].args[0] == 'd'
                                      || nios2_fpu_insns[i].args[0] == 'd');
      nios2_fpu_insns[i].needed_by_double = (i == nios2_fpu_nios2_fwrx
                                             || i == nios2_fpu_nios2_fwry
                                             || i == nios2_fpu_nios2_frdxlo
                                             || i == nios2_fpu_nios2_frdxhi
                                             || i == nios2_fpu_nios2_frdy);
      nios2_fpu_insns[i].needs_unsafe = (i == nios2_fpu_cossf2
                                         || i == nios2_fpu_cosdf2
                                         || i == nios2_fpu_sinsf2
                                         || i == nios2_fpu_sindf2
                                         || i == nios2_fpu_tansf2
                                         || i == nios2_fpu_tandf2
                                         || i == nios2_fpu_atansf2
                                         || i == nios2_fpu_atandf2
                                         || i == nios2_fpu_expsf2
                                         || i == nios2_fpu_expdf2
                                         || i == nios2_fpu_logsf2
                                         || i == nios2_fpu_logdf2);
      nios2_fpu_insns[i].needs_finite = (i == nios2_fpu_minsf3
                                         || i == nios2_fpu_maxsf3
                                         || i == nios2_fpu_mindf3
                                         || i == nios2_fpu_maxdf3);
    }

  /*
   * We haven't seen any __builtin_custom functions yet.
   */
  for (i = 0; i < 256; i++)
    {
      builtin_custom_seen[i] = 0;
    }

  /*
   * Set up default handling for floating point custom instructions.
   *
   * Putting things in this order means that the -mcustom-fpu-cfg=
   * switch will always be overridden by individual -mcustom-fadds=
   * switches, regardless of the order in which they were specified
   * on the command line.  ??? Remember to document this.
   */
  if (nios2_custom_fpu_cfg_string && *nios2_custom_fpu_cfg_string)
    {
      nios2_handle_custom_fpu_cfg (nios2_custom_fpu_cfg_string, 0);
    }

  for (i = 0; i < nios2_fpu_max_insn; i++)
    {
      nios2_custom_switch (nios2_fpu_insns[i].value,
                           &nios2_fpu_insns[i].N,
                           nios2_fpu_insns[i].option);
    }

  nios2_custom_check_insns (0);
}

void
optimization_options (int level, int size)
{
  if (level || size)
    {
      target_flags |= INLINE_MEMCPY_FLAG;
    }

  if (level >= 3 && !size)
    {
      target_flags |= FAST_SW_DIV_FLAG;
    }
}

/* Allocate a chunk of memory for per-function machine-dependent data.  */
static struct machine_function *
nios2_init_machine_status ()
{
  return ((struct machine_function *)
	  ggc_alloc_cleared (sizeof (struct machine_function)));
}



/*****************
 * Describing Relative Costs of Operations
 *****************/

/* Compute a (partial) cost for rtx X.  Return true if the complete
   cost has been computed, and false if subexpressions should be
   scanned.  In either case, *TOTAL contains the cost result.  */



static bool
nios2_rtx_costs (rtx x, int code, int outer_code ATTRIBUTE_UNUSED, int *total)
{
  switch (code)
    {
      case CONST_INT:
	if (INTVAL (x) == 0)
	  {
	    *total = COSTS_N_INSNS (0);
	    return true;
	  }
	else if (SMALL_INT (INTVAL (x))
		|| SMALL_INT_UNSIGNED (INTVAL (x))
		|| UPPER16_INT (INTVAL (x)))
	  {
	    *total = COSTS_N_INSNS (2);
	    return true;
	  }
	else
	  {
	    *total = COSTS_N_INSNS (4);
	    return true;
	  }

      case LABEL_REF:
      case SYMBOL_REF:
	/* ??? gp relative stuff will fit in here */
	/* fall through */
      case CONST:
      case CONST_DOUBLE:
	{
	  *total = COSTS_N_INSNS (4);
	  return true;
	}

      case MULT:
	{
	  *total = COSTS_N_INSNS (1);
	  return false;
	}
      case SIGN_EXTEND:
	{
	  *total = COSTS_N_INSNS (3);
	  return false;
	}
      case ZERO_EXTEND:
	{
	  *total = COSTS_N_INSNS (1);
	  return false;
	}

    default:
      return false;
    }
}


/***************************************
 * INSTRUCTION SUPPORT
 *
 * These functions are used within the Machine Description to
 * handle common or complicated output and expansions from
 * instructions.
 ***************************************/

int
nios2_emit_move_sequence (rtx *operands, enum machine_mode mode)
{
  rtx to = operands[0];
  rtx from = operands[1];

  if (!register_operand (to, mode) && !reg_or_0_operand (from, mode))
    {
      if (no_new_pseudos)
	internal_error ("Trying to force_reg no_new_pseudos == 1");
      from = copy_to_mode_reg (mode, from);
    }

  operands[0] = to;
  operands[1] = from;
  return 0;
}

/* Divide Support */

/*
  If -O3 is used, we want to output a table lookup for
  divides between small numbers (both num and den >= 0
  and < 0x10). The overhead of this method in the worse
  case is 40 bytes in the text section (10 insns) and
  256 bytes in the data section. Additional divides do
  not incur additional penalties in the data section.

  Code speed is improved for small divides by about 5x
  when using this method in the worse case (~9 cycles
  vs ~45). And in the worse case divides not within the
  table are penalized by about 10% (~5 cycles vs ~45).
  However in the typical case the penalty is not as bad
  because doing the long divide in only 45 cycles is
  quite optimistic.

  ??? It would be nice to have some benchmarks other
  than Dhrystone to back this up.

  This bit of expansion is to create this instruction
  sequence as rtl.
	or	$8, $4, $5
	slli	$9, $4, 4
	cmpgeui	$3, $8, 16
	beq	$3, $0, .L3
	or	$10, $9, $5
	add	$12, $11, divide_table
	ldbu	$2, 0($12)
	br	.L1
.L3:
	call	slow_div
.L1:
#	continue here with result in $2

  ??? Ideally I would like the emit libcall block to contain
  all of this code, but I don't know how to do that. What it
  means is that if the divide can be eliminated, it may not
  completely disappear.

  ??? The __divsi3_table label should ideally be moved out
  of this block and into a global. If it is placed into the
  sdata section we can save even more cycles by doing things
  gp relative.
*/
int
nios2_emit_expensive_div (rtx *operands, enum machine_mode mode)
{
  rtx or_result, shift_left_result;
  rtx lookup_value;
  rtx lab1, lab3;
  rtx insns;
  rtx libfunc;
  rtx final_result;
  rtx tmp;

  /* it may look a little generic, but only SImode
     is supported for now */
  if (mode != SImode)
    abort ();

  libfunc = sdiv_optab->handlers[(int) SImode].libfunc;



  lab1 = gen_label_rtx ();
  lab3 = gen_label_rtx ();

  or_result = expand_simple_binop (SImode, IOR,
				   operands[1], operands[2],
				   0, 0, OPTAB_LIB_WIDEN);

  emit_cmp_and_jump_insns (or_result, GEN_INT (15), GTU, 0,
			   GET_MODE (or_result), 0, lab3);
  JUMP_LABEL (get_last_insn ()) = lab3;

  shift_left_result = expand_simple_binop (SImode, ASHIFT,
					   operands[1], GEN_INT (4),
					   0, 0, OPTAB_LIB_WIDEN);

  lookup_value = expand_simple_binop (SImode, IOR,
				      shift_left_result, operands[2],
				      0, 0, OPTAB_LIB_WIDEN);

  convert_move (operands[0],
		gen_rtx (MEM, QImode,
			 gen_rtx (PLUS, SImode,
				  lookup_value,
				  gen_rtx_SYMBOL_REF (SImode, "__divsi3_table"))),
		1);


  tmp = emit_jump_insn (gen_jump (lab1));
  JUMP_LABEL (tmp) = lab1;
  emit_barrier ();

  emit_label (lab3);
  LABEL_NUSES (lab3) = 1;

  start_sequence ();
  final_result = emit_library_call_value (libfunc, NULL_RTX,
					  LCT_CONST, SImode, 2,
					  operands[1], SImode,
					  operands[2], SImode);


  insns = get_insns ();
  end_sequence ();
  emit_libcall_block (insns, operands[0], final_result,
		      gen_rtx (DIV, SImode, operands[1], operands[2]));

  emit_label (lab1);
  LABEL_NUSES (lab1) = 1;
  return 1;
}

/* Branches/Compares */

/* the way of handling branches/compares
   in gcc is heavily borrowed from MIPS */

enum internal_test
{
  ITEST_EQ,
  ITEST_NE,
  ITEST_GT,
  ITEST_GE,
  ITEST_LT,
  ITEST_LE,
  ITEST_GTU,
  ITEST_GEU,
  ITEST_LTU,
  ITEST_LEU,
  ITEST_MAX
};

static enum internal_test map_test_to_internal_test (enum rtx_code);

/* Cached operands, and operator to compare for use in set/branch/trap
   on condition codes.  */
rtx branch_cmp[2];
enum cmp_type branch_type;

/* Make normal rtx_code into something we can index from an array */

static enum internal_test
map_test_to_internal_test (enum rtx_code test_code)
{
  enum internal_test test = ITEST_MAX;

  switch (test_code)
    {
    case EQ:
      test = ITEST_EQ;
      break;
    case NE:
      test = ITEST_NE;
      break;
    case GT:
      test = ITEST_GT;
      break;
    case GE:
      test = ITEST_GE;
      break;
    case LT:
      test = ITEST_LT;
      break;
    case LE:
      test = ITEST_LE;
      break;
    case GTU:
      test = ITEST_GTU;
      break;
    case GEU:
      test = ITEST_GEU;
      break;
    case LTU:
      test = ITEST_LTU;
      break;
    case LEU:
      test = ITEST_LEU;
      break;
    default:
      break;
    }

  return test;
}

bool have_nios2_fpu_cmp_insn( enum rtx_code cond_t, enum cmp_type cmp_t );
enum rtx_code get_reverse_cond(enum rtx_code cond_t);

bool
have_nios2_fpu_cmp_insn( enum rtx_code cond_t, enum cmp_type cmp_t )
{
  if (cmp_t == CMP_SF)
    {
      switch (cond_t) {
      case EQ: 
        return (nios2_fpu_insns[nios2_fpu_nios2_seqsf].N >= 0);
      case NE: 
        return (nios2_fpu_insns[nios2_fpu_nios2_snesf].N >= 0);
      case GT: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sgtsf].N >= 0);
      case GE: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sgesf].N >= 0);
      case LT: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sltsf].N >= 0);
      case LE: 
        return (nios2_fpu_insns[nios2_fpu_nios2_slesf].N >= 0);
      default: 
        break;
      }
    }
  else if (cmp_t == CMP_DF)
    {
      switch (cond_t) {
      case EQ: 
        return (nios2_fpu_insns[nios2_fpu_nios2_seqdf].N >= 0);
      case NE: 
        return (nios2_fpu_insns[nios2_fpu_nios2_snedf].N >= 0);
      case GT: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sgtdf].N >= 0);
      case GE: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sgedf].N >= 0);
      case LT: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sltdf].N >= 0);
      case LE: 
        return (nios2_fpu_insns[nios2_fpu_nios2_sledf].N >= 0);
      default: 
        break;
      }
    }

  return false;
}

/* Note that get_reverse_cond() is not the same as get_inverse_cond()
    get_reverse_cond() means that if the operand order is reversed,
    what is the operand that is needed to generate the same condition?
*/
enum rtx_code
get_reverse_cond(enum rtx_code cond_t)
{
	switch (cond_t)
	{
     	case GT: return LT;
        case GE: return LE;
        case LT: return GT;
        case LE: return GE;
        case GTU: return LTU;
        case GEU: return LEU;
        case LTU: return GTU;
        case LEU: return GEU;
        default: break;
    }

	return cond_t;
}


/* Generate the code to compare (and possibly branch) two integer values
   TEST_CODE is the comparison code we are trying to emulate 
     (or implement directly)
   RESULT is where to store the result of the comparison, 
     or null to emit a branch
   CMP0 CMP1 are the two comparison operands
   DESTINATION is the destination of the branch, or null to only compare
   */

void
gen_int_relational (enum rtx_code test_code, /* relational test (EQ, etc) */
		    rtx result,		/* result to store comp. or 0 if branch */
		    rtx cmp0,		/* first operand to compare */
		    rtx cmp1,		/* second operand to compare */
		    rtx destination)	/* destination of the branch, or 0 if compare */
{
  struct cmp_info
  {
    /* for register (or 0) compares */
    enum rtx_code test_code_reg;	/* code to use in instruction (LT vs. LTU) */
    int reverse_regs;		/* reverse registers in test */

    /* for immediate compares */
    enum rtx_code test_code_const;	
         /* code to use in instruction (LT vs. LTU) */
    int const_low;		/* low bound of constant we can accept */
    int const_high;		/* high bound of constant we can accept */
    int const_add;		/* constant to add */

    /* generic info */
    int unsignedp;		/* != 0 for unsigned comparisons.  */
  };

  static const struct cmp_info info[(int) ITEST_MAX] = {

    {EQ, 0, EQ, -32768, 32767, 0, 0}, /* EQ  */
    {NE, 0, NE, -32768, 32767, 0, 0}, /* NE  */

    {LT, 1, GE, -32769, 32766, 1, 0}, /* GT  */
    {GE, 0, GE, -32768, 32767, 0, 0}, /* GE  */
    {LT, 0, LT, -32768, 32767, 0, 0}, /* LT  */
    {GE, 1, LT, -32769, 32766, 1, 0}, /* LE  */

    {LTU, 1, GEU, 0, 65534, 1, 0}, /* GTU */
    {GEU, 0, GEU, 0, 65535, 0, 0}, /* GEU */
    {LTU, 0, LTU, 0, 65535, 0, 0}, /* LTU */
    {GEU, 1, LTU, 0, 65534, 1, 0}, /* LEU */
  };

  enum internal_test test;
  enum machine_mode mode;
  const struct cmp_info *p_info;
  int branch_p;


  test = map_test_to_internal_test (test_code);
  if (test == ITEST_MAX)
    abort ();

  p_info = &info[(int) test];

  mode = GET_MODE (cmp0);
  if (mode == VOIDmode)
    mode = GET_MODE (cmp1);

  branch_p = (destination != 0);

  /* Handle floating point comparison directly. */
  if (branch_type == CMP_SF || branch_type == CMP_DF)
    {

      bool reverse_operands = false;

      enum machine_mode float_mode = (branch_type == CMP_SF) ? SFmode : DFmode;

      if (!register_operand (cmp0, float_mode)
          || !register_operand (cmp1, float_mode))
        {
          abort ();
        }

      if (branch_p)
        {
        test_code = p_info->test_code_reg;
        reverse_operands = (p_info->reverse_regs);
      }

      if ( !have_nios2_fpu_cmp_insn(test_code, branch_type) &&
           have_nios2_fpu_cmp_insn(get_reverse_cond(test_code), branch_type) )
        {
          test_code = get_reverse_cond(test_code);
          reverse_operands = !reverse_operands;
        }
      
      if (reverse_operands)
        {
          rtx temp = cmp0;
          cmp0 = cmp1;
          cmp1 = temp;
        }

      if (branch_p)
        {
          rtx cond = gen_rtx (test_code, SImode, cmp0, cmp1);
          rtx label = gen_rtx_LABEL_REF (VOIDmode, destination);
          rtx insn = gen_rtx_SET (VOIDmode, pc_rtx,
                                  gen_rtx_IF_THEN_ELSE (VOIDmode,
                                                        cond, label, pc_rtx));
          emit_jump_insn (insn);
        }
      else
        {
          emit_move_insn (result, gen_rtx (test_code, SImode, cmp0, cmp1));
        }
      return;
    }

  /* We can't, under any circumstances, have const_ints in cmp0
     ??? Actually we could have const0 */
  if (GET_CODE (cmp0) == CONST_INT)
    cmp0 = force_reg (mode, cmp0);

  /* if the comparison is against an int not in legal range
     move it into a register */
  if (GET_CODE (cmp1) == CONST_INT)
    {
      HOST_WIDE_INT value = INTVAL (cmp1);

      if (value < p_info->const_low || value > p_info->const_high)
	cmp1 = force_reg (mode, cmp1);
    }

  /* Comparison to constants, may involve adding 1 to change a GT into GE.
     Comparison between two registers, may involve switching operands.  */
  if (GET_CODE (cmp1) == CONST_INT)
    {
      if (p_info->const_add != 0)
	{
	  HOST_WIDE_INT new = INTVAL (cmp1) + p_info->const_add;

	  /* If modification of cmp1 caused overflow,
	     we would get the wrong answer if we follow the usual path;
	     thus, x > 0xffffffffU would turn into x > 0U.  */
	  if ((p_info->unsignedp
	       ? (unsigned HOST_WIDE_INT) new >
	       (unsigned HOST_WIDE_INT) INTVAL (cmp1)
	       : new > INTVAL (cmp1)) != (p_info->const_add > 0))
	    {
	      /* ??? This case can never happen with the current numbers,
	         but I am paranoid and would rather an abort than
	         a bug I will never find */
	      abort ();
	    }
	  else
	    cmp1 = GEN_INT (new);
	}
    }

  else if (p_info->reverse_regs)
    {
      rtx temp = cmp0;
      cmp0 = cmp1;
      cmp1 = temp;
    }



  if (branch_p)
    {
      if (register_operand (cmp0, mode) && register_operand (cmp1, mode))
	{
	  rtx insn;
	  rtx cond = gen_rtx (p_info->test_code_reg, mode, cmp0, cmp1);
	  rtx label = gen_rtx_LABEL_REF (VOIDmode, destination);

	  insn = gen_rtx_SET (VOIDmode, pc_rtx,
			      gen_rtx_IF_THEN_ELSE (VOIDmode,
						    cond, label, pc_rtx));
	  emit_jump_insn (insn);
	}
      else
	{
	  rtx cond, label;

	  result = gen_reg_rtx (mode);

	  emit_move_insn (result,
			  gen_rtx (p_info->test_code_const, mode, cmp0,
				   cmp1));

	  cond = gen_rtx (NE, mode, result, const0_rtx);
	  label = gen_rtx_LABEL_REF (VOIDmode, destination);

	  emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
				       gen_rtx_IF_THEN_ELSE (VOIDmode,
							     cond,
							     label, pc_rtx)));
	}
    }
  else
    {
      if (register_operand (cmp0, mode) && register_operand (cmp1, mode))
	{
	  emit_move_insn (result,
			  gen_rtx (p_info->test_code_reg, mode, cmp0, cmp1));
	}
      else
	{
	  emit_move_insn (result,
			  gen_rtx (p_info->test_code_const, mode, cmp0,
				   cmp1));
	}
    }

}


/* ??? For now conditional moves are only supported
   when the mode of the operands being compared are
   the same as the ones being moved */

void
gen_conditional_move (rtx *operands, enum machine_mode mode)
{
  rtx insn, cond;
  rtx cmp_reg = gen_reg_rtx (mode);
  enum rtx_code cmp_code = GET_CODE (operands[1]);
  enum rtx_code move_code = EQ;

  /* emit a comparison if it is not "simple".
     Simple comparisons are X eq 0 and X ne 0 */
  if ((cmp_code == EQ || cmp_code == NE) && branch_cmp[1] == const0_rtx)
    {
      cmp_reg = branch_cmp[0];
      move_code = cmp_code;
    }
  else if ((cmp_code == EQ || cmp_code == NE) && branch_cmp[0] == const0_rtx)
    {
      cmp_reg = branch_cmp[1];
      move_code = cmp_code == EQ ? NE : EQ;
    }
  else
    gen_int_relational (cmp_code, cmp_reg, branch_cmp[0], branch_cmp[1],
			NULL_RTX);

  cond = gen_rtx (move_code, VOIDmode, cmp_reg, CONST0_RTX (mode));
  insn = gen_rtx_SET (mode, operands[0],
		      gen_rtx_IF_THEN_ELSE (mode,
					    cond, operands[2], operands[3]));
  emit_insn (insn);
}

/*******************
 * Addressing Modes
 *******************/

int
nios2_legitimate_address (rtx operand, enum machine_mode mode ATTRIBUTE_UNUSED, 
                          int strict)
{
  int ret_val = 0;

  switch (GET_CODE (operand))
    {
      /* direct.  */
    case SYMBOL_REF:
      if (SYMBOL_REF_IN_NIOS2_SMALL_DATA_P (operand))
        {
          ret_val = 1;
          break;
	}
      /* else, fall through */
    case LABEL_REF:
    case CONST_INT:
    case CONST:
    case CONST_DOUBLE:
      /* ??? In here I need to add gp addressing */
      ret_val = 0;

      break;

      /* Register indirect.  */
    case REG:
      ret_val = REG_OK_FOR_BASE_P2 (operand, strict);
      break;

      /* Register indirect with displacement */
    case PLUS:
      {
	rtx op0 = XEXP (operand, 0);
	rtx op1 = XEXP (operand, 1);

	if (REG_P (op0) && REG_P (op1))
	  ret_val = 0;
    else if (REG_P (op0) && GET_CODE (op1) == CONST_INT)
	  ret_val = REG_OK_FOR_BASE_P2 (op0, strict)
	    && SMALL_INT (INTVAL (op1));
    else if (REG_P (op1) && GET_CODE (op0) == CONST_INT)
	  ret_val = REG_OK_FOR_BASE_P2 (op1, strict)
	    && SMALL_INT (INTVAL (op0));
	else
	  ret_val = 0;
      }
      break;

    default:
      ret_val = 0;
      break;
    }

  return ret_val;
}

/* Return true if EXP should be placed in the small data section.  */

static bool
nios2_in_small_data_p (tree exp)
{
  /* We want to merge strings, so we never consider them small data.  */
  if (TREE_CODE (exp) == STRING_CST)
    return false;

  if (TREE_CODE (exp) == VAR_DECL && DECL_SECTION_NAME (exp))
    {
      const char *section = TREE_STRING_POINTER (DECL_SECTION_NAME (exp));
      /* ??? these string names need moving into 
         an array in some header file */
      if (nios2_section_threshold > 0
          && (strcmp (section, ".sbss") == 0
	      || strncmp (section, ".sbss.", 6) == 0
	      || strcmp (section, ".sdata") == 0
	      || strncmp (section, ".sdata.", 7) == 0))
	return true;
    }
  else if (TREE_CODE (exp) == VAR_DECL)
    {
      HOST_WIDE_INT size = int_size_in_bytes (TREE_TYPE (exp));

      /* If this is an incomplete type with size 0, then we can't put it
         in sdata because it might be too big when completed.  */
      if (size > 0 && (unsigned HOST_WIDE_INT)size <= nios2_section_threshold)
	return true;
    }

  return false;
}

static void
nios2_encode_section_info (tree decl, rtx rtl, int first)
{

  rtx symbol;
  int flags;

  default_encode_section_info (decl, rtl, first);
  
  /* Careful not to prod global register variables.  */
  if (GET_CODE (rtl) != MEM)
    return;
  symbol = XEXP (rtl, 0);
  if (GET_CODE (symbol) != SYMBOL_REF)
    return;

  flags = SYMBOL_REF_FLAGS (symbol);
    
  /* We don't want weak variables to be addressed with gp in case they end up with
     value 0 which is not within 2^15 of $gp */
  if (DECL_P (decl) && DECL_WEAK (decl))
    flags |= SYMBOL_FLAG_WEAK_DECL;

  SYMBOL_REF_FLAGS (symbol) = flags;
}


static unsigned int
nios2_section_type_flags (tree decl, const char *name, int reloc)
{
  unsigned int flags;

  flags = default_section_type_flags (decl, name, reloc);

  /* ??? these string names need moving into an array in some header file */
  if (strcmp (name, ".sbss") == 0
      || strncmp (name, ".sbss.", 6) == 0
      || strcmp (name, ".sdata") == 0
      || strncmp (name, ".sdata.", 7) == 0)
    flags |= SECTION_SMALL;

  return flags;
}

/* Handle a #pragma reverse_bitfields */
static void
nios2_pragma_reverse_bitfields (struct cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  nios2_pragma_reverse_bitfields_flag = 1; /* Reverse */
}

/* Handle a #pragma no_reverse_bitfields */
static void
nios2_pragma_no_reverse_bitfields (struct cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  nios2_pragma_reverse_bitfields_flag = -1; /* Forward */
}

/* Handle the various #pragma custom_<switch>s */
static void
nios2_pragma_fpu (int *value, const char *opt, int *seen)
{
  tree t;
  if (c_lex (&t) != CPP_NUMBER)
    {
      error ("`#pragma custom_%s' value must be a number between 0 and 255",
             opt);
      return;
    }

  if (TREE_INT_CST_HIGH (t) == 0
      && TREE_INT_CST_LOW (t) <= 255)
    {
      *value = (int)TREE_INT_CST_LOW (t);
      *seen = 1;
    }
  else
    {
      error ("`#pragma custom_%s' value must be between 0 and 255", opt);
    }
  nios2_custom_check_insns (1);
}

/* Handle the various #pragma no_custom_<switch>s */
static void
nios2_pragma_no_fpu (int *value, const char *opt ATTRIBUTE_UNUSED)
{
  *value = -1;
  nios2_custom_check_insns (1);
}

#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
static void \
NIOS2_CONCAT (nios2_pragma_, insn) \
  (struct cpp_reader *pfile ATTRIBUTE_UNUSED) \
{ \
  nios2_fpu_info *inf = &(nios2_fpu_insns[NIOS2_CONCAT (nios2_fpu_, insn)]); \
  nios2_pragma_fpu (&(inf->N), inf->option, &(inf->pragma_seen)); \
} \
static void \
NIOS2_CONCAT (nios2_pragma_no_, insn) \
  (struct cpp_reader *pfile ATTRIBUTE_UNUSED) \
{ \
  nios2_fpu_info *inf = &(nios2_fpu_insns[NIOS2_CONCAT (nios2_fpu_, insn)]); \
  nios2_pragma_no_fpu (&(inf->N), inf->option); \
}
NIOS2_FOR_ALL_FPU_INSNS

static void
nios2_pragma_handle_custom_fpu_cfg (struct cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  tree t;
  if (c_lex (&t) != CPP_STRING)
    {
      error ("`#pragma custom_fpu_cfg' value must be a string");
      return;
    }

  if (TREE_STRING_LENGTH (t) > 0)
    {
      nios2_handle_custom_fpu_cfg (TREE_STRING_POINTER (t), 1);
    }
}

void
nios2_register_target_pragmas ()
{
  int i;

  c_register_pragma (0, "reverse_bitfields",
                     nios2_pragma_reverse_bitfields);
  c_register_pragma (0, "no_reverse_bitfields",
                     nios2_pragma_no_reverse_bitfields);

  for (i = 0; i < nios2_fpu_max_insn; i++)
    {
      nios2_fpu_info *inf = &(nios2_fpu_insns[i]);
      c_register_pragma (0, inf->pname, inf->pragma);
      c_register_pragma (0, inf->nopname, inf->nopragma);
    }

  c_register_pragma (0, "custom_fpu_cfg",
                     nios2_pragma_handle_custom_fpu_cfg);
}

/* Handle a "reverse_bitfields" or "no_reverse_bitfields" attribute.
   ??? What do these attributes mean on a union? */
static tree
nios2_handle_struct_attribute (tree *node, tree name,
                               tree args ATTRIBUTE_UNUSED,
                               int flags ATTRIBUTE_UNUSED,
                               bool *no_add_attrs)
{
  tree *type = NULL;
  if (DECL_P (*node))
    {
      if (TREE_CODE (*node) == TYPE_DECL)
        {
          type = &TREE_TYPE (*node);
        }
    }
  else
    {
      type = node;
    }

  if (!(type && (TREE_CODE (*type) == RECORD_TYPE
                 || TREE_CODE (*type) == UNION_TYPE)))
    {
      warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
    }

  else if ((is_attribute_p ("reverse_bitfields", name)
            && lookup_attribute ("no_reverse_bitfields",
                                 TYPE_ATTRIBUTES (*type)))
           || ((is_attribute_p ("no_reverse_bitfields", name)
                && lookup_attribute ("reverse_bitfields",
                                     TYPE_ATTRIBUTES (*type)))))
    {
      warning ("`%s' incompatible attribute ignored",
               IDENTIFIER_POINTER (name));
      *no_add_attrs = true;
    }

  return NULL_TREE;
}

/*
  Add __attribute__ ((pragma_reverse_bitfields)) when in the scope of a
  #pragma reverse_bitfields, or __attribute__
  ((pragma_no_reverse_bitfields)) when in the scope of a #pragma
  no_reverse_bitfields.  This gets called before
  nios2_handle_struct_attribute above, so we can't just reuse the same
  attributes.
*/
static void
nios2_insert_attributes (tree node, tree *attr_ptr)
{
  tree type = NULL;
  if (DECL_P (node))
    {
      if (TREE_CODE (node) == TYPE_DECL)
        {
          type = TREE_TYPE (node);
        }
    }
  else
    {
      type = node;
    }

  if (!type
      || (TREE_CODE (type) != RECORD_TYPE
          && TREE_CODE (type) != UNION_TYPE))
    {
      /* We can ignore things other than structs & unions */
      return;
    }

  if (lookup_attribute ("reverse_bitfields", TYPE_ATTRIBUTES (type))
      || lookup_attribute ("no_reverse_bitfields", TYPE_ATTRIBUTES (type)))
    {
      /* If an attribute is already set, it silently overrides the
         current #pragma, if any */
      return;
    }

  if (nios2_pragma_reverse_bitfields_flag)
    {
      const char *id = (nios2_pragma_reverse_bitfields_flag == 1 ?
                        "pragma_reverse_bitfields" :
                        "pragma_no_reverse_bitfields");
      /* No attribute set, and we are in the scope of a #pragma */
      *attr_ptr = tree_cons (get_identifier (id), NULL, *attr_ptr);
    }
}


/*
 * The attributes take precedence over the pragmas, which in turn take
 * precedence over the compile-time switches.
 */
static bool
nios2_reverse_bitfield_layout_p (tree record_type)
{
  return ((TARGET_REVERSE_BITFIELDS
           && !lookup_attribute ("pragma_no_reverse_bitfields",
                                 TYPE_ATTRIBUTES (record_type))
           && !lookup_attribute ("no_reverse_bitfields",
                                 TYPE_ATTRIBUTES (record_type)))
          || (lookup_attribute ("pragma_reverse_bitfields",
                                TYPE_ATTRIBUTES (record_type))
              && !lookup_attribute ("no_reverse_bitfields",
                                    TYPE_ATTRIBUTES (record_type)))
          || lookup_attribute ("reverse_bitfields",
                               TYPE_ATTRIBUTES (record_type)));
}


/*****************************************
 * Defining the Output Assembler Language
 *****************************************/

/* -------------- *
 * Output of Data
 * -------------- */


/* -------------------------------- *
 * Output of Assembler Instructions
 * -------------------------------- */


/* print the operand OP to file stream
   FILE modified by LETTER. LETTER
   can be one of:
     i: print "i" if OP is an immediate, except 0
     o: print "io" if OP is volatile

     z: for const0_rtx print $0 instead of 0
     H: for %hiadj
     L: for %lo
     U: for upper half of 32 bit value
     D: for the upper 32-bits of a 64-bit double value
 */

void
nios2_print_operand (FILE *file, rtx op, int letter)
{

  switch (letter)
    {
    case 'i':
      if (CONSTANT_P (op) && (op != const0_rtx))
	fprintf (file, "i");
      return;

    case 'o':
      if (GET_CODE (op) == MEM
          && ((MEM_VOLATILE_P (op) && !TARGET_CACHE_VOLATILE)
              || TARGET_BYPASS_CACHE))
	fprintf (file, "io");
      return;

    default:
      break;
    }

  if (comparison_operator (op, VOIDmode))
    {
      if (letter == 0)
	{
	  fprintf (file, "%s", GET_RTX_NAME (GET_CODE (op)));
	  return;
	}
    }


  switch (GET_CODE (op))
    {
    case REG:
      if (letter == 0 || letter == 'z')
	{
	  fprintf (file, "%s", reg_names[REGNO (op)]);
	  return;
	}
      else if (letter == 'D')
        {
          fprintf (file, "%s", reg_names[REGNO (op)+1]);
          return;
        }
      break;

    case CONST_INT:
      if (INTVAL (op) == 0 && letter == 'z')
	{
	  fprintf (file, "zero");
	  return;
	}
      else if (letter == 'U')
	{
	  HOST_WIDE_INT val = INTVAL (op);
	  rtx new_op;
	  val = (val / 65536) & 0xFFFF;
	  new_op = GEN_INT (val);
	  output_addr_const (file, new_op);
	  return;
	}

      /* else, fall through */
    case CONST:
    case LABEL_REF:
    case SYMBOL_REF:
    case CONST_DOUBLE:
      if (letter == 0 || letter == 'z')
	{
	  output_addr_const (file, op);
	  return;
	}
      else if (letter == 'H')
	{
	  fprintf (file, "%%hiadj(");
	  output_addr_const (file, op);
	  fprintf (file, ")");
	  return;
	}
      else if (letter == 'L')
	{
	  fprintf (file, "%%lo(");
	  output_addr_const (file, op);
	  fprintf (file, ")");
	  return;
	}
      break;


    case SUBREG:
    case MEM:
      if (letter == 0)
	{
	  output_address (op);
	  return;
	}
      break;

    case CODE_LABEL:
      if (letter == 0)
	{
	  output_addr_const (file, op);
	  return;
	}
      break;

    default:
      break;
    }

  fprintf (stderr, "Missing way to print (%c) ", letter);
  debug_rtx (op);
  abort ();
}

static int gprel_constant (rtx);

static int
gprel_constant (rtx op)
{
  if (GET_CODE (op) == SYMBOL_REF
      && SYMBOL_REF_IN_NIOS2_SMALL_DATA_P (op))
    {
      return 1;
    }
  else if (GET_CODE (op) == CONST
           && GET_CODE (XEXP (op, 0)) == PLUS)
    {
      return gprel_constant (XEXP (XEXP (op, 0), 0));
    }
  else
    {
      return 0;
    }
}

void
nios2_print_operand_address (FILE *file, rtx op)
{
  switch (GET_CODE (op))
    {
    case CONST:
    case CONST_INT:
    case LABEL_REF:
    case CONST_DOUBLE:
    case SYMBOL_REF:
      if (gprel_constant (op))
        {
          fprintf (file, "%%gprel(");
          output_addr_const (file, op);
          fprintf (file, ")(%s)", reg_names[GP_REGNO]);
          return;
        }

      break;

    case PLUS:
      {
	rtx op0 = XEXP (op, 0);
	rtx op1 = XEXP (op, 1);

	if (REG_P (op0) && CONSTANT_P (op1))
	  {
	    output_addr_const (file, op1);
	    fprintf (file, "(%s)", reg_names[REGNO (op0)]);
	    return;
	  }
	else if (REG_P (op1) && CONSTANT_P (op0))
	  {
	    output_addr_const (file, op0);
	    fprintf (file, "(%s)", reg_names[REGNO (op1)]);
	    return;
	  }
      }
      break;

    case REG:
      fprintf (file, "0(%s)", reg_names[REGNO (op)]);
      return;

    case MEM:
      {
	rtx base = XEXP (op, 0);
	PRINT_OPERAND_ADDRESS (file, base);
	return;
      }
    default:
      break;
    }

  fprintf (stderr, "Missing way to print address\n");
  debug_rtx (op);
  abort ();
}





/****************************
 * Predicates
 ****************************/

int
arith_operand (rtx op, enum machine_mode mode)
{
  if (GET_CODE (op) == CONST_INT && SMALL_INT (INTVAL (op)))
    return 1;

  return register_operand (op, mode);
}

int
uns_arith_operand (rtx op, enum machine_mode mode)
{
  if (GET_CODE (op) == CONST_INT && SMALL_INT_UNSIGNED (INTVAL (op)))
    return 1;

  return register_operand (op, mode);
}

int
logical_operand (rtx op, enum machine_mode mode)
{
  if (GET_CODE (op) == CONST_INT
      && (SMALL_INT_UNSIGNED (INTVAL (op)) || UPPER16_INT (INTVAL (op))))
    return 1;

  return register_operand (op, mode);
}

int
shift_operand (rtx op, enum machine_mode mode)
{
  if (GET_CODE (op) == CONST_INT && SHIFT_INT (INTVAL (op)))
    return 1;

  return register_operand (op, mode);
}

int
rdwrctl_operand (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return GET_CODE (op) == CONST_INT && RDWRCTL_INT (INTVAL (op));
}

/* Return truth value of whether OP is a register or the constant 0. */

int
reg_or_0_operand (rtx op, enum machine_mode mode)
{
  switch (GET_CODE (op))
    {
    case CONST_INT:
      return INTVAL (op) == 0;

    case CONST_DOUBLE:
      return op == CONST0_RTX (mode);

    default:
      break;
    }

  return register_operand (op, mode);
}


int
equality_op (rtx op, enum machine_mode mode)
{
  if (mode != GET_MODE (op))
    return 0;

  return GET_CODE (op) == EQ || GET_CODE (op) == NE;
}

int
custom_insn_opcode (rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return GET_CODE (op) == CONST_INT && CUSTOM_INSN_OPCODE (INTVAL (op));
}




/*****************************************************************************
**
** custom fpu instruction output
**
*****************************************************************************/

static const char *nios2_custom_fpu_insn_zdz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_zsz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_szz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_sss (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_ssz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_iss (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_ddd (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_ddz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_idd (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_siz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_suz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_diz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_duz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_isz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_usz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_idz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_udz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_dsz (rtx, int, const char *);
static const char *nios2_custom_fpu_insn_sdz (rtx, int, const char *);

static const char *
nios2_custom_fpu_insn_zdz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, zero, %%0, %%D0 # %s %%0",
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_zsz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, zero, %%0, zero # %s %%0",
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_szz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, %%0, zero, zero # %s %%0",
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_sss (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, %%0, %%1, %%2 # %s %%0, %%1, %%2",
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_ssz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, %%0, %%1, zero # %s %%0, %%1",
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_iss (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_sss (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_ddd (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0
      || nios2_fpu_insns[nios2_fpu_nios2_frdy].N < 0
      || nios2_fpu_insns[nios2_fpu_nios2_fwrx].N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, zero, %%1, %%D1 # fwrx %%1\n\t"
                "custom\t%d, %%D0, %%2, %%D2 # %s %%0, %%1, %%2\n\t"
                "custom\t%d, %%0, zero, zero # frdy %%0",
                nios2_fpu_insns[nios2_fpu_nios2_fwrx].N,
                N, opt,
                nios2_fpu_insns[nios2_fpu_nios2_frdy].N) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_ddz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0 || nios2_fpu_insns[nios2_fpu_nios2_frdy].N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, %%D0, %%1, %%D1 # %s %%0, %%1\n\t"
                "custom\t%d, %%0, zero, zero # frdy %%0",
                N, opt,
                nios2_fpu_insns[nios2_fpu_nios2_frdy].N) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_idd (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0 || nios2_fpu_insns[nios2_fpu_nios2_fwrx].N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, zero, %%1, %%D1 # fwrx %%1\n\t"
                "custom\t%d, %%0, %%2, %%D2 # %s %%0, %%1, %%2",
                nios2_fpu_insns[nios2_fpu_nios2_fwrx].N,
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_siz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_ssz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_suz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_ssz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_diz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_dsz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_duz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_dsz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_isz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_ssz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_usz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_ssz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_idz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_sdz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_udz (rtx insn, int N, const char *opt)
{
  return nios2_custom_fpu_insn_sdz (insn, N, opt);
}

static const char *
nios2_custom_fpu_insn_dsz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0 || nios2_fpu_insns[nios2_fpu_nios2_frdy].N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, %%D0, %%1, zero # %s %%0, %%1\n\t"
                "custom\t%d, %%0, zero, zero # frdy %%0",
                N, opt,
                nios2_fpu_insns[nios2_fpu_nios2_frdy].N) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

static const char *
nios2_custom_fpu_insn_sdz (rtx insn, int N, const char *opt)
{
  static char buf[1024];

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                "custom\t%d, %%0, %%1, %%D1 # %s %%0, %%1",
                N, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
static const char * \
NIOS2_CONCAT (nios2_output_fpu_insn_, insn) (rtx i) \
{ \
  return NIOS2_CONCAT (nios2_custom_fpu_insn_, args) \
           (i, \
            nios2_fpu_insns[NIOS2_CONCAT (nios2_fpu_, insn)].N, \
            nios2_fpu_insns[NIOS2_CONCAT (nios2_fpu_, insn)].option); \
}
NIOS2_FOR_ALL_FPU_INSNS



const char *
nios2_output_fpu_insn_cmps (rtx insn, enum rtx_code cond)
{
  static char buf[1024];
  int N;
  const char *opt;

  int operandL = 2;
  int operandR = 3;

  if ( !have_nios2_fpu_cmp_insn(cond, CMP_SF) && 
       have_nios2_fpu_cmp_insn(get_reverse_cond(cond), CMP_SF) ) {

    int temp = operandL;
    operandL = operandR;
    operandR = temp;

    cond = get_reverse_cond(cond);
  }

  switch (cond)
    {
    case EQ:
      N = nios2_fpu_insns[nios2_fpu_nios2_seqsf].N;
      opt = "fcmpeqs";
      break;
    case NE: 
      N = nios2_fpu_insns[nios2_fpu_nios2_snesf].N;
      opt = "fcmpnes";
      break;
    case GT: 
      N = nios2_fpu_insns[nios2_fpu_nios2_sgtsf].N;
      opt = "fcmpgts"; 
      break;
    case GE: 
      N = nios2_fpu_insns[nios2_fpu_nios2_sgesf].N;
      opt = "fcmpges";
      break;
    case LT:
      N = nios2_fpu_insns[nios2_fpu_nios2_sltsf].N;
      opt = "fcmplts";
      break;
    case LE:
      N = nios2_fpu_insns[nios2_fpu_nios2_slesf].N;
      opt = "fcmples"; break;
    default: 
      fatal_insn ("bad single compare", insn);
    }

  if (N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }

  /*
   * ??? This raises the whole vexing issue of how to handle
   * out-of-range branches.  Punt for now, seeing as how nios2-elf-as
   * doesn't even _try_ to handle out-of-range branches yet!
   */
  if (snprintf (buf, sizeof (buf),
                ".set\tnoat\n\t"
                "custom\t%d, at, %%%d, %%%d # %s at, %%%d, %%%d\n\t"
                "bne\tat, zero, %%l1\n\t"
                ".set\tat",
                N, operandL, operandR, opt, operandL, operandR) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}

const char *
nios2_output_fpu_insn_cmpd (rtx insn, enum rtx_code cond)
{
  static char buf[1024];
  int N;
  const char *opt;

  int operandL = 2;
  int operandR = 3;

  if ( !have_nios2_fpu_cmp_insn(cond, CMP_DF) && 
       have_nios2_fpu_cmp_insn(get_reverse_cond(cond), CMP_DF) ) {

    int temp = operandL;
    operandL = operandR;
    operandR = temp;

    cond = get_reverse_cond(cond);
  }

  switch (cond)
    {
    case EQ: 
      N = nios2_fpu_insns[nios2_fpu_nios2_seqdf].N; 
      opt = "fcmpeqd"; 
      break;
    case NE: 
      N = nios2_fpu_insns[nios2_fpu_nios2_snedf].N; 
      opt = "fcmpned"; 
      break;
    case GT: 
      N = nios2_fpu_insns[nios2_fpu_nios2_sgtdf].N; 
      opt = "fcmpgtd"; 
      break;
    case GE: 
      N = nios2_fpu_insns[nios2_fpu_nios2_sgedf].N; 
      opt = "fcmpged"; 
      break;
    case LT: 
      N = nios2_fpu_insns[nios2_fpu_nios2_sltdf].N; 
      opt = "fcmpltd"; 
      break;
    case LE: 
      N = nios2_fpu_insns[nios2_fpu_nios2_sledf].N; 
      opt = "fcmpled"; 
      break;
    default: 
      fatal_insn ("bad double compare", insn);
    }

  if (N < 0 || nios2_fpu_insns[nios2_fpu_nios2_fwrx].N < 0)
    {
      fatal_insn ("attempt to use disabled fpu instruction", insn);
    }
  if (snprintf (buf, sizeof (buf),
                ".set\tnoat\n\t"
                "custom\t%d, zero, %%%d, %%D%d # fwrx %%%d\n\t"
                "custom\t%d, at, %%%d, %%D%d # %s at, %%%d, %%%d\n\t"
                "bne\tat, zero, %%l1\n\t"
                ".set\tat",
                nios2_fpu_insns[nios2_fpu_nios2_fwrx].N, operandL, operandL, operandL,
                N, operandR, operandR, operandL, operandR, opt) >= (int)sizeof (buf))
    {
      fatal_insn ("buffer overflow", insn);
    }
  return buf;
}




/*****************************************************************************
**
** instruction scheduler
**
*****************************************************************************/
static int
nios2_use_dfa_pipeline_interface ()
{
  return 1;
}


static int
nios2_issue_rate ()
{
#ifdef MAX_DFA_ISSUE_RATE
  return MAX_DFA_ISSUE_RATE;
#else
  return 1;
#endif
}


const char *
asm_output_opcode (FILE *file ATTRIBUTE_UNUSED, 
                   const char *ptr ATTRIBUTE_UNUSED)
{
  const char *p;

  p = ptr;
  return ptr;
}



/*****************************************************************************
**
** function arguments
**
*****************************************************************************/

void
init_cumulative_args (CUMULATIVE_ARGS *cum, 
                      tree fntype ATTRIBUTE_UNUSED, 
                      rtx libname ATTRIBUTE_UNUSED, 
                      tree fndecl ATTRIBUTE_UNUSED, 
                      int n_named_args ATTRIBUTE_UNUSED)
{
  cum->regs_used = 0;
}


/* Define where to put the arguments to a function.  Value is zero to
   push the argument on the stack, or a hard register in which to
   store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
   This is null for libcalls where that information may
   not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
   the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
   (otherwise it is an extra parameter matching an ellipsis).  */
rtx
function_arg (const CUMULATIVE_ARGS *cum, enum machine_mode mode, 
              tree type ATTRIBUTE_UNUSED, int named ATTRIBUTE_UNUSED)
{
  rtx return_rtx = NULL_RTX;

  if (cum->regs_used < NUM_ARG_REGS)
    {
      return_rtx = gen_rtx_REG (mode, FIRST_ARG_REGNO + cum->regs_used);
    }

  return return_rtx;
}

/*
 * This is just default_must_pass_in_stack from calls.c sans the final
 * test for padding which isn't needed: we define BLOCK_REG_PADDING
 * instead.
 */
int
nios2_must_pass_in_stack (enum machine_mode mode, tree type)
{
  if (!type)
    return false;

  /* If the type has variable size...  */
  if (TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST)
    return true;

  /* If the type is marked as addressable (it is required
     to be constructed into the stack)...  */
  if (TREE_ADDRESSABLE (type))
    return true;

  return false;
}

int
function_arg_partial_nregs (const CUMULATIVE_ARGS *cum,
                            enum machine_mode mode, tree type, 
                            int named ATTRIBUTE_UNUSED)
{
  HOST_WIDE_INT param_size;

  if (mode == BLKmode)
    {
      param_size = int_size_in_bytes (type);
      if (param_size < 0)
	internal_error
	  ("Do not know how to handle large structs or variable length types");
    }
  else
    {
      param_size = GET_MODE_SIZE (mode);
    }

  /* convert to words (round up) */
  param_size = (3 + param_size) / 4;

  if (cum->regs_used < NUM_ARG_REGS
      && cum->regs_used + param_size > NUM_ARG_REGS)
    {
      return NUM_ARG_REGS - cum->regs_used;
    }
  else
    {
      return 0;
    }
}


/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)  */

void
function_arg_advance (CUMULATIVE_ARGS *cum, enum machine_mode mode, 
                      tree type ATTRIBUTE_UNUSED, int named ATTRIBUTE_UNUSED)
{
  HOST_WIDE_INT param_size;

  if (mode == BLKmode)
    {
      param_size = int_size_in_bytes (type);
      if (param_size < 0)
        internal_error
          ("Do not know how to handle large structs or variable length types");
    }
  else
    {
      param_size = GET_MODE_SIZE (mode);
    }

  /* convert to words (round up) */
  param_size = (3 + param_size) / 4;

  if (cum->regs_used + param_size > NUM_ARG_REGS)
    {
      cum->regs_used = NUM_ARG_REGS;
    }
  else
    {
      cum->regs_used += param_size;
    }

  return;
}

int
nios2_function_arg_padding_upward (enum machine_mode mode, tree type)
{
  /* On little-endian targets, the first byte of every stack argument
     is passed in the first byte of the stack slot.  */
  if (!BYTES_BIG_ENDIAN)
    return 1;

  /* Otherwise, integral types are padded downward: the last byte of a
     stack argument is passed in the last byte of the stack slot.  */
  if (type != 0
      ? INTEGRAL_TYPE_P (type) || POINTER_TYPE_P (type)
      : GET_MODE_CLASS (mode) == MODE_INT)
    return 0;

  /* Arguments smaller than a stack slot are padded downward.  */
  if (mode != BLKmode)
    return (GET_MODE_BITSIZE (mode) >= PARM_BOUNDARY) ? 1 : 0;
  else
    return ((int_size_in_bytes (type) >= (PARM_BOUNDARY / BITS_PER_UNIT))
            ? 1 : 0);
}

int
nios2_block_reg_padding_upward (enum machine_mode mode, tree type,
                         int first ATTRIBUTE_UNUSED)
{
  /* ??? Do we need to treat floating point specially, ala MIPS? */
  return nios2_function_arg_padding_upward (mode, type);
}

int
nios2_return_in_memory (tree type)
{
  int res = ((int_size_in_bytes (type) > (2 * UNITS_PER_WORD))
  	     || (int_size_in_bytes (type) == -1));

  return res;
}

/* ??? It may be possible to eliminate the copyback and implement
       my own va_arg type, but that is more work for now. */
int
nios2_setup_incoming_varargs (const CUMULATIVE_ARGS *cum, 
                              enum machine_mode mode, tree type, 
                              int no_rtl)
{
  CUMULATIVE_ARGS local_cum;
  int regs_to_push;

  local_cum = *cum;
  FUNCTION_ARG_ADVANCE (local_cum, mode, type, 1);

  regs_to_push = NUM_ARG_REGS - local_cum.regs_used;

  if (!no_rtl)
    {
      if (regs_to_push > 0)
	{
	  rtx ptr, mem;

	  ptr = virtual_incoming_args_rtx;
	  mem = gen_rtx_MEM (BLKmode, ptr);

	  /* va_arg is an array access in this case, which causes
	     it to get MEM_IN_STRUCT_P set.  We must set it here
	     so that the insn scheduler won't assume that these
	     stores can't possibly overlap with the va_arg loads.  */
	  MEM_SET_IN_STRUCT_P (mem, 1);

	  emit_insn (gen_blockage ());
	  move_block_from_reg (local_cum.regs_used + FIRST_ARG_REGNO, mem,
			       regs_to_push);
	  emit_insn (gen_blockage ());
	}
    }

  return regs_to_push * UNITS_PER_WORD;

}



/*****************************************************************************
**
** builtins
**
** This method for handling builtins is from CSP where _many_ more types of
** expanders have already been written. Check there first before writing
** new ones.
**
*****************************************************************************/

enum nios2_builtins
{
  NIOS2_BUILTIN_LDBIO,
  NIOS2_BUILTIN_LDBUIO,
  NIOS2_BUILTIN_LDHIO,
  NIOS2_BUILTIN_LDHUIO,
  NIOS2_BUILTIN_LDWIO,
  NIOS2_BUILTIN_STBIO,
  NIOS2_BUILTIN_STHIO,
  NIOS2_BUILTIN_STWIO,
  NIOS2_BUILTIN_SYNC,
  NIOS2_BUILTIN_RDCTL,
  NIOS2_BUILTIN_WRCTL,

#undef NIOS2_DO_BUILTIN
#define NIOS2_DO_BUILTIN(upper, lower, handler) \
  NIOS2_CONCAT (NIOS2_BUILTIN_CUSTOM_, upper),
NIOS2_FOR_ALL_CUSTOM_BUILTINS

  NIOS2_FIRST_FPU_INSN,

#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
  NIOS2_CONCAT (NIOS2_BUILTIN_FPU_, opt),
NIOS2_FOR_ALL_FPU_INSNS

  NIOS2_LAST_FPU_INSN,

  LIM_NIOS2_BUILTINS
};

struct builtin_description
{
    const enum insn_code icode;
    const char *const name;
    const enum nios2_builtins code;
    const tree *type;
    rtx (* expander) (const struct builtin_description *,
                      tree, rtx, rtx, enum machine_mode, int);
};

static rtx nios2_expand_STXIO (const struct builtin_description *, 
                               tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_LDXIO (const struct builtin_description *, 
                               tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_sync (const struct builtin_description *, 
                              tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_rdctl (const struct builtin_description *, 
                               tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_wrctl (const struct builtin_description *, 
                               tree, rtx, rtx, enum machine_mode, int);

static rtx nios2_expand_custom_n (const struct builtin_description *, 
                                  tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_Xn (const struct builtin_description *, 
                                   tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_nX (const struct builtin_description *, 
                                   tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_XnX (const struct builtin_description *, 
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_nXX (const struct builtin_description *, 
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_XnXX (const struct builtin_description *, 
                                     tree, rtx, rtx, enum machine_mode, int);

static rtx nios2_expand_custom_zdz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_zsz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_szz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_sss (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_ssz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_iss (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_ddd (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_ddz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_idd (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_siz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_suz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_diz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_duz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_isz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_usz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_idz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_udz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_dsz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);
static rtx nios2_expand_custom_sdz (const struct builtin_description *,
                                    tree, rtx, rtx, enum machine_mode, int);

static tree endlink;

/* int fn (volatile const void *)
 */
static tree int_ftype_volatile_const_void_p;

/* int fn (int)
 */
static tree int_ftype_int;

/* void fn (int, int)
 */
static tree void_ftype_int_int;

/* void fn (volatile void *, int)
 */
static tree void_ftype_volatile_void_p_int;

/* void fn (void)
 */
static tree void_ftype_void;

#undef NIOS2_DO_BUILTIN
#define NIOS2_DO_BUILTIN(upper, lower, handler) \
  static tree NIOS2_CONCAT (custom_, lower);
NIOS2_FOR_ALL_CUSTOM_BUILTINS

static tree custom_zdz;
static tree custom_zsz;
static tree custom_szz;
static tree custom_sss;
static tree custom_ssz;
static tree custom_iss;
static tree custom_ddd;
static tree custom_ddz;
static tree custom_idd;
static tree custom_siz;
static tree custom_suz;
static tree custom_diz;
static tree custom_duz;
static tree custom_isz;
static tree custom_usz;
static tree custom_idz;
static tree custom_udz;
static tree custom_dsz;
static tree custom_sdz;

static const struct builtin_description bdesc[] = {
    {CODE_FOR_ldbio, "__builtin_ldbio", NIOS2_BUILTIN_LDBIO, &int_ftype_volatile_const_void_p, nios2_expand_LDXIO},
    {CODE_FOR_ldbuio, "__builtin_ldbuio", NIOS2_BUILTIN_LDBUIO, &int_ftype_volatile_const_void_p, nios2_expand_LDXIO},
    {CODE_FOR_ldhio, "__builtin_ldhio", NIOS2_BUILTIN_LDHIO, &int_ftype_volatile_const_void_p, nios2_expand_LDXIO},
    {CODE_FOR_ldhuio, "__builtin_ldhuio", NIOS2_BUILTIN_LDHUIO, &int_ftype_volatile_const_void_p, nios2_expand_LDXIO},
    {CODE_FOR_ldwio, "__builtin_ldwio", NIOS2_BUILTIN_LDWIO, &int_ftype_volatile_const_void_p, nios2_expand_LDXIO},

    {CODE_FOR_stbio, "__builtin_stbio", NIOS2_BUILTIN_STBIO, &void_ftype_volatile_void_p_int, nios2_expand_STXIO},
    {CODE_FOR_sthio, "__builtin_sthio", NIOS2_BUILTIN_STHIO, &void_ftype_volatile_void_p_int, nios2_expand_STXIO},
    {CODE_FOR_stwio, "__builtin_stwio", NIOS2_BUILTIN_STWIO, &void_ftype_volatile_void_p_int, nios2_expand_STXIO},

    {CODE_FOR_sync, "__builtin_sync", NIOS2_BUILTIN_SYNC, &void_ftype_void, nios2_expand_sync},
    {CODE_FOR_rdctl, "__builtin_rdctl", NIOS2_BUILTIN_RDCTL, &int_ftype_int, nios2_expand_rdctl},
    {CODE_FOR_wrctl, "__builtin_wrctl", NIOS2_BUILTIN_WRCTL, &void_ftype_int_int, nios2_expand_wrctl},

#undef NIOS2_DO_BUILTIN
#define NIOS2_DO_BUILTIN(upper, lower, handler) \
    {NIOS2_CONCAT (CODE_FOR_custom_, lower), \
     "__builtin_custom_" NIOS2_STRINGIFY (lower), \
     NIOS2_CONCAT (NIOS2_BUILTIN_CUSTOM_, upper), \
     &NIOS2_CONCAT (custom_, lower), \
     NIOS2_CONCAT (nios2_expand_custom_, handler)},
NIOS2_FOR_ALL_CUSTOM_BUILTINS

#undef NIOS2_FPU_INSN
#define NIOS2_FPU_INSN(opt, insn, args) \
    {NIOS2_CONCAT (CODE_FOR_, insn), \
     "__builtin_custom_" NIOS2_STRINGIFY (opt), \
     NIOS2_CONCAT (NIOS2_BUILTIN_FPU_, opt), \
     &NIOS2_CONCAT (custom_, args), \
     NIOS2_CONCAT (nios2_expand_custom_, args)},
NIOS2_FOR_ALL_FPU_INSNS

    {0, 0, 0, 0, 0},
};

/* This does not have a closing bracket on purpose (see use) */
#define def_param(TYPE) \
  tree_cons (NULL_TREE, TYPE,

static void
nios2_init_builtins ()
{
  const struct builtin_description *d;


  endlink = void_list_node;

  /* Special indenting here because one of the brackets is in def_param */
  /* *INDENT-OFF* */

  /* int fn (volatile const void *)
   */
  int_ftype_volatile_const_void_p
    = build_function_type (integer_type_node,
			   def_param (build_qualified_type (ptr_type_node,
			                                    TYPE_QUAL_CONST | TYPE_QUAL_VOLATILE))
			   endlink));


  /* void fn (volatile void *, int)
   */
  void_ftype_volatile_void_p_int
    = build_function_type (void_type_node,
			   def_param (build_qualified_type (ptr_type_node,
			                                    TYPE_QUAL_VOLATILE))
			   def_param (integer_type_node)
			   endlink)));

  /* void fn (void)
   */
  void_ftype_void
      = build_function_type (void_type_node,
                             endlink);

  /* int fn (int)
   */
  int_ftype_int
      = build_function_type (integer_type_node,
                             def_param (integer_type_node)
                             endlink));

  /* void fn (int, int)
   */
  void_ftype_int_int
      = build_function_type (void_type_node,
                             def_param (integer_type_node)
                             def_param (integer_type_node)
                             endlink)));


#define CUSTOM_NUM def_param (integer_type_node)

  custom_n
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     endlink));
  custom_ni
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     endlink)));
  custom_nf
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     endlink)));
  custom_np
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     endlink)));
  custom_nii
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_nif
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_nip
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_nfi
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_nff
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_nfp
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_npi
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_npf
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_npp
      = build_function_type (void_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));

  custom_in
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     endlink));
  custom_ini
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     endlink)));
  custom_inf
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     endlink)));
  custom_inp
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     endlink)));
  custom_inii
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_inif
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_inip
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_infi
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_inff
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_infp
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_inpi
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_inpf
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_inpp
      = build_function_type (integer_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));

  custom_fn
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     endlink));
  custom_fni
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     endlink)));
  custom_fnf
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     endlink)));
  custom_fnp
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     endlink)));
  custom_fnii
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_fnif
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_fnip
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_fnfi
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_fnff
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_fnfp
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_fnpi
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_fnpf
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_fnpp
      = build_function_type (float_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));


  custom_pn
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     endlink));
  custom_pni
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     endlink)));
  custom_pnf
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     endlink)));
  custom_pnp
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     endlink)));
  custom_pnii
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_pnif
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_pnip
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (integer_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_pnfi
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_pnff
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_pnfp
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (float_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));
  custom_pnpi
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (integer_type_node)
  			     endlink))));
  custom_pnpf
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (float_type_node)
  			     endlink))));
  custom_pnpp
      = build_function_type (ptr_type_node,
  			     CUSTOM_NUM
  			     def_param (ptr_type_node)
  			     def_param (ptr_type_node)
  			     endlink))));

  custom_zdz
      = build_function_type (void_type_node,
                             def_param (double_type_node)
                             endlink));

  custom_zsz
      = build_function_type (void_type_node,
                             def_param (float_type_node)
                             endlink));

  custom_szz
      = build_function_type (float_type_node,
                             def_param (void_type_node)
                             endlink));

  custom_sss
      = build_function_type (float_type_node,
                             def_param (float_type_node)
                             def_param (float_type_node)
                             endlink)));

  custom_ssz
      = build_function_type (float_type_node,
                             def_param (float_type_node)
                             endlink));

  custom_iss
      = build_function_type (integer_type_node,
                             def_param (float_type_node)
                             def_param (float_type_node)
                             endlink)));

  custom_ddd
      = build_function_type (double_type_node,
                             def_param (double_type_node)
                             def_param (double_type_node)
                             endlink)));

  custom_ddz
      = build_function_type (double_type_node,
                             def_param (double_type_node)
                             endlink));

  custom_idd
      = build_function_type (integer_type_node,
                             def_param (double_type_node)
                             def_param (double_type_node)
                             endlink)));

  custom_siz
      = build_function_type (float_type_node,
                             def_param (integer_type_node)
                             endlink));

  custom_suz
      = build_function_type (float_type_node,
                             def_param (unsigned_type_node)
                             endlink));

  custom_diz
      = build_function_type (double_type_node,
                             def_param (integer_type_node)
                             endlink));

  custom_duz
      = build_function_type (double_type_node,
                             def_param (unsigned_type_node)
                             endlink));

  custom_isz
      = build_function_type (integer_type_node,
                             def_param (float_type_node)
                             endlink));

  custom_usz
      = build_function_type (unsigned_type_node,
                             def_param (float_type_node)
                             endlink));

  custom_idz
      = build_function_type (integer_type_node,
                             def_param (double_type_node)
                             endlink));

  custom_udz
      = build_function_type (unsigned_type_node,
                             def_param (double_type_node)
                             endlink));

  custom_dsz
      = build_function_type (double_type_node,
                             def_param (float_type_node)
                             endlink));

  custom_sdz
      = build_function_type (float_type_node,
                             def_param (double_type_node)
                             endlink));

  /* *INDENT-ON* */


  for (d = bdesc; d->name; d++)
    {
      builtin_function (d->name, *d->type, d->code,
			BUILT_IN_MD, NULL, NULL);
    }
}

/* Expand an expression EXP that calls a built-in function,
   with result going to TARGET if that's convenient
   (and in mode MODE if that's convenient).
   SUBTARGET may be used as the target for computing one of EXP's operands.
   IGNORE is nonzero if the value is to be ignored.  */

static rtx
nios2_expand_builtin (tree exp, rtx target, rtx subtarget, 
                      enum machine_mode mode, int ignore)
{
  const struct builtin_description *d;
  tree fndecl = TREE_OPERAND (TREE_OPERAND (exp, 0), 0);
  unsigned int fcode = DECL_FUNCTION_CODE (fndecl);

  for (d = bdesc; d->name; d++)
    if (d->code == fcode)
      {
        if (d->code > NIOS2_FIRST_FPU_INSN && d->code < NIOS2_LAST_FPU_INSN)
          {
            nios2_fpu_info *inf = &nios2_fpu_insns[d->code - (NIOS2_FIRST_FPU_INSN + 1)];
            const struct insn_data *idata = &insn_data[d->icode];
            if (inf->N < 0)
              {
                fatal_error ("Cannot call `%s' without specifying switch `-mcustom-%s'",
                       d->name,
                       inf->option);
              }
            if (inf->args[0] != 'z'
                && (!target
                    || !(idata->operand[0].predicate) (target,
                                                       idata->operand[0].mode)))
              target = gen_reg_rtx (idata->operand[0].mode);
          }
      return (d->expander) (d, exp, target, subtarget, mode, ignore);
      }

  /* we should have seen one of the functins we registered */
  abort ();
}

static rtx nios2_create_target (const struct builtin_description *, rtx);


static rtx
nios2_create_target (const struct builtin_description *d, rtx target)
{
  if (!target
      || !(*insn_data[d->icode].operand[0].predicate) (target,
                                                       insn_data[d->icode].operand[0].mode))
    {
      target = gen_reg_rtx (insn_data[d->icode].operand[0].mode);
    }

  return target;
}


static rtx nios2_extract_opcode (const struct builtin_description *, int, tree);
static rtx nios2_extract_operand (const struct builtin_description *, int, int, tree);

static rtx
nios2_extract_opcode (const struct builtin_description *d, int op, tree arglist)
{
  enum machine_mode mode = insn_data[d->icode].operand[op].mode;
  tree arg = TREE_VALUE (arglist);
  rtx opcode = expand_expr (arg, NULL_RTX, mode, 0);
  opcode = protect_from_queue (opcode, 0);

  if (!(*insn_data[d->icode].operand[op].predicate) (opcode, mode))
    error ("Custom instruction opcode must be compile time constant in the range 0-255 for %s", d->name);

  builtin_custom_seen[INTVAL (opcode)] = d->name;
  nios2_custom_check_insns (0);
  return opcode;
}

static rtx
nios2_extract_operand (const struct builtin_description *d, int op, int argnum, tree arglist)
{
  enum machine_mode mode = insn_data[d->icode].operand[op].mode;
  tree arg = TREE_VALUE (arglist);
  rtx operand = expand_expr (arg, NULL_RTX, mode, 0);
  operand = protect_from_queue (operand, 0);

  if (!(*insn_data[d->icode].operand[op].predicate) (operand, mode))
    operand = copy_to_mode_reg (mode, operand);

  /* ??? Better errors would be nice */
  if (!(*insn_data[d->icode].operand[op].predicate) (operand, mode))
    error ("Invalid argument %d to %s", argnum, d->name);

  return operand;
}


static rtx
nios2_expand_custom_n (const struct builtin_description *d, tree exp, 
                       rtx target ATTRIBUTE_UNUSED, rtx subtarget ATTRIBUTE_UNUSED, 
                       enum machine_mode mode ATTRIBUTE_UNUSED, int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx opcode;

  /* custom_n should have exactly one operand */
  if (insn_data[d->icode].n_operands != 1)
    abort ();

  opcode = nios2_extract_opcode (d, 0, arglist);

  pat = GEN_FCN (d->icode) (opcode);
  if (!pat)
    return 0;
  emit_insn (pat);
  return 0;
}

static rtx
nios2_expand_custom_Xn (const struct builtin_description *d, tree exp, 
                        rtx target, rtx subtarget ATTRIBUTE_UNUSED, 
                        enum machine_mode mode ATTRIBUTE_UNUSED, 
                        int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx opcode;

  /* custom_Xn should have exactly two operands */
  if (insn_data[d->icode].n_operands != 2)
    abort ();

  target = nios2_create_target (d, target);
  opcode = nios2_extract_opcode (d, 1, arglist);

  pat = GEN_FCN (d->icode) (target, opcode);
  if (!pat)
    return 0;
  emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_nX (const struct builtin_description *d, tree exp, 
                        rtx target ATTRIBUTE_UNUSED, rtx subtarget ATTRIBUTE_UNUSED, 
                        enum machine_mode mode ATTRIBUTE_UNUSED, int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx opcode;
  rtx operands[1];
  int i;


  /* custom_nX should have exactly two operands */
  if (insn_data[d->icode].n_operands != 2)
    abort ();

  opcode = nios2_extract_opcode (d, 0, arglist);
  for (i = 0; i < 1; i++)
    {
      arglist = TREE_CHAIN (arglist);
      operands[i] = nios2_extract_operand (d, i + 1, i + 1, arglist);
    }

  pat = GEN_FCN (d->icode) (opcode, operands[0]);
  if (!pat)
    return 0;
  emit_insn (pat);
  return 0;
}

static rtx
nios2_expand_custom_XnX (const struct builtin_description *d, tree exp, rtx target, 
                         rtx subtarget ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED, 
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx opcode;
  rtx operands[1];
  int i;

  /* custom_Xn should have exactly three operands */
  if (insn_data[d->icode].n_operands != 3)
    abort ();

  target = nios2_create_target (d, target);
  opcode = nios2_extract_opcode (d, 1, arglist);

  for (i = 0; i < 1; i++)
    {
      arglist = TREE_CHAIN (arglist);
      operands[i] = nios2_extract_operand (d, i + 2, i + 1, arglist);
    }

  pat = GEN_FCN (d->icode) (target, opcode, operands[0]);

  if (!pat)
    return 0;
  emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_nXX (const struct builtin_description *d, tree exp, rtx target ATTRIBUTE_UNUSED, 
                         rtx subtarget ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED, 
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx opcode;
  rtx operands[2];
  int i;


  /* custom_nX should have exactly three operands */
  if (insn_data[d->icode].n_operands != 3)
    abort ();

  opcode = nios2_extract_opcode (d, 0, arglist);
  for (i = 0; i < 2; i++)
    {
      arglist = TREE_CHAIN (arglist);
      operands[i] = nios2_extract_operand (d, i + 1, i + 1, arglist);
    }

  pat = GEN_FCN (d->icode) (opcode, operands[0], operands[1]);
  if (!pat)
    return 0;
  emit_insn (pat);
  return 0;
}

static rtx
nios2_expand_custom_XnXX (const struct builtin_description *d, tree exp, rtx target, 
                          rtx subtarget ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED, 
                          int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx opcode;
  rtx operands[2];
  int i;


  /* custom_XnX should have exactly four operands */
  if (insn_data[d->icode].n_operands != 4)
    abort ();

  target = nios2_create_target (d, target);
  opcode = nios2_extract_opcode (d, 1, arglist);
  for (i = 0; i < 2; i++)
    {
      arglist = TREE_CHAIN (arglist);
      operands[i] = nios2_extract_operand (d, i + 2, i + 1, arglist);
    }

  pat = GEN_FCN (d->icode) (target, opcode, operands[0], operands[1]);

  if (!pat)
    return 0;
  emit_insn (pat);
  return target;
}



static rtx
nios2_expand_STXIO (const struct builtin_description *d, tree exp, rtx target ATTRIBUTE_UNUSED, 
                    rtx subtarget ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED, 
                    int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx store_dest, store_val;
  enum insn_code icode = d->icode;

  /* stores should have exactly two operands */
  if (insn_data[icode].n_operands != 2)
    abort ();

  /* process the destination of the store */
  {
    enum machine_mode mode = insn_data[icode].operand[0].mode;
    tree arg = TREE_VALUE (arglist);
    store_dest = expand_expr (arg, NULL_RTX, VOIDmode, 0);
    store_dest = protect_from_queue (store_dest, 0);

    store_dest = gen_rtx_MEM (mode, copy_to_mode_reg (Pmode, store_dest));

    /* ??? Better errors would be nice */
    if (!(*insn_data[icode].operand[0].predicate) (store_dest, mode))
      error ("Invalid argument 1 to %s", d->name);
  }


  /* process the value to store */
  {
    enum machine_mode mode = insn_data[icode].operand[1].mode;
    tree arg = TREE_VALUE (TREE_CHAIN (arglist));
    store_val = expand_expr (arg, NULL_RTX, mode, 0);
    store_val = protect_from_queue (store_val, 0);

    if (!(*insn_data[icode].operand[1].predicate) (store_val, mode))
      store_val = copy_to_mode_reg (mode, store_val);

    /* ??? Better errors would be nice */
    if (!(*insn_data[icode].operand[1].predicate) (store_val, mode))
      error ("Invalid argument 2 to %s", d->name);
  }

  pat = GEN_FCN (d->icode) (store_dest, store_val);
  if (!pat)
    return 0;
  emit_insn (pat);
  return 0;
}


static rtx
nios2_expand_LDXIO (const struct builtin_description * d, tree exp, rtx target, 
                    rtx subtarget ATTRIBUTE_UNUSED, enum machine_mode mode ATTRIBUTE_UNUSED, 
                    int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx ld_src;
  enum insn_code icode = d->icode;

  /* loads should have exactly two operands */
  if (insn_data[icode].n_operands != 2)
    abort ();

  target = nios2_create_target (d, target);

  {
    enum machine_mode mode = insn_data[icode].operand[1].mode;
    tree arg = TREE_VALUE (arglist);
    ld_src = expand_expr (arg, NULL_RTX, VOIDmode, 0);
    ld_src = protect_from_queue (ld_src, 0);

    ld_src = gen_rtx_MEM (mode, copy_to_mode_reg (Pmode, ld_src));

    /* ??? Better errors would be nice */
    if (!(*insn_data[icode].operand[1].predicate) (ld_src, mode))
      {
        error ("Invalid argument 1 to %s", d->name);
      }
  }

  pat = GEN_FCN (d->icode) (target, ld_src);
  if (!pat)
    return 0;
  emit_insn (pat);
  return target;
}


static rtx
nios2_expand_sync (const struct builtin_description * d ATTRIBUTE_UNUSED, 
                   tree exp ATTRIBUTE_UNUSED, rtx target ATTRIBUTE_UNUSED, 
                   rtx subtarget ATTRIBUTE_UNUSED, 
                   enum machine_mode mode ATTRIBUTE_UNUSED, 
                   int ignore ATTRIBUTE_UNUSED)
{
  emit_insn (gen_sync ());
  return 0;
}

static rtx
nios2_expand_rdctl (const struct builtin_description * d ATTRIBUTE_UNUSED, 
                   tree exp ATTRIBUTE_UNUSED, rtx target ATTRIBUTE_UNUSED, 
                   rtx subtarget ATTRIBUTE_UNUSED, 
                   enum machine_mode mode ATTRIBUTE_UNUSED, 
                   int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx rdctl_reg;
  enum insn_code icode = d->icode;

  /* rdctl should have exactly two operands */
  if (insn_data[icode].n_operands != 2)
    abort ();

  target = nios2_create_target (d, target);

  {
    enum machine_mode mode = insn_data[icode].operand[1].mode;
    tree arg = TREE_VALUE (arglist);
    rdctl_reg = expand_expr (arg, NULL_RTX, VOIDmode, 0);
    rdctl_reg = protect_from_queue (rdctl_reg, 0);

    if (!(*insn_data[icode].operand[1].predicate) (rdctl_reg, mode))
      {
        error ("Control register number must be in range 0-31 for %s", d->name);
      }
  }

  pat = GEN_FCN (d->icode) (target, rdctl_reg);
  if (!pat)
    return 0;
  emit_insn (pat);
  return target;
}

static rtx
nios2_expand_wrctl (const struct builtin_description * d ATTRIBUTE_UNUSED, 
                   tree exp ATTRIBUTE_UNUSED, rtx target ATTRIBUTE_UNUSED, 
                   rtx subtarget ATTRIBUTE_UNUSED, 
                   enum machine_mode mode ATTRIBUTE_UNUSED, 
                   int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat;
  rtx wrctl_reg, store_val;
  enum insn_code icode = d->icode;

  /* stores should have exactly two operands */
  if (insn_data[icode].n_operands != 2)
    abort ();

  /* process the destination of the store */
  {
    enum machine_mode mode = insn_data[icode].operand[0].mode;
    tree arg = TREE_VALUE (arglist);
    wrctl_reg = expand_expr (arg, NULL_RTX, VOIDmode, 0);
    wrctl_reg = protect_from_queue (wrctl_reg, 0);

    if (!(*insn_data[icode].operand[0].predicate) (wrctl_reg, mode))
      error ("Control register number must be in range 0-31 for %s", d->name);
  }


  /* process the value to store */
  {
    enum machine_mode mode = insn_data[icode].operand[1].mode;
    tree arg = TREE_VALUE (TREE_CHAIN (arglist));
    store_val = expand_expr (arg, NULL_RTX, mode, 0);
    store_val = protect_from_queue (store_val, 0);

    if (!(*insn_data[icode].operand[1].predicate) (store_val, mode))
      store_val = copy_to_mode_reg (mode, store_val);

    /* ??? Better errors would be nice */
    if (!(*insn_data[icode].operand[1].predicate) (store_val, mode))
      error ("Invalid argument 2 to %s", d->name);
  }

  pat = GEN_FCN (d->icode) (wrctl_reg, store_val);
  if (!pat)
    return 0;
  emit_insn (pat);
  return 0;
}

static rtx
nios2_extract_double (const struct insn_data *idata, tree arglist, int index)
{
  rtx arg;

  while (index--)
  {
    arglist = TREE_CHAIN (arglist);
  }
  arg = expand_expr (TREE_VALUE (arglist), NULL_RTX, DFmode, 0);
  arg = protect_from_queue (arg, 0);
  if (!(*(idata->operand[index+1].predicate)) (arg, DFmode))
    {
      arg = copy_to_mode_reg (DFmode, arg);
    }
  return arg;
}

static rtx
nios2_extract_float (const struct insn_data *idata, tree arglist, int index)
{
  rtx arg;

  while (index--)
  {
    arglist = TREE_CHAIN (arglist);
  }
  arg = expand_expr (TREE_VALUE (arglist), NULL_RTX, SFmode, 0);
  arg = protect_from_queue (arg, 0);
  if (!(*(idata->operand[index+1].predicate)) (arg, SFmode))
    {
      arg = copy_to_mode_reg (SFmode, arg);
    }
  return arg;
}

static rtx
nios2_extract_integer (const struct insn_data *idata, tree arglist, int index)
{
  rtx arg;

  while (index--)
  {
    arglist = TREE_CHAIN (arglist);
  }
  arg = expand_expr (TREE_VALUE (arglist), NULL_RTX, SImode, 0);
  arg = protect_from_queue (arg, 0);
  if (!(*(idata->operand[index+1].predicate)) (arg, SImode))
    {
      arg = copy_to_mode_reg (SImode, arg);
    }
  return protect_from_queue (arg, 0);
}

static rtx
nios2_expand_custom_zdz (const struct builtin_description *d,
                         tree exp,
                         rtx target ATTRIBUTE_UNUSED,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0));
  if (pat)
    emit_insn (pat);
  return 0;
}

static rtx
nios2_expand_custom_zsz (const struct builtin_description *d,
                         tree exp,
                         rtx target ATTRIBUTE_UNUSED,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0));
  if (pat)
    emit_insn (pat);
  return 0;
}

static rtx
nios2_expand_custom_szz (const struct builtin_description *d,
                         tree exp ATTRIBUTE_UNUSED,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  rtx pat = GEN_FCN (d->icode) (target);
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_sss (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0),
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 1));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_ssz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_iss (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0),
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 1));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_ddd (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0),
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 1));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_ddz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_idd (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0),
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 1));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_siz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_integer (&insn_data[d->icode],
                                                       arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_suz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_integer (&insn_data[d->icode],
                                                       arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_diz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_integer (&insn_data[d->icode],
                                                       arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_duz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_integer (&insn_data[d->icode],
                                                       arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_isz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_usz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_idz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_udz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_dsz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_float (&insn_data[d->icode],
                                                     arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

static rtx
nios2_expand_custom_sdz (const struct builtin_description *d,
                         tree exp,
                         rtx target,
                         rtx subtarget ATTRIBUTE_UNUSED,
                         enum machine_mode mode ATTRIBUTE_UNUSED,
                         int ignore ATTRIBUTE_UNUSED)
{
  tree arglist = TREE_OPERAND (exp, 1);
  rtx pat = GEN_FCN (d->icode) (target,
                                nios2_extract_double (&insn_data[d->icode],
                                                      arglist, 0));
  if (pat)
    emit_insn (pat);
  return target;
}

#include "gt-nios2.h"

