/*****************************************************************************
*  File:    tpo_lcd_controller.c
*
*  Purpose: Driver code for the TPO LCD Controller
*
*  Author: NGK
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tpo_lcd_controller.h"
#include "io.h"
#include "sys/alt_alarm.h"
#include "sys/alt_cache.h"
#include "system.h"
#include "priv/alt_file.h"


/******************************************************************
*  Function: lcd_controller_dev_init
*
*  Purpose: HAL device driver initialization.  
*           Called by alt_sys_init.
*
******************************************************************/
int lcd_controller_dev_init ( lcd_controller_dev* lcd_dev )
{
  int ret_code = 0;
 
  ret_code = alt_dev_reg( &(lcd_dev->dev) );
  // Make sure controller and its interrupts are disabled
  IOWR( lcd_dev->base, LCD_CONTROL_REG_OFFSET, 0x0 );
 
  return ret_code;
}

/******************************************************************
*  Function: tpo_lcd_open
*
*  Purpose: Opens the LCD controller for use.  
*           Returns a file descriptor for the controller
*
******************************************************************/
alt_dev* tpo_lcd_open(const char* name)
{
  alt_dev* dev = (alt_dev*)alt_find_dev( name, &alt_dev_list );

  return dev;
}

/******************************************************************
*  Function: alt_video_display_close
*
*  Purpose: Closes the display and frees all malloc'd memory
*
*  Returns:  void 
*
******************************************************************/
void alt_video_display_close( alt_video_display* display, 
                              int buffer_location, 
                              int dummy )
{
  int i;
  
  // Free our frame buffers and descriptor buffers
  for( i = 0; i < display->num_frame_buffers; i++ )
  {
    // Free the frame buffer and frame buffer pointer if they were on the heap
    if( buffer_location == ALT_VIDEO_DISPLAY_USE_HEAP )
    {
      free( display->buffer_ptrs[i]->buffer );
  	  free( display->buffer_ptrs[i] );
  	}
  }

  // Finally free our display struct
  free( display );
}

