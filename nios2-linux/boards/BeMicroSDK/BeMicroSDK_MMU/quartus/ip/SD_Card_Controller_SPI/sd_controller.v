/*****************************************************************************
*  File:    sd_controller.v
*
*  Purpose: Verilog code that implements an SPI-mode SD Card controller
*           with an Avalon Memory Mapped interface.  SOPC Builder compatible.
*
*  Author: NGK
*
*****************************************************************************/
module sd_controller
(
  // Avalon Slave
  clk,
  reset_n,
  avalon_slave_address,
  avalon_slave_readdata,
  avalon_slave_writedata,
  avalon_slave_write,
  avalon_slave_read,
  avalon_slave_chipselect,
  avalon_slave_byteenable,

  // SPI Exports
  spi_data_out,
  spi_data_in,
  spi_cs_n,
  spi_clk
);

// Avalon Slave
input clk;
input reset_n;
input [7:0] avalon_slave_address;
input [3:0] avalon_slave_byteenable;
input avalon_slave_chipselect;
input avalon_slave_write;
input [31:0] avalon_slave_writedata;
output [31:0] avalon_slave_readdata;
input avalon_slave_read;

// SPI Exports
output spi_data_out;
input spi_data_in;
output spi_cs_n;
output spi_clk;

reg [31:0] control_status_reg;
reg [47:0] command_reg;
reg [31:0] avalon_slave_readdata_reg;

reg [39:0] cmd_response_register;
reg [7:0]  data_response_register;

reg [15:0] bit_counter;
reg [7:0] clock_divider_counter;
reg this_is_a_write_command;

reg spi_clk;
reg spi_cs_n;
reg spi_data_out;

reg [2:0] data_bit_counter;
reg [31:0] data_shift_reg;
reg [15:0] crc16_shift_reg;

reg [3:0] current_state;
reg [3:0] last_state;
reg [3:0] state_on_last_spi_clk;

parameter IDLE =                 0,
          SHIFT_OUT_CMD =        1,
          WAIT_FOR_RSP_START =   2,
          SHIFT_IN_CMD_RSP =     3,
          WAIT_FOR_DATA_START =  4,
          SHIFT_IN_DATA =        5,
          SHIFT_IN_CRC16 =       6,
          SHIFT_OUT_DATA_TOKEN = 7,
          SHIFT_OUT_DATA =       8,
          SHIFT_OUT_CRC16 =      9,
          SHIFT_IN_DATA_RSP =   10,
          DATA_BUSY =           11,
          ERROR_STATE =         12,
          WAIT =                13;


wire go_bit;
wire sync_reset_bit;
wire ready_bit;
wire error_bit;
wire [15:0] resp_length;
wire [15:0] data_length;
wire [7:0] clock_divide_value;

wire falling_edge_spi_clk;
wire rising_edge_spi_clk;

wire [6:0] data_ram_addr;
wire [31:0] data_ram_data_in;
wire [31:0] data_ram_data_out;
wire data_ram_wren;
wire data_ram_rden;
wire [3:0]data_ram_byte_en;
wire ram_read;

wire ram_selected_by_avalon;
wire bit_counter_is_zero;
wire state_just_changed;

wire [31:0] avalon_slave_readdata;

reg [8:0] data_ram_addr_counter;


`define DATA_BLOCK_BITS 4096
`define CRC16_BITS      16
`define COMMAND_BITS    47
`define DATA_RESPONSE_BITS 7

`define DATA_TOKEN_LENGTH 16

`define SD_WRITE_BLOCK_COMMAND 8'h58

`define MAX_TIMEOUT 16'hFFFF

`define WAIT_TIME 8


// SD timing values
`define NAC      8        // Data access time (min)
`define NAC_MAX  16'hFFFF // Data access time (max) (most we can represent in 16 bits)
`define NRC      8        // Response-to-command time (min)
`define WR       8        // Response-to-write time (min)

