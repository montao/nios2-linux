module eth_dc_reg(
	input	d,
	input	inclk,
	input	outclk,
	input	reset,
	output	q
);
reg     q1, q2, q3, q4;
wire    q1_reset;
	
assign 	q1_reset = (~q1 & q3) | reset;
assign	q = q4;

//q1 output decoder (simply latch the input to the input clock domain
always @(posedge inclk or posedge reset)
	if(reset)		q1 <= 1'b0;
	else			q1 <= d;
	 
//q2 output decoder based on async q1 input
always @(posedge q1 or posedge q1_reset)
	if(q1_reset)	q2 <= 1'b0;
	else			q2 <= 1'b1;

//q2 and q3 output decoders
always @(posedge outclk or posedge reset)
	if(reset) 		{q3, q4} <= 2'b00;
	else			{q3, q4} <= {q2, q3};

endmodule

