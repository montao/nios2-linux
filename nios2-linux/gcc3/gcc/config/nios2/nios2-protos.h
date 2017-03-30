/* Subroutines for assembler code output for Altera NIOS 2G NIOS2 version.
   Copyright (C) 2003 Altera 
   Contributed by Jonah Graham (jgraham@altera.com).

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

extern void dump_frame_size (FILE *);
extern HOST_WIDE_INT compute_frame_size (void);
extern int nios2_initial_elimination_offset (int, int);
extern void override_options (void);
extern void optimization_options (int, int);
extern int nios2_can_use_return_insn (void);
extern void expand_prologue (void);
extern void expand_epilogue (bool);
extern void function_profiler (FILE *, int);
extern enum reg_class reg_class_from_constraint (char, const char *);
extern void nios2_register_target_pragmas (void);

#ifdef RTX_CODE
extern int nios2_legitimate_address (rtx, enum machine_mode, int);
extern void nios2_print_operand (FILE *, rtx, int);
extern void nios2_print_operand_address (FILE *, rtx);

extern int nios2_emit_move_sequence (rtx *, enum machine_mode);
extern int nios2_emit_expensive_div (rtx *, enum machine_mode);

extern void gen_int_relational (enum rtx_code, rtx, rtx, rtx, rtx);
extern void gen_conditional_move (rtx *, enum machine_mode);
extern const char *asm_output_opcode (FILE *, const char *);

/* predicates */
extern int arith_operand (rtx, enum machine_mode);
extern int uns_arith_operand (rtx, enum machine_mode);
extern int logical_operand (rtx, enum machine_mode);
extern int shift_operand (rtx, enum machine_mode);
extern int reg_or_0_operand (rtx, enum machine_mode);
extern int equality_op (rtx, enum machine_mode);
extern int custom_insn_opcode (rtx, enum machine_mode);
extern int rdwrctl_operand (rtx, enum machine_mode);

/* custom fpu instruction output */
extern const char *nios2_output_fpu_insn_cmps (rtx, enum rtx_code);
extern const char *nios2_output_fpu_insn_cmpd (rtx, enum rtx_code);

# ifdef HAVE_MACHINE_MODES
#  if defined TREE_CODE
extern rtx function_arg (const CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern int nios2_must_pass_in_stack (enum machine_mode, tree);
extern int function_arg_partial_nregs (const CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern void function_arg_advance (CUMULATIVE_ARGS *, enum machine_mode, tree, int);
extern int nios2_function_arg_padding (enum machine_mode, tree);
extern int nios2_block_reg_padding (enum machine_mode, tree, int);
extern void init_cumulative_args (CUMULATIVE_ARGS *, tree, rtx, tree, int);
extern int nios2_setup_incoming_varargs (const CUMULATIVE_ARGS *, enum machine_mode, tree, int);

#  endif /* TREE_CODE */
# endif	/* HAVE_MACHINE_MODES */
#endif

#ifdef TREE_CODE
extern int nios2_return_in_memory (tree);

#endif /* TREE_CODE */