/******************************************************************
*  Function: alt_video_display_init
*
*  Purpose: Initializes the LCD controller for the TPO TD043MTEA1 
*           LCD Panel. Gets memory for the frame buffer,
*           sets the resolution of the frame buffer, resets the 
*           controller hardware, gives the controller the base 
*           address of the frame buffer, then enables
*           the controller.
*
******************************************************************/
alt_video_display* alt_video_display_init( char* lcd_name, 
                                           int width,
                                           int height,
                                           int color_depth,
                                           int buffer_location, 
                                           int dummy, 
                                           int num_buffers )
{
  alt_video_display* display;
  lcd_controller_dev* lcd;
  int bytes_per_pixel, bytes_per_frame;
  int i, ctrl_status;
  
  lcd = (lcd_controller_dev*)tpo_lcd_open( lcd_name );
  
  // We'll need these values more than once, so let's pre-calculate them.
  bytes_per_pixel = color_depth >> 3; // same as /8
  bytes_per_frame = (( width * height ) * bytes_per_pixel);

  if( num_buffers > ALT_VIDEO_DISPLAY_MAX_BUFFERS )
  {
  	num_buffers = ALT_VIDEO_DISPLAY_MAX_BUFFERS;
  }


	// Allocate our frame buffers
	if( buffer_location == ALT_VIDEO_DISPLAY_USE_HEAP )
	{
		display = (alt_video_display*) malloc(sizeof (alt_video_display));

		for( i = 0; i < num_buffers; i++ )
	  {
	  	display->buffer_ptrs[i] = (alt_video_frame*) malloc( sizeof( alt_video_frame ));
	  	display->buffer_ptrs[i]->buffer = (void*) alt_uncached_malloc(( bytes_per_frame ));
	  }
	}
	else
	{
		display = (alt_video_display*)buffer_location;
    buffer_location += sizeof(alt_video_display);
		for( i = 0; i < num_buffers; i++ )
	  {
	  	display->buffer_ptrs[i] = (void*)(buffer_location);
	  	buffer_location += sizeof( alt_video_frame );
	  	
	  	display->buffer_ptrs[i]->buffer = (void*)(buffer_location);
	  	buffer_location += bytes_per_frame;
	  }
	}

	// Fill out the frame buffer structure
	display->width = width;
	display->height = height;
	display->color_depth = color_depth;
	display->num_frame_buffers = num_buffers;
	display->bytes_per_frame = bytes_per_frame;
	display->bytes_per_pixel = ( color_depth / 8 );
	display->controller_base = lcd->base;
	display->irq_ena = LCD_ENABLE_IRQ;
	display->irq_num = lcd->irq_num;
	display->buffer_being_displayed = num_buffers / 2;
	display->buffer_being_written = 0;

  // Register the interrupt.
  alt_irq_register( display->irq_num, display, tpo_lcd_isr );
  
  //Clear all frame buffers to black
  for( i = 0; i < num_buffers; i++ )
  {
    memset( (void*)(display->buffer_ptrs[i]->buffer), ALT_VIDEO_DISPLAY_BLACK_8, display->bytes_per_frame );
	}
  
  // Start the Controller
  IOWR_32DIRECT( lcd->base, 0, 0x0 ); /* Reset the LCD controller */
  IOWR_32DIRECT( lcd->base, 4, ( int )display->buffer_ptrs[display->buffer_being_displayed] ); /* Where our frame buffer starts */
  IOWR_32DIRECT( lcd->base, 8, ( ( width * height ) * bytes_per_pixel ) ); /* amount of memory needed */   
 
	// Start the controller and release the panel from reset
	ctrl_status = LCD_GO_BIT | LCD_INT_EN_BIT;
	
  
  if( color_depth == 16 )
  {
    ctrl_status |= LCD_16BITCOLOR_MASK;
  }
  else if( color_depth == 24 )
  {
    ctrl_status |= LCD_24BITCOLOR_MASK; 
  }
  else if( color_depth == 32 )
  {
    ctrl_status |= LCD_32BITCOLOR_MASK; 
  }
    
  IOWR_32DIRECT( lcd->base, 0, ctrl_status ); /* Set the go bit and the interrupt enable bit. */


  // Wait at least 100ms
  usleep(10000);
  
  // Now that the panel is out of reset, configure the panel hardware
  alt_tpo_lcd_config( display, 0 );
  	
  return ( display );
}


/******************************************************************
*  Function: alt_video_display_register_written_buffer
*
*  Purpose: Registers the buffer pointed to by buffer_being_written
*           as being finished and ready for display.
*
*  Returns: 0 - Everything is groovy.
*           1 - buffer_being_written is now equal to buffer_being_displayed
*                 - It would be a good idea to wait for buffer_being_displayed 
*                   to increment before writing to buffer_being_written.
*                   lcd_wait_for_free_buffer() is provided for this purpose
*
******************************************************************/
int alt_video_display_register_written_buffer( alt_video_display* display )
{
	int ret_code;
	
	// We got called because frame_being_written is finished, so we need to increment
	// the buffer_being_written index, effectively registering it as a written frame
	// which is ready for display
	display->buffer_being_written = ( display->buffer_being_written + 1 ) % display->num_frame_buffers;
	
	if( display->buffer_being_written == display->buffer_being_displayed )
	{
		ret_code = 1;
	}
	else
	{
		ret_code = 0;
	}
	
	return(ret_code);
}

/******************************************************************
*  Function: lcd_wait_for_free_buffer
*
*  Purpose: Checks for a free frame buffer to write to.
*
*  Returns:  0 - Found free buffer.  buffer_being_written now points to it.
*           -1 - Free buffer not available at this time. 
*
******************************************************************/
int alt_video_display_buffer_is_available( alt_video_display* display )
{
	int ret_code = 0;
	
  if(( IORD_32DIRECT( display->controller_base, 0xC ) == (int)display->buffer_ptrs[display->buffer_being_written] ) &&
     ( display->num_frame_buffers > 1 ))
	{	
		ret_code = -1;
	}
	else 
	{
		ret_code = 0;
	}
	
	return (ret_code);
}

