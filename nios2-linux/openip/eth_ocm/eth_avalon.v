//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_avalon.v                                                ////
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
//      12/01/08 - Added proper support for aborted RX packets.
//                 This addresses an issue with the MAC not aborting
//                 promiscuous packets in non-promiscuous mode.
//

`include "eth_defines.v"
`include "timescale.v"

// Parameter DESC_COUNT is the number of descriptors to use.
// Parameter RX_FIFO_DEPTH is the Depth of the Receive FIFO (in bytes)
// Parameter TX_FIFO_DEPTH is the Depth of the Transmit FIFO (in bytes)
module eth_avalon   #(  parameter   DESC_COUNT      = 128,
                        parameter   RX_FIFO_DEPTH   = 4096,
                        parameter   TX_FIFO_DEPTH   = 128   ) (
    input                   av_reset,           //Asynchronous reset (Avalon side)
    input                   av_clk,             //Avalon clock

    //Avalon Control Port
     //inputs
    input                   av_cs,              //Avalon slave BD chipselect
    input                   av_write,           //Avalon slave write
    input                   av_read,            //Avalon slave read
    input       [7:0]       av_address,         //Avalon slave address
    input       [31:0]      av_writedata,       //Avalon slave writedata
      //outputs
    output      [31:0]      av_readdata,        //Avalon slave readdata
    output                  av_waitrequest_n,   //Avalon slave waitrequest

    //Avalon TX memory port
      //input
    input                   av_tx_waitrequest,  //Avalon TX master waitrequest
    input                   av_tx_readdatavalid,//Avalon TX master readdatavalid
    input       [31:0]      av_tx_readdata,     //Avalon TX master readdata
      //output
    output      [31:0]      av_tx_address,      //Avalon TX master address
    output                  av_tx_read,         //Avalon TX master read

    //Avalon RX memory port
      //inputs
    input                   av_rx_waitrequest,  //Avalon RX master waitrequest
      //outputs
    output      [31:0]      av_rx_address,      //Avalon RX master address
    output                  av_rx_write,        //Avalon RX master write
    output      [31:0]      av_rx_writedata,    //Avalon RX master writedata
    output      [3:0]       av_rx_byteenable,   //Avalon RX master byteenable

    // Rx Status signals
    input                   InvalidSymbol,      // Invalid symbol was received during reception in 100 Mbps mode
    input                   LatchedCrcError,    // CRC error
    input                   RxLateCollision,    // Late collision occured while receiving frame
    input                   ShortFrame,         // Frame shorter then the minimum size (r_MinFL) was received while small packets are enabled (r_RecSmall)
    input                   DribbleNibble,      // Extra nibble received
    input                   ReceivedPacketTooBig,// Received packet is bigger than r_MaxFL
    input        [15:0]     RxLength,           // Length of the incoming frame
    input                   LoadRxStatus,       // Rx status was loaded
    input                   ReceivedPacketGood, // Received packet's length and CRC are good
    input                   AddressMiss,        // When a packet is received AddressMiss status is written to the Rx BD
    input                   r_RxFlow,           /*TODO*/
    input                   r_PassAll,          /*TODO*/
    input                   ReceivedPauseFrm,   /*TODO*/
    
    // Tx Status signals
    input       [3:0]       RetryCntLatched,    // Latched Retry Counter
    input                   RetryLimit,         // Retry limit reached (Retry Max value + 1 attempts were made)
    input                   LateCollLatched,    // Late collision occured
    input                   DeferLatched,       // Defer indication (Frame was defered before sucessfully sent)
    output                  RstDeferLatched,    // Ack DeferLatched
    input                   CarrierSenseLost,   // Carrier Sense was lost during the frame transmission
    
    // Tx
    input                   MTxClk,             // Transmit clock (from PHY)
    input                   TxUsedData,         // Transmit packet used data (this is an ack)
    input                   TxRetry,            // Transmit packet retry
    input                   TxAbort,            // Transmit packet abort
    input                   TxDone,             // Transmission ended
    output                  TxStartFrm,         // Transmit packet start frame
    output                  TxEndFrm,           // Transmit packet end frame
    output      [7:0]       TxData,             // Transmit packet data byte
    output                  TxUnderRun,         // Transmit packet under-run
    output                  PerPacketCrcEn,     // Per packet crc enable
    output                  PerPacketPad,       // Per packet pading
    
    // Rx
    input                   MRxClk,             // Receive clock (from PHY)
    input       [7:0]       RxData,             // Received data byte
    input                   RxValid,            // Receive data valid
    input                   RxStartFrm,         // Receive start of frame
    input                   RxEndFrm,           // Receive end of frame
    input                   RxAbort,            // This signal is set when address doesn't match.
    output  reg             RxStatusWriteLatched_sync2, //indication back 
    
    //Register
    input                   r_TxEn,             // Transmit enable
    input                   r_RxEn,             // Receive enable
    input       [7:0]       r_TxBDNum,          // Receive buffer descriptor number
    
    // Interrupts
    output                  TxB_IRQ,            // Transmit successful IRQ
    output                  TxE_IRQ,            // Transmit error IRQ
    output                  RxB_IRQ,            // Receive successful IRQ
    output                  RxE_IRQ,            // Receive error IRQ
    output                  Busy_IRQ            // Receive busy IRQ
    
    
    // Bist
    `ifdef ETH_BIST
    ,
    input                   mbist_si_i,       // bist scan serial in
    output                  mbist_so_o,       // bist scan serial out
    input [`ETH_MBIST_CTRL_WIDTH - 1:0] mbist_ctrl_i    // bist chain shift control
    `endif
    );

//Some useful constant functions
`include "eth_avalon_functions.v"

