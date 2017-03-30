/*****************************************************************************
 *                                                                           *
 * Module:       Altera_UP_Avalon_Audio                                      *
 * Description:                                                              *
 *      This module reads and writes data to the Audio chip on Altera's DE2  *
 *   Development and Education Board. The audio chip must be in master mode  *
 *   and the digital format must be left justified.                          *
 *                                                                           *
 *****************************************************************************/

module Altera_UP_Avalon_Audio (
	// Inputs
	clk,
	reset,
	
	address,
	chipselect,
	read,
	write,
	writedata,

	AUD_ADCDAT,

	// Bidirectionals
	AUD_BCLK,
	AUD_ADCLRCK,
	AUD_DACLRCK,

	// Outputs
	irq,
	readdata,

	AUD_DACDAT
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter AUDIO_DATA_WIDTH	= 16;
parameter BIT_COUNTER_INIT	= 5'h0F;

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/
// Inputs
input				clk;
input				reset;

input		[1:0]	address;
input				chipselect;
input				read;
input				write;
input		[31:0]	writedata;

input				AUD_ADCDAT;

// Bidirectionals
inout				AUD_BCLK;
inout				AUD_ADCLRCK;
inout				AUD_DACLRCK;

// Outputs
output	reg			irq;
output	reg	[31:0]	readdata;

output				AUD_DACDAT;

/*****************************************************************************
 *                 Internal wires and registers Declarations                 *
 *****************************************************************************/

// Internal Wires
wire				bclk_rising_edge;
wire				bclk_falling_edge;

`IF USE_AUDIO_IN
wire				adc_lrclk_rising_edge;
wire				adc_lrclk_falling_edge;

wire		[AUDIO_DATA_WIDTH:1] new_left_channel_audio;
wire		[AUDIO_DATA_WIDTH:1] new_right_channel_audio;

wire		[7:0]	left_channel_read_available;
wire		[7:0]	right_channel_read_available;
`ENDIF
`IF USE_AUDIO_OUT
wire				dac_lrclk_rising_edge;
wire				dac_lrclk_falling_edge;

wire		[7:0]	left_channel_write_space;
wire		[7:0]	right_channel_write_space;
`ENDIF

// Internal Registers
`IF USE_AUDIO_IN
reg					done_adc_channel_sync;
reg					read_interrupt_en;
reg					clear_read_fifos;
reg					read_interrupt;

`ENDIF
`IF USE_AUDIO_OUT
reg					done_dac_channel_sync;
reg					write_interrupt_en;
reg					clear_write_fifos;
reg					write_interrupt;
`ENDIF

// State Machine Registers


/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/


/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/

always @(posedge clk)
begin
	if (reset == 1'b1)
		irq <= 1'b0;
	else
		irq <= 
`IF USE_AUDIO_OUT
			write_interrupt |
`ELSE
			1'b0 |
`ENDIF
`IF USE_AUDIO_IN
			read_interrupt;
`ELSE
			1'b0;
`ENDIF
end

always @(posedge clk)
begin
	if (reset == 1'b1)
		readdata <= 32'h00000000;
	else if (chipselect == 1'b1)
	begin
		if (address == 2'h0)
			readdata <= 
				{22'h000000,
`IF USE_AUDIO_OUT
				 write_interrupt,
`ELSE
				1'b0,
`ENDIF
`IF USE_AUDIO_IN
				 read_interrupt,
`ELSE
				1'b0,
`ENDIF
				 4'h0,
`IF USE_AUDIO_OUT
				 clear_write_fifos,
`ELSE
				1'b0,
`ENDIF
`IF USE_AUDIO_IN
				 clear_read_fifos,
`ELSE
				1'b0,
`ENDIF
`IF USE_AUDIO_OUT
				 write_interrupt_en,
`ELSE
				1'b0,
`ENDIF
`IF USE_AUDIO_IN
				 read_interrupt_en};
`ELSE
				1'b0};
`ENDIF
		else if (address == 2'h1)
		begin
`IF USE_AUDIO_OUT
			readdata[31:24] <= left_channel_write_space;
			readdata[23:16] <= right_channel_write_space;
`ELSE
			readdata[31:24] <= 8'h00;
			readdata[23:16] <= 8'h00;
`ENDIF
`IF USE_AUDIO_IN
			readdata[15: 8] <= left_channel_read_available;
			readdata[ 7: 0] <= right_channel_read_available;
`ELSE
			readdata[15: 8] <= 8'h00;
			readdata[ 7: 0] <= 8'h00;
`ENDIF
		end
`IF USE_AUDIO_IN
		else if (address == 2'h2)
			readdata <= 32'h00000000 | 
				new_left_channel_audio;
		else
			readdata <= 32'h00000000 | 
				new_right_channel_audio;
`ELSE
		else
			readdata <= 32'h00000000;
`ENDIF
	end
end


`IF USE_AUDIO_IN
always @(posedge clk)
begin
	if (reset == 1'b1)
		read_interrupt_en <= 1'b0;
	else if ((chipselect == 1'b1) && (write == 1'b1) && (address == 2'h0))
		read_interrupt_en <= writedata[0];
end

always @(posedge clk)
begin
	if (reset == 1'b1)
		clear_read_fifos <= 1'b0;
	else if ((chipselect == 1'b1) && (write == 1'b1) && (address == 2'h0))
		clear_read_fifos <= writedata[2];
end

always @(posedge clk)
begin
	if (reset == 1'b1)
		read_interrupt <= 1'b0;
	else if (read_interrupt_en == 1'b0)
		read_interrupt <= 1'b0;
	else
		read_interrupt <=
			(&(left_channel_read_available[6:5])  | left_channel_read_available[7]) | 
			(&(right_channel_read_available[6:5]) | right_channel_read_available[7]);
end

always @ (posedge clk)
begin
	if (reset == 1'b1)
		done_adc_channel_sync <= 1'b0;
	else if (adc_lrclk_rising_edge == 1'b1)
		done_adc_channel_sync <= 1'b1;
end
`ENDIF

`IF USE_AUDIO_OUT
always @(posedge clk)
begin
	if (reset == 1'b1)
		write_interrupt_en <= 1'b0;
	else if ((chipselect == 1'b1) && (write == 1'b1) && (address == 2'h0))
		write_interrupt_en <= writedata[1];
end

always @(posedge clk)
begin
	if (reset == 1'b1)
		clear_write_fifos <= 1'b0;
	else if ((chipselect == 1'b1) && (write == 1'b1) && (address == 2'h0))
		clear_write_fifos <= writedata[3];
end

always @(posedge clk)
begin
	if (reset == 1'b1)
		write_interrupt <= 1'b0;
	else if (write_interrupt_en == 1'b0)
		write_interrupt <= 1'b0;
	else
		write_interrupt <= 
			(&(left_channel_write_space[6:5])  | left_channel_write_space[7]) | 
			(&(right_channel_write_space[6:5]) | right_channel_write_space[7]);
end


always @ (posedge clk)
begin
	if (reset == 1'b1)
		done_dac_channel_sync <= 1'b0;
	else if (dac_lrclk_falling_edge == 1'b1)
		done_dac_channel_sync <= 1'b1;
end
`ENDIF

/*****************************************************************************
 *                            Combinational logic                            *
 *****************************************************************************/

assign AUD_BCLK		= 1'bZ;
assign AUD_ADCLRCK	= 1'bZ;
assign AUD_DACLRCK	= 1'bZ;

`IF USE_AUDIO_OUT
`ELSE
assign AUD_DACDAT	= 1'b0;
`ENDIF

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/

Altera_UP_Clock_Edge Bit_Clock_Edges (
	// Inputs
	.clk			(clk),
	.reset			(reset),
	
	.test_clk		(AUD_BCLK),
	
	// Bidirectionals

	// Outputs
	.rising_edge	(bclk_rising_edge),
	.falling_edge	(bclk_falling_edge)
);

`IF USE_AUDIO_IN
Altera_UP_Clock_Edge ADC_Left_Right_Clock_Edges (
	// Inputs
	.clk			(clk),
	.reset			(reset),
	
	.test_clk		(AUD_ADCLRCK),
	
	// Bidirectionals

	// Outputs
	.rising_edge	(adc_lrclk_rising_edge),
	.falling_edge	(adc_lrclk_falling_edge)
);
`ENDIF

`IF USE_AUDIO_OUT
Altera_UP_Clock_Edge DAC_Left_Right_Clock_Edges (
	// Inputs
	.clk			(clk),
	.reset			(reset),
	
	.test_clk		(AUD_DACLRCK),
	
	// Bidirectionals

	// Outputs
	.rising_edge	(dac_lrclk_rising_edge),
	.falling_edge	(dac_lrclk_falling_edge)
);
`ENDIF





`IF USE_AUDIO_IN
Altera_UP_Audio_In_Deserializer Audio_In_Deserializer (
	// Inputs
	.clk							(clk),
	.reset							(reset | clear_read_fifos),
	
	.bit_clk_rising_edge			(bclk_rising_edge),
	.bit_clk_falling_edge			(bclk_falling_edge),
	.left_right_clk_rising_edge		(adc_lrclk_rising_edge),
	.left_right_clk_falling_edge	(adc_lrclk_falling_edge),

	.done_channel_sync				(done_adc_channel_sync),

	.serial_audio_in_data			(AUD_ADCDAT),

	.read_left_audio_data_en		((address == 2'h2) & chipselect & read),
	.read_right_audio_data_en		((address == 2'h3) & chipselect & read),

	// Bidirectionals

	// Outputs
	.left_audio_fifo_read_space		(left_channel_read_available),
	.right_audio_fifo_read_space	(right_channel_read_available),

	.left_channel_data				(new_left_channel_audio),
	.right_channel_data				(new_right_channel_audio)
);
defparam
	Audio_In_Deserializer.AUDIO_DATA_WIDTH = AUDIO_DATA_WIDTH,
	Audio_In_Deserializer.BIT_COUNTER_INIT = BIT_COUNTER_INIT;
`ENDIF


`IF USE_AUDIO_OUT
Altera_UP_Audio_Out_Serializer Audio_Out_Serializer (
	// Inputs
	.clk							(clk),
	.reset							(reset | clear_write_fifos),
	
	.bit_clk_rising_edge			(bclk_rising_edge),
	.bit_clk_falling_edge			(bclk_falling_edge),
	.left_right_clk_rising_edge		(done_dac_channel_sync & dac_lrclk_rising_edge),
	.left_right_clk_falling_edge	(done_dac_channel_sync & dac_lrclk_falling_edge),
	
	.left_channel_data				(writedata[(AUDIO_DATA_WIDTH - 1):0]),
	.left_channel_data_en			((address == 2'h2) & chipselect & write),

	.right_channel_data				(writedata[(AUDIO_DATA_WIDTH - 1):0]),
	.right_channel_data_en			((address == 2'h3) & chipselect & write),
	
	// Bidirectionals

	// Outputs
	.left_channel_fifo_write_space	(left_channel_write_space),
	.right_channel_fifo_write_space	(right_channel_write_space),

	.serial_audio_out_data			(AUD_DACDAT)
);
defparam
	Audio_Out_Serializer.AUDIO_DATA_WIDTH = AUDIO_DATA_WIDTH;
`ENDIF

endmodule

