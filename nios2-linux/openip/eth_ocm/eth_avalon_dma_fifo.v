//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_avalon_dma_fifo.v                                       ////
////                                                              ////
////  This file is a patch used in conjunction with the           ////
////  Ethernet IP core project.                                   ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Jakob Jones (jrjonsie@gmail.com)                      ////
////                                                              ////
////  All additional information is available in the Readme.txt   ////
////  file.                                                       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
module eth_avalon_dma_fifo #(   parameter   DEPTH=1024,
                                parameter   WIDTH=36) (
    input                   aclr,
    input   [WIDTH - 1:0]   data,
    input                   rdclk,
    input                   rdreq,
    input                   wrclk,
    input                   wrreq,
    output  [WIDTH - 1:0]   q,
    output                  rdempty,
    output  [(clogb2(DEPTH)-2):0]   rdusedw,
    output  [(clogb2(DEPTH)-2):0]   wrusedw,
    output                  wrfull
);

`include "eth_avalon_functions.v"

localparam  AWIDTH = clogb2(DEPTH) - 1;

dcfifo  dcfifo_component (
    .aclr       (aclr       ),
    .wrclk      (wrclk      ),
    .rdreq      (rdreq      ),
    .rdclk      (rdclk      ),
    .wrreq      (wrreq      ),
    .data       (data       ),
    .rdempty    (rdempty    ),
    .wrfull     (wrfull     ),
    .q          (q          ),
    .rdusedw    (rdusedw    ),
    .wrusedw    (wrusedw    )
    // synopsys translate_off
    ,
    .rdfull     (),
    .wrempty    (),
    // synopsys translate_on
    );
    defparam
//      dcfifo_component.intended_device_family = "Cyclone II",
		dcfifo_component.lpm_hint = "MAXIMIZE_SPEED=7,",
		dcfifo_component.lpm_numwords = DEPTH,
		dcfifo_component.lpm_showahead = "ON",
		dcfifo_component.lpm_type = "dcfifo",
		dcfifo_component.lpm_width = WIDTH,
		dcfifo_component.lpm_widthu = AWIDTH,
		dcfifo_component.overflow_checking = "OFF",
		dcfifo_component.rdsync_delaypipe = 5,
		dcfifo_component.underflow_checking = "OFF",
		dcfifo_component.use_eab = "ON",
		dcfifo_component.write_aclr_synch = "OFF",
		dcfifo_component.wrsync_delaypipe = 5;
endmodule