/******************************************************************
*  Function: lcd_stop
*
*  Purpose: Stops the LCD Controller
*
*  Returns: 0
*
******************************************************************/
int lcd_stop ( lcd_controller_dev* lcd )
{

	IOWR_32DIRECT( lcd->base, 0, 0x0 ); /* Reset the LCD controller */

  return (0);
}

/******************************************************************
*  Function: alt_video_display_clear_screen
*
*  Purpose: Uses the fast memset routine to clear the entire frame
*           buffer.  User can specify black(0x00) or white(0xFF).
*
******************************************************************/
inline void alt_video_display_clear_screen(alt_video_display* display, char color)
{
	memset( (void*)(display->buffer_ptrs[display->buffer_being_written]), color, display->bytes_per_frame );
}


/******************************************************************
*  Function: tpo_lcd_isr
*
*  Purpose: This is the interrupt service routine for the LCD controller
*           The routine checks if there is a new frame buffer ready for
*           display.  If so, it sets that buffer as the current buffer 
*           being displayed, then writes it to the LCD controller to 
*           be displayed on the next frame cycle.  The routine then clears 
*           the interrupt.
*
******************************************************************/
void tpo_lcd_isr (void* context, alt_u32 id) 
{
  int temp, base_addr, next_disp_buf; 
  alt_video_display* display = (alt_video_display*)context;
  
  base_addr = display->controller_base;
  next_disp_buf = (( display->buffer_being_displayed + 1 ) % display->num_frame_buffers );

  // Check if the next frame buffer is ready for display.
  if(( next_disp_buf != display->buffer_being_written ) ||
    ( display->num_frame_buffers == 1 ))
  {
  	display->buffer_being_displayed = next_disp_buf;
  	alt_tpo_lcd_set_display_buffer( display, display->buffer_ptrs[next_disp_buf] );
  }
 
  // Clear interrupt bit 
  temp = IORD (base_addr , LCD_CONTROL_REG_OFFSET);
  IOWR (base_addr, LCD_CONTROL_REG_OFFSET, (temp & LCD_IRQ_PENDING_MASK));
}

/******************************************************************
*  Function: alt_tpo_lcd_set_display_buffer
*
*  Purpose: Writes the specified frame buffer address to the LCD Controller
*           so that it is displayed on the next frame cycle	
*
*  Returns: 0
*
******************************************************************/
int alt_tpo_lcd_set_display_buffer( alt_video_display* display, void* buffer )
{
	IOWR_32DIRECT( display->controller_base, 4, (int)buffer );
	
	return (0);
}

/******************************************************************
*  Function: alt_tpo_lcd_write_serial_config_word
*
*  Purpose: This routine writes a configuration word to the LCD
*           panel's serial interface port.
*
******************************************************************/
int alt_tpo_lcd_write_serial_config_word( unsigned char addr, unsigned char data, alt_video_display* display )
{
  unsigned int config_word;
  int busy;
  int been_waiting = 0;
  int ret_code = 0;
  int bit_index;
  
  //  Register definition.
  //     2     1     0
  //  ------------------
  // | SCEN | SCL | SDA |
  //  ------------------
  unsigned char SCL_RISING_EDGE = 0x2;
  unsigned char SCL_FALLING_EDGE = 0x0;
  unsigned char SCEN_ACTIVE = 0x0;
  unsigned char SCEN_INACTIVE = 0x4;
  unsigned char SDA_HIGH = 0x1;
  unsigned char SDA_LOW = 0x0;
  unsigned char data_bit;
  
  // Make sure it's been clocked once with SCEN inactive  
  IOWR_32DIRECT( display->controller_base, 0x10, 
                 ( SCL_FALLING_EDGE | SCEN_INACTIVE | SDA_HIGH ));
//  usleep( 500 );
//  IOWR_32DIRECT( display->controller_base, 0x10, 
//                 ( SCL_RISING_EDGE | SCEN_INACTIVE | SDA_HIGH ));
//  usleep( 500 );
    
  config_word = ( addr << 10 | data | LCD_SERIAL_WRITE_MASK | LCD_SERIAL_TURN_AROUND_BIT );
    
  for( bit_index = 0; bit_index < 16; bit_index++ )
  {
    data_bit = (( config_word >> ( 15 - bit_index )) & 0x1 );
    // Send SCEN and SDA active
    IOWR_32DIRECT( display->controller_base, 0x10, 
                   ( SCL_FALLING_EDGE | SCEN_ACTIVE | data_bit ));
    usleep( 1 );

    // Clock in the data
    IOWR_32DIRECT( display->controller_base, 0x10, 
                   ( SCL_RISING_EDGE | SCEN_ACTIVE | data_bit ));
    usleep( 1 );
  }
  
  // Clock once with SCEN inactive
  IOWR_32DIRECT( display->controller_base, 0x10, 
                 ( SCL_FALLING_EDGE | SCEN_INACTIVE | SDA_HIGH ));
  usleep( 1 );
  
//  IOWR_32DIRECT( display->controller_base, 0x10, 
//                 ( SCL_RISING_EDGE | SCEN_INACTIVE | SDA_HIGH ));
//  usleep( 500 );
    
  return( ret_code );
}

