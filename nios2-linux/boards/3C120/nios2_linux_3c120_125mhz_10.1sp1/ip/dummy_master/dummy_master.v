
module dummy_master

#(
    parameter MASTER_ADDRESS_WIDTH      = 8
)

(
    // master clock interface
    input                                       csi_master_clk_clk,
    input                                       csi_master_clk_reset,
    
    // master interface
    output  [(MASTER_ADDRESS_WIDTH - 1):0]      avm_m0_address,
    output  [3:0]   							avm_m0_byteenable,
    output                                      avm_m0_read,
    output                                      avm_m0_write,
    output  [31:0]         						avm_m0_writedata,
    input                                       avm_m0_waitrequest,
    input   [31:0]         						avm_m0_readdata
);

assign avm_m0_address		= { MASTER_ADDRESS_WIDTH { 1'b0 }};
assign avm_m0_byteenable	= { 4 { 1'b0 }};
assign avm_m0_read			= 1'b0;
assign avm_m0_write			= 1'b0;
assign avm_m0_writedata		= { 32 { 1'b0 }};

endmodule