localparam  Tp = 1;
//Determine the number
localparam  RDC = min(max(nextPow2(DESC_COUNT), 2), 128);   //Real descriptor count 
localparam  MAX_DESC = RDC - 1;     //highest index descriptor

//Avalon interface signals
wire            av_waitrequest;     // Avalon slave waitrequest
wire            av_bd_desc_cs;      // Avalon Descriptor RAM is selected
wire    [31:0]  av_desc_readdata;   // Avalon Descriptor readback data
wire            av_bd_ptr_cs;       // Avalon Pointer RAM is selected
wire    [31:0]  av_ptr_readdata;    // Avalon Pointer readback data
reg             av_read_r;          // Avalon read registered

//Descriptor interface signals
wire    [7:0]   max_rx_bd;          // Highest RX descriptor index
//RX BD interface
wire            rx_bd_wait;         // RX BD wait signal
wire            rx_bd_write;        // RX BD write signal
wire            rx_bd_read;         // RX BD read signal
reg             rx_bd_read_r;       // RX BD read registered (used for wait)
wire    [6:0]   rx_bd_index;        // RX BD descriptor or pointer index
wire    [31:0]  rx_bd_writedata;    // RX BD descriptor writeback data
//TX BD interface
wire            tx_bd_wait;         // TX BD wait signal
wire            tx_bd_write;        // TX BD write signal
wire            tx_bd_read;         // TX BD read signal
reg             tx_bd_read_r;       // TX BD read registered (used for wait)
wire    [6:0]   tx_bd_index;        // TX BD descriptor or pointer index
wire    [31:0]  tx_bd_writedata;    // TX BD descriptor writeback data
//Muxed BD interface
wire    [31:0]  bd_desc;            // Descriptor data from BD RAM 
wire    [31:0]  bd_ptr;             // Pointer data from BD RAM
wire            bd_write;           // Descriptor data write to BD RAM
wire    [6:0]   bd_index;           // Descriptor/Pointer index to BD RAM
wire    [31:0]  bd_writedata;       // Descriptor writeback data to BD RAM
reg             rx_txn_sel;         // 1 = MUX RX to BD RAM, 0 = MUX TX

//Receive side signals
reg             rx_reset;           // Reset Receive interface
wire    [8:0]   RxStatus;           // RX Status from MAC core
reg     [8:0]   RxStatus_r;         // RX Status latched

//Transmit side signals
reg             tx_reset;           // Reset Transmit interface
reg             Flop;               // Follow nomenclature from eth_wishbone
                                    // toggles to activate TxUsedData
reg             TxUnderRun_r;       // Underrun of TX FIFO 
reg             tx_retry;           // TX retry signal
wire            tx_stat_ack;        // TX status ack

`ifdef ETH_BIST
assign  mbist_so_o = mbist_si_i;
`endif

assign  av_waitrequest_n    = ~av_waitrequest;

//***************************************************************************
//************************** Descriptor Interface ***************************
// Avalon bus is in wait if:
// 1 - The BD RAM is not selected
// 2 - The Avalon BUS issued a read but data is not available yet
assign  av_waitrequest  = (~av_cs) | (av_read & ~av_read_r);

// Select pointer RAM or descriptor RAM based on lowest address bit
assign  av_bd_ptr_cs    = av_address[0];
assign  av_bd_desc_cs   = ~av_address[0];

// Mux Pointer or descriptor readback data to Avalon BUS
assign  av_readdata     = av_bd_ptr_cs? av_ptr_readdata: av_desc_readdata;

