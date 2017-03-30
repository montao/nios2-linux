// accelerator_FIR_FIR_HW_0.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

module accelerator_FIR_FIR_HW_0 (
		input  wire        clk,                                                                 //                                                       clock.clk
		input  wire        reset_n,                                                             //                                                 clock_reset.reset_n
		input  wire        cpu_clk,                                                             //                                                   cpu_clock.clk
		input  wire        cpu_reset_n,                                                         //                                             cpu_clock_reset.reset_n
		input  wire [9:0]  accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0,            // accelerator_FIR_FIR_HW_output_buffer_internal_memory0_slave.address
		input  wire [31:0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0,            //                                                            .writedata
		output wire [31:0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0,            //                                                            .readdata
		input  wire [31:0] accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0, //                                                            .baseaddress
		input  wire        accelerator_FIR_FIR_HW_output_buffer_internal_memory0_write0,        //                                                            .write
		input  wire        accelerator_FIR_FIR_HW_output_buffer_internal_memory0_chipselect0,   //                                                            .chipselect
		input  wire [3:0]  accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0,           //                                                            .byteenable
		input  wire        FIR_HW_read0,                                                        //                                                 sub_FIR_HW0.read
		output wire [31:0] cpu_readdata0,                                                       //                                                            .readdata
		output wire        FIR_HW_waitrequest_n0,                                               //                                                            .waitrequest_n
		input  wire        FIR_HW_select0,                                                      //                                                            .chipselect
		input  wire        FIR_HW_begin0,                                                       //                                                            .begintransfer
		output wire        accelerator_FIR_FIR_HW_master_resource0_write0,                      //                     accelerator_FIR_FIR_HW_master_resource0.write
		output wire [31:0] accelerator_FIR_FIR_HW_master_resource0_address0,                    //                                                            .address
		input  wire        accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0,              //                                                            .waitrequest_n
		output wire [31:0] accelerator_FIR_FIR_HW_master_resource0_writedata0,                  //                                                            .writedata
		output wire [3:0]  accelerator_FIR_FIR_HW_master_resource0_byteenable0,                 //                                                            .byteenable
		output wire        accelerator_FIR_FIR_HW_master_resource1_read0,                       //                     accelerator_FIR_FIR_HW_master_resource1.read
		input  wire        accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0,              //                                                            .waitrequest_n
		output wire [31:0] accelerator_FIR_FIR_HW_master_resource1_address0,                    //                                                            .address
		output wire        accelerator_FIR_FIR_HW_master_resource1_flush0,                      //                                                            .flush
		input  wire        accelerator_FIR_FIR_HW_master_resource1_readdatavalid0,              //                                                            .readdatavalid
		input  wire [15:0] accelerator_FIR_FIR_HW_master_resource1_readdata0,                   //                                                            .readdata
		output wire        accelerator_FIR_FIR_HW_master_resource3_write0,                      //                     accelerator_FIR_FIR_HW_master_resource3.write
		input  wire        accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0,              //                                                            .waitrequest_n
		output wire [31:0] accelerator_FIR_FIR_HW_master_resource3_address0,                    //                                                            .address
		output wire [1:0]  accelerator_FIR_FIR_HW_master_resource3_byteenable0,                 //                                                            .byteenable
		output wire [15:0] accelerator_FIR_FIR_HW_master_resource3_writedata0,                  //                                                            .writedata
		input  wire        accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0,              //                     accelerator_FIR_FIR_HW_master_resource2.waitrequest_n
		output wire [31:0] accelerator_FIR_FIR_HW_master_resource2_address0,                    //                                                            .address
		output wire        accelerator_FIR_FIR_HW_master_resource2_flush0,                      //                                                            .flush
		input  wire [31:0] accelerator_FIR_FIR_HW_master_resource2_readdata0,                   //                                                            .readdata
		input  wire        accelerator_FIR_FIR_HW_master_resource2_readdatavalid0,              //                                                            .readdatavalid
		output wire        accelerator_FIR_FIR_HW_master_resource2_read0,                       //                                                            .read
		output wire [31:0] cpu_readdata1,                                                       //                                              cpu_interface0.readdata
		input  wire [31:0] cpu_writedata0,                                                      //                                                            .writedata
		output wire        cpu_waitrequest_n0,                                                  //                                                            .waitrequest_n
		input  wire [3:0]  cpu_address0,                                                        //                                                            .address
		input  wire        cpu_write0,                                                          //                                                            .write
		input  wire        cpu_select0,                                                         //                                                            .chipselect
		input  wire [31:0] slave_readdata0,                                                     //                                            internal_master0.readdata
		output wire [31:0] slave_address0,                                                      //                                                            .address
		output wire        slave_read0,                                                         //                                                            .read
		input  wire        slave_waitrequest_n0,                                                //                                                            .waitrequest_n
		output wire        dummy_master_write,                                                  //                                                dummy_master.write
		output wire [31:0] dummy_master_writedata,                                              //                                                            .writedata
		output wire [31:0] dummy_master_address,                                                //                                                            .address
		input  wire        dummy_master_waitrequest,                                            //                                                            .waitrequest
		input  wire        dummy_slave_chipselect,                                              //                                                 dummy_slave.chipselect
		input  wire        dummy_slave_address,                                                 //                                                            .address
		output wire [31:0] dummy_slave_readdata                                                 //                                                            .readdata
	);

	accelerator_FIR_FIR_HW accelerator_fir_fir_hw_0 (
		.clk                                                                 (clk),                                                                 //                                                       clock.clk
		.reset_n                                                             (reset_n),                                                             //                                                 clock_reset.reset_n
		.cpu_clk                                                             (cpu_clk),                                                             //                                                   cpu_clock.clk
		.cpu_reset_n                                                         (cpu_reset_n),                                                         //                                             cpu_clock_reset.reset_n
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0            (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_a0),            // accelerator_FIR_FIR_HW_output_buffer_internal_memory0_slave.address
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0            (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_d0),            //                                                            .writedata
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0            (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_q0),            //                                                            .readdata
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0 (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_base_address0), //                                                            .baseaddress
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_write0        (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_write0),        //                                                            .write
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_chipselect0   (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_chipselect0),   //                                                            .chipselect
		.accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0           (accelerator_FIR_FIR_HW_output_buffer_internal_memory0_be0),           //                                                            .byteenable
		.FIR_HW_read0                                                        (FIR_HW_read0),                                                        //                                                 sub_FIR_HW0.read
		.cpu_readdata0                                                       (cpu_readdata0),                                                       //                                                            .readdata
		.FIR_HW_waitrequest_n0                                               (FIR_HW_waitrequest_n0),                                               //                                                            .waitrequest_n
		.FIR_HW_select0                                                      (FIR_HW_select0),                                                      //                                                            .chipselect
		.FIR_HW_begin0                                                       (FIR_HW_begin0),                                                       //                                                            .begintransfer
		.accelerator_FIR_FIR_HW_master_resource0_write0                      (accelerator_FIR_FIR_HW_master_resource0_write0),                      //                     accelerator_FIR_FIR_HW_master_resource0.write
		.accelerator_FIR_FIR_HW_master_resource0_address0                    (accelerator_FIR_FIR_HW_master_resource0_address0),                    //                                                            .address
		.accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource0_waitrequest_n0),              //                                                            .waitrequest_n
		.accelerator_FIR_FIR_HW_master_resource0_writedata0                  (accelerator_FIR_FIR_HW_master_resource0_writedata0),                  //                                                            .writedata
		.accelerator_FIR_FIR_HW_master_resource0_byteenable0                 (accelerator_FIR_FIR_HW_master_resource0_byteenable0),                 //                                                            .byteenable
		.accelerator_FIR_FIR_HW_master_resource1_read0                       (accelerator_FIR_FIR_HW_master_resource1_read0),                       //                     accelerator_FIR_FIR_HW_master_resource1.read
		.accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource1_waitrequest_n0),              //                                                            .waitrequest_n
		.accelerator_FIR_FIR_HW_master_resource1_address0                    (accelerator_FIR_FIR_HW_master_resource1_address0),                    //                                                            .address
		.accelerator_FIR_FIR_HW_master_resource1_flush0                      (accelerator_FIR_FIR_HW_master_resource1_flush0),                      //                                                            .flush
		.accelerator_FIR_FIR_HW_master_resource1_readdatavalid0              (accelerator_FIR_FIR_HW_master_resource1_readdatavalid0),              //                                                            .readdatavalid
		.accelerator_FIR_FIR_HW_master_resource1_readdata0                   (accelerator_FIR_FIR_HW_master_resource1_readdata0),                   //                                                            .readdata
		.accelerator_FIR_FIR_HW_master_resource3_write0                      (accelerator_FIR_FIR_HW_master_resource3_write0),                      //                     accelerator_FIR_FIR_HW_master_resource3.write
		.accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource3_waitrequest_n0),              //                                                            .waitrequest_n
		.accelerator_FIR_FIR_HW_master_resource3_address0                    (accelerator_FIR_FIR_HW_master_resource3_address0),                    //                                                            .address
		.accelerator_FIR_FIR_HW_master_resource3_byteenable0                 (accelerator_FIR_FIR_HW_master_resource3_byteenable0),                 //                                                            .byteenable
		.accelerator_FIR_FIR_HW_master_resource3_writedata0                  (accelerator_FIR_FIR_HW_master_resource3_writedata0),                  //                                                            .writedata
		.accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0              (accelerator_FIR_FIR_HW_master_resource2_waitrequest_n0),              //                     accelerator_FIR_FIR_HW_master_resource2.waitrequest_n
		.accelerator_FIR_FIR_HW_master_resource2_address0                    (accelerator_FIR_FIR_HW_master_resource2_address0),                    //                                                            .address
		.accelerator_FIR_FIR_HW_master_resource2_flush0                      (accelerator_FIR_FIR_HW_master_resource2_flush0),                      //                                                            .flush
		.accelerator_FIR_FIR_HW_master_resource2_readdata0                   (accelerator_FIR_FIR_HW_master_resource2_readdata0),                   //                                                            .readdata
		.accelerator_FIR_FIR_HW_master_resource2_readdatavalid0              (accelerator_FIR_FIR_HW_master_resource2_readdatavalid0),              //                                                            .readdatavalid
		.accelerator_FIR_FIR_HW_master_resource2_read0                       (accelerator_FIR_FIR_HW_master_resource2_read0),                       //                                                            .read
		.cpu_readdata1                                                       (cpu_readdata1),                                                       //                                              cpu_interface0.readdata
		.cpu_writedata0                                                      (cpu_writedata0),                                                      //                                                            .writedata
		.cpu_waitrequest_n0                                                  (cpu_waitrequest_n0),                                                  //                                                            .waitrequest_n
		.cpu_address0                                                        (cpu_address0),                                                        //                                                            .address
		.cpu_write0                                                          (cpu_write0),                                                          //                                                            .write
		.cpu_select0                                                         (cpu_select0),                                                         //                                                            .chipselect
		.slave_readdata0                                                     (slave_readdata0),                                                     //                                            internal_master0.readdata
		.slave_address0                                                      (slave_address0),                                                      //                                                            .address
		.slave_read0                                                         (slave_read0),                                                         //                                                            .read
		.slave_waitrequest_n0                                                (slave_waitrequest_n0),                                                //                                                            .waitrequest_n
		.dummy_master_write                                                  (dummy_master_write),                                                  //                                                dummy_master.write
		.dummy_master_writedata                                              (dummy_master_writedata),                                              //                                                            .writedata
		.dummy_master_address                                                (dummy_master_address),                                                //                                                            .address
		.dummy_master_waitrequest                                            (dummy_master_waitrequest),                                            //                                                            .waitrequest
		.dummy_slave_chipselect                                              (dummy_slave_chipselect),                                              //                                                 dummy_slave.chipselect
		.dummy_slave_address                                                 (dummy_slave_address),                                                 //                                                            .address
		.dummy_slave_readdata                                                (dummy_slave_readdata)                                                 //                                                            .readdata
	);

endmodule
