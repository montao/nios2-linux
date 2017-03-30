// product_info_0.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

module product_info_0 (
		input  wire        chipselect_n, //   avalon_slave_0.chipselect_n
		input  wire        read_n,       //                 .read_n
		output wire [31:0] av_data_read, //                 .readdata
		input  wire [1:0]  av_address,   //                 .address
		input  wire        clk,          //       clock_sink.clk
		input  wire        reset_n       // clock_sink_reset.reset_n
	);

	product_info product_info_0 (
		.chipselect_n (chipselect_n), //   avalon_slave_0.chipselect_n
		.read_n       (read_n),       //                 .read_n
		.av_data_read (av_data_read), //                 .readdata
		.av_address   (av_address),   //                 .address
		.clk          (clk),          //       clock_sink.clk
		.reset_n      (reset_n)       // clock_sink_reset.reset_n
	);

endmodule