// Mux TX or RX address, data, and control signals
assign  bd_write    = rx_txn_sel ? rx_bd_write              : tx_bd_write;
assign  bd_writedata= rx_txn_sel ? rx_bd_writedata          : tx_bd_writedata;
assign  bd_index    = rx_txn_sel ? (rx_bd_index + r_TxBDNum) : tx_bd_index;

// Determine BD wait signal back to RX interface
assign  rx_bd_wait  = ~rx_txn_sel | (rx_txn_sel & rx_bd_read & ~rx_bd_read_r);
// Determine BD wait signal back to TX interface
assign  tx_bd_wait  = rx_txn_sel | (~rx_txn_sel & tx_bd_read & ~tx_bd_read_r);

// Register BD RAM read signals used to control wait
always @(posedge av_clk) begin
    av_read_r       <= av_read;
    rx_bd_read_r    <= rx_bd_read & rx_txn_sel;
    tx_bd_read_r    <= tx_bd_read & ~rx_txn_sel;
    if((rx_txn_sel & ~rx_bd_wait) | (~rx_txn_sel & ~tx_bd_wait))
        rx_txn_sel  <= ~rx_txn_sel;
end

// Pointer BD RAM
eth_avalon_bd_ram #(.DEPTH(RDC) ) ptr_ram(
    .clock      (av_clk                 ),
    // port A (Avalon interface)
    .wren_a     (av_write&av_bd_ptr_cs  ),
    .address_a  (av_address>>1          ),
    .data_a     (av_writedata           ),
    .q_a        (av_ptr_readdata        ),
    // port B (DMA interface)
    .wren_b     (1'b0                   ),
    .address_b  (bd_index               ),
    .data_b     (32'd0                  ),
    .q_b        (bd_ptr                 )
    );

// Descriptor BD RAM
eth_avalon_bd_ram #(.DEPTH(RDC) ) desc_ram(
    .clock      (av_clk                 ),
    // port A (Avalon interface)
    .wren_a     (av_write&av_bd_desc_cs ),
    .address_a  (av_address>>1          ),
    .data_a     (av_writedata           ),
    .q_a        (av_desc_readdata       ),
    // port B (DMA interface)
    .wren_b     (bd_write               ),
    .address_b  (bd_index               ),
    .data_b     (bd_writedata           ),
    .q_b        (bd_desc                )
    );
//************************ End Descriptor Interface *************************
//***************************************************************************

//***************************************************************************
//****************************** RX Interface *******************************

// Determine maximum RX descriptor index 
assign  max_rx_bd   = MAX_DESC - r_TxBDNum;
// Decode RX Status inputs
assign  RxStatus    = { ReceivedPauseFrm, 
                        AddressMiss         , 1'b0      , InvalidSymbol  , DribbleNibble,
                        ReceivedPacketTooBig, ShortFrame, LatchedCrcError, RxLateCollision};

//RX Interface is held in reset when not enabled
always @(posedge av_clk)
    rx_reset    <= av_reset | ~r_RxEn;

//RX Interface
eth_avalon_rxdma    #(.FIFO_DEPTH(RX_FIFO_DEPTH)    )   eth_rxdma_inst(
    .clk                (av_clk             ),  // Avalon clock
    .reset              (rx_reset           ),  // Avalon reset

    //Descriptor ram interface
    .max_rx_bd          (max_rx_bd          ),  // Highest index RX Descriptor
    .bd_desc            (bd_desc            ),  // Descriptor Control data input
    .bd_ptr             (bd_ptr             ),  // Descriptor pointer input
    .bd_wait            (rx_bd_wait         ),  // Descriptor RAM is busy

    .bd_write           (rx_bd_write        ),  // write control data to BD RAM
    .bd_read            (rx_bd_read         ),  // read control and pointer data from BD RAM
    .bd_index           (rx_bd_index        ),  // Which descriptor to read
    .bd_writedata       (rx_bd_writedata    ),  // Control data to be written to descriptor

    //Memory port interface
    .av_waitrequest     (av_rx_waitrequest  ),  // Memory port is busy

    .av_write           (av_rx_write        ),  // Memory port write
    .av_address         (av_rx_address      ),  // Memory port address
    .av_writedata       (av_rx_writedata    ),  // Memory port writedata
    .av_byteenable      (av_rx_byteenable   ),  // Memory port byteenable

    //Streaming interface
    .RxEn               (r_RxEn             ),  // Receive enable
    .rxclk              (MRxClk             ),  // Receive clock
    .rx_data            (RxData             ),  // input data
    .rx_dv              (RxValid            ),  // qualifies datain, startofpacket, and endofpacket
    .rx_err             (RxStatus_r         ),  // error bits
    .rx_sop             (RxStartFrm         ),  // start of data packet
    .rx_eop             (RxEndFrm | RxAbort ),  // end of data packet
    .rx_abort           (RxAbort            ),  // abort packet

    //Interrupt outputs
    .RxB_IRQ            (RxB_IRQ            ),  // Receive success IRQ
    .RxE_IRQ            (RxE_IRQ            ),  // Receive error IRQ
    .Busy_IRQ           (Busy_IRQ           )   // Receive busy IRQ
);