/******************************************************************
*  Function: lcd_gamma_correct
*
*  Purpose: This routine configures the LCD panel through its
*           serial interface port.
*
******************************************************************/
int alt_tpo_lcd_config(alt_video_display* display, int c)
{
  // Gamma curves               0      8      16     32     64     96    128    192    224    240    248    256
  unsigned short g[5][12] = {{ 106,   200,   289,   375,   460,   543,   625,   705,   785,   864,   942,   1020 },  // Terasic's choice
                             { 128,   229,   321,   408,   492,   572,   651,   728,   803,   876,   949,   1020 },  // r = 1.2
                             { 45,    108,   180,   258,   341,   428,   519,   613,   711,   811,   913,   1019 },  // r = 0.9
                             { 99,    195,   284,   366,   440,   510,   580,   654,   735,   823,   919,   1019 },  // natural
                             { 0x00,  0x20,  0x40,  0x80,  0x100, 0x180, 0x200, 0x300, 0x380, 0x3C0, 0x3E0, 0x3FF }}; // default
  
  int ret_code = 0;        
  
  // Gamma
//  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x3F, 0xFF, display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x11, ( ((g[c][0] & 0x300 ) >> 2) | ((g[c][1] & 0x300 ) >> 4) | ((g[c][2] & 0x300 ) >> 6) | ((g[c][3] & 0x300 ) >> 8 )), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x12, ( ((g[c][4] & 0x300 ) >> 2) | ((g[c][5] & 0x300 ) >> 4) | ((g[c][6] & 0x300 ) >> 6) | ((g[c][7] & 0x300 ) >> 8 )), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x13, ( ((g[c][8] & 0x300 ) >> 2) | ((g[c][9] & 0x300 ) >> 4) | ((g[c][10] & 0x300 ) >> 6) | ((g[c][11] & 0x300 ) >> 8 )), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x14, (unsigned char)( g[c][0] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x15, (unsigned char)( g[c][1] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x16, (unsigned char)( g[c][2] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x17, (unsigned char)( g[c][3] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x18, (unsigned char)( g[c][4] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x19, (unsigned char)( g[c][5] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x1a, (unsigned char)( g[c][6] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x1b, (unsigned char)( g[c][7] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x1c, (unsigned char)( g[c][8] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x1d, (unsigned char)( g[c][9] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x1e, (unsigned char)( g[c][10] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x1f, (unsigned char)( g[c][11] & 0xFF ), display );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x20, 0xF0, display  );
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x21, 0xF0, display  );
  
  // Various config
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x03, 0x5F, display  );
  
  // Resolution (800x480)
  ret_code |= alt_tpo_lcd_write_serial_config_word( 0x02, 0x07, display  );

  return( ret_code );
}
