



/*###################################################################################
#                    SIGNAL TIMING FOR 800 x 480 TD043MTEA1 LCD                     #
###################################################################################*/

`define LCD_800x480_HSCAN_WIDTH 1056
`define LCD_800x480_VSCAN_DEPTH 525

`define LCD_800x480_H_BP 216
`define LCD_800x480_H_FP 40
`define LCD_800x480_V_BP 35
`define LCD_800x480_V_FP 10

`define LCD_800x480_HD_HIGH 1
`define LCD_800x480_HD_LOW 0
`define LCD_800x480_VD_HIGH 0
`define LCD_800x480_VD_LOW 0

`define COLOR_DEPTH_CODE_16 0
`define COLOR_DEPTH_CODE_24 1
`define COLOR_DEPTH_CODE_32 2


module tpo_lcd_controller
(
  // Global Signals
  clk,
  reset_n,

  // Avalon Slave
  slave_address,
  slave_readdata,
  slave_writedata,
  slave_write,
  slave_chipselect,
  slave_irq,

  // Avalon Master
  master_address,
  master_readdata,
  master_read,
  master_waitrequest,
  master_data_valid,
  master_byteenable,

  // LCD Panel Signals
  lcd_clk_in,
  RGB,
  NCLK,
  HD,
  VD,
  DEN,
  SERIAL_SDA,
  SERIAL_SCL,
  SERIAL_SCEN,
  LCD_RESET
);

  /*###################################################################################
  #                             COMPONENT PARAMETERS                                  #
  ###################################################################################*/
  parameter NUM_COLUMNS = 800;
  parameter NUM_ROWS = 480;
  parameter FIFO_DEPTH = 1024;
  parameter DMA_DATA_WIDTH = 32;

  localparam HSCAN_WIDTH = ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_HSCAN_WIDTH : 0;
  localparam VSCAN_DEPTH = ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_VSCAN_DEPTH : 0;
  localparam H_BP =        ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_H_BP : 0;
  localparam H_FP =        ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_H_FP : 0;
  localparam V_BP =        ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_V_BP : 0;
  localparam V_FP =        ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_V_FP : 0;

  localparam HD_HIGH =     ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_HD_HIGH : 0;
  localparam HD_LOW =      ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_HD_LOW : 0;
  localparam VD_HIGH =     ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_VD_HIGH : 0;
  localparam VD_LOW =      ( NUM_COLUMNS == 800 && NUM_ROWS == 480 ) ? `LCD_800x480_VD_LOW : 0;

  localparam FIFO_COUNTER_WIDTH = (log2(FIFO_DEPTH));
  localparam FIFO_WRITE_THRESHOLD = FIFO_DEPTH * 7 / 8;
  localparam FIFO_READ_THRESHOLD = FIFO_DEPTH / 2;
  localparam COLUMN_COUNTER_WIDTH = (log2(HSCAN_WIDTH));
  localparam ROW_COUNTER_WIDTH = (log2(VSCAN_DEPTH));

  localparam DMA_BYTE_EN_WIDTH = DMA_DATA_WIDTH / 8;

  function integer log2;
    input [31:0] value;
    for (log2=0; value>0; log2=log2+1)
      value = value>>1;
  endfunction


  /*###################################################################################
  #                                COMPONENT PORTS                                    #
  ###################################################################################*/
  // Global Signals
  input                          clk;
  input                          reset_n;

  // Avalon Slave
  input [  2: 0]                 slave_address;
  output[ 31: 0]                 slave_readdata;
  input [ 31: 0]                 slave_writedata;
  input                          slave_write;
  input                          slave_chipselect;
  output                         slave_irq;

  // Avalon Master
  output[ 31: 0]                 master_address;
  input [DMA_DATA_WIDTH-1: 0]    master_readdata;
  output                         master_read;
  input                          master_waitrequest;
  input                          master_data_valid;
  output[DMA_BYTE_EN_WIDTH-1: 0] master_byteenable;

  // LCD Panel Signals
  input                          lcd_clk_in;
  output[  7: 0]                 RGB;
  output                         NCLK;
  output                         HD;
  output                         VD;
  output                         DEN;
  inout                          SERIAL_SDA;
  output                         SERIAL_SCL;
  output                         SERIAL_SCEN;
  output                         LCD_RESET;


  /*###################################################################################
  #                         INTERNAL COMPONENT SIGNALS                                #
  ###################################################################################*/

  // Avalon Slave Signals
  reg                            irq;
  wire                           irq_enable;
  wire[ 31: 0]                   slave_control_reg;
  reg [ 30: 0]                   slave_control_reg_30_0;

  // FIFO Signals
  wire[DMA_DATA_WIDTH-1: 0]      fifo_data_in;
  wire[DMA_DATA_WIDTH-1: 0]      fifo_data_out;
  wire                           fifo_rdempty;
  wire                           fifo_read_req;
  wire                           fifo_write_clk;
  wire                           fifo_write_req;
  wire[FIFO_COUNTER_WIDTH-1 : 0] fifo_used;
  reg                            fifo_emptied;
  reg                            fifo_has_data;
  reg                            fifo_has_data_reg1;
  reg                            fifo_has_room;
  reg                            fifo_has_room_reg1;

  // DMA Signals
  wire[ 31: 0]                   address_counter;
  wire                           address_counter_incr;
  wire                           address_counter_sload;
  wire[ 29: 0]                   address_counter_load_value;
  wire[ 29: 0]                   address_counter_temp;
  reg [ 31: 0]                   dma_source_reg;
  reg [ 31: 0]                   last_dma_addr_reg;
  reg [ 31: 0]                   dma_modulus_reg;
  reg [ 31: 0]                   current_dma;

  // LCD Signals
  wire[COLUMN_COUNTER_WIDTH-1:0] column_counter;
  wire[ROW_COUNTER_WIDTH-1 : 0]  row_counter;
  reg                            lcd_start;
  wire                           lcd_clk_out_en;
  wire[  1: 0]                   lcd_clk_out_en_cntr;
  reg                            hd_delayed;
  wire[  1: 0]                   rgb_mux_count;
  wire[  7: 0]                   rgb_out;
  wire[  1: 0]                   rgb_mux_toggle;
  reg [  7: 0]                   R;
  reg [  7: 0]                   G;
  reg [  7: 0]                   B;
  reg                            DEN;
  reg                            HD;
  reg                            VD;
  wire[ 15: 0]                   lcd_16bit_out;
  wire[ 31: 0]                   lcd_32bit_out;

  // Serial Interface Signals
  reg [  4: 0]                   serial_bit_counter;
  reg                            serial_clk;
  wire[ 11: 0]                   serial_clk_div_counter;
  wire                           serial_clk_falling_edge;
  wire                           serial_clk_rising_edge;
  wire                           serial_start;
  wire                           serial_in_bit;
  reg [ 31: 0]                   serial_interface_control_reg;
  reg [ 15: 0]                   serial_shift_reg;
  reg                            serial_shift_in_progress;
  wire                           serial_busy_bit;
  reg                            serial_r_nw;

  // Other Control Signals
  reg                            go_bit;
  reg                            go_bit_lcd;
  reg                            go_bit_lcd_reg1;
  wire                           ctrl_reg_go_bit;
  wire                           empty_the_fifo;
  wire [1:0]                     color_depth;


  /*##############################################################
  #                     THIS IS THE DMA FIFO                     #
  ##############################################################*/

  dcfifo the_dcfifo
    (
      .aclr (!reset_n),
      .data (fifo_data_in),
      .q (fifo_data_out),
      .rdclk (lcd_clk_in),
      .rdempty (fifo_rdempty),
      .rdreq (fifo_read_req & lcd_clk_out_en),
      .wrclk (fifo_write_clk),
      .wrreq (fifo_write_req),
      .wrusedw (fifo_used)
    );
  defparam the_dcfifo.LPM_NUMWORDS = FIFO_DEPTH,
           the_dcfifo.LPM_SHOWAHEAD = "ON",
           the_dcfifo.LPM_WIDTH = DMA_DATA_WIDTH;

  assign fifo_data_in = master_readdata;
  assign fifo_write_clk = clk;

  // This is our signal that the fifo has fallen below the write threshold, and
  // should be written with some data as soon as possible.  We double register
  // it to avoid any possible clock-crossing metastability issues.
  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      fifo_has_room_reg1 <= 0;
    else if (1)
      fifo_has_room_reg1 <= fifo_used < FIFO_WRITE_THRESHOLD;

  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      fifo_has_room <= 0;
    else if (1)
      fifo_has_room <= fifo_has_room_reg1;

  // This is our signal that the fifo has reached the read threshold
  // and we can start reading out of it.  We double register it to
  // avoid any possible clock-crossing metastability issues.
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      fifo_has_data_reg1 <= 0;
    else if (lcd_clk_out_en)
      fifo_has_data_reg1 <= fifo_used > 512;

  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      fifo_has_data <= 0;
    else if (lcd_clk_out_en)
      fifo_has_data <= fifo_has_data_reg1;


  /*##############################################################
  #                      THIS IS THE DMA                         #
  ##############################################################*/

  lpm_counter dma_address_counter
    (
      .aclr (!reset_n),
      .clock (clk),
      .cnt_en (address_counter_incr),
      .data (address_counter_load_value),
      .q (address_counter_temp),
      .sload (address_counter_sload)
    );
  defparam dma_address_counter.LPM_WIDTH = 30;

  assign address_counter_incr = (master_read == 1) && (master_waitrequest == 0) && (go_bit == 1);
  assign address_counter_sload = (go_bit == 0) ||(address_counter_incr && (address_counter == last_dma_addr_reg));
  assign address_counter_load_value = ( DMA_DATA_WIDTH == 32 ) ? dma_source_reg[31 : 2] :
                                      ( DMA_DATA_WIDTH == 64 ) ? { 1'b0, dma_source_reg[31 : 3] } :
                                       0 ;
  assign address_counter = ( DMA_DATA_WIDTH == 32 ) ? {address_counter_temp[29:0], 2'b00} :
                           ( DMA_DATA_WIDTH == 64 ) ? {address_counter_temp[28:0], 3'b000} :
                            0 ;

  // Always full data-width reads.
  // Will automatically be truncated to the actual width of master_byteenable.
  assign master_byteenable = 'hFFFFFFFF;
//  assign master_byteenable = 8'b11111111;

  // Register the DMA end address too, since the sw driver can change the
  // dma_source_reg register at any time.
  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      last_dma_addr_reg <= 0;
    else if (address_counter_sload)
    	if( DMA_DATA_WIDTH == 32 )
    		last_dma_addr_reg <= dma_source_reg + dma_modulus_reg - 4;
    	else if( DMA_DATA_WIDTH == 64 )
    		last_dma_addr_reg <= dma_source_reg + dma_modulus_reg - 8;

  assign master_address = address_counter;
  assign master_read = fifo_has_room & go_bit;
  assign fifo_write_req = master_data_valid & go_bit;


  /*##############################################################
  #            THIS IS THE LCD_CLK_OUT_EN GENERATOR              #
  ##############################################################*/
  // Used to create a 1/3 speed clock for internal operation
  // Counter is also used to sequence the RGB output mux.
  lpm_counter hck_div_counter
    (
      .aclr (!reset_n),
      .clock (lcd_clk_in),
      .cnt_en (1),
      .q (lcd_clk_out_en_cntr)
    );
  defparam hck_div_counter.LPM_MODULUS = 3,
           hck_div_counter.LPM_WIDTH = 2;

  // This signal is used as an enable to all the registered
  // logic that runs at NCLK speed (lcd_clk_in / 3).  Allows us
  // to use the same clock signal for all the lcd logic
  assign lcd_clk_out_en = lcd_clk_out_en_cntr == 2;
  assign NCLK = lcd_clk_out_en_cntr[1];


  /*##############################################################
  #            THIS IS THE LCD COLUMN (PIXEL) COUNTER            #
  ##############################################################*/
  lpm_counter lcd_column_counter
    (
      .aclr (!reset_n),
      .clock (lcd_clk_in),
      .cnt_en (lcd_start & lcd_clk_out_en),
      .q (column_counter),
      .sclr (!lcd_start)
    );

  defparam lcd_column_counter.LPM_MODULUS = HSCAN_WIDTH,
           lcd_column_counter.LPM_WIDTH = COLUMN_COUNTER_WIDTH;


  /*##############################################################
  #               THIS IS THE LCD ROW COUNTER                    #
  ##############################################################*/
  lpm_counter lcd_row_counter
    (
      .aclr (!reset_n),
      .clock (lcd_clk_in),
      .cnt_en ((lcd_start & (column_counter == (HSCAN_WIDTH - 1))) & lcd_clk_out_en),
      .q (row_counter),
      .sclr (!lcd_start)
    );

  defparam lcd_row_counter.LPM_MODULUS = VSCAN_DEPTH,
           lcd_row_counter.LPM_WIDTH = ROW_COUNTER_WIDTH;


  /*##############################################################
  #                  THIS IS THE LCD INIT LOGIC                  #
  ##############################################################*/

  // We need to wait until everything's set up before we release
  // the fury of the go bit.
  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      go_bit <= 0;
    else if (1)
      go_bit <= ctrl_reg_go_bit & fifo_emptied;

  // This holds the fifo read_req signal high until the thing's empty.  Used for bring-up
  // and failure recovery.
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      fifo_emptied <= 0;
    else if (lcd_clk_out_en)
      fifo_emptied <= ctrl_reg_go_bit & (fifo_emptied | fifo_rdempty);

  assign empty_the_fifo = !fifo_emptied;

  // This is the Reset signal to the LCD panel.
  assign LCD_RESET = go_bit;


  /*##############################################################
  #              THIS IS THE LCD CONTROL LOGIC                   #
  ##############################################################*/

  // This is a version of the go bit in the control register, but here, it's
  // double-registered on the 25MHz clock to sync it up.  We'll use it in our
  // logic that determines when we're all set to start the LCD sequencer
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      go_bit_lcd_reg1 <= 0;
    else if (lcd_clk_out_en)
      go_bit_lcd_reg1 <= go_bit;

  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      go_bit_lcd <= 0;
    else if (lcd_clk_out_en)
      go_bit_lcd <= go_bit_lcd_reg1;

  // This signals when we're okay to start the LCD sequencer
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      lcd_start <= 0;
    else if (lcd_clk_out_en)
      lcd_start <= (lcd_start & go_bit_lcd) | (fifo_has_data & go_bit_lcd);

  // This is the data splitter mux.  We get data out of the fifo as 32-bit or 64-bit
  // words, depending on the DMA data width.  But we only need 16-bit or 32-bit words,
  // depending on the color depth.  So we split it out with a counter and a mux
  // and only do a real read of the fifo every (DMA_DATA_WIDTH / color_depth) cycles.
  assign lcd_16bit_out = ( DMA_DATA_WIDTH == 32 ) ? 
                           ( rgb_mux_toggle[0] ? fifo_data_out[31 : 16] : fifo_data_out[15 : 0]) :
                         ( DMA_DATA_WIDTH == 64 ) ?
												   (( rgb_mux_toggle == 3 ) ? fifo_data_out[63 : 48] : 
                            ( rgb_mux_toggle == 2 ) ? fifo_data_out[47 : 32] :
                            ( rgb_mux_toggle == 1 ) ? fifo_data_out[31 : 16] : 
                            ( rgb_mux_toggle == 0 ) ? fifo_data_out[15 : 0] : 0 ) :
                          0 ;

  assign lcd_32bit_out = ( DMA_DATA_WIDTH == 32 ) ? fifo_data_out[31 : 0] :
  											 ( DMA_DATA_WIDTH == 64 ) ? (( rgb_mux_toggle[0] ) ? fifo_data_out[63 : 32] : fifo_data_out[31 : 0]) :
  											  0 ;

  // The read request that actually goes to the fifo only fires off when we need it,
  // depending on the data width coming out of it, and the color depth that we need.  
  assign fifo_read_req = ( DMA_DATA_WIDTH == 32 ) ?
  											   (( color_depth == `COLOR_DEPTH_CODE_16 ) ? (( rgb_mux_toggle[0] & display_active ) | empty_the_fifo ) :
                            ( color_depth == `COLOR_DEPTH_CODE_32 ) ? ( display_active | empty_the_fifo) : 0 ) :
                         ( DMA_DATA_WIDTH == 64 ) ? 
                           (( color_depth == `COLOR_DEPTH_CODE_16 ) ? ((( rgb_mux_toggle == 3 ) & display_active ) | empty_the_fifo ) :
                            ( color_depth == `COLOR_DEPTH_CODE_32 ) ? (( rgb_mux_toggle[0] & display_active ) | empty_the_fifo) : 0 ) :
                          0 ;

  // This is the counter that drives the width translation logic above.
  lpm_counter mux_toggler
  (
    .aclr (!reset_n),
    .clock (lcd_clk_in),
    .cnt_en ( lcd_clk_out_en & display_active ),
    .q (rgb_mux_toggle),
  );
  defparam mux_toggler.LPM_WIDTH = 2;

  // Here, we issue the display active signal during the proper horizontal scan cycles
  //  We only do reads during the active rows, not during the "front porch".
  assign display_active = (column_counter >= (H_BP)) && (column_counter < ((H_BP) + (NUM_COLUMNS))) && (row_counter >= (V_BP)) && (row_counter < ((V_BP) + (NUM_ROWS)));

  // This splits up the full-width pixel data into RGB.
  // Red
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      R <= 0;
    else if (lcd_clk_out_en)
      if( color_depth == `COLOR_DEPTH_CODE_16 )
        R <= display_active ? ({lcd_16bit_out[15 : 11], 3'b111}) : 8'b00000000;
//        R <= display_active ? ({ lcd_16bit_out[4 : 0], 3'b111 }) : 8'b00000000;
      else if( color_depth == `COLOR_DEPTH_CODE_32 )
        R <= display_active ? ( lcd_32bit_out[23 : 16] ) : 8'b00000000;
//        R <= display_active ? ( lcd_32bit_out[7 : 0] ) : 8'b00000000;

  // Green
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      G <= 0;
    else if (lcd_clk_out_en)
      if( color_depth == `COLOR_DEPTH_CODE_16 )
        G <= display_active ? ({lcd_16bit_out[10 : 5], 2'b11}) : 8'b00000000;
      else if( color_depth == `COLOR_DEPTH_CODE_32 )
        G <= display_active ? ( lcd_32bit_out[15 : 8] ) : 8'b00000000;

  // Blue
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      B <= 0;
    else if (lcd_clk_out_en)
      if( color_depth == `COLOR_DEPTH_CODE_16 )
        B <= display_active ? ({lcd_16bit_out[4 : 0], 3'b111}) : 8'b00000000;
//        B <= display_active ? ({lcd_16bit_out[15 : 11], 3'b111}) : 8'b00000000;
      else if( color_depth == `COLOR_DEPTH_CODE_32 )
        B <= display_active ? ( lcd_32bit_out[7 : 0] ) : 8'b00000000;
//        B <= display_active ? ( lcd_32bit_out[23 : 16] ) : 8'b00000000;

  // Here we generate the sync signals to the LCD Panel
  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      DEN <= 0;
    else if (lcd_clk_out_en)
      DEN <= (column_counter >= (H_BP)) && (column_counter < ((H_BP) + (NUM_COLUMNS))) && (row_counter >= (V_BP)) && (row_counter < ((V_BP) + (NUM_ROWS)));

  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      HD <= 0;
    else if (lcd_clk_out_en)
      HD <= (column_counter >= (HD_HIGH)) || (column_counter < (HD_LOW));

  always @(posedge lcd_clk_in or negedge reset_n)
    if (reset_n == 0)
      VD <= 0;
    else if (lcd_clk_out_en)
      VD <= ((column_counter >= (VD_HIGH)) || (column_counter < (VD_LOW))) && (row_counter == 0);

  /*##############################################################
  #                THIS IS THE RGB OUTPUT MUX                    #
  ##############################################################*/

  assign rgb_out = ((lcd_clk_out_en_cntr == 0))? B :
                   ((lcd_clk_out_en_cntr == 1))? G :
                   ((lcd_clk_out_en_cntr == 2))? R :
                   8'b00000000;


  assign RGB = rgb_out;


  /*##############################################################
  #             THIS IS THE SLAVE READ INTERFACE                 #
  ##############################################################*/

  // This is the slave register read mux
  assign slave_readdata = ((slave_address == 0))? slave_control_reg :
    ((slave_address == 1))? dma_source_reg :
    ((slave_address == 2))? dma_modulus_reg :
    ((slave_address == 3))? current_dma :
    ((slave_address == 4))? serial_interface_control_reg :
    slave_control_reg;


  /*##############################################################
  #               THESE ARE THE SLAVE REGISTERS                  #
  ##############################################################*/

  // This is the control/status register
  //  31                                      3-2   1   0
  // ------------------------------------------------------
  // | I |                                  |  C  | E | G |
  // ------------------------------------------------------
  //
  // G (0)   - Go Bit, Enables initialization and operation of controller
  // E (1)   - IRQ Enable, enables interrupt requests to be issued
  // C (3-2) - Color mode ( b00 = 16-bit, b01 = 24-bit, b10 = 32-bit 
  // I (31)  - IRQ Status, shows IRQ status. Clearable by writing '0'

  assign slave_control_reg = {irq, slave_control_reg_30_0};

  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      slave_control_reg_30_0 <= 0;
    else if (slave_write && slave_chipselect && (slave_address == 0))
      slave_control_reg_30_0 <= slave_writedata[30 : 0];

  assign ctrl_reg_go_bit = slave_control_reg[0];
  assign irq_enable = slave_control_reg[1];
  assign color_depth = slave_control_reg[3:2];

  // This is the irq signaling we're ready for a new frame buffer address.
  // The interrupt triggers when the controller begins displaying the buffer pointed to by
  // dma_source_reg, freeing dma_source_reg to be written with a new frame buffer address.
  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      irq <= 0;
    else
      irq <= ( slave_write && slave_chipselect && (slave_address == 0)) ? slave_writedata[31] : ( address_counter_sload ? 1'b1 : irq );

  // slave_irq is the actual irq signal to the CPU.  Only enable it if the irq_enable
  //  bit's been set.
  assign slave_irq = irq & irq_enable;

  // This is the DMA address counter reload value for the next frame
  //  31                                                 0
  // ------------------------------------------------------
  // |          DMA Source Address Register               |
  // ------------------------------------------------------

  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      dma_source_reg <= 0;
    else if (slave_write && slave_chipselect && (slave_address == 1))
      dma_source_reg <= slave_writedata;

  // This is the end value of the DMA address counter.
  // (reload counter with dma_source_reg when count = dma_source_reg + dma_modulus_reg - 4)
  // This is the DMA address counter reload value for the next frame
  //  31                                                 0
  // ------------------------------------------------------
  // |               DMA Modulus Register                 |
  // ------------------------------------------------------
  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      dma_modulus_reg <= 0;
    else if (slave_write && slave_chipselect && (slave_address == 2))
      dma_modulus_reg <= slave_writedata;

  // This reg is the start address the DMA is currenlty transfering from.
  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      current_dma <= 0;
    else if (address_counter_sload)
      current_dma <= dma_source_reg;

  // This is the serial interface control register
  //  31                                        2   1   0
  // ------------------------------------------------------
  // |                                        | E | C | D |
  // ------------------------------------------------------
  // D (0)   - Serial Data
  // C (1)   - Serial Clock
  // E (2)   - Serial Enable

  always @(posedge clk or negedge reset_n)
    if (reset_n == 0)
      serial_interface_control_reg <= 0;
    else if (slave_write && slave_chipselect && (slave_address == 4))
      serial_interface_control_reg <= slave_writedata;
    else if (serial_start && serial_clk_falling_edge)
      serial_interface_control_reg <= serial_interface_control_reg & 32'hFFFEFFFF;

  // This is the serial chipselect pin.
  assign SERIAL_SCEN = serial_interface_control_reg[2];
  // This is the serial clock pin
  assign SERIAL_SCL = serial_interface_control_reg[1];
  // This is the bidirectional serial data pin
  assign SERIAL_SDA = serial_interface_control_reg[0];


  assign serial_start = serial_interface_control_reg[16];

endmodule
