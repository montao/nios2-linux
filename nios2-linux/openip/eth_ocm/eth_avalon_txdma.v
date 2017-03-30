//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_avalon_txdma.v                                          ////
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
//      07/03/08 - Repaired bug with determining number of valid
//                 bytes corresponding to last data read from
//                 memory. Code was examining length field from
//                 descriptor input rather than registered
//                 descriptor data.
//
//      08/12/08 - Added logic to prevent FIFO overflows when 
//                 Avalon BUS has large latency.
//
//      12/02/08 - Modified condition for passing start flag to
//                 transmit state machine. This resolves an issue
//                 with memory interfaces with very long read
//                 latencies when transmitting short packets.
//
module eth_avalon_txdma    #(parameter FIFO_DEPTH=128) (
    //Commaon signals
    input               clk,                // System clock
    input               reset,

    //Descriptor ram interface
    input       [6:0]   max_tx_bd,          // Highest index RX Descriptor
    input       [31:0]  bd_desc,            // Descriptor Control data input
    input       [31:0]  bd_ptr,             // Descriptor pointer input
    input               bd_wait,            // Descriptor RAM is busy

    output              bd_write,           // write control data to BD RAM
    output              bd_read,            // read control and pointer data from BD RAM
    output  reg [6:0]   bd_index,           // Which descriptor to read
    output      [31:0]  bd_writedata,       // Control data to be written to descriptor

    //Memory port interface
    input               av_waitrequest,     // Memory port is busy
    input       [31:0]  av_readdata,        // Memory port readdata
    input               av_readdatavalid,   // Memory port readdata valid signal

    output  reg         av_read,            // Memory port read
    output  reg [31:0]  av_address,         // Memory port address

    //Streaming interface
    input               TxEn,               // Enable transmit
    input               txclk,              // Transmit clock
    output      [7:0]   tx_data,            // output data
    output              tx_dv,              // qualifies dataout, startofpacket, and endofpacket
    output              tx_sop,             // start of data packet
    output              tx_eop,             // end of data packet
    input               tx_ack,             // Acknowledge TX data
    input       [8:0]   tx_stat,            // Status bits
    input               tx_stat_valid,      // Status is valid
    output              tx_stat_ack,
    input               tx_retry,

    output              PerPacketPad,       // Per packet pad
    output              PerPacketCrc,       // Per packet crc
    output              TxUnderRun,         // An underrun occured

    //Interrupt outputs
    output  reg         TxB_IRQ,
    output  reg         TxE_IRQ 
);