//Register RxStatus
always @(posedge MRxClk)
    if (LoadRxStatus)       
        RxStatus_r  <= RxStatus;

//We save the data and cross the clock domains in the RX DMA module so 
//there is no need for synchronization here. We do it to be compliant
//with Igor's implementaion
always @(posedge MRxClk)
    RxStatusWriteLatched_sync2  <= LoadRxStatus;

//***************************************************************************
//**************************** TX Interface *********************************

// TX interface is held in reset when not enabled
always @(posedge av_clk)
    tx_reset    <= av_reset | ~r_TxEn;

//TX Interface
eth_avalon_txdma   #(.FIFO_DEPTH(TX_FIFO_DEPTH)) eth_txdma_inst(
    //Common signals
    .clk                (av_clk             ),  // Avalon clock
    .reset              (tx_reset           ),  // Avalon reset

    //Descriptor ram interface
    .max_tx_bd          (r_TxBDNum - 7'd1   ),  // Highest index RX Descriptor
    .bd_desc            (bd_desc            ),  // Descriptor Control data input
    .bd_ptr             (bd_ptr             ),  // Descriptor pointer input
    .bd_wait            (tx_bd_wait         ),  // Descriptor RAM is busy

    .bd_write           (tx_bd_write        ),  // write control data to BD RAM
    .bd_read            (tx_bd_read         ),  // read control and pointer data from BD RAM
    .bd_index           (tx_bd_index        ),  // Which descriptor to read
    .bd_writedata       (tx_bd_writedata    ),  // Control data to be written to descriptor

    //Memory port interface
    .av_waitrequest     (av_tx_waitrequest  ),  // Memory port is busy
    .av_readdata        (av_tx_readdata     ),  // Memory port readdata
    .av_readdatavalid   (av_tx_readdatavalid),  // Memory port readdata valid signal

    .av_read            (av_tx_read         ),  // Memory port read
    .av_address         (av_tx_address      ),  // Memory port address

    //Streaming interface
    .TxEn               (r_TxEn             ),  // Enable transmit
    .txclk              (MTxClk             ),  // Transmit clock
    .tx_data            (TxData             ),  // output data
    .tx_dv              (                   ),  // qualifies dataout, startofpacket, and endofpacket
    .tx_sop             (TxStartFrm         ),  // start of data packet
    .tx_eop             (TxEndFrm           ),  // end of data packet
    .tx_ack             (TxUsedData & Flop  ),  // Acknowledge TX data
    .tx_stat            ({TxUnderRun_r,
                            RetryCntLatched,
                            RetryLimit,
                            LateCollLatched,
                            DeferLatched,
                            CarrierSenseLost}), // Status bits
    .tx_stat_valid      (TxRetry|TxAbort|TxDone),// Status is valid
    .tx_stat_ack        (tx_stat_ack        ),
    .tx_retry           (tx_retry           ),

    .PerPacketPad       (PerPacketPad       ),
    .PerPacketCrc       (PerPacketCrcEn     ),
    .TxUnderRun         (TxUnderRun         ),

    //Interrupt outputs
    .TxB_IRQ            (TxB_IRQ            ),  // TX success IRQ
    .TxE_IRQ            (TxE_IRQ            )   // TX error IRQ
    );

//Send Reset defered latch back to core
assign  RstDeferLatched = (TxRetry|TxAbort|TxDone);

//The Flop signal is used to toggle valid TxUsedData
//because the core only uses the data on every other
//clock (8 bits local interface vs. 4 bits MII interface)
always @(posedge MTxClk or posedge tx_reset)
    if(tx_reset)            Flop            <= 1'b0;
    else if(TxDone|TxAbort|TxRetry)
                            Flop            <= 1'b0;
    else if(TxUsedData)
                            Flop            <= ~Flop;

always @(posedge MTxClk or posedge tx_reset)
    if(tx_reset)            TxUnderRun_r    <= 1'b0;
    else begin
        if(tx_stat_ack)     TxUnderRun_r    <= 1'b0;
        if(TxUnderRun)      TxUnderRun_r    <= 1'b1;
    end

always @(posedge MTxClk or posedge tx_reset)
    if(tx_reset)            tx_retry        <= 1'b0;
    else if(TxRetry)        tx_retry        <= 1'b1;
    else if(tx_stat_ack)    tx_retry        <= 1'b0;



endmodule
