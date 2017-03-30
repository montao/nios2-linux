//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_avalon_rxdma.v                                          ////
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
//    History:
//      12/01/08 - Added proper support for aborted packets. This
//                 addresses an issue with the MAC not aborting
//                 promiscuous packets in non-promiscuous mode.
//
module  eth_avalon_rxdma   #(  parameter FIFO_DEPTH = 4096) (
    //Commaon signals
    input               clk,
    input               reset,

    //Descriptor ram interface
    input       [6:0]   max_rx_bd,          //Highest index RX Descriptor
    input       [31:0]  bd_desc,            //Descriptor Control data input
    input       [31:0]  bd_ptr,             //Descriptor pointer input
    input               bd_wait,            //Descriptor RAM is busy

    output              bd_write,           //write control data to BD RAM
    output              bd_read,            //read control and pointer data from BD RAM
    output  reg [6:0]   bd_index,           //Which descriptor to read
    output      [31:0]  bd_writedata,       //Control data to be written to descriptor

    //Memory port interface
    input               av_waitrequest,     //Memory port is busy

    output  reg         av_write,           //Memory port write
    output  reg [31:0]  av_address,         //Memory port address
    output  reg [31:0]  av_writedata,       //Memory port writedata
    output  reg [3:0]   av_byteenable,      //Memory port byteenable

    //Streaming interface
    input               RxEn,               //Receive Enable
    input               rxclk,              //Receive clock
    input       [7:0]   rx_data,            //input data
    input               rx_dv,              //qualifies datain, startofpacket, and endofpacket
    input       [8:0]   rx_err,             //error bits
    input               rx_sop,             //start of data packet
    input               rx_eop,             //end of data packet
    input               rx_abort,           //abort packet

    //Interrupt outputs
    output  reg         RxB_IRQ,
    output  reg         RxE_IRQ,
    output              Busy_IRQ  
);