/****************************************************************************
*                         Avalon slave register map                         *
*****************************************************************************

Offset Name             Function
                        |31 ...... 24|23 ....... 16|15 ....... 8|7 ....... 0|
=============================================================================
 0x0   Control/Status   | DATA LENGTH| RESP LENGTH | CLK DIVIDE |ERR|DONE|GO|
------------------------+------------+-------------+------------+-----------+
 0x4   Command / CRC    |                          |  CMD CRC   |  COMMAND  |
------------------------+------------+-------------+------------+-----------+
 0x8   Command Arg      |             COMMAND_ARGUMENT[31:0]                |
------------------------+------------+-------------+------------+-----------+
 0xC   Command Resp0    |                  RESP[31:0]                       |
------------------------+------------+-------------+------------+-----------+
 0x10  Command Resp1    |         CRC16            |  DATA_RESP |RESP[39:32]|
------------------------+------------+-------------+------------+-----------+


****************************************************************************/


/****************************************************************************
*                     Avalon slave register write logic                     *
****************************************************************************/

/*****************************************************************************/
// Control/Status Register - Read/Writeable by Avalon master
//  Offset = 0x0
/*****************************************************************************/

always @(posedge clk or negedge reset_n)
  if (~reset_n)
    control_status_reg[7:0] <= 0;
  else if ( sync_reset_bit )
    control_status_reg[7:0] <= 8'h04; // Clear the entire register except set the ready bit
  else if ( avalon_slave_chipselect & avalon_slave_write & 
          ( avalon_slave_address == 0 ) & avalon_slave_byteenable[0] )
    control_status_reg[7:0] <= avalon_slave_writedata[7:0];
  else if ( current_state == IDLE && last_state != IDLE )
    begin
      control_status_reg[2]   <= 1;  // Set the ready_bit
      control_status_reg[0]   <= 0;  // Zero the go bit
    end
  else if ( current_state != IDLE && last_state == IDLE )
    control_status_reg[2] <= 0; // Clear the ready_bit
  else if ( current_state == ERROR_STATE )
    control_status_reg[3] <= 1; // Set the error bit
  else 
    control_status_reg[7:0] <= control_status_reg[7:0];
    
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    control_status_reg[15:8] <= 0;
  else if ( sync_reset_bit )
    control_status_reg[15:8] <= 8'h00; // Clear the entire register except set the ready bit
  else if ( avalon_slave_chipselect & avalon_slave_write & 
          ( avalon_slave_address == 0 ) & avalon_slave_byteenable[1] )
    control_status_reg[15:8] <= avalon_slave_writedata[15:8];
  else
    control_status_reg[15:8] <= control_status_reg[15:8];

always @(posedge clk or negedge reset_n)
  if (~reset_n)
    control_status_reg[23:16] <= 0;
  else if ( sync_reset_bit )
    control_status_reg[23:16] <= 8'h00; // Clear the entire register except set the ready bit
  else if ( avalon_slave_chipselect & avalon_slave_write & 
          ( avalon_slave_address == 0 ) & avalon_slave_byteenable[2] )
    control_status_reg[23:16] <= avalon_slave_writedata[23:16];
  else
    control_status_reg[23:16] <= control_status_reg[23:16];

always @(posedge clk or negedge reset_n)
  if (~reset_n)
    control_status_reg[31:24] <= 0;
  else if ( sync_reset_bit )
    control_status_reg[31:24] <= 8'h00; // Clear the entire register except set the ready bit
  else if ( avalon_slave_chipselect & avalon_slave_write & 
          ( avalon_slave_address == 0 ) & avalon_slave_byteenable[3] )
    control_status_reg[31:24] <= avalon_slave_writedata[31:24];
  else
    control_status_reg[31:24] <= control_status_reg[31:24];

