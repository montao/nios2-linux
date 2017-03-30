//
// This global_reset_generator will assert global_resetn LO for 2^RESET_COUNTER_WIDTH
// clocks immediately upon power up or at the event of any of the active low
// resetn_sources triggering a reset.
//
module global_reset_generator
#(
    parameter RESET_SOURCES_WIDTH = 1,
    parameter RESET_COUNTER_WIDTH = 16
)
(
    input                                   clk,
    input   [(RESET_SOURCES_WIDTH - 1):0]   resetn_sources,
    output                                  global_resetn,
    output                                  pll_resetn
);

wire    [(RESET_SOURCES_WIDTH - 1):0]   sync_resetn_sources;
wire    [(RESET_SOURCES_WIDTH - 1):0]   edge_detect;
wire                                    internal_global_resetn;

assign global_resetn = internal_global_resetn;
assign pll_resetn = ~|edge_detect;

generate
genvar i;
for(i = 0; i < (RESET_SOURCES_WIDTH); i = i + 1) 
begin : reset_sync_block

    //
    // Synchronize the reset sources from their clock domain, into the clock
    // domain of this module.
    // 
    bit_synchronizer bit_synchronizer_inst
    (
        .clk        (clk),
        .data_in    (resetn_sources[i]),
        .data_out   (sync_resetn_sources[i])
    );

    //
    // Detect falling edges on any of the synchronized reset sources
    //
    falling_edge_detector falling_edge_detector_inst
    (
        .clk                    (clk),
        .resetn                 (internal_global_resetn),
        .data_in                (sync_resetn_sources[i]),
        .falling_edge_detected  (edge_detect[i])
    );

end
endgenerate
                        
//
// Trigger the reset counter when a falling edge detection event occurs.
//
reset_counter
#(
    .COUNTER_WIDTH  (RESET_COUNTER_WIDTH)
) reset_counter_inst
(
    .clk            (clk),
    .reset_in       (|edge_detect),
    .resetn_out     (internal_global_resetn)
);

endmodule

//
// bit_synchronizer is simply a two flip flop synchronization stage intended to
// synchronize a signal from one clock domain into another and minimize the
// metastability effects of doing so.
//
module bit_synchronizer
(
    input       clk,
    input       data_in,
    output  reg data_out
);

reg p1;

always @ (posedge clk) begin
    p1          <= data_in;
    data_out    <= p1;
end

endmodule

//
// falling_edge_detector simply generates a one clock active high pulse when
// it detects the input signal transitioning from HI to LO, a falling edge.
//
module falling_edge_detector
(
    input       clk,
    input       resetn,
    input       data_in,
    output  reg falling_edge_detected
);

reg p1;
reg p2;

always @ (posedge clk or negedge resetn) begin
    if(!resetn) begin
        p1  <= 0;
        p2  <= 0;
        falling_edge_detected   <= 0;
    end else begin
        p1  <= data_in;
        p2  <= p1;
        falling_edge_detected   <= (!p1 & p2) ? (1'b1) : (1'b0);
    end
end

endmodule

//
// reset_counter module is synchronously triggered to produce an active low
// resetn_out strobe for the duration of 2^COUNTER_WIDTH clock cycles.  The
// delay counter is triggered at power on, or at the positive assertion of
// reset_in for one clock.  This delay counter is reset by each positive
// assertion of reset_in.
//
module reset_counter
#(
    parameter COUNTER_WIDTH = 16
)
(
    input       clk,
    input       reset_in,
    output  reg resetn_out
);

reg [(COUNTER_WIDTH - 1):0] counter;

always @ (posedge clk) begin
    if(reset_in) begin
        resetn_out  <= 0;
        counter     <= 0;
    end else if(~&counter) begin
        counter     <= counter + 1;
    end else begin
        resetn_out  <= 1;
    end
end

endmodule