//Some useful constant functions
`include "eth_avalon_functions.v"

localparam  MINFD   = max(FIFO_DEPTH,128);  // Minimum 128 byte FIFO depth
localparam  RFD     = nextPow2(MINFD)>>2;   // FIFO depth next power of 2 (and divide by 4)

localparam  FIFO_MARGIN     = 5;            // Cushion between FIFO depth and our perceived depth
localparam  FIFO_THRESHOLD  = RFD - FIFO_MARGIN;

//Bit descriptions for TX descriptor
localparam  BIT_LEN_H   = 31,   // Upper bit of length field
            BIT_LEN_L   = 16,   // Lower bit of length field
            //Control bits
            BIT_READY   = 15,   // Ready control bit (1=READY Controller can write to it)
            BIT_IRQ     = 14,   // Generate an interrupt at end of TX
            BIT_WRAP    = 13,   // Wrap to first RX descriptor after this one
            BIT_PAD     = 12,   // Add Padding to small frames
            BIT_CRC     = 11,   // Add CRC
            BIT_RSVD_H  = 10,   // Upper bit of reserved field
            BIT_RSVD_L  = 9,    // Lower bit of reserved field
            //Status bits
            BIT_UR      = 8,    // Buffer Underrun
            BIT_RTRY_H  = 7,    // Upper bit of retry count
            BIT_RTRY_L  = 4,    // Lower bit of retry count
            BIT_RL      = 3,    // Retransmission Limit
            BIT_LC      = 2,    // Late Collision
            BIT_DF      = 1,    // Defer Indication
            BIT_CS      = 0;    // Carrier Sense Lost

//State bits
localparam  ST_IDLE     =   0,
            ST_BD_RD    =   1,  // Read Descriptor
            ST_DMA1     =   2,  // Transfer first word
            ST_DMA2     =   3,  // Wait for data to be written to FIFO 
            ST_STAT     =   4,  // Wait for status from MAC
            ST_BD_WR    =   5;  // Write status back to MAC

//TX State machine bits
localparam  TX_IDLE     =   0,  // Waiting for data to transmit
            TX_SEND     =   1,  // Sending transmit data
            TX_WAIT     =   2;  // Waiting for status from MAC

wire            pre_av_read;    // pre-registered avalon read signal
wire    [31:0]  pre_av_address; // pre_registered avalon address
wire            valid_tx_rd;    // Avalon bus acknowledged read

reg     [5:0]   state;          // State machine bits
wire    [15:0]  bd_len;         // Frame length from descriptor

reg     [31:0]  ptr;            // Memory read pointer
reg     [31:0]  desc;           // Registered bd_desc
wire    [15:0]  desc_len;       // Frame length from stored descriptor

wire    [1:0]   valid_cnt;      // Number of valid bytes in current data word
reg     [1:0]   first_valid_cnt;// Number of valid bytes in first data word
reg     [1:0]   last_valid_cnt; // Number of valid bytes in last data word

reg     [13:0]  tg_cnt;         // target count (number of words to read from memory)
reg     [13:0]  rd_cnt;         // read count (number of words read from memory)
reg     [13:0]  wr_cnt;         // write count (number of words written to FIFO)
reg     [13:0]  rd_pending;     // Number of reads pending (used to gate reads for FIFO protection)
reg             pipe_hold;      // Hold the pipeline until some of the reads have come back
wire    [15:0]  next_tg_cnt;    // intentionally 2 bits larger
wire    [13:0]  next_rd_cnt;    // next value of rd_cnt
wire    [13:0]  next_wr_cnt;    // next value of wr_cnt

wire            last_read;      // Last read from memory
reg             last_read_r;    // Registered last_read
wire            first_write;    // First write to FIFO
wire            last_write;     // Last write to FIFO

reg     [31:0]  rdata;          // registered av_readdata
reg             valid_rdata;    // registered av_readdatavalid

wire            stat_ready;     // Status ready from TX
reg     [BIT_UR:BIT_CS] stat;
wire            stat_error;     // Status indicates error


wire            dff_clear;      // Data FIFO clear
wire    [clogb2(RFD-1)-1:0] dff_wrused; // Amount of space used in data FIFO
reg     [clogb2(RFD-1):0]   dff_used_la;// Future value of dff_wrused when all reads are finished
wire            dff_full;       // Data FIFO full
wire            dff_write;      // Data FIFO write
wire    [35:0]  dff_din;        // Data FIFO data input

wire            dff_read;       // Data FIFO read
wire            dff_empty;      // Data FIFO empty
wire    [35:0]  dff_dout;       // Data FIFO data output

reg     [2:0]   tx_state;       // Transmit state machine bits
reg             tx_stat_valid_r;// Registered tx_stat_valid
wire            tx_start;       // Start signal to transmit state machine
wire            last_byte;      // Indicates last valid byte of current data word
wire    [7:0]   tx_ff_data [0:3];// Transmit bytes from data FIFO
wire    [1:0]   byte_cnt;       // Indicates how many bytes are valid from data FIFO
reg     [1:0]   byte_index;     // Indexes tx_ff_data for byte transmission

//Avalon bus
assign  pre_av_address  = {ptr[31:2],2'b00};
assign  pre_av_read     = state[ST_DMA1 ] & ~pipe_hold;
assign  valid_tx_rd     = pre_av_read & ~av_waitrequest;

//Descriptor bus
assign  bd_read     = state[ST_BD_RD];
assign  bd_write    = state[ST_BD_WR];
//Descriptor writeback data
assign  bd_writedata[BIT_LEN_H:BIT_LEN_L]   = desc[BIT_LEN_H:BIT_LEN_L];    //No modification to length
assign  bd_writedata[BIT_READY]             = 1'b0;             // Clear ready flag
assign  bd_writedata[BIT_IRQ]               = desc[BIT_IRQ];    //leave IRQ
assign  bd_writedata[BIT_WRAP]              = desc[BIT_WRAP];   //leave WRAP
assign  bd_writedata[BIT_PAD]               = desc[BIT_PAD];    //leave PAD
assign  bd_writedata[BIT_CRC]               = desc[BIT_CRC];    //leave CRC
assign  bd_writedata[BIT_RSVD_H:BIT_RSVD_L] = desc[BIT_RSVD_H:BIT_RSVD_L]; //leave reserved field
assign  bd_writedata[BIT_UR:BIT_CS]         = stat[BIT_UR:BIT_CS];  //set status flags

assign  bd_len      = bd_desc[BIT_LEN_H:BIT_LEN_L];
assign  desc_len    = desc[BIT_LEN_H:BIT_LEN_L];


//Add a pipeline stage for Avalon reads
always @(posedge clk or posedge reset)
    if(reset)                   av_read     <= 1'b0;
    else if(~av_waitrequest)    av_read     <= pre_av_read;

always @(posedge clk)
        if(~av_waitrequest)     av_address  <= pre_av_address;

always @(posedge clk or posedge reset)
    if(reset)                                       state   <= 6'd1;
    else begin                                      state   <= 6'd0;
        //This really is a parallel case 
        case(1'b1) // synopsys parallel_case 
            state[ST_IDLE ]: // do nothing
                if(TxEn & ~stat_ready)              state[ST_BD_RD] <= 1'b1;
                else                                state[ST_IDLE]  <= 1'b1;
            state[ST_BD_RD]: // read descriptor
                if(~bd_wait & bd_desc[BIT_READY])   state[ST_DMA1 ] <= 1'b1;
                else                                state[ST_BD_RD] <= 1'b1;
            state[ST_DMA1 ]: // issue all reads
                if(valid_tx_rd & last_read)
                                                    state[ST_DMA2 ] <= 1'b1;
                else                                state[ST_DMA1 ] <= 1'b1;
            state[ST_DMA2 ]: // wait here for stat                    
                if(last_write & valid_rdata)        state[ST_STAT ] <= 1'b1;
                else                                state[ST_DMA2 ] <= 1'b1;
            state[ST_STAT ]: begin // wait for status
                if(stat_ready) begin                      
                    if(tx_retry)                    state[ST_IDLE ] <= 1'b1;
                    else                            state[ST_BD_WR] <= 1'b1;
                end else                            state[ST_STAT ] <= 1'b1;
            end
            state[ST_BD_WR]: // write descriptor   
                if(~bd_wait)                        state[ST_IDLE ] <= 1'b1;
                else                                state[ST_BD_WR] <= 1'b1;
        endcase
    end


assign  next_tg_cnt = bd_len + ({1'b0,ptr[1:0]} + 3'd3);
assign  next_rd_cnt = rd_cnt + 14'd1;
assign  next_wr_cnt = wr_cnt + 14'd1;
assign  last_read   = (next_rd_cnt >= tg_cnt) | stat_ready;
assign  first_write = !wr_cnt;
assign  last_write  = last_read_r & (next_wr_cnt >= rd_cnt);
assign  valid_cnt   = first_write ? first_valid_cnt:
                    last_write  ? last_valid_cnt:
                    2'd3;

//Register last read signal
always @(posedge clk) begin
                                            valid_rdata <= av_readdatavalid;
                                            rdata       <= av_readdata;
                                            rd_pending  <= rd_cnt - wr_cnt;         // 1 clock lag
                                            dff_used_la <= dff_wrused + rd_pending; // 1 clock lag
        if(valid_rdata)                     wr_cnt      <= next_wr_cnt;
    //This really is a parallel case 
    case(1'b1) // synopsys parallel_case 
        state[ST_IDLE]: begin
                                            last_read_r <= 1'b0;                //clear last read flag
                                            rd_cnt      <= 14'd0;               //clear read count
                                            wr_cnt      <= 14'd0;               //clear write count
                                            TxB_IRQ     <= 1'b0;                //clear irq
                                            TxE_IRQ     <= 1'b0;                //clear error irq
                                            pipe_hold   <= 1'b0;                //clear pipe hold flag
            if(~TxEn)                       bd_index    <= 7'd0;                //clear the BD indexer
        end //state[ST_IDLE ]

        state[ST_BD_RD]: begin
            if(~bd_wait & bd_desc[BIT_READY]) begin
                                            tg_cnt      <= next_tg_cnt[15:2];   //load target count
                                            {ptr,desc}  <= {bd_ptr,bd_desc};    //load pointer and descriptor
            end
        end //state[ST_BD_RD]

        state[ST_DMA1 ]: begin
                                            first_valid_cnt <= (3'd3 - {1'b0,ptr[1:0]});
                                            last_valid_cnt  <= (desc_len[2:0] + {1'b0,ptr[1:0]}) - 3'd1;
                                            pipe_hold       <= dff_used_la > FIFO_THRESHOLD;  // 1 clock lag
            if(valid_tx_rd) begin 
                                            ptr[31:2]   <= ptr[31:2] + 30'd1;   //increment read address
                                            last_read_r <= last_read;           //set last read flag
                                            rd_cnt      <= next_rd_cnt;         //increment read count
            end
        end //state[ST_DMA1 ]

        state[ST_BD_WR]:
            if(~bd_wait) begin
                                            TxB_IRQ     <= desc[BIT_IRQ] & ~stat_error;
                                            TxE_IRQ     <= desc[BIT_IRQ] & stat_error;
                if(desc[BIT_WRAP] | (bd_index == max_tx_bd))
                                            bd_index    <= 7'd0;
                else                        bd_index    <= bd_index + 7'd1;
            end

    endcase
end


//***************************************************************************
//*************************** Data FIFO *************************************
assign  dff_clear       = reset | state[ST_IDLE];
assign  dff_write       = valid_rdata & ~stat_ready;
assign  dff_din[35]     = first_write;
assign  dff_din[34]     = last_write;
assign  dff_din[33:32]  = valid_cnt;
assign  dff_din[31:0]   = first_write? (rdata >> {ptr[1:0],3'd0}): rdata;

//We'll read from the FIFO everytime there is a word ready or if something
//happened to put us into the WAIT state. We do this to flush the FIFO so
//any pending incoming reads from the avalon bus will make it into the FIFO 
assign  dff_read        = tx_state[TX_WAIT] | (tx_state[TX_SEND] & last_byte & tx_ack); 
eth_avalon_dma_fifo #(  .DEPTH(RFD),
                        .WIDTH(36)) data_fifo(
    .aclr       (dff_clear              ),

    .wrclk      (clk                    ),
    .data       (dff_din                ),
    .wrreq      (dff_write              ),
    .wrfull     (dff_full               ),
    .wrusedw    (dff_wrused             ),

    .rdclk      (txclk                  ),
    .rdreq      (dff_read&~dff_empty    ),
    .rdempty    (dff_empty              ),
    .rdusedw    (                       ),
    .q          (dff_dout               )
); 

//Send stat_ready signal from TX to Avalon
eth_dc_reg stat_ready_dc_reg(
	.d      (tx_state[TX_WAIT] & tx_stat_valid_r),
	.inclk  (txclk              ),
	.outclk (clk                ),
	.reset  (reset              ),
	.q      (stat_ready         )
    );

//Send stat_ack signal from Avalon to TX
eth_dc_reg stat_ack_dc_reg(
	.d      (state[ST_IDLE] ),
	.inclk  (clk            ),
	.outclk (txclk          ),
	.reset  (reset          ),
	.q      (tx_stat_ack    )
    );

//Send tx_start signal from Avalon to TX
//We start sending the data when there is either
//words in the FIFO or when the DMA transfer
//has finished.
eth_dc_reg tx_start_dc_reg(
	.d      ((dff_wrused >= 8) | (state[ST_DMA2] & ~first_write) ),
	.inclk  (clk            ),
	.outclk (txclk          ),
	.reset  (reset          ),
	.q      (tx_start       )
);

//************************* End Data FIFO ***********************************
//***************************************************************************
            
//***************************************************************************
//************************* Streaming Interface *****************************
assign  stat_error  = |{stat[BIT_UR],stat[BIT_RL:BIT_CS]};


assign  tx_ff_data[0]   = dff_dout[7:0];
assign  tx_ff_data[1]   = dff_dout[15:8];
assign  tx_ff_data[2]   = dff_dout[23:16];
assign  tx_ff_data[3]   = dff_dout[31:24];
assign  byte_cnt        = dff_dout[33:32];
assign  last_byte       = (byte_index == byte_cnt);

assign  tx_data = tx_ff_data[byte_index];
assign  tx_dv   = tx_state[TX_SEND];
assign  tx_sop  = tx_state[TX_SEND] & !byte_index & dff_dout[35];
assign  tx_eop  = tx_state[TX_SEND] & last_byte & dff_dout[34];

assign  PerPacketPad    = desc[BIT_PAD];
assign  PerPacketCrc    = desc[BIT_CRC];
assign  TxUnderRun      = tx_state[TX_SEND] & tx_ack & dff_empty;

always @(posedge txclk or posedge reset)
    if(reset)                       tx_state            <= 3'd1;
    else begin
        /*default case-->*/         tx_state            <= 3'd0;
        case(1'b1) // synopsys parallel_case 
            tx_state[TX_IDLE]:// Wait for start
                if(tx_start)        tx_state[TX_SEND]   <= 1'b1;
                else                tx_state[TX_IDLE]   <= 1'b1;

            tx_state[TX_SEND]:// Send all data 
                if((last_byte & dff_dout[34] & tx_ack) | tx_stat_valid)
                                    tx_state[TX_WAIT]   <= 1'b1;
                else                tx_state[TX_SEND]   <= 1'b1;
                                

            tx_state[TX_WAIT]:// Wait for status
                if(tx_stat_ack)     tx_state[TX_IDLE]   <= 1'b1;
                else                tx_state[TX_WAIT]   <= 1'b1;

        endcase
    end

always @(posedge txclk) 
        if(tx_stat_valid)       stat    <= tx_stat;

always @(posedge txclk)
        if(tx_state[TX_IDLE])   tx_stat_valid_r <= 1'b0;
        else if(tx_stat_valid)  tx_stat_valid_r <= 1'b1;

always @(posedge txclk)
    case(1'b1)
        tx_state[TX_IDLE]: begin
                                            byte_index  <= 2'd0;
        end

        tx_state[TX_SEND]: begin
            if(tx_ack) begin
                if(last_byte)               byte_index  <= 2'd0;
                else                        byte_index  <= byte_index + 2'd1;
            end
        end
    endcase
//*********************** End Streaming Interface ***************************
//***************************************************************************

endmodule