assign go_bit =             control_status_reg[0];
assign sync_reset_bit =     control_status_reg[1];
assign ready_bit =          control_status_reg[2];
assign error_bit =          control_status_reg[3];
assign resp_length =        { 8'h00, control_status_reg[23:16] };
assign data_length =        { 8'h00, control_status_reg[31:24] };
assign clock_divide_value = control_status_reg[15:8];


/****************************************************************************/
// Command Register - Writable by Avalon master, shifts commands out via SPI
//  Occupies 2 offsets, 0x4 and 0x8
/****************************************************************************/
// command_reg[47:40]
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    command_reg[47:40] <= 0;
  else if ( sync_reset_bit )
    command_reg[47:40] <= 0;       // Clear the entire register
  else if ( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 1 ) && avalon_slave_byteenable[0] )
    command_reg[47:40] <= avalon_slave_writedata[7:0]; // command
  else if ( current_state == SHIFT_OUT_CMD && falling_edge_spi_clk )
    command_reg[47:40] <= command_reg[46:39];
  else
    command_reg[47:40] <= command_reg[47:40];
  
// command_reg[7:0]
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    command_reg[7:0] <= 0;
  else if ( sync_reset_bit )
    command_reg[7:0] <= 0;       // Clear the entire register
  else if ( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 1 ) && avalon_slave_byteenable[1] )
    command_reg[7:0] <= avalon_slave_writedata[15:8]; // command
  else if ( current_state == SHIFT_OUT_CMD && falling_edge_spi_clk )
    command_reg[7:0] <= command_reg[7:0] << 1;
  else 
    command_reg[7:0] <= command_reg[7:0];

// command_reg[15:8]
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    command_reg[15:8] <= 0;
  else if ( sync_reset_bit )
    command_reg[15:8] <= 0;       // Clear the entire register
  else if ( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 2 ) && avalon_slave_byteenable[0] )
    command_reg[15:8] <= avalon_slave_writedata[7:0];  // command argument[7:0]
  else if ( current_state == SHIFT_OUT_CMD && falling_edge_spi_clk )
    command_reg[15:8] <= command_reg[14:7];
  else
    command_reg[15:8] <= command_reg[15:8];

// command_reg[23:16]
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    command_reg[23:16] <= 0;
  else if ( sync_reset_bit )
    command_reg[23:16] <= 0;       // Clear the entire register
  else if ( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 2 ) && avalon_slave_byteenable[1] )
    command_reg[23:16] <= avalon_slave_writedata[15:8];  // command argument[15:8]
  else if ( current_state == SHIFT_OUT_CMD && falling_edge_spi_clk )
    command_reg[23:16] <= command_reg[22:15];
  else
    command_reg[23:16] <= command_reg[23:16];
      
// command_reg[[31:24]
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    command_reg[31:24] <= 0;
  else if ( sync_reset_bit )
    command_reg[31:24] <= 0;       // Clear the entire register
  else if ( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 2 ) && avalon_slave_byteenable[2] )
    command_reg[31:24] <= avalon_slave_writedata[23:16];  // command argument[23:16]
  else if ( current_state == SHIFT_OUT_CMD && falling_edge_spi_clk )
    command_reg[31:24] <= command_reg[30:23];
  else
    command_reg[31:24] <= command_reg[31:24];

// command_reg[[39:32]
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    command_reg[39:32] <= 0;
  else if ( sync_reset_bit )
    command_reg[39:32] <= 0;       // Clear the entire register
  else if ( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 2 ) && avalon_slave_byteenable[3] )
    command_reg[39:32] <= avalon_slave_writedata[31:24];  // command argument[31:24]
  else if ( current_state == SHIFT_OUT_CMD && falling_edge_spi_clk )
    command_reg[39:32] <= command_reg[38:31];
  else
    command_reg[39:32] <= command_reg[39:32];


