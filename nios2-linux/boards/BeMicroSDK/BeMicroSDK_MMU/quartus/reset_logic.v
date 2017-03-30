module reset_logic(
					input	wire a_50_MHZ_CLK,
					output	reg RESET_N
					);

wire	COUT2;
wire	inverted_COUT2;

reset_counter	b2v_inst(
	.clock(a_50_MHZ_CLK),
	.cnt_en(inverted_COUT2),
	.cout(COUT2)
	);


always@(posedge a_50_MHZ_CLK)
begin
	begin
	RESET_N = COUT2;
	end
end

assign inverted_COUT2 =  ~COUT2;

endmodule