`include "eth_avalon_functions.v"

//Let's determine the FIFO Depths. We use two FIFOs. One to pass data.
//and one to pass error information. The error FIFO does not need to be as
//large because it is only written to on an end of packet. We'll make the
//assumption that normally packets will be a minimum of 64 bytes long. So
//We divide the FIFO depth by 64 to get the Error FIFO depth. To be on the
//safe side, we double that number (really divide by 32). If Either one of
//these FIFOs overflow, on overrun will occur.
localparam  MINFD   = max(FIFO_DEPTH,128);  //Minimum 128 byte FIFO depth
localparam  RFD     = nextPow2(MINFD)>>2; //FIFO depth next power of 2 (and divide by 4)
localparam  EFD     = max((RFD>>3), 4);     //Minimum error FIFO depth of 4

//Descriptor status bit defines
localparam  BIT_LEN_H   = 31,   //Upper bit of length field
            BIT_LEN_L   = 16,   //Lower bit of length field
            //Control bits
            BIT_EMPTY   = 15,   //Empty control bit (1=EMPTY Controller can write to it)
            BIT_IRQ     = 14,   //Generate an interrupt at end of RX
            BIT_WRAP    = 13,   //Wrap to first RX descriptor after this one
            BIT_RSVD_H  = 12,   //Upper bit of reserved field
            BIT_RSVD_L  = 11,   //Lower bit of reserved field
            //Status bits
            BIT_CF      = 8,    //Control Frame was received
            BIT_M       = 7,    //Miss
            BIT_OR      = 6,    //FIFO overrun
            BIT_IS      = 5,    //Invalid Symbol
            BIT_DN      = 4,    //Dribble Nibble
            BIT_TL      = 3,    //Frame too long
            BIT_SF      = 2,    //Frame short
            BIT_CRC     = 1,    //CRC error
            BIT_LC      = 0;    //Late Collision

//DMA State Machine Bits
localparam  ST_IDLE     =   0,
            ST_BD_RD    =   1,
            ST_DMA1     =   2,
            ST_DMA2     =   3,
            ST_BD_WR    =   4;

//RX State Machine Bits
localparam  RX_IDLE = 0,
            RX_REC  = 1,
            RX_DISC = 2,
            RX_EOP  = 3;

//Avalon interface signals
wire    [31:0]  pre_av_writedata;
wire    [3:0]   pre_av_byteenable;
wire            pre_av_write;
wire    [31:0]  pre_av_address;

//Descriptor signals
reg     [31:0]  desc;       //Descriptor control word
reg     [31:0]  ptr;        //Write pointer (from descriptor)
reg     [15:0]  len;        //Length counter

//DMA controller signals
reg     [5:0]   state;      //One-hot state machine bits
wire    [1:0]   cnt;        //Valid byte count (out of FIFO);
wire            eop;        //End of packet flag (out of FIFO)
wire            abort;      //Abort flag (out of FIFO)
reg             err_r;

reg             first_write;//First write to memory
wire    [3:0]   lsb_be;     //LSB byteenable (First write)
wire    [12:0]  msb_be;
reg     [2:0]   msb_be2;
wire    [63:0]  sdata;      //shifted data
wire            rx_write;
wire            valid_rx_wr;

//FIFO Signals
//(DMA side)
wire            dff_empty;  //data FIFO empty
wire    [35:0]  dff_dout;   //data FIFO output  
reg     [35:0]  dff_dout_r; //registered dff_out
wire            dff_read;   //data FIFO read

wire            eff_empty;  //error FIFO empty
wire    [8:0]   eff_dout;   //error FIFO output
reg     [8:0]   eff_dout_r; //registered eff_dout
wire            eff_read;   //error FIFO read
//(Streaming Side)
wire    [35:0]  dff_in;         // data fifo input data
reg     [7:0]   dff_in_reg[3:0];// registered incoming data word (to make a x4)
reg     [3:0]   dff_stat;       // {errors,eop,#valid bytes}
wire            dff_wr;         // write data to FIFO
wire            dff_full;       // data FIFO full
wire            eff_wr;
wire            eff_full;

//Streaming interface signals
reg     [3:0]   rx_state;   // RX state bits
reg     [1:0]   rx_cnt;     // # of valid bytes in data word
reg             rx_wr;      // indicates normal write to FIFO
wire            rx_rdy;     // indicates FIFO is ready to receive (not full)
reg             rx_bsy;     // Translates to Busy_IRQ but in the rx domain
reg             rx_overrun; // an overrun as occurred
reg             rx_abort_r; // latch rx_abort flag at end of packet
wire    [8:0]   rx_res;     // RX result (errors and status)

//*****************************************************************************
//************************* Avalon Interface Logic ****************************
always @(posedge clk or posedge reset)
    if(reset)                   av_write    <= 1'b0;
    else if(~av_waitrequest)    av_write    <= pre_av_write;

always @(posedge clk)
    if(~av_waitrequest) begin
                        av_writedata    <= pre_av_writedata;
                        av_address      <= pre_av_address;
                        av_byteenable   <= pre_av_byteenable;
    end
//*********************** End Avalon Interface Logic **************************
//*****************************************************************************

//*****************************************************************************
//************************** DMA Interface Logic ******************************

assign  abort       = dff_dout_r [35];
assign  eop         = dff_dout   [34];
assign  cnt         = dff_dout   [33:32];

//Avalon Memory Master interface
assign  pre_av_writedata    = sdata[63:32];
assign  pre_av_byteenable   =  first_write     ?   lsb_be: 
                            state[ST_DMA2 ] ?   {1'b0,msb_be2}:
                            msb_be[9:6];

assign  pre_av_address      = {ptr[31:2],2'b00};
assign  pre_av_write        = rx_write;

assign  rx_write    = (state[ST_DMA1] & ~dff_empty) | (state[ST_DMA2] & ~eff_empty);
assign  valid_rx_wr = rx_write & ~av_waitrequest;
assign  dff_read    = state[ST_DMA1] & ~dff_empty & ~av_waitrequest;
assign  eff_read    = state[ST_DMA2] & ~eff_empty & ~av_waitrequest;

//Shift data for unaligned transfers
assign  sdata   = {dff_dout[31:0], dff_dout_r[31:0]}  << {ptr[1:0],3'd0};
//Shift byteenable for first unaligned transfer
assign  lsb_be  = 4'b1111 << ptr[1:0];
assign  msb_be  = 13'b0000001111111 << ({1'b0,ptr[1:0]} + {1'b0,cnt});

//State machine (one-hot) encoding
always @(posedge clk or posedge reset)
    if(reset)   begin                               state           <= 5'd0;
                                                    state[ST_IDLE]  <= 1'b1;
    end else begin
        /*default case-->*/                         state           <= 5'd0;
        case(1'b1) // synopsys parallel_case 
            state[ST_IDLE ]:
                if(RxEn)                            state[ST_BD_RD] <= 1'b1;
                else                                state[ST_IDLE ] <= 1'b1;
            state[ST_BD_RD]:    
                if(~bd_wait & bd_desc[BIT_EMPTY])   state[ST_DMA1 ] <= 1'b1;
                else                                state[ST_BD_RD] <= 1'b1;
            state[ST_DMA1 ]:
                if(valid_rx_wr & eop)               state[ST_DMA2 ] <= 1'b1;
                else                                state[ST_DMA1 ] <= 1'b1;
            state[ST_DMA2 ]: 
                if(valid_rx_wr)
                                                    state[ST_BD_WR] <= 1'b1;
                else                                state[ST_DMA2 ] <= 1'b1;
            state[ST_BD_WR]:
                if(~bd_wait)                        state[ST_IDLE ] <= 1'b1;
                else                                state[ST_BD_WR] <= 1'b1;
        endcase
    end

always @(posedge clk) begin
    if(state[ST_IDLE ])                 first_write <= 1'b1;
    if(state[ST_DMA1 ] & valid_rx_wr)   first_write <= 1'b0; 
end

//Length decoder
always @(posedge clk)
    if(state[ST_BD_RD])     len <= 16'd0;
    else if(valid_rx_wr)    len <= len + ({&pre_av_byteenable[1:0],^pre_av_byteenable[1:0]} + {&pre_av_byteenable[3:2],^pre_av_byteenable[3:2]});
                                        //((av_byteenable[0] + av_byteenable[1]) + (av_byteenable[2] + av_byteenable[3]));


always @(posedge clk)
    if(valid_rx_wr) msb_be2 <= msb_be[12:10];

//Latch Data FIFO output whenever a write to memory takes place
always @(posedge clk)
    if(state[ST_DMA1] & valid_rx_wr)        dff_dout_r  <= dff_dout;

//****************************************************************************
//********************** Descriptor Interface Logic **************************
assign  bd_read     = state[ST_BD_RD];
assign  bd_write    = state[ST_BD_WR] & ~abort;
assign  bd_writedata= {len[15:0],1'b0,desc[BIT_IRQ:BIT_WRAP],4'b000,eff_dout_r};

//Load pointer and descriptor data in BD Read state
always @(posedge clk) begin
    if(bd_read)                 {ptr,desc}  <= {bd_ptr,bd_desc};
    if(valid_rx_wr)             ptr[31:2]   <= ptr[31:2] + 30'd1;
end

always @(posedge clk)
    if(state[ST_DMA2 ] & valid_rx_wr)  begin  
                                eff_dout_r  <= eff_dout;
                                err_r       <= |{eff_dout[8:3],eff_dout[1:0]};
    end

//bd_index decoder
always @(posedge clk or posedge reset)
    if(reset)                   bd_index    <= 7'd0;
    else if(~RxEn)              bd_index    <= 7'd0;
    else if(bd_write & ~bd_wait) begin
        if((bd_index == max_rx_bd) | desc[BIT_WRAP])
                                bd_index    <= 7'd0;
        else
                                bd_index    <= bd_index + 7'd1;
    end

//IRQ generation to feed back to registers module
always @(posedge clk or posedge reset)
    if(reset)   begin           RxE_IRQ <= 1'b0;
                                RxB_IRQ <= 1'b0;
    end else begin              
                                RxE_IRQ <= 1'b0;
                                RxB_IRQ <= 1'b0;              
        if(bd_write & ~bd_wait)  begin
                                RxE_IRQ <= desc[BIT_IRQ] & err_r;
                                RxB_IRQ <= desc[BIT_IRQ] & ~err_r;
        end 
    end
//********************** Descriptor Interface Logic **************************
//****************************************************************************

//****************************************************************************
//****************** Dual-Clock Data and Error FIFOs *************************

//Data FIFO
eth_avalon_dma_fifo #(  .DEPTH(RFD),
                        .WIDTH(36)      ) eth_rxdma_datafifo(
    .aclr   (reset              ),
    .wrclk  (rxclk              ),
    .wrreq  (dff_wr & rx_rdy    ),
    .wrfull (dff_full           ),
    .wrusedw(                   ),
    .data   (dff_in             ),

    .rdclk  (clk                ),
    .rdreq  (dff_read           ),
    .rdempty(dff_empty          ),
    .rdusedw(                   ),
    .q      (dff_dout           )
    );

//Error FIFO
eth_avalon_dma_fifo #(  .DEPTH(EFD),
                        .WIDTH(9)       ) eth_rxdma_errfifo(
    .aclr   (reset                  ),
    .wrclk  (rxclk                  ),
    .wrreq  (eff_wr & rx_rdy        ),
    .wrfull (eff_full               ),
    .wrusedw(                       ),
    .data   (rx_res                 ),

    .rdclk  (clk                    ),
    .rdreq  (eff_read               ),
    .rdempty(eff_empty              ),
    .rdusedw(                       ),
    .q      (eff_dout               )
    );

//Set Busy IRQ if a new frame comes in and
//we're too busy to receive it.
always @(posedge rxclk)     rx_bsy  <= rx_sop & rx_dv & ~rx_rdy;

eth_dc_reg rx_busy_dc_reg(
    .d      (rx_bsy             ),
    .inclk  (rxclk              ),
    .outclk (clk                ),
    .reset  (reset              ),
    .q      (Busy_IRQ           )
);

//**************** End Dual-Clock Data and Error FIFOs ***********************
//****************************************************************************

//*****************************************************************************
//********************* Streaming Interface Logic *****************************
assign  rx_rdy  = ~dff_full & ~eff_full;
assign  dff_in  = {dff_stat,dff_in_reg[3],dff_in_reg[2],dff_in_reg[1],dff_in_reg[0]};
assign  dff_wr  = (rx_state[RX_REC ] & rx_wr) | rx_state[RX_EOP];
assign  eff_wr  = rx_state[RX_EOP ];
assign  rx_res  = {rx_err[8:7],rx_overrun,rx_err[5:0]};

always @(posedge rxclk or posedge reset)
    if(reset) begin                     rx_state            <= 4'd0;
                                        rx_state[RX_IDLE]   <= 1'b1;
    end else begin           
        /*default case-->*/             rx_state            <= 4'd0;
        case(1'b1) // synopsys parallel_case
            //Wait for start of FRAME
            //We'll never leave this
            //state if the FIFO isn't
            //ready
            rx_state[RX_IDLE]:
                if(rx_dv & rx_sop)
                    if(rx_rdy)          rx_state[RX_REC ]   <= 1'b1;
                    else                rx_state[RX_IDLE]   <= 1'b1;
                else                    rx_state[RX_IDLE]   <= 1'b1;
            //Receiving Frame
            rx_state[RX_REC ]:
                if(rx_dv) begin
                    if(rx_eop)          rx_state[RX_EOP ]   <= 1'b1;
                    else if(~rx_rdy)    rx_state[RX_DISC]   <= 1'b1;
                    else                rx_state[RX_REC ]   <= 1'b1;
                end else                rx_state[RX_REC ]   <= 1'b1;
            //Throw away the frame. We
            //Overran the FIFO in the
            //middle of the frame. We
            //discard the rest of the
            //frame.
            rx_state[RX_DISC]: 
                if(rx_eop & rx_dv)      rx_state[RX_EOP ]   <= 1'b1;
                else                    rx_state[RX_DISC]   <= 1'b1;
            //Write last word to FIFO.
            //We will sit here indefintely
            //until the FIFO is ready
            rx_state[RX_EOP]:
                if(rx_rdy)              rx_state[RX_IDLE]   <= 1'b1;
                else                    rx_state[RX_EOP ]   <= 1'b1;
        endcase
    end

always @(posedge rxclk or posedge reset)
    if(reset)                           rx_overrun      <= 1'b0;
    else if(rx_state[RX_REC])           rx_overrun      <= rx_dv & ~rx_rdy;
    else if(rx_state[RX_EOP] & rx_rdy)  rx_overrun      <= 1'b0;      

always @(posedge rxclk)
    if(rx_state[RX_IDLE])               rx_abort_r      <= 1'b0;
    else if(rx_state[RX_REC] | rx_state[RX_DISC])
                                        rx_abort_r      <= rx_dv & rx_eop & rx_abort;

//We'll allow this to overrun
always @(posedge rxclk or posedge reset)
    if(reset)                               rx_cnt      <= 2'd0;
    else 
        case(1'b1) // synopsys parallel_case
            rx_state[RX_IDLE]:
                if(rx_dv & rx_sop & rx_rdy) rx_cnt      <= rx_cnt + 2'd1;
                else                        rx_cnt      <= 2'd0;

            rx_state[RX_REC ]:
                if(rx_dv)                   rx_cnt      <= rx_cnt + 2'd1;

            rx_state[RX_EOP ]:              rx_cnt      <= 2'd0;
        endcase

//set the write flag when all 4 bytes are received
always @(posedge rxclk or posedge reset)
    if(reset)                               rx_wr       <= 1'b0;
    else 
        if(rx_state[RX_REC])                rx_wr       <= rx_dv & (&rx_cnt);
        else                                rx_wr       <= 1'b0;
                        

//We'll alow these to overrun
always @* begin             dff_stat[2]     = rx_state[RX_EOP ];
    if(rx_state[RX_EOP])    dff_stat[3]     = rx_abort_r;
    else                    dff_stat[3]     = 1'b0;
end

always @(posedge rxclk)
    if(rx_dv)   begin       dff_in_reg[rx_cnt]  <= rx_data;
                            dff_stat[1:0]       <= rx_cnt;
    end

//******************* End Streaming Interface Logic ***************************
//*****************************************************************************
endmodule