// This register affects the sequence of the main state machine.  It indicates that 
// we're executing a data write command.
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    this_is_a_write_command <= 0;
  else if ( sync_reset_bit )
    this_is_a_write_command <= 0;
  else if ( current_state == IDLE )
    if ( command_reg[47:40] == `SD_WRITE_BLOCK_COMMAND )
      this_is_a_write_command <= 1;
    else
      this_is_a_write_command <= 0;

/****************************************************************************
*                   Avalon slave register read logic                        *
****************************************************************************/
always @(posedge clk or negedge reset_n)
  begin
    if( ~reset_n )
      avalon_slave_readdata_reg <= 0;
    else if( avalon_slave_chipselect && avalon_slave_read )
      if( avalon_slave_address >= 8'h80 )
        avalon_slave_readdata_reg <= data_ram_data_out;
      else
        case( avalon_slave_address )
          8'h0:  // Control / Status Register
            avalon_slave_readdata_reg <= control_status_reg;
          8'h1:  // Command CRC and Command Value
            avalon_slave_readdata_reg <= { 16'h0000, command_reg[7:0], command_reg[47:40] };
          8'h2:  // Command Argument
            avalon_slave_readdata_reg <= command_reg[39:8];
          8'h3:  // Command Response [31:0]
            avalon_slave_readdata_reg <= cmd_response_register[31:0];
          8'h4:  // Command Response [39:32]
            avalon_slave_readdata_reg <= { crc16_shift_reg, data_response_register, cmd_response_register[39:32] };
          default:
            avalon_slave_readdata_reg <= 32'h0;
        endcase
    else
      avalon_slave_readdata_reg <= avalon_slave_readdata_reg;
  end

// In order to make sure that readdata responds fast enough, we immediatly mux out 
// the data ram output anytime we detect a read of the data ram.  The registered
// version of readdata catches up a clock cycle later.
assign ram_read = ( avalon_slave_chipselect && avalon_slave_read && ( avalon_slave_address >= 8'h80 ));
assign avalon_slave_readdata = ram_read ?  data_ram_data_out : avalon_slave_readdata_reg;


/****************************************************************************
*                              Main FSM Logic                               *
****************************************************************************/

always @(posedge clk or negedge reset_n)
  begin
    if( ~reset_n )
      current_state = IDLE;
    else if( sync_reset_bit )
      current_state <= IDLE;
    else if ( rising_edge_spi_clk )
      case (current_state)
        IDLE:
          if( go_bit && ready_bit && !error_bit )
            current_state <= SHIFT_OUT_CMD;
          else
            current_state <= IDLE;
        SHIFT_OUT_CMD:
          if ( bit_counter_is_zero )
            current_state <= WAIT_FOR_RSP_START;
          else
            current_state <= SHIFT_OUT_CMD;
        WAIT_FOR_RSP_START:
          if( spi_data_in == 0 )
            current_state <= SHIFT_IN_CMD_RSP;
          else if( bit_counter_is_zero )
            current_state <= ERROR_STATE;
          else
          current_state <= WAIT_FOR_RSP_START;
        SHIFT_IN_CMD_RSP:
          if( bit_counter_is_zero )
            if( data_length == 0 )
              current_state <= WAIT;
            else if( this_is_a_write_command )
              current_state <= SHIFT_OUT_DATA_TOKEN;
            else
              current_state <= WAIT_FOR_DATA_START;
          else
            current_state <= SHIFT_IN_CMD_RSP;
        WAIT_FOR_DATA_START:
          if( spi_data_in == 0 )
            current_state <= SHIFT_IN_DATA;
          else if( bit_counter_is_zero )
            current_state <= ERROR_STATE;
          else
            current_state <= WAIT_FOR_DATA_START;
        SHIFT_IN_DATA:
          if( bit_counter_is_zero )
            current_state <= SHIFT_IN_CRC16;
          else
            current_state <= SHIFT_IN_DATA;
        SHIFT_IN_CRC16:
          if( bit_counter_is_zero )
            current_state <= WAIT;
          else
            current_state <= SHIFT_IN_CRC16;
        SHIFT_OUT_DATA_TOKEN:
          if( bit_counter_is_zero )
            current_state <= SHIFT_OUT_DATA;
          else
            current_state <= SHIFT_OUT_DATA_TOKEN;
        SHIFT_OUT_DATA:
          if( bit_counter_is_zero )
            current_state <= SHIFT_OUT_CRC16;
          else
            current_state <= SHIFT_OUT_DATA;
        SHIFT_OUT_CRC16:
          if( bit_counter_is_zero )
            current_state <= SHIFT_IN_DATA_RSP;
          else
            current_state <= SHIFT_OUT_CRC16;          
        SHIFT_IN_DATA_RSP:
          if( bit_counter_is_zero )
            current_state <= DATA_BUSY;
          else
            current_state <= SHIFT_IN_DATA_RSP;
        DATA_BUSY:
          if( spi_data_in == 1 )
            current_state <= WAIT;
          else
            current_state <= DATA_BUSY;
        ERROR_STATE:
          current_state <= IDLE;
        WAIT:
          if( bit_counter_is_zero )
            current_state <= IDLE;
          else
            current_state <= WAIT;
        default:
          current_state <= IDLE;
      endcase
    else
      current_state <= current_state;    
  end

// Keep track of state transitions
always @( posedge clk )
  last_state <= current_state;

always @( posedge clk )
  if( rising_edge_spi_clk )
    state_on_last_spi_clk = current_state;

/****************************************************************************
*                            SPI Bit Counter                                *
****************************************************************************/

// Counts SPI cycles for commands, responses and wait times
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    bit_counter <= 0;
  else if ( sync_reset_bit )
    bit_counter <= 0;
  else if ( state_just_changed )
    case (current_state)
      SHIFT_OUT_CMD:
        bit_counter <= `COMMAND_BITS;
      SHIFT_IN_CMD_RSP:
        bit_counter <= resp_length - 2;
      WAIT_FOR_DATA_START:
        bit_counter <= `MAX_TIMEOUT;
      SHIFT_IN_DATA:
        bit_counter <= ( data_length << 5 );  // mult by 32 to convert data_length from words to bits
      SHIFT_IN_CRC16:
        bit_counter <= ( `CRC16_BITS - 2 );
      DATA_BUSY:
        bit_counter <= `MAX_TIMEOUT;
      WAIT_FOR_RSP_START:
        bit_counter <= `MAX_TIMEOUT;
      SHIFT_OUT_DATA:
        bit_counter <= (( data_length << 5 ) - 1 );
      SHIFT_OUT_DATA_TOKEN:
        bit_counter <= ( `DATA_TOKEN_LENGTH - 1 );  
      SHIFT_OUT_CRC16:
        bit_counter <= ( `CRC16_BITS - 1 );
      WAIT:
        bit_counter <= 8;
      SHIFT_IN_DATA_RSP:
        bit_counter <= 7;
      default:
        bit_counter <= bit_counter;
    endcase
  else if( !bit_counter_is_zero )
    if(( current_state == SHIFT_OUT_CMD ) && falling_edge_spi_clk )
      bit_counter <= bit_counter - 1;
    else if(( current_state != SHIFT_OUT_CMD ) && rising_edge_spi_clk )
      bit_counter <= bit_counter - 1;
  else
    bit_counter <= bit_counter;

