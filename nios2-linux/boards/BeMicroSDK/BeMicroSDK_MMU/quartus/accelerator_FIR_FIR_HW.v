//Legal Notice: (C)2010 Altera Corporation. All rights reserved.  Your
//use of Altera Corporation's design tools, logic functions and other
//software and tools, and its AMPP partner logic functions, and any
//output files any of the foregoing (including device programming or
//simulation files), and any associated documentation or information are
//expressly subject to the terms and conditions of the Altera Program
//License Subscription Agreement or other applicable license agreement,
//including, without limitation, that your use is for the sole purpose
//of programming logic devices manufactured by Altera and sold by Altera
//or its authorized distributors.  Please refer to the applicable
//agreement for further details.

// synthesis translate_off
`timescale 1ns / 1ps
// synthesis translate_on

// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_output_buffer0 (
                                               // inputs:
                                                accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0,
                                                accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0,
                                                accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0,
                                                accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0,
                                                clk,

                                               // outputs:
                                                accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0
                                             )
;

  output  [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0;
  input   [  9: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0;
  input   [  3: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0;
  input            accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0;
  input            clk;

  wire    [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0;
  altsyncram the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0
    (
      .address_a (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0),
      .byteena_a (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0),
      .clock0 (clk),
      .data_a (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0),
      .q_a (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0),
      .wren_a (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0)
    );

  defparam the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.byte_size = 8,
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.implement_in_les = "OFF",
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.lpm_type = "altsyncram",
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.numwords_a = 1024,
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.operation_mode = "SINGLE_PORT",
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.outdata_reg_a = "UNREGISTERED",
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.ram_block_type = "AUTO",
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.read_during_write_mode_mixed_ports = "DONT_CARE",
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.width_a = 32,
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.width_byteena_a = 4,
           the_accelerator_FIR_FIR_HW_output_buffer_internal_memory0.widthad_a = 10;


endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

// c_subroutine_block
//../fir_hw.c (45): void FIR_HW(short * input_data,
module accelerator_FIR_FIR_HW_FIR_HW0_state_machine_subroutine_0 (
                                                                   // inputs:
                                                                    FIR_HW_begin0,
                                                                    FIR_HW_read0,
                                                                    FIR_HW_select0,
                                                                    clk,
                                                                    inhibit_from_c_do_block0,
                                                                    reset_n,

                                                                   // outputs:
                                                                    FIR_HW_state0,
                                                                    advance_to_FIR_HW_state0,
                                                                    enable_FIR_HW_state0
                                                                 )
;

  output  [  3: 0] FIR_HW_state0;
  output  [  3: 0] advance_to_FIR_HW_state0;
  output  [  3: 0] enable_FIR_HW_state0;
  input            FIR_HW_begin0;
  input            FIR_HW_read0;
  input            FIR_HW_select0;
  input            clk;
  input            inhibit_from_c_do_block0;
  input            reset_n;

  reg     [  3: 0] FIR_HW_state0;
  wire    [  3: 0] advance_to_FIR_HW_state0;
  wire             block_wide_enable0;
  wire    [  3: 0] enable_FIR_HW_state0;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          FIR_HW_state0[0] <= 0;
      else if (enable_FIR_HW_state0[0])
          FIR_HW_state0[0] <= advance_to_FIR_HW_state0[0];
    end


  assign enable_FIR_HW_state0[0] = block_wide_enable0;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          FIR_HW_state0[1] <= 0;
      else if (enable_FIR_HW_state0[1])
          FIR_HW_state0[1] <= advance_to_FIR_HW_state0[1];
    end


  assign enable_FIR_HW_state0[1] = block_wide_enable0;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          FIR_HW_state0[2] <= 0;
      else if (enable_FIR_HW_state0[2])
          FIR_HW_state0[2] <= advance_to_FIR_HW_state0[2];
    end


  assign enable_FIR_HW_state0[2] = block_wide_enable0;
  assign enable_FIR_HW_state0[3] = block_wide_enable0;
  assign advance_to_FIR_HW_state0[0] = FIR_HW_read0 & FIR_HW_select0 & FIR_HW_begin0;
  //advance_to_FIR_HW_state0[1] assign, which is an e_assign
  assign advance_to_FIR_HW_state0[1] = FIR_HW_state0[0];

  //advance_to_FIR_HW_state0[2] assign, which is an e_assign
  assign advance_to_FIR_HW_state0[2] = FIR_HW_state0[1];

  //advance_to_FIR_HW_state0[3] assign, which is an e_assign
  assign advance_to_FIR_HW_state0[3] = FIR_HW_state0[2];

  //block_wide_enable0 inhibitor mux, which is an e_mux
  assign block_wide_enable0 = !inhibit_from_c_do_block0 &
    1;


endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

// c_subroutine_block
//../fir_hw.c (87):   {
module accelerator_FIR_FIR_HW_FIR_HW0_state_machine_do_0 (
                                                           // inputs:
                                                            advance_to_FIR_HW_state0,
                                                            break55,
                                                            break56,
                                                            clk,
                                                            enable_FIR_HW_state0,
                                                            go_bit_control_output1,
                                                            inhibit_from_c_do_block1,
                                                            inhibit_from_c_do_block2,
                                                            reset_n,

                                                           // outputs:
                                                            advance_to_state_do_2,
                                                            enable_state_do_2,
                                                            inhibit_from_c_do_block0,
                                                            state_do_2
                                                         )
;

  output  [  5: 0] advance_to_state_do_2;
  output  [  5: 0] enable_state_do_2;
  output           inhibit_from_c_do_block0;
  output  [  5: 0] state_do_2;
  input   [  3: 0] advance_to_FIR_HW_state0;
  input   [ 31: 0] break55;
  input   [ 31: 0] break56;
  input            clk;
  input   [  3: 0] enable_FIR_HW_state0;
  input   [ 31: 0] go_bit_control_output1;
  input            inhibit_from_c_do_block1;
  input            inhibit_from_c_do_block2;
  input            reset_n;

  wire    [  5: 0] advance_to_state_do_2;
  wire             block_wide_enable1;
  wire    [  5: 0] enable_state_do_2;
  reg              inhibit_from_c_do_block0;
  reg     [  5: 0] state_do_2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_2[0] <= 0;
      else if (enable_state_do_2[0])
          state_do_2[0] <= advance_to_state_do_2[0];
    end


  assign enable_state_do_2[0] = block_wide_enable1;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_2[1] <= 0;
      else if (enable_state_do_2[1])
          state_do_2[1] <= advance_to_state_do_2[1];
    end


  assign enable_state_do_2[1] = block_wide_enable1;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_2[2] <= 0;
      else if (enable_state_do_2[2])
          state_do_2[2] <= advance_to_state_do_2[2];
    end


  assign enable_state_do_2[2] = block_wide_enable1;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_2[3] <= 0;
      else if (enable_state_do_2[3])
          state_do_2[3] <= advance_to_state_do_2[3];
    end


  assign enable_state_do_2[3] = block_wide_enable1;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_2[4] <= 0;
      else if (enable_state_do_2[4])
          state_do_2[4] <= advance_to_state_do_2[4];
    end


  assign enable_state_do_2[4] = block_wide_enable1;
  assign enable_state_do_2[5] = block_wide_enable1;
  //advance_to_state_do_2[0] assign, which is an e_assign
  assign advance_to_state_do_2[0] = (enable_FIR_HW_state0[1] & advance_to_FIR_HW_state0[1]) & go_bit_control_output1 & !state_do_2[0];

  //advance_to_state_do_2[1] assign, which is an e_assign
  assign advance_to_state_do_2[1] = (state_do_2[0] | advance_to_state_do_2[3]) & !(|break55);

  //advance_to_state_do_2[2] assign, which is an e_assign
  assign advance_to_state_do_2[2] = (state_do_2[1]) & !(|break55);

  //advance_to_state_do_2[3] assign, which is an e_assign
  assign advance_to_state_do_2[3] = (state_do_2[2]) & !(|break55);

  //advance_to_state_do_2[4] assign, which is an e_assign
  assign advance_to_state_do_2[4] = state_do_2[3];

  //advance_to_state_do_2[5] assign, which is an e_assign
  assign advance_to_state_do_2[5] = state_do_2[4];

  //on/inhibit register for c_do_block, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          inhibit_from_c_do_block0 <= 0;
      else if (break56 & (enable_state_do_2[5] & advance_to_state_do_2[5]))
          inhibit_from_c_do_block0 <= 0;
      else if (go_bit_control_output1 & (enable_FIR_HW_state0[1] & advance_to_FIR_HW_state0[1]))
          inhibit_from_c_do_block0 <= -1;
    end


  //block_wide_enable1 inhibitor mux, which is an e_mux
  assign block_wide_enable1 = !inhibit_from_c_do_block1 &
    !inhibit_from_c_do_block2 &
    1;


endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

// c_subroutine_block
//../fir_hw.c (90):     {
module accelerator_FIR_FIR_HW_FIR_HW0_state_machine_do_1 (
                                                           // inputs:
                                                            accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0,
                                                            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0,
                                                            accelerator_FIR_FIR_HW_master_resource1_fifo_empty0,
                                                            advance_to_state_do_2,
                                                            break2,
                                                            break40,
                                                            clk,
                                                            enable_state_do_2,
                                                            go_bit_control_output3,
                                                            reset_n,

                                                           // outputs:
                                                            advance_to_state_do_0,
                                                            enable_state_do_0,
                                                            inhibit_from_c_do_block1,
                                                            state_do_0
                                                         )
;

  output  [ 22: 0] advance_to_state_do_0;
  output  [ 22: 0] enable_state_do_0;
  output           inhibit_from_c_do_block1;
  output  [ 22: 0] state_do_0;
  input            accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0;
  input            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0;
  input            accelerator_FIR_FIR_HW_master_resource1_fifo_empty0;
  input   [  5: 0] advance_to_state_do_2;
  input   [ 31: 0] break2;
  input   [ 31: 0] break40;
  input            clk;
  input   [  5: 0] enable_state_do_2;
  input   [ 31: 0] go_bit_control_output3;
  input            reset_n;

  wire    [ 22: 0] advance_to_state_do_0;
  wire             block_wide_enable2;
  wire    [ 22: 0] enable_state_do_0;
  reg              inhibit_from_c_do_block1;
  reg     [ 22: 0] state_do_0;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[0] <= 0;
      else if (enable_state_do_0[0])
          state_do_0[0] <= advance_to_state_do_0[0];
    end


  assign enable_state_do_0[0] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[1] <= 0;
      else if (enable_state_do_0[1])
          state_do_0[1] <= advance_to_state_do_0[1];
    end


  assign enable_state_do_0[1] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[2] <= 0;
      else if (enable_state_do_0[2])
          state_do_0[2] <= advance_to_state_do_0[2];
    end


  assign enable_state_do_0[2] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[3] <= 0;
      else if (enable_state_do_0[3])
          state_do_0[3] <= advance_to_state_do_0[3];
    end


  assign enable_state_do_0[3] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[4] <= 0;
      else if (enable_state_do_0[4])
          state_do_0[4] <= advance_to_state_do_0[4];
    end


  assign enable_state_do_0[4] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[5] <= 0;
      else if (enable_state_do_0[5])
          state_do_0[5] <= advance_to_state_do_0[5];
    end


  assign enable_state_do_0[5] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[6] <= 0;
      else if (enable_state_do_0[6])
          state_do_0[6] <= advance_to_state_do_0[6];
    end


  assign enable_state_do_0[6] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[7] <= 0;
      else if (enable_state_do_0[7])
          state_do_0[7] <= advance_to_state_do_0[7];
    end


  assign enable_state_do_0[7] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[8] <= 0;
      else if (enable_state_do_0[8])
          state_do_0[8] <= advance_to_state_do_0[8];
    end


  assign enable_state_do_0[8] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[9] <= 0;
      else if (enable_state_do_0[9])
          state_do_0[9] <= advance_to_state_do_0[9];
    end


  assign enable_state_do_0[9] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[10] <= 0;
      else if (enable_state_do_0[10])
          state_do_0[10] <= advance_to_state_do_0[10];
    end


  assign enable_state_do_0[10] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[11] <= 0;
      else if (enable_state_do_0[11])
          state_do_0[11] <= advance_to_state_do_0[11];
    end


  assign enable_state_do_0[11] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[12] <= 0;
      else if (enable_state_do_0[12])
          state_do_0[12] <= advance_to_state_do_0[12];
    end


  assign enable_state_do_0[12] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[13] <= 0;
      else if (enable_state_do_0[13])
          state_do_0[13] <= advance_to_state_do_0[13];
    end


  assign enable_state_do_0[13] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[14] <= 0;
      else if (enable_state_do_0[14])
          state_do_0[14] <= advance_to_state_do_0[14];
    end


  assign enable_state_do_0[14] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[15] <= 0;
      else if (enable_state_do_0[15])
          state_do_0[15] <= advance_to_state_do_0[15];
    end


  assign enable_state_do_0[15] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[16] <= 0;
      else if (enable_state_do_0[16])
          state_do_0[16] <= advance_to_state_do_0[16];
    end


  assign enable_state_do_0[16] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[17] <= 0;
      else if (enable_state_do_0[17])
          state_do_0[17] <= advance_to_state_do_0[17];
    end


  assign enable_state_do_0[17] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[18] <= 0;
      else if (enable_state_do_0[18])
          state_do_0[18] <= advance_to_state_do_0[18];
    end


  assign enable_state_do_0[18] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[19] <= 0;
      else if (enable_state_do_0[19])
          state_do_0[19] <= advance_to_state_do_0[19];
    end


  assign enable_state_do_0[19] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[20] <= 0;
      else if (enable_state_do_0[20])
          state_do_0[20] <= advance_to_state_do_0[20];
    end


  assign enable_state_do_0[20] = block_wide_enable2;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_0[21] <= 0;
      else if (enable_state_do_0[21])
          state_do_0[21] <= advance_to_state_do_0[21];
    end


  assign enable_state_do_0[21] = block_wide_enable2;
  assign enable_state_do_0[22] = block_wide_enable2;
  //advance_to_state_do_0[0] assign, which is an e_assign
  assign advance_to_state_do_0[0] = (enable_state_do_2[1] & advance_to_state_do_2[1]) & go_bit_control_output3 & !state_do_0[0];

  //advance_to_state_do_0[1] assign, which is an e_assign
  assign advance_to_state_do_0[1] = (state_do_0[0] | advance_to_state_do_0[2]) & !(|break2);

  //advance_to_state_do_0[2] assign, which is an e_assign
  assign advance_to_state_do_0[2] = (state_do_0[1]) & !(|break2);

  //advance_to_state_do_0[3] assign, which is an e_assign
  assign advance_to_state_do_0[3] = state_do_0[2];

  //advance_to_state_do_0[4] assign, which is an e_assign
  assign advance_to_state_do_0[4] = state_do_0[3];

  //advance_to_state_do_0[5] assign, which is an e_assign
  assign advance_to_state_do_0[5] = state_do_0[4];

  //advance_to_state_do_0[6] assign, which is an e_assign
  assign advance_to_state_do_0[6] = state_do_0[5];

  //advance_to_state_do_0[7] assign, which is an e_assign
  assign advance_to_state_do_0[7] = state_do_0[6];

  //advance_to_state_do_0[8] assign, which is an e_assign
  assign advance_to_state_do_0[8] = state_do_0[7];

  //advance_to_state_do_0[9] assign, which is an e_assign
  assign advance_to_state_do_0[9] = state_do_0[8];

  //advance_to_state_do_0[10] assign, which is an e_assign
  assign advance_to_state_do_0[10] = state_do_0[9];

  //advance_to_state_do_0[11] assign, which is an e_assign
  assign advance_to_state_do_0[11] = state_do_0[10];

  //advance_to_state_do_0[12] assign, which is an e_assign
  assign advance_to_state_do_0[12] = state_do_0[11];

  //advance_to_state_do_0[13] assign, which is an e_assign
  assign advance_to_state_do_0[13] = state_do_0[12];

  //advance_to_state_do_0[14] assign, which is an e_assign
  assign advance_to_state_do_0[14] = state_do_0[13];

  //advance_to_state_do_0[15] assign, which is an e_assign
  assign advance_to_state_do_0[15] = state_do_0[14];

  //advance_to_state_do_0[16] assign, which is an e_assign
  assign advance_to_state_do_0[16] = state_do_0[15];

  //advance_to_state_do_0[17] assign, which is an e_assign
  assign advance_to_state_do_0[17] = state_do_0[16];

  //advance_to_state_do_0[18] assign, which is an e_assign
  assign advance_to_state_do_0[18] = state_do_0[17];

  //advance_to_state_do_0[19] assign, which is an e_assign
  assign advance_to_state_do_0[19] = state_do_0[18];

  //advance_to_state_do_0[20] assign, which is an e_assign
  assign advance_to_state_do_0[20] = state_do_0[19];

  //advance_to_state_do_0[21] assign, which is an e_assign
  assign advance_to_state_do_0[21] = state_do_0[20];

  //advance_to_state_do_0[22] assign, which is an e_assign
  assign advance_to_state_do_0[22] = state_do_0[21];

  //on/inhibit register for c_do_block, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          inhibit_from_c_do_block1 <= 0;
      else if (break40 & (enable_state_do_0[22] & advance_to_state_do_0[22]))
          inhibit_from_c_do_block1 <= 0;
      else if (go_bit_control_output3 & (enable_state_do_2[1] & advance_to_state_do_2[1]))
          inhibit_from_c_do_block1 <= -1;
    end


  //block_wide_enable2 inhibitor mux, which is an e_mux
  assign block_wide_enable2 = ((~accelerator_FIR_FIR_HW_master_resource1_fifo_empty0 | ~(state_do_0[14]))) &
    accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0 &
    accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0 &
    1;


endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_master_resource0 (
                                                 // inputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2,
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0,
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0,
                                                  accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0,
                                                  advance_to_state_do_0,
                                                  clk,
                                                  enable_state_do_0,
                                                  go_bit_control_output7,
                                                  reset_n,

                                                 // outputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0,
                                                  accelerator_FIR_FIR_HW_master_resource0_address0,
                                                  accelerator_FIR_FIR_HW_master_resource0_byteenable0,
                                                  accelerator_FIR_FIR_HW_master_resource0_write0,
                                                  accelerator_FIR_FIR_HW_master_resource0_writedata0
                                               )
;

  output           accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address0;
  output  [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable0;
  output           accelerator_FIR_FIR_HW_master_resource0_write0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2;
  input            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0;
  input            accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0;
  input   [ 22: 0] advance_to_state_do_0;
  input            clk;
  input   [ 22: 0] enable_state_do_0;
  input   [ 31: 0] go_bit_control_output7;
  input            reset_n;

  wire             accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0;
  reg     [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address_mux_out0;
  reg     [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable0;
  wire    [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable_mux_out0;
  wire    [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable_shifted0;
  wire             accelerator_FIR_FIR_HW_master_resource0_write0;
  reg     [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata_mux_out0;
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0 = accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0;
  assign accelerator_FIR_FIR_HW_master_resource0_address_mux_out0 = {32 {(((enable_state_do_0[20] & advance_to_state_do_0[20]) & go_bit_control_output7[0]))}} & accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2;
  //accelerator_FIR_FIR_HW_master_resource0 address register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource0_address0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource0_address0 <= accelerator_FIR_FIR_HW_master_resource0_address_mux_out0;
    end


  assign accelerator_FIR_FIR_HW_master_resource0_writedata_mux_out0 = {32 {(((enable_state_do_0[20] & advance_to_state_do_0[20]) & go_bit_control_output7[0]))}} & accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0;
  //accelerator_FIR_FIR_HW_master_resource0 writedata register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource0_writedata0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource0_writedata0 <= accelerator_FIR_FIR_HW_master_resource0_writedata_mux_out0;
    end


  assign accelerator_FIR_FIR_HW_master_resource0_write0 = accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0;
  assign accelerator_FIR_FIR_HW_master_resource0_byteenable_mux_out0 = {4 {(((enable_state_do_0[20] & advance_to_state_do_0[20]) & go_bit_control_output7[0]))}} & 4'b1111;
  assign accelerator_FIR_FIR_HW_master_resource0_byteenable_shifted0 = accelerator_FIR_FIR_HW_master_resource0_byteenable_mux_out0;
  //accelerator_FIR_FIR_HW_master_resource0 byteenable register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource0_byteenable0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource0_byteenable0 <= accelerator_FIR_FIR_HW_master_resource0_byteenable_shifted0;
    end



endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_master_resource1 (
                                                 // inputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2,
                                                  accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0,
                                                  accelerator_FIR_FIR_HW_master_resource1_readdata0,
                                                  accelerator_FIR_FIR_HW_master_resource1_readdatavalid0,
                                                  accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0,
                                                  advance_to_state_do_0,
                                                  clk,
                                                  enable_state_do_0,
                                                  reset_n,

                                                 // outputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0,
                                                  accelerator_FIR_FIR_HW_master_resource1_address0,
                                                  accelerator_FIR_FIR_HW_master_resource1_fifo_empty0,
                                                  accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0,
                                                  accelerator_FIR_FIR_HW_master_resource1_flush0,
                                                  accelerator_FIR_FIR_HW_master_resource1_read0
                                               )
;

  output           accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address0;
  output           accelerator_FIR_FIR_HW_master_resource1_fifo_empty0;
  output  [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0;
  output           accelerator_FIR_FIR_HW_master_resource1_flush0;
  output           accelerator_FIR_FIR_HW_master_resource1_read0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2;
  input            accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0;
  input   [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_readdata0;
  input            accelerator_FIR_FIR_HW_master_resource1_readdatavalid0;
  input            accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0;
  input   [ 22: 0] advance_to_state_do_0;
  input            clk;
  input   [ 22: 0] enable_state_do_0;
  input            reset_n;

  wire             accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0;
  reg     [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address_mux_out0;
  wire             accelerator_FIR_FIR_HW_master_resource1_fifo_aclr0;
  wire             accelerator_FIR_FIR_HW_master_resource1_fifo_empty0;
  wire             accelerator_FIR_FIR_HW_master_resource1_fifo_full0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_fifo_out0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0;
  wire             accelerator_FIR_FIR_HW_master_resource1_fifo_read0;
  wire             accelerator_FIR_FIR_HW_master_resource1_fifo_sclr0;
  wire             accelerator_FIR_FIR_HW_master_resource1_flush0;
  wire             accelerator_FIR_FIR_HW_master_resource1_read0;
  assign accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0 = accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0;
  assign accelerator_FIR_FIR_HW_master_resource1_address_mux_out0 = {32 {(enable_state_do_0[2] & advance_to_state_do_0[2])}} & accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2;
  //accelerator_FIR_FIR_HW_master_resource1 address register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource1_address0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource1_address0 <= accelerator_FIR_FIR_HW_master_resource1_address_mux_out0;
    end


  assign accelerator_FIR_FIR_HW_master_resource1_fifo_sclr0 = enable_state_do_0[0] & advance_to_state_do_0[0];
  assign accelerator_FIR_FIR_HW_master_resource1_fifo_aclr0 = ~reset_n;
  scfifo accelerator_FIR_FIR_HW_shift_fifo0
    (
      .aclr (accelerator_FIR_FIR_HW_master_resource1_fifo_aclr0),
      .clock (clk),
      .data (accelerator_FIR_FIR_HW_master_resource1_readdata0),
      .empty (accelerator_FIR_FIR_HW_master_resource1_fifo_empty0),
      .full (accelerator_FIR_FIR_HW_master_resource1_fifo_full0),
      .q (accelerator_FIR_FIR_HW_master_resource1_fifo_out0),
      .rdreq (accelerator_FIR_FIR_HW_master_resource1_fifo_read0),
      .sclr (accelerator_FIR_FIR_HW_master_resource1_fifo_sclr0),
      .wrreq (accelerator_FIR_FIR_HW_master_resource1_readdatavalid0)
    );

  defparam accelerator_FIR_FIR_HW_shift_fifo0.lpm_numwords = 14,
           accelerator_FIR_FIR_HW_shift_fifo0.lpm_showahead = "ON",
           accelerator_FIR_FIR_HW_shift_fifo0.lpm_width = 16,
           accelerator_FIR_FIR_HW_shift_fifo0.lpm_widthu = 4,
           accelerator_FIR_FIR_HW_shift_fifo0.use_eab = "OFF";

  assign accelerator_FIR_FIR_HW_master_resource1_flush0 = enable_state_do_0[0] & advance_to_state_do_0[0];
  assign accelerator_FIR_FIR_HW_master_resource1_read0 = accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0;
  assign accelerator_FIR_FIR_HW_master_resource1_fifo_read0 = enable_state_do_0[15] & advance_to_state_do_0[15];
  assign accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0 = accelerator_FIR_FIR_HW_master_resource1_fifo_out0;

endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply0 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply0



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply1 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply1



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply2 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply2



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply3 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply3



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply4 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply4



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply5 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply5



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply6 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply6



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 


module accelerator_FIR_FIR_HW_registered_multiply7 
       (
          // inputs:
          a,
          b,
          clk,
          enable,
          reset_n,

          // outputs:
          product
       );

   output [ 63 : 0 ] product;
   input  [ 31 : 0 ] a;
   input  [ 31 : 0 ] b;
   input             clk;
   input             enable;
   input             reset_n;

   wire  [ 63 : 0 ] product;
   reg  [ 31 : 0 ] a_reg;
   reg  [ 31 : 0 ] b_reg;

   assign product = $signed(a_reg) * $signed(b_reg);

   always @(posedge clk or negedge reset_n)
     begin
        if (reset_n == 0)
        begin
          a_reg <= 0;
          b_reg <= 0;
        end
        else if (enable)
        begin
          a_reg <= a;
          b_reg <= b;
       end
     end

endmodule // accelerator_FIR_FIR_HW_registered_multiply7



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

//c_pointer_dereference module for ../fir_hw.c:
//dataA = *input_data++;
//Connections: sdram/avalon_slave_0(64)
module accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_module0 (
                                                                       // inputs:
                                                                        accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1,
                                                                        accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0,
                                                                        advance_to_state_do_0,
                                                                        clk,
                                                                        enable_state_do_0,
                                                                        reset_n,

                                                                       // outputs:
                                                                        accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2,
                                                                        accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0
                                                                     )
;

  output  [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2;
  output           accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1;
  input            accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0;
  input   [ 22: 0] advance_to_state_do_0;
  input            clk;
  input   [ 22: 0] enable_state_do_0;
  input            reset_n;

  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2;
  reg              accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0;
  assign accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2 = accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1;
  //read is set by the previous state enable
  //and reset by no ~accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0 <= 0;
      else if (enable_state_do_0[2] & advance_to_state_do_0[2])
          accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0 <= -1;
      else if (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0)
          accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0 <= 0;
    end



endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

//c_pointer_dereference module for ../fir_hw.c:
//output_buffer[indexA] = adderA + adderB;
//Connections: accelerator_FIR_FIR_HW/accelerator_FIR_FIR_HW_output_buffer_internal_memory0_slave(1)
module accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_module0 (
                                                                                    // inputs:
                                                                                     accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1,
                                                                                     accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0,
                                                                                     advance_to_state_do_0,
                                                                                     clk,
                                                                                     dereferenced_output_buffer_indexA_p3,
                                                                                     enable_state_do_0,
                                                                                     go_bit_control_output7,
                                                                                     reset_n,

                                                                                    // outputs:
                                                                                     accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2,
                                                                                     accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0,
                                                                                     accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0
                                                                                  )
;

  output  [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2;
  output           accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1;
  input            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0;
  input   [ 22: 0] advance_to_state_do_0;
  input            clk;
  input   [ 31: 0] dereferenced_output_buffer_indexA_p3;
  input   [ 22: 0] enable_state_do_0;
  input   [ 31: 0] go_bit_control_output7;
  input            reset_n;

  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2;
  reg              accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0;
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2 = accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1;
  //write is set by the previous state enable
  //and reset by no ~accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0 <= 0;
      else if ((enable_state_do_0[20] & advance_to_state_do_0[20]) & go_bit_control_output7[0])
          accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0 <= -1;
      else if (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0)
          accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0 <= 0;
    end


  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0 = dereferenced_output_buffer_indexA_p3;

endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

// c_subroutine_block
//../fir_hw.c (138):     {
module accelerator_FIR_FIR_HW_FIR_HW0_state_machine_do_2 (
                                                           // inputs:
                                                            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0,
                                                            accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0,
                                                            accelerator_FIR_FIR_HW_master_resource2_fifo_empty0,
                                                            advance_to_state_do_2,
                                                            break42,
                                                            break54,
                                                            clk,
                                                            enable_state_do_2,
                                                            go_bit_control_output9,
                                                            reset_n,

                                                           // outputs:
                                                            advance_to_state_do_1,
                                                            enable_state_do_1,
                                                            inhibit_from_c_do_block2,
                                                            state_do_1
                                                         )
;

  output  [  9: 0] advance_to_state_do_1;
  output  [  9: 0] enable_state_do_1;
  output           inhibit_from_c_do_block2;
  output  [  9: 0] state_do_1;
  input            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0;
  input            accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0;
  input            accelerator_FIR_FIR_HW_master_resource2_fifo_empty0;
  input   [  5: 0] advance_to_state_do_2;
  input   [ 31: 0] break42;
  input   [ 31: 0] break54;
  input            clk;
  input   [  5: 0] enable_state_do_2;
  input   [ 31: 0] go_bit_control_output9;
  input            reset_n;

  wire    [  9: 0] advance_to_state_do_1;
  wire             block_wide_enable3;
  wire    [  9: 0] enable_state_do_1;
  reg              inhibit_from_c_do_block2;
  reg     [  9: 0] state_do_1;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[0] <= 0;
      else if (enable_state_do_1[0])
          state_do_1[0] <= advance_to_state_do_1[0];
    end


  assign enable_state_do_1[0] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[1] <= 0;
      else if (enable_state_do_1[1])
          state_do_1[1] <= advance_to_state_do_1[1];
    end


  assign enable_state_do_1[1] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[2] <= 0;
      else if (enable_state_do_1[2])
          state_do_1[2] <= advance_to_state_do_1[2];
    end


  assign enable_state_do_1[2] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[3] <= 0;
      else if (enable_state_do_1[3])
          state_do_1[3] <= advance_to_state_do_1[3];
    end


  assign enable_state_do_1[3] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[4] <= 0;
      else if (enable_state_do_1[4])
          state_do_1[4] <= advance_to_state_do_1[4];
    end


  assign enable_state_do_1[4] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[5] <= 0;
      else if (enable_state_do_1[5])
          state_do_1[5] <= advance_to_state_do_1[5];
    end


  assign enable_state_do_1[5] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[6] <= 0;
      else if (enable_state_do_1[6])
          state_do_1[6] <= advance_to_state_do_1[6];
    end


  assign enable_state_do_1[6] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[7] <= 0;
      else if (enable_state_do_1[7])
          state_do_1[7] <= advance_to_state_do_1[7];
    end


  assign enable_state_do_1[7] = block_wide_enable3;
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          state_do_1[8] <= 0;
      else if (enable_state_do_1[8])
          state_do_1[8] <= advance_to_state_do_1[8];
    end


  assign enable_state_do_1[8] = block_wide_enable3;
  assign enable_state_do_1[9] = block_wide_enable3;
  //advance_to_state_do_1[0] assign, which is an e_assign
  assign advance_to_state_do_1[0] = (enable_state_do_2[2] & advance_to_state_do_2[2]) & go_bit_control_output9 & !state_do_1[0];

  //advance_to_state_do_1[1] assign, which is an e_assign
  assign advance_to_state_do_1[1] = (state_do_1[0] | advance_to_state_do_1[2]) & !(|break42);

  //advance_to_state_do_1[2] assign, which is an e_assign
  assign advance_to_state_do_1[2] = (state_do_1[1]) & !(|break42);

  //advance_to_state_do_1[3] assign, which is an e_assign
  assign advance_to_state_do_1[3] = state_do_1[2];

  //advance_to_state_do_1[4] assign, which is an e_assign
  assign advance_to_state_do_1[4] = state_do_1[3];

  //advance_to_state_do_1[5] assign, which is an e_assign
  assign advance_to_state_do_1[5] = state_do_1[4];

  //advance_to_state_do_1[6] assign, which is an e_assign
  assign advance_to_state_do_1[6] = state_do_1[5];

  //advance_to_state_do_1[7] assign, which is an e_assign
  assign advance_to_state_do_1[7] = state_do_1[6];

  //advance_to_state_do_1[8] assign, which is an e_assign
  assign advance_to_state_do_1[8] = state_do_1[7];

  //advance_to_state_do_1[9] assign, which is an e_assign
  assign advance_to_state_do_1[9] = state_do_1[8];

  //on/inhibit register for c_do_block, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          inhibit_from_c_do_block2 <= 0;
      else if (break54 & (enable_state_do_1[9] & advance_to_state_do_1[9]))
          inhibit_from_c_do_block2 <= 0;
      else if (go_bit_control_output9 & (enable_state_do_2[2] & advance_to_state_do_2[2]))
          inhibit_from_c_do_block2 <= -1;
    end


  //block_wide_enable3 inhibitor mux, which is an e_mux
  assign block_wide_enable3 = ((~accelerator_FIR_FIR_HW_master_resource2_fifo_empty0 | ~(state_do_1[4]))) &
    accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0 &
    accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0 &
    1;


endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_master_resource3 (
                                                 // inputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2,
                                                  accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0,
                                                  accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0,
                                                  accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0,
                                                  advance_to_state_do_1,
                                                  clk,
                                                  enable_state_do_1,
                                                  go_bit_control_output19,
                                                  reset_n,

                                                 // outputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0,
                                                  accelerator_FIR_FIR_HW_master_resource3_address0,
                                                  accelerator_FIR_FIR_HW_master_resource3_byteenable0,
                                                  accelerator_FIR_FIR_HW_master_resource3_write0,
                                                  accelerator_FIR_FIR_HW_master_resource3_writedata0
                                               )
;

  output           accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address0;
  output  [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable0;
  output           accelerator_FIR_FIR_HW_master_resource3_write0;
  output  [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2;
  input            accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0;
  input   [ 15: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0;
  input            accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0;
  input   [  9: 0] advance_to_state_do_1;
  input            clk;
  input   [  9: 0] enable_state_do_1;
  input   [ 31: 0] go_bit_control_output19;
  input            reset_n;

  wire             accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0;
  reg     [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address_mux_out0;
  reg     [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable0;
  wire    [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable_mux_out0;
  wire    [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable_shifted0;
  wire             accelerator_FIR_FIR_HW_master_resource3_write0;
  reg     [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata_mux_out0;
  assign accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0 = accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0;
  assign accelerator_FIR_FIR_HW_master_resource3_address_mux_out0 = {32 {(((enable_state_do_1[7] & advance_to_state_do_1[7]) & go_bit_control_output19[0]))}} & accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2;
  //accelerator_FIR_FIR_HW_master_resource3 address register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource3_address0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource3_address0 <= accelerator_FIR_FIR_HW_master_resource3_address_mux_out0;
    end


  assign accelerator_FIR_FIR_HW_master_resource3_writedata_mux_out0 = {16 {(((enable_state_do_1[7] & advance_to_state_do_1[7]) & go_bit_control_output19[0]))}} & accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0;
  //accelerator_FIR_FIR_HW_master_resource3 writedata register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource3_writedata0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource3_writedata0 <= accelerator_FIR_FIR_HW_master_resource3_writedata_mux_out0;
    end


  assign accelerator_FIR_FIR_HW_master_resource3_write0 = accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0;
  assign accelerator_FIR_FIR_HW_master_resource3_byteenable_mux_out0 = {2 {(((enable_state_do_1[7] & advance_to_state_do_1[7]) & go_bit_control_output19[0]))}} & 2'b11;
  assign accelerator_FIR_FIR_HW_master_resource3_byteenable_shifted0 = accelerator_FIR_FIR_HW_master_resource3_byteenable_mux_out0;
  //accelerator_FIR_FIR_HW_master_resource3 byteenable register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource3_byteenable0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource3_byteenable0 <= accelerator_FIR_FIR_HW_master_resource3_byteenable_shifted0;
    end



endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_master_resource2 (
                                                 // inputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2,
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0,
                                                  accelerator_FIR_FIR_HW_master_resource2_readdata0,
                                                  accelerator_FIR_FIR_HW_master_resource2_readdatavalid0,
                                                  accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0,
                                                  advance_to_state_do_1,
                                                  clk,
                                                  enable_state_do_1,
                                                  reset_n,

                                                 // outputs:
                                                  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0,
                                                  accelerator_FIR_FIR_HW_master_resource2_address0,
                                                  accelerator_FIR_FIR_HW_master_resource2_fifo_empty0,
                                                  accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0,
                                                  accelerator_FIR_FIR_HW_master_resource2_flush0,
                                                  accelerator_FIR_FIR_HW_master_resource2_read0
                                               )
;

  output           accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address0;
  output           accelerator_FIR_FIR_HW_master_resource2_fifo_empty0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0;
  output           accelerator_FIR_FIR_HW_master_resource2_flush0;
  output           accelerator_FIR_FIR_HW_master_resource2_read0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2;
  input            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_readdata0;
  input            accelerator_FIR_FIR_HW_master_resource2_readdatavalid0;
  input            accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0;
  input   [  9: 0] advance_to_state_do_1;
  input            clk;
  input   [  9: 0] enable_state_do_1;
  input            reset_n;

  wire             accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0;
  reg     [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address_mux_out0;
  wire             accelerator_FIR_FIR_HW_master_resource2_fifo_aclr0;
  wire             accelerator_FIR_FIR_HW_master_resource2_fifo_empty0;
  wire             accelerator_FIR_FIR_HW_master_resource2_fifo_full0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_fifo_out0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0;
  wire             accelerator_FIR_FIR_HW_master_resource2_fifo_read0;
  wire             accelerator_FIR_FIR_HW_master_resource2_fifo_sclr0;
  wire             accelerator_FIR_FIR_HW_master_resource2_flush0;
  wire             accelerator_FIR_FIR_HW_master_resource2_read0;
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0 = accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0;
  assign accelerator_FIR_FIR_HW_master_resource2_address_mux_out0 = {32 {(enable_state_do_1[2] & advance_to_state_do_1[2])}} & accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2;
  //accelerator_FIR_FIR_HW_master_resource2 address register, which is an e_register
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_master_resource2_address0 <= 0;
      else if (accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0)
          accelerator_FIR_FIR_HW_master_resource2_address0 <= accelerator_FIR_FIR_HW_master_resource2_address_mux_out0;
    end


  assign accelerator_FIR_FIR_HW_master_resource2_fifo_sclr0 = enable_state_do_1[0] & advance_to_state_do_1[0];
  assign accelerator_FIR_FIR_HW_master_resource2_fifo_aclr0 = ~reset_n;
  scfifo accelerator_FIR_FIR_HW_shift_fifo1
    (
      .aclr (accelerator_FIR_FIR_HW_master_resource2_fifo_aclr0),
      .clock (clk),
      .data (accelerator_FIR_FIR_HW_master_resource2_readdata0),
      .empty (accelerator_FIR_FIR_HW_master_resource2_fifo_empty0),
      .full (accelerator_FIR_FIR_HW_master_resource2_fifo_full0),
      .q (accelerator_FIR_FIR_HW_master_resource2_fifo_out0),
      .rdreq (accelerator_FIR_FIR_HW_master_resource2_fifo_read0),
      .sclr (accelerator_FIR_FIR_HW_master_resource2_fifo_sclr0),
      .wrreq (accelerator_FIR_FIR_HW_master_resource2_readdatavalid0)
    );

  defparam accelerator_FIR_FIR_HW_shift_fifo1.lpm_numwords = 4,
           accelerator_FIR_FIR_HW_shift_fifo1.lpm_showahead = "ON",
           accelerator_FIR_FIR_HW_shift_fifo1.lpm_width = 32,
           accelerator_FIR_FIR_HW_shift_fifo1.lpm_widthu = 2,
           accelerator_FIR_FIR_HW_shift_fifo1.use_eab = "OFF";

  assign accelerator_FIR_FIR_HW_master_resource2_flush0 = enable_state_do_1[0] & advance_to_state_do_1[0];
  assign accelerator_FIR_FIR_HW_master_resource2_read0 = accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0;
  assign accelerator_FIR_FIR_HW_master_resource2_fifo_read0 = enable_state_do_1[5] & advance_to_state_do_1[5];
  assign accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0 = accelerator_FIR_FIR_HW_master_resource2_fifo_out0;

endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

//c_pointer_dereference module for ../fir_hw.c:
//*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
//Connections: accelerator_FIR_FIR_HW/accelerator_FIR_FIR_HW_output_buffer_internal_memory0_slave(1)
module accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_module0 (
                                                                                   // inputs:
                                                                                    accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1,
                                                                                    accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0,
                                                                                    advance_to_state_do_1,
                                                                                    clk,
                                                                                    enable_state_do_1,
                                                                                    reset_n,

                                                                                   // outputs:
                                                                                    accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2,
                                                                                    accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0
                                                                                 )
;

  output  [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2;
  output           accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1;
  input            accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0;
  input   [  9: 0] advance_to_state_do_1;
  input            clk;
  input   [  9: 0] enable_state_do_1;
  input            reset_n;

  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2;
  reg              accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0;
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2 = accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1;
  //read is set by the previous state enable
  //and reset by no ~accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0 <= 0;
      else if (enable_state_do_1[2] & advance_to_state_do_1[2])
          accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0 <= -1;
      else if (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0)
          accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0 <= 0;
    end



endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

//c_pointer_dereference module for ../fir_hw.c:
//*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
//Connections: sdram/avalon_slave_0(64)
module accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_module0 (
                                                                         // inputs:
                                                                          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1,
                                                                          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0,
                                                                          advance_to_state_do_1,
                                                                          clk,
                                                                          dereferenced_output_data2,
                                                                          enable_state_do_1,
                                                                          go_bit_control_output19,
                                                                          reset_n,

                                                                         // outputs:
                                                                          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2,
                                                                          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0,
                                                                          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0
                                                                       )
;

  output  [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2;
  output           accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0;
  output  [ 15: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1;
  input            accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0;
  input   [  9: 0] advance_to_state_do_1;
  input            clk;
  input   [ 15: 0] dereferenced_output_data2;
  input   [  9: 0] enable_state_do_1;
  input   [ 31: 0] go_bit_control_output19;
  input            reset_n;

  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2;
  reg              accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0;
  assign accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2 = accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1;
  //write is set by the previous state enable
  //and reset by no ~accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0
  always @(posedge clk or negedge reset_n)
    begin
      if (reset_n == 0)
          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0 <= 0;
      else if ((enable_state_do_1[7] & advance_to_state_do_1[7]) & go_bit_control_output19[0])
          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0 <= -1;
      else if (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0)
          accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0 <= 0;
    end


  assign accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0 = dereferenced_output_data2;

endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_FIR_HW0 (
                                        // inputs:
                                         FIR_HW_begin0,
                                         FIR_HW_functiondata0,
                                         FIR_HW_read0,
                                         FIR_HW_select0,
                                         accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0,
                                         accelerator_FIR_FIR_HW_master_resource1_readdata0,
                                         accelerator_FIR_FIR_HW_master_resource1_readdatavalid0,
                                         accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0,
                                         accelerator_FIR_FIR_HW_master_resource2_readdata0,
                                         accelerator_FIR_FIR_HW_master_resource2_readdatavalid0,
                                         accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0,
                                         accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0,
                                         accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0,
                                         clk,
                                         reset_n,

                                        // outputs:
                                         FIR_HW_waitrequest_n0,
                                         accelerator_FIR_FIR_HW_master_resource0_address0,
                                         accelerator_FIR_FIR_HW_master_resource0_byteenable0,
                                         accelerator_FIR_FIR_HW_master_resource0_write0,
                                         accelerator_FIR_FIR_HW_master_resource0_writedata0,
                                         accelerator_FIR_FIR_HW_master_resource1_address0,
                                         accelerator_FIR_FIR_HW_master_resource1_flush0,
                                         accelerator_FIR_FIR_HW_master_resource1_read0,
                                         accelerator_FIR_FIR_HW_master_resource2_address0,
                                         accelerator_FIR_FIR_HW_master_resource2_flush0,
                                         accelerator_FIR_FIR_HW_master_resource2_read0,
                                         accelerator_FIR_FIR_HW_master_resource3_address0,
                                         accelerator_FIR_FIR_HW_master_resource3_byteenable0,
                                         accelerator_FIR_FIR_HW_master_resource3_write0,
                                         accelerator_FIR_FIR_HW_master_resource3_writedata0,
                                         cpu_readdata0
                                      )
;

  output           FIR_HW_waitrequest_n0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address0;
  output  [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable0;
  output           accelerator_FIR_FIR_HW_master_resource0_write0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address0;
  output           accelerator_FIR_FIR_HW_master_resource1_flush0;
  output           accelerator_FIR_FIR_HW_master_resource1_read0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address0;
  output           accelerator_FIR_FIR_HW_master_resource2_flush0;
  output           accelerator_FIR_FIR_HW_master_resource2_read0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address0;
  output  [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable0;
  output           accelerator_FIR_FIR_HW_master_resource3_write0;
  output  [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata0;
  output  [ 31: 0] cpu_readdata0;
  input            FIR_HW_begin0;
  input   [239: 0] FIR_HW_functiondata0;
  input            FIR_HW_read0;
  input            FIR_HW_select0;
  input            accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0;
  input   [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_readdata0;
  input            accelerator_FIR_FIR_HW_master_resource1_readdatavalid0;
  input            accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_readdata0;
  input            accelerator_FIR_FIR_HW_master_resource2_readdatavalid0;
  input            accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0;
  input            accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0;
  input            clk;
  input            reset_n;

  wire    [  3: 0] FIR_HW_state0;
  wire             FIR_HW_waitrequest_n0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2;
  wire             accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0;
  wire             accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2;
  wire             accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0;
  wire             accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p2_read0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p3_read0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p4_read0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2;
  wire             accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0;
  wire             accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2;
  wire             accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0;
  wire             accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address0;
  wire    [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable0;
  wire             accelerator_FIR_FIR_HW_master_resource0_write0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address0;
  wire             accelerator_FIR_FIR_HW_master_resource1_fifo_empty0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0;
  wire             accelerator_FIR_FIR_HW_master_resource1_flush0;
  wire             accelerator_FIR_FIR_HW_master_resource1_read0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address0;
  wire             accelerator_FIR_FIR_HW_master_resource2_fifo_empty0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0;
  wire             accelerator_FIR_FIR_HW_master_resource2_flush0;
  wire             accelerator_FIR_FIR_HW_master_resource2_read0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address0;
  wire    [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable0;
  wire             accelerator_FIR_FIR_HW_master_resource3_write0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource0_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource0_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource0_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource1_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource1_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource1_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource2_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource2_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource2_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource3_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource3_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource3_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource4_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource4_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource4_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource5_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource5_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource5_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource6_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource6_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource6_res0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource7_op_a0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_multiplier_resource7_op_b0;
  wire    [ 63: 0] accelerator_FIR_FIR_HW_multiplier_resource7_res0;
  wire    [ 31: 0] adderA0;
  reg     [ 31: 0] adderA1;
  reg     [ 31: 0] adderA2;
  reg     [ 31: 0] adderA3;
  wire    [ 31: 0] adderB0;
  reg     [ 31: 0] adderB1;
  reg     [ 31: 0] adderB2;
  reg     [ 31: 0] adderB3;
  wire    [  3: 0] advance_to_FIR_HW_state0;
  wire    [ 22: 0] advance_to_state_do_0;
  wire    [  9: 0] advance_to_state_do_1;
  wire    [  5: 0] advance_to_state_do_2;
  wire    [ 31: 0] break0;
  wire    [ 31: 0] break1;
  reg     [ 31: 0] break10;
  reg     [ 31: 0] break11;
  reg     [ 31: 0] break12;
  reg     [ 31: 0] break13;
  reg     [ 31: 0] break14;
  reg     [ 31: 0] break15;
  reg     [ 31: 0] break16;
  reg     [ 31: 0] break17;
  reg     [ 31: 0] break18;
  reg     [ 31: 0] break19;
  reg     [ 31: 0] break2;
  reg     [ 31: 0] break20;
  reg     [ 31: 0] break21;
  reg     [ 31: 0] break22;
  reg     [ 31: 0] break23;
  reg     [ 31: 0] break24;
  reg     [ 31: 0] break25;
  reg     [ 31: 0] break26;
  reg     [ 31: 0] break27;
  reg     [ 31: 0] break28;
  reg     [ 31: 0] break29;
  reg     [ 31: 0] break3;
  reg     [ 31: 0] break30;
  reg     [ 31: 0] break31;
  reg     [ 31: 0] break32;
  reg     [ 31: 0] break33;
  reg     [ 31: 0] break34;
  reg     [ 31: 0] break35;
  reg     [ 31: 0] break36;
  reg     [ 31: 0] break37;
  reg     [ 31: 0] break38;
  reg     [ 31: 0] break39;
  reg     [ 31: 0] break4;
  wire    [ 31: 0] break40;
  wire    [ 31: 0] break41;
  reg     [ 31: 0] break42;
  reg     [ 31: 0] break43;
  reg     [ 31: 0] break44;
  reg     [ 31: 0] break45;
  reg     [ 31: 0] break46;
  reg     [ 31: 0] break47;
  reg     [ 31: 0] break48;
  reg     [ 31: 0] break49;
  reg     [ 31: 0] break5;
  reg     [ 31: 0] break50;
  reg     [ 31: 0] break51;
  reg     [ 31: 0] break52;
  reg     [ 31: 0] break53;
  wire    [ 31: 0] break54;
  reg     [ 31: 0] break55;
  wire    [ 31: 0] break56;
  reg     [ 31: 0] break6;
  reg     [ 31: 0] break7;
  reg     [ 31: 0] break8;
  reg     [ 31: 0] break9;
  wire    [ 15: 0] coefA0;
  wire    [ 15: 0] coefB0;
  wire    [ 15: 0] coefC0;
  wire    [ 15: 0] coefD0;
  wire    [ 15: 0] coefE0;
  wire    [ 15: 0] coefF0;
  wire    [ 15: 0] coefG0;
  wire    [ 15: 0] coefH0;
  wire    [ 31: 0] continue0;
  wire    [ 31: 0] continue1;
  wire    [ 31: 0] continue2;
  wire    [ 31: 0] control_output_break1;
  wire    [ 31: 0] control_output_break3;
  wire    [ 31: 0] control_output_break5;
  wire    [ 31: 0] cpu_readdata0;
  wire    [ 15: 0] dataA0;
  wire    [ 15: 0] dataA1;
  wire    [ 15: 0] dataA2;
  wire    [ 15: 0] dataA3;
  wire    [ 15: 0] dataA4;
  reg     [ 15: 0] dataA5;
  reg     [ 15: 0] dataA6;
  wire    [ 15: 0] dataA7;
  wire    [ 15: 0] dataA8;
  reg     [ 15: 0] dataA9;
  wire    [ 15: 0] dataA_init1;
  wire    [ 15: 0] dataA_init3;
  wire    [ 15: 0] dataB0;
  wire    [ 15: 0] dataB1;
  wire    [ 15: 0] dataB2;
  wire    [ 15: 0] dataB3;
  wire    [ 15: 0] dataB4;
  reg     [ 15: 0] dataB5;
  reg     [ 15: 0] dataB6;
  wire    [ 15: 0] dataB7;
  wire    [ 15: 0] dataB8;
  reg     [ 15: 0] dataB9;
  wire    [ 15: 0] dataB_init1;
  wire    [ 15: 0] dataB_init3;
  wire    [ 15: 0] dataC0;
  wire    [ 15: 0] dataC1;
  wire    [ 15: 0] dataC2;
  wire    [ 15: 0] dataC3;
  wire    [ 15: 0] dataC4;
  reg     [ 15: 0] dataC5;
  reg     [ 15: 0] dataC6;
  wire    [ 15: 0] dataC7;
  wire    [ 15: 0] dataC8;
  reg     [ 15: 0] dataC9;
  wire    [ 15: 0] dataC_init1;
  wire    [ 15: 0] dataC_init3;
  wire    [ 15: 0] dataD0;
  wire    [ 15: 0] dataD1;
  wire    [ 15: 0] dataD2;
  wire    [ 15: 0] dataD3;
  wire    [ 15: 0] dataD4;
  reg     [ 15: 0] dataD5;
  reg     [ 15: 0] dataD6;
  wire    [ 15: 0] dataD7;
  wire    [ 15: 0] dataD8;
  reg     [ 15: 0] dataD9;
  wire    [ 15: 0] dataD_init1;
  wire    [ 15: 0] dataD_init3;
  wire    [ 15: 0] dataE0;
  wire    [ 15: 0] dataE1;
  wire    [ 15: 0] dataE2;
  wire    [ 15: 0] dataE3;
  wire    [ 15: 0] dataE4;
  reg     [ 15: 0] dataE5;
  reg     [ 15: 0] dataE6;
  wire    [ 15: 0] dataE7;
  wire    [ 15: 0] dataE8;
  reg     [ 15: 0] dataE9;
  wire    [ 15: 0] dataE_init1;
  wire    [ 15: 0] dataE_init3;
  wire    [ 15: 0] dataF0;
  wire    [ 15: 0] dataF1;
  wire    [ 15: 0] dataF2;
  wire    [ 15: 0] dataF3;
  wire    [ 15: 0] dataF4;
  reg     [ 15: 0] dataF5;
  reg     [ 15: 0] dataF6;
  wire    [ 15: 0] dataF7;
  wire    [ 15: 0] dataF8;
  reg     [ 15: 0] dataF9;
  wire    [ 15: 0] dataF_init1;
  wire    [ 15: 0] dataF_init3;
  wire    [ 15: 0] dataG0;
  wire    [ 15: 0] dataG1;
  wire    [ 15: 0] dataG2;
  wire    [ 15: 0] dataG3;
  wire    [ 15: 0] dataG4;
  reg     [ 15: 0] dataG5;
  reg     [ 15: 0] dataG6;
  wire    [ 15: 0] dataG7;
  wire    [ 15: 0] dataG8;
  reg     [ 15: 0] dataG9;
  wire    [ 15: 0] dataG_init1;
  wire    [ 15: 0] dataG_init3;
  wire    [ 15: 0] dataH0;
  wire    [ 15: 0] dataH1;
  wire    [ 15: 0] dataH2;
  wire    [ 15: 0] dataH3;
  wire    [ 15: 0] dataH4;
  reg     [ 15: 0] dataH5;
  reg     [ 15: 0] dataH6;
  wire    [ 15: 0] dataH7;
  wire    [ 15: 0] dataH8;
  reg     [ 15: 0] dataH9;
  wire    [ 15: 0] dataH_init1;
  wire    [ 15: 0] dataH_init3;
  wire    [ 15: 0] dataI0;
  wire    [ 15: 0] dataI1;
  reg     [ 15: 0] dataI2;
  wire    [ 15: 0] dataI3;
  wire    [ 15: 0] dataI4;
  reg     [ 15: 0] dataI5;
  wire    [ 15: 0] dataI6;
  wire    [ 15: 0] dataI7;
  reg     [ 15: 0] dataI8;
  wire    [ 15: 0] dataI_init1;
  wire    [ 15: 0] dataI_init3;
  wire    [ 15: 0] dataJ0;
  wire    [ 15: 0] dataJ1;
  reg     [ 15: 0] dataJ2;
  wire    [ 15: 0] dataJ3;
  wire    [ 15: 0] dataJ4;
  reg     [ 15: 0] dataJ5;
  reg     [ 15: 0] dataJ6;
  wire    [ 15: 0] dataJ7;
  wire    [ 15: 0] dataJ8;
  reg     [ 15: 0] dataJ9;
  wire    [ 15: 0] dataJ_init1;
  wire    [ 15: 0] dataJ_init3;
  wire    [ 15: 0] dataK0;
  wire    [ 15: 0] dataK1;
  wire    [ 15: 0] dataK2;
  wire    [ 15: 0] dataK3;
  wire    [ 15: 0] dataK4;
  reg     [ 15: 0] dataK5;
  reg     [ 15: 0] dataK6;
  wire    [ 15: 0] dataK7;
  wire    [ 15: 0] dataK8;
  reg     [ 15: 0] dataK9;
  wire    [ 15: 0] dataK_init1;
  wire    [ 15: 0] dataK_init3;
  wire    [ 15: 0] dataL0;
  wire    [ 15: 0] dataL1;
  wire    [ 15: 0] dataL2;
  wire    [ 15: 0] dataL3;
  wire    [ 15: 0] dataL4;
  reg     [ 15: 0] dataL5;
  reg     [ 15: 0] dataL6;
  wire    [ 15: 0] dataL7;
  wire    [ 15: 0] dataL8;
  reg     [ 15: 0] dataL9;
  wire    [ 15: 0] dataL_init1;
  wire    [ 15: 0] dataL_init3;
  wire    [ 15: 0] dataM0;
  wire    [ 15: 0] dataM1;
  wire    [ 15: 0] dataM2;
  wire    [ 15: 0] dataM3;
  wire    [ 15: 0] dataM4;
  reg     [ 15: 0] dataM5;
  reg     [ 15: 0] dataM6;
  wire    [ 15: 0] dataM7;
  wire    [ 15: 0] dataM8;
  reg     [ 15: 0] dataM9;
  wire    [ 15: 0] dataM_init1;
  wire    [ 15: 0] dataM_init3;
  wire    [ 15: 0] dataN0;
  wire    [ 15: 0] dataN1;
  wire    [ 15: 0] dataN2;
  wire    [ 15: 0] dataN3;
  wire    [ 15: 0] dataN4;
  reg     [ 15: 0] dataN5;
  reg     [ 15: 0] dataN6;
  wire    [ 15: 0] dataN7;
  wire    [ 15: 0] dataN8;
  reg     [ 15: 0] dataN9;
  wire    [ 15: 0] dataN_init1;
  wire    [ 15: 0] dataN_init3;
  wire    [ 15: 0] dataO0;
  wire    [ 15: 0] dataO1;
  wire    [ 15: 0] dataO2;
  wire    [ 15: 0] dataO3;
  wire    [ 15: 0] dataO4;
  reg     [ 15: 0] dataO5;
  reg     [ 15: 0] dataO6;
  wire    [ 15: 0] dataO7;
  wire    [ 15: 0] dataO8;
  reg     [ 15: 0] dataO9;
  wire    [ 15: 0] dataO_init1;
  wire    [ 15: 0] dataO_init3;
  wire    [ 15: 0] dataP0;
  wire    [ 15: 0] dataP1;
  wire    [ 15: 0] dataP2;
  reg     [ 15: 0] dataP3;
  reg     [ 15: 0] dataP4;
  reg     [ 15: 0] dataP5;
  reg     [ 15: 0] dereferenced_input_data2;
  reg     [ 31: 0] dereferenced_output_buffer_indexA_p3;
  wire    [ 31: 0] dereferenced_output_buffer_indexB_p3;
  wire    [ 31: 0] dereferenced_output_buffer_indexB_p4;
  wire    [ 31: 0] dereferenced_output_buffer_indexB_p5;
  reg     [ 31: 0] dereferenced_output_buffer_indexB_p6;
  reg     [ 15: 0] dereferenced_output_data2;
  wire    [  7: 0] divide_order0;
  wire    [  3: 0] enable_FIR_HW_state0;
  wire    [ 22: 0] enable_state_do_0;
  wire    [  9: 0] enable_state_do_1;
  wire    [  5: 0] enable_state_do_2;
  reg     [ 31: 0] first_time_at_clock_0_state_do_1;
  reg     [ 31: 0] first_time_at_clock_0_state_do_4;
  reg     [ 31: 0] first_time_at_clock_14_state_do_1;
  reg     [ 31: 0] first_time_at_clock_15_state_do_1;
  reg     [ 31: 0] first_time_at_clock_20_state_do_1;
  reg     [ 31: 0] first_time_at_clock_2_state_do_3;
  reg     [ 31: 0] first_time_at_clock_3_state_do_3;
  reg     [ 31: 0] first_time_at_clock_7_state_do_2;
  wire    [ 31: 0] go_bit_control_output1;
  wire    [ 31: 0] go_bit_control_output19;
  wire    [ 31: 0] go_bit_control_output3;
  wire    [ 31: 0] go_bit_control_output7;
  wire    [ 31: 0] go_bit_control_output9;
  wire    [ 31: 0] if0;
  wire    [ 31: 0] if1;
  reg     [ 31: 0] if10;
  reg     [ 31: 0] if11;
  reg     [ 31: 0] if12;
  reg     [ 31: 0] if13;
  reg     [ 31: 0] if14;
  reg     [ 31: 0] if15;
  reg     [ 31: 0] if16;
  reg     [ 31: 0] if17;
  reg     [ 31: 0] if18;
  reg     [ 31: 0] if19;
  wire    [ 31: 0] if2;
  reg     [ 31: 0] if20;
  wire    [ 31: 0] if21;
  reg     [ 31: 0] if22;
  reg     [ 31: 0] if23;
  reg     [ 31: 0] if24;
  reg     [ 31: 0] if25;
  reg     [ 31: 0] if26;
  reg     [ 31: 0] if3;
  reg     [ 31: 0] if4;
  reg     [ 31: 0] if5;
  reg     [ 31: 0] if6;
  reg     [ 31: 0] if7;
  reg     [ 31: 0] if8;
  reg     [ 31: 0] if9;
  wire    [ 15: 0] indexA0;
  wire    [ 15: 0] indexA1;
  reg     [ 15: 0] indexA10;
  reg     [ 15: 0] indexA11;
  reg     [ 15: 0] indexA12;
  reg     [ 15: 0] indexA13;
  reg     [ 15: 0] indexA14;
  reg     [ 15: 0] indexA15;
  reg     [ 15: 0] indexA16;
  reg     [ 15: 0] indexA17;
  reg     [ 15: 0] indexA18;
  reg     [ 15: 0] indexA19;
  reg     [ 15: 0] indexA2;
  reg     [ 15: 0] indexA20;
  reg     [ 15: 0] indexA21;
  reg     [ 15: 0] indexA22;
  reg     [ 15: 0] indexA23;
  reg     [ 15: 0] indexA24;
  reg     [ 15: 0] indexA25;
  reg     [ 15: 0] indexA26;
  wire    [ 15: 0] indexA3;
  wire    [ 15: 0] indexA4;
  reg     [ 15: 0] indexA5;
  reg     [ 15: 0] indexA6;
  reg     [ 15: 0] indexA7;
  reg     [ 15: 0] indexA8;
  reg     [ 15: 0] indexA9;
  wire    [ 15: 0] indexA_init1;
  wire    [ 15: 0] indexB0;
  wire    [ 15: 0] indexB1;
  reg     [ 15: 0] indexB10;
  reg     [ 15: 0] indexB11;
  reg     [ 15: 0] indexB12;
  reg     [ 15: 0] indexB2;
  wire    [ 15: 0] indexB3;
  wire    [ 15: 0] indexB4;
  reg     [ 15: 0] indexB5;
  reg     [ 15: 0] indexB6;
  reg     [ 15: 0] indexB7;
  reg     [ 15: 0] indexB8;
  reg     [ 15: 0] indexB9;
  wire    [ 15: 0] indexB_init1;
  wire             inhibit_from_c_do_block0;
  wire             inhibit_from_c_do_block1;
  wire             inhibit_from_c_do_block2;
  wire    [ 31: 0] input_data0;
  reg     [ 31: 0] input_data10;
  reg     [ 31: 0] input_data11;
  reg     [ 31: 0] input_data12;
  reg     [ 31: 0] input_data13;
  reg     [ 31: 0] input_data14;
  reg     [ 31: 0] input_data15;
  reg     [ 31: 0] input_data16;
  reg     [ 31: 0] input_data17;
  reg     [ 31: 0] input_data18;
  reg     [ 31: 0] input_data19;
  reg     [ 31: 0] input_data2;
  reg     [ 31: 0] input_data20;
  reg     [ 31: 0] input_data21;
  reg     [ 31: 0] input_data22;
  wire    [ 31: 0] input_data23;
  wire    [ 31: 0] input_data24;
  reg     [ 31: 0] input_data25;
  wire    [ 31: 0] input_data3;
  wire    [ 31: 0] input_data4;
  reg     [ 31: 0] input_data5;
  reg     [ 31: 0] input_data6;
  reg     [ 31: 0] input_data7;
  reg     [ 31: 0] input_data8;
  reg     [ 31: 0] input_data9;
  wire    [ 31: 0] input_data_init1;
  wire    [ 31: 0] input_data_init3;
  wire    [ 31: 0] length0;
  reg     [ 31: 0] length1;
  wire    [ 31: 0] length2;
  wire    [ 31: 0] length3;
  reg     [ 31: 0] length4;
  reg     [ 31: 0] length5;
  wire    [ 31: 0] length6;
  wire    [ 31: 0] length7;
  reg     [ 31: 0] length8;
  wire    [ 31: 0] length_init1;
  wire    [ 31: 0] length_init3;
  wire             lpm_mult_clken0;
  wire             lpm_mult_clken1;
  wire             lpm_mult_clken2;
  wire             lpm_mult_clken3;
  wire             lpm_mult_clken4;
  wire             lpm_mult_clken5;
  wire             lpm_mult_clken6;
  wire             lpm_mult_clken7;
  wire    [ 63: 0] lpm_multiply_result0;
  wire    [ 63: 0] lpm_multiply_result1;
  wire    [ 63: 0] lpm_multiply_result2;
  wire    [ 63: 0] lpm_multiply_result3;
  wire    [ 63: 0] lpm_multiply_result4;
  wire    [ 63: 0] lpm_multiply_result5;
  wire    [ 63: 0] lpm_multiply_result6;
  wire    [ 63: 0] lpm_multiply_result7;
  reg     [ 31: 0] multiply1;
  reg     [ 31: 0] multiply11;
  reg     [ 31: 0] multiply13;
  reg     [ 31: 0] multiply15;
  reg     [ 31: 0] multiply3;
  reg     [ 31: 0] multiply5;
  reg     [ 31: 0] multiply7;
  reg     [ 31: 0] multiply9;
  wire    [ 31: 0] multiply_assignment0_op_a1;
  wire    [ 31: 0] multiply_assignment0_op_a2;
  wire    [ 31: 0] multiply_assignment0_op_b1;
  wire    [ 31: 0] multiply_assignment0_op_b2;
  wire    [ 63: 0] multiply_assignment0_prod0;
  wire    [ 31: 0] multiply_assignment1_op_a1;
  wire    [ 31: 0] multiply_assignment1_op_a2;
  wire    [ 31: 0] multiply_assignment1_op_b1;
  wire    [ 31: 0] multiply_assignment1_op_b2;
  wire    [ 63: 0] multiply_assignment1_prod0;
  wire    [ 31: 0] multiply_assignment2_op_a1;
  wire    [ 31: 0] multiply_assignment2_op_a2;
  wire    [ 31: 0] multiply_assignment2_op_b1;
  wire    [ 31: 0] multiply_assignment2_op_b2;
  wire    [ 63: 0] multiply_assignment2_prod0;
  wire    [ 31: 0] multiply_assignment3_op_a1;
  wire    [ 31: 0] multiply_assignment3_op_a2;
  wire    [ 31: 0] multiply_assignment3_op_b1;
  wire    [ 31: 0] multiply_assignment3_op_b2;
  wire    [ 63: 0] multiply_assignment3_prod0;
  wire    [ 31: 0] multiply_assignment4_op_a1;
  wire    [ 31: 0] multiply_assignment4_op_a2;
  wire    [ 31: 0] multiply_assignment4_op_b1;
  wire    [ 31: 0] multiply_assignment4_op_b2;
  wire    [ 63: 0] multiply_assignment4_prod0;
  wire    [ 31: 0] multiply_assignment5_op_a1;
  wire    [ 31: 0] multiply_assignment5_op_a2;
  wire    [ 31: 0] multiply_assignment5_op_b1;
  wire    [ 31: 0] multiply_assignment5_op_b2;
  wire    [ 63: 0] multiply_assignment5_prod0;
  wire    [ 31: 0] multiply_assignment6_op_a1;
  wire    [ 31: 0] multiply_assignment6_op_a2;
  wire    [ 31: 0] multiply_assignment6_op_b1;
  wire    [ 31: 0] multiply_assignment6_op_b2;
  wire    [ 63: 0] multiply_assignment6_prod0;
  wire    [ 31: 0] multiply_assignment7_op_a1;
  wire    [ 31: 0] multiply_assignment7_op_a2;
  wire    [ 31: 0] multiply_assignment7_op_b1;
  wire    [ 31: 0] multiply_assignment7_op_b2;
  wire    [ 63: 0] multiply_assignment7_prod0;
  wire    [ 31: 0] output_buffer0;
  wire    [ 31: 0] output_data0;
  reg     [ 31: 0] output_data10;
  reg     [ 31: 0] output_data11;
  reg     [ 31: 0] output_data12;
  wire    [ 31: 0] output_data13;
  wire    [ 31: 0] output_data14;
  reg     [ 31: 0] output_data15;
  reg     [ 31: 0] output_data2;
  wire    [ 31: 0] output_data3;
  wire    [ 31: 0] output_data4;
  reg     [ 31: 0] output_data5;
  reg     [ 31: 0] output_data6;
  reg     [ 31: 0] output_data7;
  reg     [ 31: 0] output_data8;
  reg     [ 31: 0] output_data9;
  wire    [ 31: 0] output_data_init1;
  wire    [ 31: 0] output_data_init3;
  wire    [ 31: 0] return0;
  wire    [ 31: 0] signal0;
  wire    [ 31: 0] signal1;
  wire    [ 31: 0] signal10;
  wire    [ 31: 0] signal2;
  wire    [ 31: 0] signal3;
  wire    [ 31: 0] signal4;
  wire    [ 31: 0] signal5;
  wire    [ 31: 0] signal6;
  wire    [ 31: 0] signal7;
  wire    [ 31: 0] signal8;
  wire    [ 31: 0] signal9;
  wire    [ 22: 0] state_do_0;
  wire    [  9: 0] state_do_1;
  wire    [  5: 0] state_do_2;
  wire    [ 31: 0] subexp0;
  wire    [ 31: 0] subexp1;
  wire    [ 31: 0] subexp10;
  wire    [ 31: 0] subexp11;
  wire    [ 31: 0] subexp12;
  wire    [ 31: 0] subexp13;
  wire    [ 15: 0] subexp14;
  wire    [ 31: 0] subexp15;
  wire    [ 31: 0] subexp16;
  wire    [ 31: 0] subexp17;
  wire    [ 31: 0] subexp18;
  wire    [ 31: 0] subexp19;
  wire    [ 31: 0] subexp2;
  wire    [ 31: 0] subexp20;
  wire    [ 31: 0] subexp21;
  wire    [ 31: 0] subexp22;
  wire    [ 31: 0] subexp23;
  wire    [ 31: 0] subexp24;
  wire    [ 31: 0] subexp25;
  wire    [ 31: 0] subexp26;
  wire    [ 31: 0] subexp27;
  wire    [ 31: 0] subexp28;
  wire    [ 31: 0] subexp29;
  wire    [ 31: 0] subexp3;
  wire    [ 31: 0] subexp30;
  wire    [ 31: 0] subexp31;
  wire    [ 31: 0] subexp32;
  wire    [ 31: 0] subexp33;
  wire    [ 31: 0] subexp34;
  wire    [ 31: 0] subexp35;
  wire    [ 31: 0] subexp36;
  wire    [ 31: 0] subexp37;
  wire    [ 31: 0] subexp38;
  wire    [ 31: 0] subexp39;
  wire    [ 31: 0] subexp4;
  wire    [ 31: 0] subexp40;
  wire    [ 31: 0] subexp41;
  wire    [ 31: 0] subexp42;
  wire    [ 31: 0] subexp43;
  wire    [ 31: 0] subexp44;
  wire    [ 31: 0] subexp45;
  wire    [ 31: 0] subexp46;
  wire    [ 31: 0] subexp47;
  wire    [ 15: 0] subexp48;
  wire    [ 31: 0] subexp49;
  wire    [ 31: 0] subexp5;
  wire    [ 31: 0] subexp50;
  wire    [ 31: 0] subexp51;
  wire    [ 31: 0] subexp52;
  wire    [ 31: 0] subexp53;
  wire    [ 31: 0] subexp54;
  wire    [ 31: 0] subexp55;
  wire    [ 31: 0] subexp56;
  wire    [ 31: 0] subexp57;
  wire    [ 31: 0] subexp58;
  wire    [ 31: 0] subexp59;
  wire    [ 31: 0] subexp6;
  wire    [ 31: 0] subexp60;
  wire    [ 31: 0] subexp61;
  wire    [ 31: 0] subexp62;
  wire    [ 31: 0] subexp63;
  wire    [ 31: 0] subexp64;
  wire    [ 31: 0] subexp65;
  wire    [ 31: 0] subexp7;
  wire    [ 31: 0] subexp8;
  wire    [ 31: 0] subexp9;
  wire    [  7: 0] tap_length0;
  wire    [ 31: 0] termA0;
  wire    [ 31: 0] termA1;
  reg     [ 31: 0] termA2;
  reg     [ 31: 0] termA3;
  wire    [ 31: 0] termB0;
  wire    [ 31: 0] termB1;
  reg     [ 31: 0] termB2;
  reg     [ 31: 0] termB3;
  wire    [ 31: 0] termC0;
  wire    [ 31: 0] termC1;
  reg     [ 31: 0] termC2;
  reg     [ 31: 0] termC3;
  wire    [ 31: 0] termD0;
  wire    [ 31: 0] termD1;
  reg     [ 31: 0] termD2;
  reg     [ 31: 0] termD3;
  wire    [ 31: 0] termE0;
  wire    [ 31: 0] termE1;
  reg     [ 31: 0] termE2;
  reg     [ 31: 0] termE3;
  wire    [ 31: 0] termF0;
  wire    [ 31: 0] termF1;
  reg     [ 31: 0] termF2;
  reg     [ 31: 0] termF3;
  wire    [ 31: 0] termG0;
  wire    [ 31: 0] termG1;
  reg     [ 31: 0] termG2;
  reg     [ 31: 0] termG3;
  wire    [ 31: 0] termH0;
  wire    [ 31: 0] termH1;
  reg     [ 31: 0] termH2;
  reg     [ 31: 0] termH3;
  assign FIR_HW_waitrequest_n0 = enable_FIR_HW_state0[3] & advance_to_FIR_HW_state0[3];
  //c_subroutine_block
  accelerator_FIR_FIR_HW_FIR_HW0_state_machine_subroutine_0 accelerator_FIR_FIR_HW_FIR_HW0_the_state_machine_subroutine_0
    (
      .FIR_HW_begin0            (FIR_HW_begin0),
      .FIR_HW_read0             (FIR_HW_read0),
      .FIR_HW_select0           (FIR_HW_select0),
      .FIR_HW_state0            (FIR_HW_state0),
      .advance_to_FIR_HW_state0 (advance_to_FIR_HW_state0),
      .clk                      (clk),
      .enable_FIR_HW_state0     (enable_FIR_HW_state0),
      .inhibit_from_c_do_block0 (inhibit_from_c_do_block0),
      .reset_n                  (reset_n)
    );

  assign adderA0 = 32'd0;
  assign adderB0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataA = 0;
  assign dataA1 = 32'd0;

  assign dataA0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataB = 0;
  assign dataB1 = 32'd0;

  assign dataB0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataC = 0;
  assign dataC1 = 32'd0;

  assign dataC0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataD = 0;
  assign dataD1 = 32'd0;

  assign dataD0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataE = 0;
  assign dataE1 = 32'd0;

  assign dataE0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataF = 0;
  assign dataF1 = 32'd0;

  assign dataF0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataG = 0;
  assign dataG1 = 32'd0;

  assign dataG0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataH = 0;
  assign dataH1 = 32'd0;

  assign dataH0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataI = 0;
  assign dataI1 = 32'd0;

  assign dataI0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataJ = 0;
  assign dataJ1 = 32'd0;

  assign dataJ0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataK = 0;
  assign dataK1 = 32'd0;

  assign dataK0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataL = 0;
  assign dataL1 = 32'd0;

  assign dataL0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataM = 0;
  assign dataM1 = 32'd0;

  assign dataM0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataN = 0;
  assign dataN1 = 32'd0;

  assign dataN0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataO = 0;
  assign dataO1 = 32'd0;

  assign dataO0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //dataP = 0;
  assign dataP1 = 32'd0;

  assign dataP0 = 32'd0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //void FIR_HW(short * input_data,
  assign if0 = 32'd1;

  assign indexA0 = 32'd0;
  assign indexB0 = 32'd0;
  assign output_buffer0 = accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //void FIR_HW(short * input_data,
  assign return0 = 32'd0;

  assign termA0 = 32'd0;
  assign termB0 = 32'd0;
  assign termC0 = 32'd0;
  assign termD0 = 32'd0;
  assign termE0 = 32'd0;
  assign termF0 = 32'd0;
  assign termG0 = 32'd0;
  assign termH0 = 32'd0;
  assign {input_data0,
length0,
output_data0,
tap_length0,
coefA0,
coefB0,
coefC0,
coefD0,
coefE0,
coefF0,
coefG0,
coefH0,
divide_order0} = FIR_HW_functiondata0;
  assign subexp0 =  ! return0;
  assign subexp1 = subexp0 && if0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //{
  assign go_bit_control_output1 = subexp1;

  //c_do_block
  accelerator_FIR_FIR_HW_FIR_HW0_state_machine_do_0 accelerator_FIR_FIR_HW_FIR_HW0_the_state_machine_do_0
    (
      .advance_to_FIR_HW_state0 (advance_to_FIR_HW_state0),
      .advance_to_state_do_2    (advance_to_state_do_2),
      .break55                  (break55),
      .break56                  (break56),
      .clk                      (clk),
      .enable_FIR_HW_state0     (enable_FIR_HW_state0),
      .enable_state_do_2        (enable_state_do_2),
      .go_bit_control_output1   (go_bit_control_output1),
      .inhibit_from_c_do_block0 (inhibit_from_c_do_block0),
      .inhibit_from_c_do_block1 (inhibit_from_c_do_block1),
      .inhibit_from_c_do_block2 (inhibit_from_c_do_block2),
      .reset_n                  (reset_n),
      .state_do_2               (state_do_2)
    );

  assign dataA_init3 = dataA1;
  assign dataB_init3 = dataB1;
  assign dataC_init3 = dataC1;
  assign dataD_init3 = dataD1;
  assign dataE_init3 = dataE1;
  assign dataF_init3 = dataF1;
  assign dataG_init3 = dataG1;
  assign dataH_init3 = dataH1;
  assign dataI_init3 = dataI1;
  assign dataJ_init3 = dataJ1;
  assign dataK_init3 = dataK1;
  assign dataL_init3 = dataL1;
  assign dataM_init3 = dataM1;
  assign dataN_init3 = dataN1;
  assign dataO_init3 = dataO1;
  assign input_data_init3 = input_data0;
  assign length_init3 = length0;
  assign output_data_init3 = output_data0;
  // e_assignment on clock 0 for ../fir_hw.c:
  //{
  assign break0 = 32'd0;

  // e_assignment on clock 0 for ../fir_hw.c:
  //{
  assign continue0 = 32'd0;

  // e_assignment on clock 0 for ../fir_hw.c:
  //{
  assign if1 = 32'd1;

  assign subexp2 =  ! return0;
  assign subexp3 =  ! continue0;
  assign subexp4 =  ! break0;
  assign subexp5 = subexp3 && subexp4;
  assign subexp6 = if1 && subexp5;
  assign subexp7 = subexp2 && subexp6;
  // e_assignment on clock 0 for ../fir_hw.c:
  //{
  assign go_bit_control_output3 = subexp7;

  assign dataA8 = dataA7;
  assign dataB8 = dataB7;
  assign dataC8 = dataC7;
  assign dataD8 = dataD7;
  assign dataE8 = dataE7;
  assign dataF8 = dataF7;
  assign dataG8 = dataG7;
  assign dataH8 = dataH7;
  assign dataI7 = dataI6;
  assign dataJ8 = dataJ7;
  assign dataK8 = dataK7;
  assign dataL8 = dataL7;
  assign dataM8 = dataM7;
  assign dataN8 = dataN7;
  assign dataO8 = dataO7;
  // e_assignment on clock 1 for ../fir_hw.c:
  //indexA = 0;
  assign indexA1 = 32'd0;

  assign input_data24 = input_data23;
  assign length7 = length6;
  assign subexp8 =  ! return0;
  assign subexp9 =  ! continue0;
  assign subexp10 =  ! break0;
  assign subexp11 = subexp9 && subexp10;
  assign subexp12 = if1 && subexp11;
  assign subexp13 = subexp8 && subexp12;
  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign go_bit_control_output9 = subexp13;

  //c_do_block
  accelerator_FIR_FIR_HW_FIR_HW0_state_machine_do_1 accelerator_FIR_FIR_HW_FIR_HW0_the_state_machine_do_1
    (
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0              (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource1_fifo_empty0                               (accelerator_FIR_FIR_HW_master_resource1_fifo_empty0),
      .advance_to_state_do_0                                                             (advance_to_state_do_0),
      .advance_to_state_do_2                                                             (advance_to_state_do_2),
      .break2                                                                            (break2),
      .break40                                                                           (break40),
      .clk                                                                               (clk),
      .enable_state_do_0                                                                 (enable_state_do_0),
      .enable_state_do_2                                                                 (enable_state_do_2),
      .go_bit_control_output3                                                            (go_bit_control_output3),
      .inhibit_from_c_do_block1                                                          (inhibit_from_c_do_block1),
      .reset_n                                                                           (reset_n),
      .state_do_0                                                                        (state_do_0)
    );

  assign dataA_init1 = dataA8;
  assign dataB_init1 = dataB8;
  assign dataC_init1 = dataC8;
  assign dataD_init1 = dataD8;
  assign dataE_init1 = dataE8;
  assign dataF_init1 = dataF8;
  assign dataG_init1 = dataG8;
  assign dataH_init1 = dataH8;
  assign dataI_init1 = dataI7;
  assign dataJ_init1 = dataJ8;
  assign dataK_init1 = dataK8;
  assign dataL_init1 = dataL8;
  assign dataM_init1 = dataM8;
  assign dataN_init1 = dataN8;
  assign dataO_init1 = dataO8;
  assign indexA_init1 = indexA1;
  assign input_data_init1 = input_data24;
  assign length_init1 = length7;
  accelerator_FIR_FIR_HW_master_resource0 the_accelerator_FIR_FIR_HW_master_resource0
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2       (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0         (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0     (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0),
      .accelerator_FIR_FIR_HW_master_resource0_address0                                  (accelerator_FIR_FIR_HW_master_resource0_address0),
      .accelerator_FIR_FIR_HW_master_resource0_byteenable0                               (accelerator_FIR_FIR_HW_master_resource0_byteenable0),
      .accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0                            (accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource0_write0                                    (accelerator_FIR_FIR_HW_master_resource0_write0),
      .accelerator_FIR_FIR_HW_master_resource0_writedata0                                (accelerator_FIR_FIR_HW_master_resource0_writedata0),
      .advance_to_state_do_0                                                             (advance_to_state_do_0),
      .clk                                                                               (clk),
      .enable_state_do_0                                                                 (enable_state_do_0),
      .go_bit_control_output7                                                            (go_bit_control_output7),
      .reset_n                                                                           (reset_n)
    );

  accelerator_FIR_FIR_HW_master_resource1 the_accelerator_FIR_FIR_HW_master_resource1
    (
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2       (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0          (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0),
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource1_address0                     (accelerator_FIR_FIR_HW_master_resource1_address0),
      .accelerator_FIR_FIR_HW_master_resource1_fifo_empty0                  (accelerator_FIR_FIR_HW_master_resource1_fifo_empty0),
      .accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0            (accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0),
      .accelerator_FIR_FIR_HW_master_resource1_flush0                       (accelerator_FIR_FIR_HW_master_resource1_flush0),
      .accelerator_FIR_FIR_HW_master_resource1_read0                        (accelerator_FIR_FIR_HW_master_resource1_read0),
      .accelerator_FIR_FIR_HW_master_resource1_readdata0                    (accelerator_FIR_FIR_HW_master_resource1_readdata0),
      .accelerator_FIR_FIR_HW_master_resource1_readdatavalid0               (accelerator_FIR_FIR_HW_master_resource1_readdatavalid0),
      .accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0               (accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0),
      .advance_to_state_do_0                                                (advance_to_state_do_0),
      .clk                                                                  (clk),
      .enable_state_do_0                                                    (enable_state_do_0),
      .reset_n                                                              (reset_n)
    );

  assign multiply_assignment0_prod0 = accelerator_FIR_FIR_HW_multiplier_resource0_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource0_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment0_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource0_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment0_op_b2;
  assign lpm_mult_clken0 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource0_res0 = lpm_multiply_result0[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply0, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply0 the_accelerator_FIR_FIR_HW_registered_multiply0
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource0_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource0_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken0),
      .product (lpm_multiply_result0),
      .reset_n (reset_n)
    );

  assign multiply_assignment7_prod0 = accelerator_FIR_FIR_HW_multiplier_resource1_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource1_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment7_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource1_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment7_op_b2;
  assign lpm_mult_clken1 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource1_res0 = lpm_multiply_result1[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply1, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply1 the_accelerator_FIR_FIR_HW_registered_multiply1
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource1_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource1_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken1),
      .product (lpm_multiply_result1),
      .reset_n (reset_n)
    );

  assign multiply_assignment6_prod0 = accelerator_FIR_FIR_HW_multiplier_resource2_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource2_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment6_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource2_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment6_op_b2;
  assign lpm_mult_clken2 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource2_res0 = lpm_multiply_result2[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply2, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply2 the_accelerator_FIR_FIR_HW_registered_multiply2
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource2_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource2_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken2),
      .product (lpm_multiply_result2),
      .reset_n (reset_n)
    );

  assign multiply_assignment2_prod0 = accelerator_FIR_FIR_HW_multiplier_resource6_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource6_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment2_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource6_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment2_op_b2;
  assign lpm_mult_clken3 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource6_res0 = lpm_multiply_result3[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply3, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply3 the_accelerator_FIR_FIR_HW_registered_multiply3
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource6_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource6_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken3),
      .product (lpm_multiply_result3),
      .reset_n (reset_n)
    );

  assign multiply_assignment5_prod0 = accelerator_FIR_FIR_HW_multiplier_resource3_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource3_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment5_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource3_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment5_op_b2;
  assign lpm_mult_clken4 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource3_res0 = lpm_multiply_result4[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply4, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply4 the_accelerator_FIR_FIR_HW_registered_multiply4
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource3_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource3_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken4),
      .product (lpm_multiply_result4),
      .reset_n (reset_n)
    );

  assign multiply_assignment1_prod0 = accelerator_FIR_FIR_HW_multiplier_resource7_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource7_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment1_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource7_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment1_op_b2;
  assign lpm_mult_clken5 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource7_res0 = lpm_multiply_result5[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply5, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply5 the_accelerator_FIR_FIR_HW_registered_multiply5
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource7_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource7_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken5),
      .product (lpm_multiply_result5),
      .reset_n (reset_n)
    );

  assign multiply_assignment4_prod0 = accelerator_FIR_FIR_HW_multiplier_resource4_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource4_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment4_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource4_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment4_op_b2;
  assign lpm_mult_clken6 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource4_res0 = lpm_multiply_result6[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply6, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply6 the_accelerator_FIR_FIR_HW_registered_multiply6
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource4_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource4_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken6),
      .product (lpm_multiply_result6),
      .reset_n (reset_n)
    );

  assign multiply_assignment3_prod0 = accelerator_FIR_FIR_HW_multiplier_resource5_res0;
  assign accelerator_FIR_FIR_HW_multiplier_resource5_op_a0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment3_op_a2;
  assign accelerator_FIR_FIR_HW_multiplier_resource5_op_b0 = {32 {(enable_state_do_0[16] & advance_to_state_do_0[16])}} & multiply_assignment3_op_b2;
  assign lpm_mult_clken7 = (enable_state_do_0[16] & advance_to_state_do_0[16]) | (enable_state_do_0[17] & advance_to_state_do_0[17]);
  assign accelerator_FIR_FIR_HW_multiplier_resource5_res0 = lpm_multiply_result7[31 : 0];
  //the_accelerator_FIR_FIR_HW_registered_multiply7, which is an e_instance
  accelerator_FIR_FIR_HW_registered_multiply7 the_accelerator_FIR_FIR_HW_registered_multiply7
    (
      .a       (accelerator_FIR_FIR_HW_multiplier_resource5_op_a0),
      .b       (accelerator_FIR_FIR_HW_multiplier_resource5_op_b0),
      .clk     (clk),
      .enable  (lpm_mult_clken7),
      .product (lpm_multiply_result7),
      .reset_n (reset_n)
    );

  assign indexA4 = indexA3;
  assign input_data4 = input_data3;
  assign length3 = length2;
  assign subexp14 = indexA4 + 32'd1;
  assign subexp15 = input_data4 + 32'd2;
  assign subexp16 = length3 - 32'd1;
  // e_assignment on clock 1 for ../fir_hw.c:
  //dataA = *input_data++;
  assign accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1 = input_data20;

  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign break1 = 32'd0;

  assign signal0 = 32'd0;
  assign subexp17 = {~length1[31], length1[30 : 0]} > {~signal0[31], signal0[30 : 0]};
  assign signal1 = {{16{1'b0}}, indexA2};
  assign signal2 = 32'd1024;
  assign subexp18 = {~signal1[31], signal1[30 : 0]} < {~signal2[31], signal2[30 : 0]};
  assign subexp19 = (subexp17 & subexp18) == 32'd0;
  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign control_output_break1 = subexp19;

  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign if2 = 32'd1;

  assign indexA3 = first_time_at_clock_0_state_do_1 ? indexA_init1 : indexA2;
  assign input_data3 = first_time_at_clock_0_state_do_1 ? input_data_init1 : input_data2;
  assign length2 = first_time_at_clock_0_state_do_1 ? length_init1 : length1;
  assign subexp20 = control_output_break1 && if2;
  accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_module0 the_accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_module0
    (
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1       (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address1),
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2       (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0          (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_read0),
      .accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_input_data1_read0_waitrequest_n0),
      .advance_to_state_do_0                                                (advance_to_state_do_0),
      .clk                                                                  (clk),
      .enable_state_do_0                                                    (enable_state_do_0),
      .reset_n                                                              (reset_n)
    );

  assign dataH4 = dataH3;
  assign dataI4 = dataI3;
  assign subexp21 = ({{24{1'b0}}, tap_length0} & 32'h1) == 32'h1;
  assign subexp22 = ({{24{1'b0}}, tap_length0} & 32'h1) == 32'h1;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataA = *input_data++;
  assign dataA2 = dereferenced_input_data2;

  assign dataA4 = dataA3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataB = dataA;
  assign dataB2 = dataA4;

  assign dataB4 = dataB3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataC = dataB;
  assign dataC2 = dataB4;

  assign dataC4 = dataC3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataD = dataC;
  assign dataD2 = dataC4;

  assign dataD4 = dataD3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataE = dataD;
  assign dataE2 = dataD4;

  assign dataE4 = dataE3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataF = dataE;
  assign dataF2 = dataE4;

  assign dataF4 = dataF3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataG = dataF;
  assign dataG2 = dataF4;

  assign dataG4 = dataG3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataH = dataG;
  assign dataH2 = dataG4;

  assign dataH3 = first_time_at_clock_14_state_do_1 ? dataH_init1 : dataH2;
  assign dataI3 = first_time_at_clock_14_state_do_1 ? dataI_init1 : dataI2;
  assign dataJ4 = dataJ3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataK = dataJ;
  assign dataK2 = dataJ4;

  assign dataK4 = dataK3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataL = dataK;
  assign dataL2 = dataK4;

  assign dataL4 = dataL3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataM = dataL;
  assign dataM2 = dataL4;

  assign dataM4 = dataM3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataN = dataM;
  assign dataN2 = dataM4;

  assign dataN4 = dataN3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataO = dataN;
  assign dataO2 = dataN4;

  assign dataO4 = dataO3;
  // e_assignment on clock 15 for ../fir_hw.c:
  //dataP = dataO;
  assign dataP2 = dataO4;

  assign subexp23 = {{16{dataA2[15]}}, dataA2} + {{16{dataP2[15]}}, dataP2};
  assign multiply_assignment0_op_a1 = subexp23;
  assign multiply_assignment0_op_b1 = {{16{coefA0[15]}}, coefA0};
  assign subexp24 = {{16{dataB2[15]}}, dataB2} + {{16{dataO2[15]}}, dataO2};
  assign multiply_assignment1_op_a1 = subexp24;
  assign multiply_assignment1_op_b1 = {{16{coefB0[15]}}, coefB0};
  assign subexp25 = {{16{dataC2[15]}}, dataC2} + {{16{dataN2[15]}}, dataN2};
  assign multiply_assignment2_op_a1 = subexp25;
  assign multiply_assignment2_op_b1 = {{16{coefC0[15]}}, coefC0};
  assign subexp26 = {{16{dataD2[15]}}, dataD2} + {{16{dataM2[15]}}, dataM2};
  assign multiply_assignment3_op_a1 = subexp26;
  assign multiply_assignment3_op_b1 = {{16{coefD0[15]}}, coefD0};
  assign subexp27 = {{16{dataE2[15]}}, dataE2} + {{16{dataL2[15]}}, dataL2};
  assign multiply_assignment4_op_a1 = subexp27;
  assign multiply_assignment4_op_b1 = {{16{coefE0[15]}}, coefE0};
  assign subexp28 = {{16{dataF2[15]}}, dataF2} + {{16{dataK2[15]}}, dataK2};
  assign multiply_assignment5_op_a1 = subexp28;
  assign multiply_assignment5_op_b1 = {{16{coefF0[15]}}, coefF0};
  assign subexp29 = {{16{dataG2[15]}}, dataG2} + {{16{dataJ2[15]}}, dataJ2};
  assign multiply_assignment6_op_a1 = subexp29;
  assign multiply_assignment6_op_b1 = {{16{coefG0[15]}}, coefG0};
  assign subexp30 = {{16{dataH2[15]}}, dataH2} + {{16{dataI2[15]}}, dataI2};
  assign multiply_assignment7_op_a1 = subexp30;
  assign multiply_assignment7_op_b1 = {{16{coefH0[15]}}, coefH0};
  assign multiply_assignment0_op_a2 = multiply_assignment0_op_a1;
  assign multiply_assignment0_op_b2 = multiply_assignment0_op_b1;
  assign multiply_assignment5_op_a2 = multiply_assignment5_op_a1;
  assign multiply_assignment5_op_b2 = multiply_assignment5_op_b1;
  assign multiply_assignment6_op_a2 = multiply_assignment6_op_a1;
  assign multiply_assignment6_op_b2 = multiply_assignment6_op_b1;
  assign multiply_assignment7_op_a2 = multiply_assignment7_op_a1;
  assign multiply_assignment7_op_b2 = multiply_assignment7_op_b1;
  assign multiply_assignment1_op_a2 = multiply_assignment1_op_a1;
  assign multiply_assignment1_op_b2 = multiply_assignment1_op_b1;
  assign multiply_assignment2_op_a2 = multiply_assignment2_op_a1;
  assign multiply_assignment2_op_b2 = multiply_assignment2_op_b1;
  assign multiply_assignment3_op_a2 = multiply_assignment3_op_a1;
  assign multiply_assignment3_op_b2 = multiply_assignment3_op_b1;
  assign multiply_assignment4_op_a2 = multiply_assignment4_op_a1;
  assign multiply_assignment4_op_b2 = multiply_assignment4_op_b1;
  assign dataA3 = first_time_at_clock_15_state_do_1 ? dataA_init1 : dataA5;
  assign dataB3 = first_time_at_clock_15_state_do_1 ? dataB_init1 : dataB5;
  assign dataC3 = first_time_at_clock_15_state_do_1 ? dataC_init1 : dataC5;
  assign dataD3 = first_time_at_clock_15_state_do_1 ? dataD_init1 : dataD5;
  assign dataE3 = first_time_at_clock_15_state_do_1 ? dataE_init1 : dataE5;
  assign dataF3 = first_time_at_clock_15_state_do_1 ? dataF_init1 : dataF5;
  assign dataG3 = first_time_at_clock_15_state_do_1 ? dataG_init1 : dataG5;
  assign dataJ3 = first_time_at_clock_15_state_do_1 ? dataJ_init1 : dataJ5;
  assign dataK3 = first_time_at_clock_15_state_do_1 ? dataK_init1 : dataK5;
  assign dataL3 = first_time_at_clock_15_state_do_1 ? dataL_init1 : dataL5;
  assign dataM3 = first_time_at_clock_15_state_do_1 ? dataM_init1 : dataM5;
  assign dataN3 = first_time_at_clock_15_state_do_1 ? dataN_init1 : dataN5;
  assign dataO3 = first_time_at_clock_15_state_do_1 ? dataO_init1 : dataO5;
  // e_assignment on clock 17 for ../fir_hw.c:
  //termA = (dataA + dataP) * coefA;
  assign termA1 = multiply1;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termB = (dataB + dataO) * coefB;
  assign termB1 = multiply3;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termC = (dataC + dataN) * coefC;
  assign termC1 = multiply5;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termD = (dataD + dataM) * coefD;
  assign termD1 = multiply7;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termE = (dataE + dataL) * coefE;
  assign termE1 = multiply9;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termF = (dataF + dataK) * coefF;
  assign termF1 = multiply11;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termG = (dataG + dataJ) * coefG;
  assign termG1 = multiply13;

  // e_assignment on clock 17 for ../fir_hw.c:
  //termH = (dataH + dataI) * coefH;
  assign termH1 = multiply15;

  assign subexp31 = termA1 + termB1;
  assign subexp32 = termC1 + termD1;
  assign subexp33 = subexp31 + subexp32;
  assign subexp34 = termE1 + termF1;
  assign subexp35 = termG1 + termH1;
  assign subexp36 = subexp34 + subexp35;
  assign subexp37 = adderA1 + adderB1;
  assign signal3 = {{16{1'b0}}, indexA23};
  assign subexp38 = output_buffer0 + {signal3[29 : 0], 2'h0};
  // e_assignment on clock 19 for ../fir_hw.c:
  //output_buffer[indexA] = adderA + adderB;
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1 = subexp38;

  // e_assignment on clock 19 for ../fir_hw.c:
  //{
  assign continue1 = 32'd0;

  assign subexp39 =  ! return0;
  assign subexp40 =  ! continue1;
  assign subexp41 =  ! break20;
  assign subexp42 = subexp40 && subexp41;
  assign subexp43 = if20 && subexp42;
  assign subexp44 = subexp39 && subexp43;
  // e_assignment on clock 19 for ../fir_hw.c:
  //output_buffer[indexA] = adderA + adderB;
  assign go_bit_control_output7 = subexp44;

  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_module0 the_accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_module0
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1       (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address1),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2       (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0         (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_write0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0     (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexA_p2_write0_writedata0),
      .advance_to_state_do_0                                                             (advance_to_state_do_0),
      .clk                                                                               (clk),
      .dereferenced_output_buffer_indexA_p3                                              (dereferenced_output_buffer_indexA_p3),
      .enable_state_do_0                                                                 (enable_state_do_0),
      .go_bit_control_output7                                                            (go_bit_control_output7),
      .reset_n                                                                           (reset_n)
    );

  assign break40 = first_time_at_clock_20_state_do_1 ? 32'd0 : break39;
  //c_do_block longest path 1
  //due to indexA
  //starts at (enable_state_do_2[1] & advance_to_state_do_2[1])
  //../fir_hw.c:
  //{
  always @(posedge clk)
    begin
      if (enable_state_do_0[0] & advance_to_state_do_0[0])
        begin
          first_time_at_clock_0_state_do_1 <= 32'd1;
          first_time_at_clock_14_state_do_1 <= 32'd1;
          first_time_at_clock_15_state_do_1 <= 32'd1;
          first_time_at_clock_20_state_do_1 <= 32'd1;
          break2 <= 0;
        end
      if (enable_state_do_0[1] & advance_to_state_do_0[1])
        begin
          first_time_at_clock_0_state_do_1 <= 32'd0;
          indexA2 <= subexp14;
          indexA5 <= indexA4;
          input_data2 <= subexp15;
          input_data5 <= input_data4;
          input_data20 <= input_data4;
          length1 <= subexp16;
        end
      if (enable_state_do_0[2] & advance_to_state_do_0[2])
        begin
          // e_assignment on clock 1 for ../fir_hw.c:
          //{
          break2 <= ((subexp20 || break1) || return0) || return0;

          break3 <= break1;
          if3 <= if2;
          indexA6 <= indexA5;
          indexA24 <= indexA4;
          input_data6 <= input_data5;
          input_data21 <= input_data4;
          length4 <= length3;
        end
      if (enable_state_do_0[3] & advance_to_state_do_0[3])
        begin
          break4 <= break3;
          break21 <= break2;
          if4 <= if3;
          indexA7 <= indexA6;
          input_data7 <= input_data6;
        end
      if (enable_state_do_0[4] & advance_to_state_do_0[4])
        begin
          break5 <= break4;
          break22 <= break21;
          if5 <= if4;
          indexA8 <= indexA7;
          input_data8 <= input_data7;
        end
      if (enable_state_do_0[5] & advance_to_state_do_0[5])
        begin
          break6 <= break5;
          break23 <= break22;
          if6 <= if5;
          indexA9 <= indexA8;
          input_data9 <= input_data8;
        end
      if (enable_state_do_0[6] & advance_to_state_do_0[6])
        begin
          break7 <= break6;
          break24 <= break23;
          if7 <= if6;
          indexA10 <= indexA9;
          input_data10 <= input_data9;
        end
      if (enable_state_do_0[7] & advance_to_state_do_0[7])
        begin
          break8 <= break7;
          break25 <= break24;
          if8 <= if7;
          indexA11 <= indexA10;
          input_data11 <= input_data10;
        end
      if (enable_state_do_0[8] & advance_to_state_do_0[8])
        begin
          break9 <= break8;
          break26 <= break25;
          if9 <= if8;
          indexA12 <= indexA11;
          input_data12 <= input_data11;
        end
      if (enable_state_do_0[9] & advance_to_state_do_0[9])
        begin
          break10 <= break9;
          break27 <= break26;
          if10 <= if9;
          indexA13 <= indexA12;
          input_data13 <= input_data12;
        end
      if (enable_state_do_0[10] & advance_to_state_do_0[10])
        begin
          break11 <= break10;
          break28 <= break27;
          if11 <= if10;
          indexA14 <= indexA13;
          input_data14 <= input_data13;
        end
      if (enable_state_do_0[11] & advance_to_state_do_0[11])
        begin
          break12 <= break11;
          break29 <= break28;
          if12 <= if11;
          indexA15 <= indexA14;
          input_data15 <= input_data14;
        end
      if (enable_state_do_0[12] & advance_to_state_do_0[12])
        begin
          break13 <= break12;
          break30 <= break29;
          if13 <= if12;
          indexA16 <= indexA15;
          input_data16 <= input_data15;
        end
      if (enable_state_do_0[13] & advance_to_state_do_0[13])
        begin
          break14 <= break13;
          break31 <= break30;
          if14 <= if13;
          indexA17 <= indexA16;
          input_data17 <= input_data16;
        end
      if (enable_state_do_0[14] & advance_to_state_do_0[14])
        begin
          break15 <= break14;
          break32 <= break31;
          if15 <= if14;
          indexA18 <= indexA17;
          input_data18 <= input_data17;
        end
      if (enable_state_do_0[15] & advance_to_state_do_0[15])
        begin
          dereferenced_input_data2 <= accelerator_FIR_FIR_HW_master_resource1_fifo_out_aligned0;
          break16 <= break15;
          break33 <= break32;
          // e_assignment on clock 14 for ../fir_hw.c:
          //dataI = ((tap_length & 0x1) == 0x1)? 0 : dataH;
          dataI2 <= subexp21 ? 32'd0 : dataH4;

          // e_assignment on clock 14 for ../fir_hw.c:
          //dataJ = ((tap_length & 0x1) == 0x1)? dataH : dataI;
          dataJ2 <= subexp22 ? dataH4 : dataI4;

          first_time_at_clock_14_state_do_1 <= 32'd0;
          if16 <= if15;
          indexA19 <= indexA18;
          input_data19 <= input_data18;
        end
      if (enable_state_do_0[16] & advance_to_state_do_0[16])
        begin
          break17 <= break16;
          break34 <= break33;
          dataA5 <= dataA2;
          dataB5 <= dataB2;
          dataC5 <= dataC2;
          dataD5 <= dataD2;
          dataE5 <= dataE2;
          dataF5 <= dataF2;
          dataG5 <= dataG2;
          dataH5 <= dataH4;
          dataJ5 <= dataJ2;
          dataK5 <= dataK2;
          dataL5 <= dataL2;
          dataM5 <= dataM2;
          dataN5 <= dataN2;
          dataO5 <= dataO2;
          dataP3 <= dataP2;
          first_time_at_clock_15_state_do_1 <= 32'd0;
          if17 <= if16;
          indexA20 <= indexA19;
        end
      if (enable_state_do_0[17] & advance_to_state_do_0[17])
        begin
          multiply1 <= multiply_assignment0_prod0;
          multiply11 <= multiply_assignment5_prod0;
          multiply13 <= multiply_assignment6_prod0;
          multiply15 <= multiply_assignment7_prod0;
          multiply3 <= multiply_assignment1_prod0;
          multiply5 <= multiply_assignment2_prod0;
          multiply7 <= multiply_assignment3_prod0;
          multiply9 <= multiply_assignment4_prod0;
          break18 <= break17;
          break35 <= break34;
          if18 <= if17;
          indexA21 <= indexA20;
        end
      if (enable_state_do_0[18] & advance_to_state_do_0[18])
        begin
          // e_assignment on clock 17 for ../fir_hw.c:
          //adderA = (termA + termB) + (termC + termD);
          adderA1 <= subexp33;

          // e_assignment on clock 17 for ../fir_hw.c:
          //adderB = (termE + termF) + (termG + termH);
          adderB1 <= subexp36;

          break19 <= break18;
          break36 <= break35;
          if19 <= if18;
          indexA22 <= indexA21;
        end
      if (enable_state_do_0[19] & advance_to_state_do_0[19])
        begin
          break20 <= break19;
          break37 <= break36;
          // e_assignment on clock 18 for ../fir_hw.c:
          //output_buffer[indexA] = adderA + adderB;
          dereferenced_output_buffer_indexA_p3 <= subexp37;

          if20 <= if19;
          indexA23 <= indexA22;
        end
      if (enable_state_do_0[20] & advance_to_state_do_0[20])
          break38 <= break37;
      if (enable_state_do_0[21] & advance_to_state_do_0[21])
        begin
          break39 <= break38;
          first_time_at_clock_20_state_do_1 <= 32'd0;
        end
    end


  assign signal4 = 32'd0;
  assign subexp45 = {~length5[31], length5[30 : 0]} > {~signal4[31], signal4[30 : 0]};
  assign subexp46 = subexp45 == 32'd0;
  // e_assignment on clock 2 for ../fir_hw.c:
  //{
  assign control_output_break5 = subexp46;

  // e_assignment on clock 2 for ../fir_hw.c:
  //indexB = 0;
  assign indexB1 = 32'd0;

  assign output_data14 = output_data13;
  assign subexp47 = control_output_break5 && if1;
  //c_do_block
  accelerator_FIR_FIR_HW_FIR_HW0_state_machine_do_2 accelerator_FIR_FIR_HW_FIR_HW0_the_state_machine_do_2
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0           (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource2_fifo_empty0                              (accelerator_FIR_FIR_HW_master_resource2_fifo_empty0),
      .advance_to_state_do_1                                                            (advance_to_state_do_1),
      .advance_to_state_do_2                                                            (advance_to_state_do_2),
      .break42                                                                          (break42),
      .break54                                                                          (break54),
      .clk                                                                              (clk),
      .enable_state_do_1                                                                (enable_state_do_1),
      .enable_state_do_2                                                                (enable_state_do_2),
      .go_bit_control_output9                                                           (go_bit_control_output9),
      .inhibit_from_c_do_block2                                                         (inhibit_from_c_do_block2),
      .reset_n                                                                          (reset_n),
      .state_do_1                                                                       (state_do_1)
    );

  assign indexB_init1 = indexB1;
  assign output_data_init1 = output_data14;
  accelerator_FIR_FIR_HW_master_resource3 the_accelerator_FIR_FIR_HW_master_resource3
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2       (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0         (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0     (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0),
      .accelerator_FIR_FIR_HW_master_resource3_address0                       (accelerator_FIR_FIR_HW_master_resource3_address0),
      .accelerator_FIR_FIR_HW_master_resource3_byteenable0                    (accelerator_FIR_FIR_HW_master_resource3_byteenable0),
      .accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0                 (accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource3_write0                         (accelerator_FIR_FIR_HW_master_resource3_write0),
      .accelerator_FIR_FIR_HW_master_resource3_writedata0                     (accelerator_FIR_FIR_HW_master_resource3_writedata0),
      .advance_to_state_do_1                                                  (advance_to_state_do_1),
      .clk                                                                    (clk),
      .enable_state_do_1                                                      (enable_state_do_1),
      .go_bit_control_output19                                                (go_bit_control_output19),
      .reset_n                                                                (reset_n)
    );

  accelerator_FIR_FIR_HW_master_resource2 the_accelerator_FIR_FIR_HW_master_resource2
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2       (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0          (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource2_address0                                 (accelerator_FIR_FIR_HW_master_resource2_address0),
      .accelerator_FIR_FIR_HW_master_resource2_fifo_empty0                              (accelerator_FIR_FIR_HW_master_resource2_fifo_empty0),
      .accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0                        (accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0),
      .accelerator_FIR_FIR_HW_master_resource2_flush0                                   (accelerator_FIR_FIR_HW_master_resource2_flush0),
      .accelerator_FIR_FIR_HW_master_resource2_read0                                    (accelerator_FIR_FIR_HW_master_resource2_read0),
      .accelerator_FIR_FIR_HW_master_resource2_readdata0                                (accelerator_FIR_FIR_HW_master_resource2_readdata0),
      .accelerator_FIR_FIR_HW_master_resource2_readdatavalid0                           (accelerator_FIR_FIR_HW_master_resource2_readdatavalid0),
      .accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0                           (accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0),
      .advance_to_state_do_1                                                            (advance_to_state_do_1),
      .clk                                                                              (clk),
      .enable_state_do_1                                                                (enable_state_do_1),
      .reset_n                                                                          (reset_n)
    );

  assign indexB4 = indexB3;
  assign output_data4 = output_data3;
  assign subexp48 = indexB4 + 32'd1;
  assign subexp49 = output_data4 + 32'd2;
  assign signal5 = {{16{1'b0}}, indexB5};
  assign subexp50 = output_buffer0 + {signal5[29 : 0], 2'h0};
  // e_assignment on clock 1 for ../fir_hw.c:
  //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p2_read0_address1 = subexp50;

  assign signal6 = {{16{1'b0}}, indexB5};
  assign subexp51 = output_buffer0 + {signal6[29 : 0], 2'h0};
  // e_assignment on clock 1 for ../fir_hw.c:
  //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p3_read0_address1 = subexp51;

  assign signal7 = {{16{1'b0}}, indexB5};
  assign subexp52 = output_buffer0 + {signal7[29 : 0], 2'h0};
  // e_assignment on clock 1 for ../fir_hw.c:
  //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p4_read0_address1 = subexp52;

  assign signal8 = {{16{1'b0}}, indexB5};
  assign subexp53 = output_buffer0 + {signal8[29 : 0], 2'h0};
  // e_assignment on clock 1 for ../fir_hw.c:
  //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
  assign accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1 = subexp53;

  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign break41 = 32'd0;

  assign signal9 = {{16{1'b0}}, indexB2};
  assign signal10 = {{16{1'b0}}, indexA25};
  assign subexp54 = {~signal9[31], signal9[30 : 0]} < {~signal10[31], signal10[30 : 0]};
  assign subexp55 = subexp54 == 32'd0;
  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign control_output_break3 = subexp55;

  // e_assignment on clock 1 for ../fir_hw.c:
  //{
  assign if21 = 32'd1;

  assign indexB3 = first_time_at_clock_0_state_do_4 ? indexB_init1 : indexB2;
  assign output_data3 = first_time_at_clock_0_state_do_4 ? output_data_init1 : output_data2;
  assign subexp56 = control_output_break3 && if21;
  //equivalent data pointer
  assign dereferenced_output_buffer_indexB_p3 = dereferenced_output_buffer_indexB_p6;

  //equivalent data pointer
  assign dereferenced_output_buffer_indexB_p4 = dereferenced_output_buffer_indexB_p6;

  //equivalent data pointer
  assign dereferenced_output_buffer_indexB_p5 = dereferenced_output_buffer_indexB_p6;

  accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_module0 the_accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_module0
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1       (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address1),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2       (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0          (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_read0),
      .accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_buffer_indexB_p5_read0_waitrequest_n0),
      .advance_to_state_do_1                                                            (advance_to_state_do_1),
      .clk                                                                              (clk),
      .enable_state_do_1                                                                (enable_state_do_1),
      .reset_n                                                                          (reset_n)
    );

  assign subexp57 = {{24{1'b0}}, divide_order0} == 32'd13;
  assign subexp58 = {{24{1'b0}}, divide_order0} == 32'd14;
  assign subexp59 = {{24{1'b0}}, divide_order0} == 32'd15;
  // e_assignment on clock 6 for ../fir_hw.c:
  //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
  assign accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1 = output_data10;

  // e_assignment on clock 6 for ../fir_hw.c:
  //{
  assign continue2 = 32'd0;

  assign subexp60 =  ! return0;
  assign subexp61 =  ! continue2;
  assign subexp62 =  ! break47;
  assign subexp63 = subexp61 && subexp62;
  assign subexp64 = if26 && subexp63;
  assign subexp65 = subexp60 && subexp64;
  // e_assignment on clock 6 for ../fir_hw.c:
  //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
  assign go_bit_control_output19 = subexp65;

  accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_module0 the_accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_module0
    (
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1       (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address1),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2       (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_address2),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0 (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0         (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_write0),
      .accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0     (accelerator_FIR_FIR_HW_dereferenced_output_data1_write0_writedata0),
      .advance_to_state_do_1                                                  (advance_to_state_do_1),
      .clk                                                                    (clk),
      .dereferenced_output_data2                                              (dereferenced_output_data2),
      .enable_state_do_1                                                      (enable_state_do_1),
      .go_bit_control_output19                                                (go_bit_control_output19),
      .reset_n                                                                (reset_n)
    );

  assign break54 = first_time_at_clock_7_state_do_2 ? 32'd0 : break53;
  //c_do_block longest path 1
  //due to indexB
  //starts at (enable_state_do_2[2] & advance_to_state_do_2[2])
  //../fir_hw.c:
  //{
  always @(posedge clk)
    begin
      if (enable_state_do_1[0] & advance_to_state_do_1[0])
        begin
          first_time_at_clock_0_state_do_4 <= 32'd1;
          first_time_at_clock_7_state_do_2 <= 32'd1;
          break42 <= 0;
        end
      if (enable_state_do_1[1] & advance_to_state_do_1[1])
        begin
          first_time_at_clock_0_state_do_4 <= 32'd0;
          indexB2 <= subexp48;
          indexB5 <= indexB4;
          output_data2 <= subexp49;
          output_data5 <= output_data4;
        end
      if (enable_state_do_1[2] & advance_to_state_do_1[2])
        begin
          // e_assignment on clock 1 for ../fir_hw.c:
          //{
          break42 <= ((subexp56 || break41) || return0) || return0;

          break43 <= break41;
          if22 <= if21;
          indexB6 <= indexB5;
          indexB10 <= indexB4;
          output_data6 <= output_data5;
          output_data11 <= output_data4;
        end
      if (enable_state_do_1[3] & advance_to_state_do_1[3])
        begin
          break44 <= break43;
          break48 <= break42;
          if23 <= if22;
          indexB7 <= indexB6;
          output_data7 <= output_data6;
        end
      if (enable_state_do_1[4] & advance_to_state_do_1[4])
        begin
          break45 <= break44;
          break49 <= break48;
          if24 <= if23;
          indexB8 <= indexB7;
          output_data8 <= output_data7;
        end
      if (enable_state_do_1[5] & advance_to_state_do_1[5])
        begin
          dereferenced_output_buffer_indexB_p6 <= accelerator_FIR_FIR_HW_master_resource2_fifo_out_aligned0;
          break46 <= break45;
          break50 <= break49;
          if25 <= if24;
          indexB9 <= indexB8;
          output_data9 <= output_data8;
        end
      if (enable_state_do_1[6] & advance_to_state_do_1[6])
        begin
          break47 <= break46;
          break51 <= break50;
          // e_assignment on clock 5 for ../fir_hw.c:
          //*output_data++ = (divide_order == 13)? (short)(output_buffer[indexB] >> 13):
          dereferenced_output_data2 <= subexp57 ? {{13{dereferenced_output_buffer_indexB_p3[31]}}, dereferenced_output_buffer_indexB_p3[31 : 13]} : (subexp58 ? {{14{dereferenced_output_buffer_indexB_p4[31]}}, dereferenced_output_buffer_indexB_p4[31 : 14]} : (subexp59 ? {{15{dereferenced_output_buffer_indexB_p5[31]}}, dereferenced_output_buffer_indexB_p5[31 : 15]} : {{16{dereferenced_output_buffer_indexB_p6[31]}}, dereferenced_output_buffer_indexB_p6[31 : 16]}));

          if26 <= if25;
          output_data10 <= output_data9;
        end
      if (enable_state_do_1[7] & advance_to_state_do_1[7])
          break52 <= break51;
      if (enable_state_do_1[8] & advance_to_state_do_1[8])
        begin
          break53 <= break52;
          first_time_at_clock_7_state_do_2 <= 32'd0;
        end
    end


  assign break56 = first_time_at_clock_2_state_do_3 ? 32'd0 : break55;
  assign dataA7 = first_time_at_clock_2_state_do_3 ? dataA_init3 : dataA6;
  assign dataB7 = first_time_at_clock_2_state_do_3 ? dataB_init3 : dataB6;
  assign dataC7 = first_time_at_clock_2_state_do_3 ? dataC_init3 : dataC6;
  assign dataD7 = first_time_at_clock_2_state_do_3 ? dataD_init3 : dataD6;
  assign dataE7 = first_time_at_clock_2_state_do_3 ? dataE_init3 : dataE6;
  assign dataF7 = first_time_at_clock_2_state_do_3 ? dataF_init3 : dataF6;
  assign dataG7 = first_time_at_clock_2_state_do_3 ? dataG_init3 : dataG6;
  assign dataH7 = first_time_at_clock_2_state_do_3 ? dataH_init3 : dataH6;
  assign dataI6 = first_time_at_clock_2_state_do_3 ? dataI_init3 : dataI5;
  assign dataJ7 = first_time_at_clock_2_state_do_3 ? dataJ_init3 : dataJ6;
  assign dataK7 = first_time_at_clock_2_state_do_3 ? dataK_init3 : dataK6;
  assign dataL7 = first_time_at_clock_2_state_do_3 ? dataL_init3 : dataL6;
  assign dataM7 = first_time_at_clock_2_state_do_3 ? dataM_init3 : dataM6;
  assign dataN7 = first_time_at_clock_2_state_do_3 ? dataN_init3 : dataN6;
  assign dataO7 = first_time_at_clock_2_state_do_3 ? dataO_init3 : dataO6;
  assign input_data23 = first_time_at_clock_2_state_do_3 ? input_data_init3 : input_data22;
  assign length6 = first_time_at_clock_2_state_do_3 ? length_init3 : length5;
  assign output_data13 = first_time_at_clock_3_state_do_3 ? output_data_init3 : output_data12;
  //c_do_block longest path 2
  //due to Loop Condition
  //starts at (enable_FIR_HW_state0[1] & advance_to_FIR_HW_state0[1])
  //../fir_hw.c:
  //{
  always @(posedge clk)
    begin
      if (enable_state_do_2[0] & advance_to_state_do_2[0])
        begin
          first_time_at_clock_2_state_do_3 <= 32'd1;
          first_time_at_clock_3_state_do_3 <= 32'd1;
          break55 <= 0;
        end
      if (enable_state_do_2[2] & advance_to_state_do_2[2])
        begin
          adderA2 <= adderA1;
          adderB2 <= adderB1;
          dataA6 <= dataA4;
          dataB6 <= dataB4;
          dataC6 <= dataC4;
          dataD6 <= dataD4;
          dataE6 <= dataE4;
          dataF6 <= dataF4;
          dataG6 <= dataG4;
          dataH6 <= dataH5;
          dataI5 <= dataI4;
          dataJ6 <= dataJ4;
          dataK6 <= dataK4;
          dataL6 <= dataL4;
          dataM6 <= dataM4;
          dataN6 <= dataN4;
          dataO6 <= dataO4;
          dataP4 <= dataP3;
          indexA25 <= indexA24;
          input_data22 <= input_data21;
          length5 <= length4;
          termA2 <= termA1;
          termB2 <= termB1;
          termC2 <= termC1;
          termD2 <= termD1;
          termE2 <= termE1;
          termF2 <= termF1;
          termG2 <= termG1;
          termH2 <= termH1;
        end
      if (enable_state_do_2[3] & advance_to_state_do_2[3])
        begin
          // e_assignment on clock 2 for ../fir_hw.c:
          //{
          break55 <= ((subexp47 || break0) || return0) || return0;

          first_time_at_clock_2_state_do_3 <= 32'd0;
          indexB11 <= indexB10;
          output_data12 <= output_data11;
        end
      if (enable_state_do_2[4] & advance_to_state_do_2[4])
          first_time_at_clock_3_state_do_3 <= 32'd0;
    end


  //c_subroutine_block process, accelerator_FIR_FIR_HW_FIR_HW0
  always @(posedge clk)
    begin
      if (enable_FIR_HW_state0[2] & advance_to_FIR_HW_state0[2])
        begin
          adderA3 <= adderA2;
          adderB3 <= adderB2;
          dataA9 <= dataA8;
          dataB9 <= dataB8;
          dataC9 <= dataC8;
          dataD9 <= dataD8;
          dataE9 <= dataE8;
          dataF9 <= dataF8;
          dataG9 <= dataG8;
          dataH9 <= dataH8;
          dataI8 <= dataI7;
          dataJ9 <= dataJ8;
          dataK9 <= dataK8;
          dataL9 <= dataL8;
          dataM9 <= dataM8;
          dataN9 <= dataN8;
          dataO9 <= dataO8;
          dataP5 <= dataP4;
          indexA26 <= indexA25;
          indexB12 <= indexB11;
          input_data25 <= input_data24;
          length8 <= length7;
          output_data15 <= output_data14;
          termA3 <= termA2;
          termB3 <= termB2;
          termC3 <= termC2;
          termD3 <= termD2;
          termE3 <= termE2;
          termF3 <= termF2;
          termG3 <= termG2;
          termH3 <= termH2;
        end
    end



endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW_cpu_registers0 (
                                               // inputs:
                                                FIR_HW0,
                                                cpu_address0,
                                                cpu_clk,
                                                cpu_reset_n,
                                                cpu_select0,
                                                cpu_write0,
                                                cpu_writedata0,
                                                reset_n,
                                                slave_readdata0,
                                                slave_waitrequest_n0,

                                               // outputs:
                                                cpu_readdata1,
                                                cpu_waitrequest_n0,
                                                slave_address0,
                                                slave_functiondata0,
                                                slave_read0
                                             )
;

  output  [ 31: 0] cpu_readdata1;
  output           cpu_waitrequest_n0;
  output  [ 31: 0] slave_address0;
  output  [239: 0] slave_functiondata0;
  output           slave_read0;
  input   [ 31: 0] FIR_HW0;
  input   [  3: 0] cpu_address0;
  input            cpu_clk;
  input            cpu_reset_n;
  input            cpu_select0;
  input            cpu_write0;
  input   [ 31: 0] cpu_writedata0;
  input            reset_n;
  input   [ 31: 0] slave_readdata0;
  input            slave_waitrequest_n0;

  reg     [ 15: 0] coefA1;
  reg     [ 15: 0] coefB1;
  reg     [ 15: 0] coefC1;
  reg     [ 15: 0] coefD1;
  reg     [ 15: 0] coefE1;
  reg     [ 15: 0] coefF1;
  reg     [ 15: 0] coefG1;
  reg     [ 15: 0] coefH1;
  wire    [  3: 0] cpu_address_c2hplus0;
  wire    [ 31: 0] cpu_readdata1;
  wire             cpu_waitrequest_n0;
  reg     [  7: 0] divide_order1;
  reg     [ 31: 0] input_data26;
  reg     [ 31: 0] length9;
  reg     [ 31: 0] output_data16;
  reg              polled_data_ready0;
  wire    [  3: 0] port_monitor0;
  wire    [ 31: 0] slave_address0;
  wire    [239: 0] slave_functiondata0;
  reg              slave_read0;
  reg     [  7: 0] tap_length1;
  reg     [ 31: 0] tmp_cpu_readdata1;
  //cpu interface register for input_data26, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          input_data26[31 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 1))
          input_data26[31 : 0] <= cpu_writedata0;
    end


  //cpu interface register for length9, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          length9[31 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 2))
          length9[31 : 0] <= cpu_writedata0;
    end


  //cpu interface register for output_data16, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          output_data16[31 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 3))
          output_data16[31 : 0] <= cpu_writedata0;
    end


  //cpu interface register for tap_length1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          tap_length1[7 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 4))
          tap_length1[7 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefA1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefA1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 5))
          coefA1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefB1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefB1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 6))
          coefB1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefC1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefC1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 7))
          coefC1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefD1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefD1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 8))
          coefD1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefE1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefE1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 9))
          coefE1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefF1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefF1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 10))
          coefF1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefG1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefG1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 11))
          coefG1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for coefH1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          coefH1[15 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 12))
          coefH1[15 : 0] <= cpu_writedata0;
    end


  //cpu interface register for divide_order1, which is an e_register
  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          divide_order1[7 : 0] <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 13))
          divide_order1[7 : 0] <= cpu_writedata0;
    end


  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          tmp_cpu_readdata1 <= 0;
      else if (slave_waitrequest_n0 & slave_read0)
          tmp_cpu_readdata1 <= slave_readdata0;
    end


  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          polled_data_ready0 <= 0;
      else if (cpu_select0 & cpu_write0 & (cpu_address_c2hplus0 == 0))
          polled_data_ready0 <= 0;
      else if (slave_waitrequest_n0)
          polled_data_ready0 <= -1;
    end


  always @(posedge cpu_clk or negedge cpu_reset_n)
    begin
      if (cpu_reset_n == 0)
          slave_read0 <= 0;
      else if (cpu_write0 & cpu_select0 & (cpu_address_c2hplus0 == 0) & cpu_writedata0[0])
          slave_read0 <= -1;
      else if (slave_waitrequest_n0)
          slave_read0 <= 0;
    end


  assign cpu_readdata1 = (cpu_address_c2hplus0 == 0)? polled_data_ready0 :
    tmp_cpu_readdata1;

  assign cpu_waitrequest_n0 = 1;
  assign slave_address0 = FIR_HW0;
  assign slave_functiondata0 = {input_data26,
    length9,
    output_data16,
    tap_length1,
    coefA1,
    coefB1,
    coefC1,
    coefD1,
    coefE1,
    coefF1,
    coefG1,
    coefH1,
    divide_order1};


//synthesis translate_off
//////////////// SIMULATION-ONLY CONTENTS
  assign cpu_address_c2hplus0 = cpu_address0;

//////////////// END SIMULATION-ONLY CONTENTS

//synthesis translate_on
//synthesis read_comments_as_HDL on
//  assign port_monitor0 = 0;
//  c2h_port_byte_refine the_c2h_port_byte_refine
//    (
//      .port_a (cpu_address0),
//      .port_b (port_monitor0),
//      .port_c (reset_n),
//      .port_d (cpu_address_c2hplus0)
//    );
//
//  defparam the_c2h_port_byte_refine.c2h_param1 = 4,
//           the_c2h_port_byte_refine.c2h_param2 = 2,
//           the_c2h_port_byte_refine.c2h_param3 = 2,
//           the_c2h_port_byte_refine.c2h_param4 = 0;
//
//synthesis read_comments_as_HDL off

endmodule



// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module accelerator_FIR_FIR_HW (
                                // inputs:
                                 FIR_HW_begin0,
                                 FIR_HW_read0,
                                 FIR_HW_select0,
                                 accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0,
                                 accelerator_FIR_FIR_HW_master_resource1_readdata0,
                                 accelerator_FIR_FIR_HW_master_resource1_readdatavalid0,
                                 accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0,
                                 accelerator_FIR_FIR_HW_master_resource2_readdata0,
                                 accelerator_FIR_FIR_HW_master_resource2_readdatavalid0,
                                 accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0,
                                 accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_chipselect0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_write0,
                                 clk,
                                 cpu_address0,
                                 cpu_clk,
                                 cpu_reset_n,
                                 cpu_select0,
                                 cpu_write0,
                                 cpu_writedata0,
                                 dummy_master_waitrequest,
                                 dummy_slave_address,
                                 dummy_slave_chipselect,
                                 reset_n,
                                 slave_readdata0,
                                 slave_waitrequest_n0,

                                // outputs:
                                 FIR_HW_waitrequest_n0,
                                 accelerator_FIR_FIR_HW_master_resource0_address0,
                                 accelerator_FIR_FIR_HW_master_resource0_byteenable0,
                                 accelerator_FIR_FIR_HW_master_resource0_write0,
                                 accelerator_FIR_FIR_HW_master_resource0_writedata0,
                                 accelerator_FIR_FIR_HW_master_resource1_address0,
                                 accelerator_FIR_FIR_HW_master_resource1_flush0,
                                 accelerator_FIR_FIR_HW_master_resource1_read0,
                                 accelerator_FIR_FIR_HW_master_resource2_address0,
                                 accelerator_FIR_FIR_HW_master_resource2_flush0,
                                 accelerator_FIR_FIR_HW_master_resource2_read0,
                                 accelerator_FIR_FIR_HW_master_resource3_address0,
                                 accelerator_FIR_FIR_HW_master_resource3_byteenable0,
                                 accelerator_FIR_FIR_HW_master_resource3_write0,
                                 accelerator_FIR_FIR_HW_master_resource3_writedata0,
                                 accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0,
                                 cpu_readdata0,
                                 cpu_readdata1,
                                 cpu_waitrequest_n0,
                                 dummy_master_address,
                                 dummy_master_write,
                                 dummy_master_writedata,
                                 dummy_slave_readdata,
                                 slave_address0,
                                 slave_read0
                              )
;

  output           FIR_HW_waitrequest_n0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address0;
  output  [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable0;
  output           accelerator_FIR_FIR_HW_master_resource0_write0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address0;
  output           accelerator_FIR_FIR_HW_master_resource1_flush0;
  output           accelerator_FIR_FIR_HW_master_resource1_read0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address0;
  output           accelerator_FIR_FIR_HW_master_resource2_flush0;
  output           accelerator_FIR_FIR_HW_master_resource2_read0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address0;
  output  [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable0;
  output           accelerator_FIR_FIR_HW_master_resource3_write0;
  output  [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata0;
  output  [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0;
  output  [ 31: 0] cpu_readdata0;
  output  [ 31: 0] cpu_readdata1;
  output           cpu_waitrequest_n0;
  output  [ 31: 0] dummy_master_address;
  output           dummy_master_write;
  output  [ 31: 0] dummy_master_writedata;
  output  [ 31: 0] dummy_slave_readdata;
  output  [ 31: 0] slave_address0;
  output           slave_read0;
  input            FIR_HW_begin0;
  input            FIR_HW_read0;
  input            FIR_HW_select0;
  input            accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0;
  input   [ 15: 0] accelerator_FIR_FIR_HW_master_resource1_readdata0;
  input            accelerator_FIR_FIR_HW_master_resource1_readdatavalid0;
  input            accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_readdata0;
  input            accelerator_FIR_FIR_HW_master_resource2_readdatavalid0;
  input            accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0;
  input            accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0;
  input   [  9: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0;
  input   [  3: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0;
  input            accelerator_FIR_FIR_HW_output_buffer_internal_memory0_chipselect0;
  input   [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0;
  input            accelerator_FIR_FIR_HW_output_buffer_internal_memory0_write0;
  input            clk;
  input   [  3: 0] cpu_address0;
  input            cpu_clk;
  input            cpu_reset_n;
  input            cpu_select0;
  input            cpu_write0;
  input   [ 31: 0] cpu_writedata0;
  input            dummy_master_waitrequest;
  input            dummy_slave_address;
  input            dummy_slave_chipselect;
  input            reset_n;
  input   [ 31: 0] slave_readdata0;
  input            slave_waitrequest_n0;

  wire    [ 31: 0] FIR_HW0;
  wire    [239: 0] FIR_HW_functiondata0;
  wire             FIR_HW_waitrequest_n0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_address0;
  wire    [  3: 0] accelerator_FIR_FIR_HW_master_resource0_byteenable0;
  wire             accelerator_FIR_FIR_HW_master_resource0_write0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource0_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource1_address0;
  wire             accelerator_FIR_FIR_HW_master_resource1_flush0;
  wire             accelerator_FIR_FIR_HW_master_resource1_read0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource2_address0;
  wire             accelerator_FIR_FIR_HW_master_resource2_flush0;
  wire             accelerator_FIR_FIR_HW_master_resource2_read0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_master_resource3_address0;
  wire    [  1: 0] accelerator_FIR_FIR_HW_master_resource3_byteenable0;
  wire             accelerator_FIR_FIR_HW_master_resource3_write0;
  wire    [ 15: 0] accelerator_FIR_FIR_HW_master_resource3_writedata0;
  wire    [ 31: 0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0;
  wire             accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0;
  wire    [ 31: 0] cpu_readdata0;
  wire    [ 31: 0] cpu_readdata1;
  wire             cpu_waitrequest_n0;
  wire             dummy_address_sink;
  wire             dummy_chipselect_sink;
  wire    [ 31: 0] dummy_master_address;
  wire             dummy_master_write;
  wire    [ 31: 0] dummy_master_writedata;
  wire    [ 31: 0] dummy_slave_readdata;
  wire             dummy_waitrequest_sink;
  wire    [ 31: 0] slave_address0;
  wire    [239: 0] slave_functiondata0;
  wire             slave_read0;
  accelerator_FIR_FIR_HW_output_buffer0 the_accelerator_FIR_FIR_HW_output_buffer0
    (
      .accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0  (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0),
      .accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0 (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0),
      .accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0  (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0),
      .accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0  (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0),
      .accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0 (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0),
      .clk                                                       (clk)
    );

  assign accelerator_FIR_FIR_HW_output_buffer_internal_memory0_we0 = accelerator_FIR_FIR_HW_output_buffer_internal_memory0_write0 & accelerator_FIR_FIR_HW_output_buffer_internal_memory0_chipselect0;
  //accelerator_FIR_FIR_HW_output_buffer_internal_memory0_slave, which is an e_avalon_slave
  accelerator_FIR_FIR_HW_FIR_HW0 the_accelerator_FIR_FIR_HW_FIR_HW0
    (
      .FIR_HW_begin0                                                       (FIR_HW_begin0),
      .FIR_HW_functiondata0                                                (FIR_HW_functiondata0),
      .FIR_HW_read0                                                        (FIR_HW_read0),
      .FIR_HW_select0                                                      (FIR_HW_select0),
      .FIR_HW_waitrequest_n0                                               (FIR_HW_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource0_address0                    (accelerator_FIR_FIR_HW_master_resource0_address0),
      .accelerator_FIR_FIR_HW_master_resource0_byteenable0                 (accelerator_FIR_FIR_HW_master_resource0_byteenable0),
      .accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource0_write0                      (accelerator_FIR_FIR_HW_master_resource0_write0),
      .accelerator_FIR_FIR_HW_master_resource0_writedata0                  (accelerator_FIR_FIR_HW_master_resource0_writedata0),
      .accelerator_FIR_FIR_HW_master_resource1_address0                    (accelerator_FIR_FIR_HW_master_resource1_address0),
      .accelerator_FIR_FIR_HW_master_resource1_flush0                      (accelerator_FIR_FIR_HW_master_resource1_flush0),
      .accelerator_FIR_FIR_HW_master_resource1_read0                       (accelerator_FIR_FIR_HW_master_resource1_read0),
      .accelerator_FIR_FIR_HW_master_resource1_readdata0                   (accelerator_FIR_FIR_HW_master_resource1_readdata0),
      .accelerator_FIR_FIR_HW_master_resource1_readdatavalid0              (accelerator_FIR_FIR_HW_master_resource1_readdatavalid0),
      .accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource2_address0                    (accelerator_FIR_FIR_HW_master_resource2_address0),
      .accelerator_FIR_FIR_HW_master_resource2_flush0                      (accelerator_FIR_FIR_HW_master_resource2_flush0),
      .accelerator_FIR_FIR_HW_master_resource2_read0                       (accelerator_FIR_FIR_HW_master_resource2_read0),
      .accelerator_FIR_FIR_HW_master_resource2_readdata0                   (accelerator_FIR_FIR_HW_master_resource2_readdata0),
      .accelerator_FIR_FIR_HW_master_resource2_readdatavalid0              (accelerator_FIR_FIR_HW_master_resource2_readdatavalid0),
      .accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource3_address0                    (accelerator_FIR_FIR_HW_master_resource3_address0),
      .accelerator_FIR_FIR_HW_master_resource3_byteenable0                 (accelerator_FIR_FIR_HW_master_resource3_byteenable0),
      .accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0),
      .accelerator_FIR_FIR_HW_master_resource3_write0                      (accelerator_FIR_FIR_HW_master_resource3_write0),
      .accelerator_FIR_FIR_HW_master_resource3_writedata0                  (accelerator_FIR_FIR_HW_master_resource3_writedata0),
      .accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0 (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0),
      .clk                                                                 (clk),
      .cpu_readdata0                                                       (cpu_readdata0),
      .reset_n                                                             (reset_n)
    );

  //sub_FIR_HW0, which is an e_avalon_slave
  //accelerator_FIR_FIR_HW_master_resource0, which is an e_avalon_master
  //accelerator_FIR_FIR_HW_master_resource1, which is an e_avalon_master
  //accelerator_FIR_FIR_HW_master_resource3, which is an e_avalon_master
  //accelerator_FIR_FIR_HW_master_resource2, which is an e_avalon_master
  //the_cpu_registers0, which is an e_instance
  accelerator_FIR_FIR_HW_cpu_registers0 the_cpu_registers0
    (
      .FIR_HW0              (FIR_HW0),
      .cpu_address0         (cpu_address0),
      .cpu_clk              (cpu_clk),
      .cpu_readdata1        (cpu_readdata1),
      .cpu_reset_n          (cpu_reset_n),
      .cpu_select0          (cpu_select0),
      .cpu_waitrequest_n0   (cpu_waitrequest_n0),
      .cpu_write0           (cpu_write0),
      .cpu_writedata0       (cpu_writedata0),
      .reset_n              (reset_n),
      .slave_address0       (slave_address0),
      .slave_functiondata0  (slave_functiondata0),
      .slave_read0          (slave_read0),
      .slave_readdata0      (slave_readdata0),
      .slave_waitrequest_n0 (slave_waitrequest_n0)
    );

  assign FIR_HW_functiondata0 = slave_functiondata0;
  //cpu_interface0, which is an e_avalon_slave
  //internal_master0, which is an e_avalon_master
  assign dummy_master_write = 0;
  assign dummy_master_writedata = 0;
  assign dummy_waitrequest_sink = dummy_master_waitrequest;
  assign dummy_master_address = 0;
  assign dummy_chipselect_sink = dummy_slave_chipselect;
  assign dummy_address_sink = dummy_slave_address;
  assign dummy_slave_readdata = 0;
  //dummy_master, which is an e_avalon_master
  //dummy_slave, which is an e_avalon_slave
  assign FIR_HW0 = 0;

endmodule