assign bit_counter_is_zero = ( bit_counter == 0 );
assign state_just_changed = ( current_state != last_state );

/****************************************************************************
*                               SPI Signals                                 *
****************************************************************************/

// Chip Select - active(low) whenever FSM is not in IDLE state
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    spi_cs_n <= 1;
  else if( falling_edge_spi_clk )  // An output, so we register it on the falling edge of spi_clk
    if( current_state == IDLE)
      spi_cs_n <= 1;
    else
      spi_cs_n <= 0;
  else
    spi_cs_n <= spi_cs_n;


// Data Out - Sends the MSB of the command register, '1' otherwise.
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    spi_data_out <= 1;
  else if( falling_edge_spi_clk )  // An output, so we register it on the falling edge of spi_clk
    if( current_state == SHIFT_OUT_CMD )
      spi_data_out <= command_reg[47];
    else if( current_state == SHIFT_OUT_DATA_TOKEN )
      if( bit_counter_is_zero )
        spi_data_out <= 0;
      else
        spi_data_out <= 1;
    else if( current_state == SHIFT_OUT_DATA )
      spi_data_out <= data_shift_reg[31];
    else if( current_state == SHIFT_OUT_CRC16 )
      spi_data_out <= crc16_shift_reg[15];
    else
      spi_data_out <= 1;
  else
    spi_data_out <= spi_data_out;


// Data In - Used as input to lots of logic in this controller
//   Search for signal spi_data_in

// SPI Clock - All SPI actions occur in sync with spi_clk
//   Driven by this divide circuit - divides input clock by
//   clock_divide_value register

// clock_divider_counter
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    clock_divider_counter <= 0;
  else if ( clock_divide_value != 0 )
    if ( clock_divider_counter < ( clock_divide_value - 1 ))
      clock_divider_counter <= clock_divider_counter + 1;
    else
      clock_divider_counter <= 0;
  else
    clock_divider_counter <= clock_divider_counter;

// spi_clk output
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    spi_clk <= 0;
  else if( clock_divider_counter == (( clock_divide_value - 1 ) >> 1 ))
    spi_clk <= 0;
  else if( clock_divider_counter == ( clock_divide_value - 1 ))
    spi_clk <= 1;
  else
    spi_clk <= spi_clk;

// All state machine transitions and input samples occur on the rising edge
// of the spi_clk, so we create an enable which mimics it.
assign rising_edge_spi_clk = ( clock_divider_counter == ( clock_divide_value - 1 ));

// However SPI output actions occur on the falling edge of the spi_clk, so we
// need an enable for that event too.
assign falling_edge_spi_clk = ( clock_divider_counter == (( clock_divide_value - 1 ) >> 1 ));


/****************************************************************************
*                           SD Response Logic                               *
****************************************************************************/

// Command Response Register
//  Shifts a command response in from the SD Card. response can be up to 
//  5 bytes, determined by 'resp_length'
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    cmd_response_register <= 0;
  else if ( sync_reset_bit )
    cmd_response_register <= 0;
  else if ( current_state == WAIT_FOR_RSP_START && last_state == IDLE )
    cmd_response_register <= 0;
  else if ( rising_edge_spi_clk && ( current_state == SHIFT_IN_CMD_RSP || ( current_state == WAIT_FOR_RSP_START && ~spi_data_in )))
    cmd_response_register <= { cmd_response_register [38:0], spi_data_in };
  else
    cmd_response_register <= cmd_response_register;

// Data Response Register
//  Shifts a data response in from the SD Card. response is always 8 bits
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    data_response_register <= 0;
  else if ( sync_reset_bit )
    data_response_register <= 0;
  else if ( current_state == WAIT_FOR_DATA_START && last_state == SHIFT_IN_CMD_RSP )
    data_response_register <= 0;
  else if ( rising_edge_spi_clk && ( current_state == SHIFT_IN_DATA_RSP || (( current_state == SHIFT_OUT_CRC16 ) && bit_counter_is_zero )))
    data_response_register <= { data_response_register [6:0], spi_data_in };
  else
    data_response_register <= data_response_register;

/****************************************************************************
*                          SD Data Block Buffer                             *
****************************************************************************/

// Data ram for 512 bytes of data to and from the SD card.
//  Occupies address offset 0x200 - 0x3FF
sd_data_ram the_sd_data_ram (
  .address (data_ram_addr),
  .clock   (clk),
  .data    (data_ram_data_in),
  .wren    (data_ram_wren),
  .byteena (data_ram_byte_en),
  .q       (data_ram_data_out) );


/****************************************************************************
*                       Data Buffer Write Control                           *
****************************************************************************/

// Data response capture and data output logic
//  Shifts in a data response from the SD Card.  Data response is always 512 bytes.
//  Each time a full byte is shifted in, the byte is stuffed into the sd_data_ram to
//   be read back later by the processor
//  Also shifts data out during data write commands.

// data_shift_reg logic
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    data_shift_reg <= 0;
  else if ( current_state == IDLE )
    data_shift_reg <= 0;
  else if ( rising_edge_spi_clk && ( current_state == SHIFT_IN_DATA ))
    data_shift_reg <= { data_shift_reg [30:0], spi_data_in };
  else if ( data_ram_rden )
    data_shift_reg <= { data_ram_data_out[7:0], data_ram_data_out[15:8], data_ram_data_out[23:16], data_ram_data_out[31:24] };
  else if ( falling_edge_spi_clk && ( current_state == SHIFT_OUT_DATA ))
    data_shift_reg <= { data_shift_reg [30:0], spi_data_in };
  else
    data_shift_reg <= data_shift_reg;

// Account for little-endian
assign data_ram_data_in = ( current_state == SHIFT_IN_DATA ) ? 
                          { data_shift_reg[7:0], data_shift_reg[15:8], data_shift_reg[23:16], data_shift_reg[31:24] } :
                          avalon_slave_writedata;

// data_ram_addr_counter logic
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    data_ram_addr_counter <= 0;
  else if( current_state == IDLE )
    data_ram_addr_counter <= 0;
  else if( data_ram_wren || data_ram_rden )
    data_ram_addr_counter <= data_ram_addr_counter + 1;
  else
    data_ram_addr_counter <= data_ram_addr_counter;
    
assign data_ram_byte_en = ram_selected_by_avalon ? avalon_slave_byteenable : 4'b1111;

assign data_ram_rden = (( falling_edge_spi_clk ) &&
                       ((( current_state == SHIFT_OUT_DATA ) && ( state_on_last_spi_clk != SHIFT_OUT_DATA_TOKEN ) && ( bit_counter[4:0] == 5'h0 )) ||
                        (( current_state == SHIFT_OUT_DATA_TOKEN ) && ( bit_counter[4:0] == 5'h0 ))));

assign data_ram_wren = (( rising_edge_spi_clk  ) && 
                        ( current_state         == SHIFT_IN_DATA ) &&
                        ( state_on_last_spi_clk != WAIT_FOR_DATA_START ) &&                        
                        ( bit_counter[4:0]      == 5'h0 )) ||
                        ( ram_selected_by_avalon && avalon_slave_write );

assign ram_selected_by_avalon = ( avalon_slave_chipselect && ( avalon_slave_address >= 8'h80 ));
assign data_ram_addr = ram_selected_by_avalon ? avalon_slave_address[6:0] : data_ram_addr_counter[6:0];

// crc16_shift_reg logic
always @(posedge clk or negedge reset_n)
  if (~reset_n)
    crc16_shift_reg <= 0;
  else if ( sync_reset_bit )
    crc16_shift_reg <= 0;
  else if (( avalon_slave_chipselect && avalon_slave_write && 
          ( avalon_slave_address  == 3 ) && avalon_slave_byteenable[1] ))
    crc16_shift_reg <= avalon_slave_writedata[15:8];
  // This next condition is a little complicated because we have to keep the state machine in SHIFT_IN_DATA
  // for the last write to data memory, but on the same cycle, we need to shift the first bit into the CRC register.
  else if ( rising_edge_spi_clk && (( current_state == SHIFT_IN_CRC16 ) ||
                                   (( current_state == SHIFT_IN_DATA) && ( bit_counter_is_zero ))))
    crc16_shift_reg <= { crc16_shift_reg [14:0], spi_data_in };
  else if ( falling_edge_spi_clk && ( current_state == SHIFT_OUT_CRC16 ))
    crc16_shift_reg <= crc16_shift_reg << 1;    
  else
    crc16_shift_reg <= crc16_shift_reg;

endmodule
