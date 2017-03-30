/*****************************************************************************
*  File:    tpo_lcd_controller.h
*
*  Purpose: Header file info for the TPO LCD Controller driver code.
*
*  Author: NGK
*
*****************************************************************************/
#ifndef __TPO_LCD_CONTROLLER_H__
#define __TPO_LCD_CONTROLLER_H__

#include <stdio.h>
#include "system.h"
#include "sys/alt_llist.h"
#include "priv/alt_dev_llist.h"
#include "priv/alt_file.h"


#define ALT_VIDEO_DISPLAY_USE_HEAP -1

#define ALT_VIDEO_DISPLAY_BLACK_8 0x00

// Maximum number of display buffers the driver will accept
#define ALT_VIDEO_DISPLAY_MAX_BUFFERS 4

#define LCD_ENABLE_IRQ 1
#define LCD_DO_NOT_ENABLE_IRQ 0


#define LCD_CONTROL_REG_OFFSET 0x0

#define LCD_GO_BIT             0x01
#define LCD_INT_EN_BIT         0x02
#define LCD_16BITCOLOR_MASK    0x00
#define LCD_24BITCOLOR_MASK    0x04
#define LCD_32BITCOLOR_MASK    0x08



#define LCD_IRQ_PENDING_MASK 0x7FFFFFFF

#define LCD_SERIAL_BUSY_MASK 0x00010000
#define LCD_SERIAL_GO_MASK 0x00010000
#define LCD_SERIAL_WRITE_MASK 0x00000000
#define LCD_SERIAL_TIMEOUT 0x3FFFFFFF
#define LCD_SERIAL_TURN_AROUND_BIT 0x0000
#define LCD_SERIAL_CNTL_OFFSET 0x10

#define LCD_GAMMA_CURVE 0

typedef struct {
//  alt_sgdma_descriptor *desc_base; /* Pointer to SGDMA descriptor chain */
  void *buffer;                    /* Pointer to video data buffer */
} alt_video_frame;


typedef struct {
  alt_video_frame* buffer_ptrs[ALT_VIDEO_DISPLAY_MAX_BUFFERS];
  int buffer_being_displayed;
  int buffer_being_written;
  int width;
  int height;
  int color_depth;
  int bytes_per_pixel;
  int bytes_per_frame;
  int num_frame_buffers;
  int orientation;
  int controller_base;
  int irq_ena;
  int irq_num;
} alt_video_display;

typedef struct lcd_controller_dev lcd_controller_dev;

struct lcd_controller_dev
{
  alt_dev dev;
  alt_u8* base;
  int irq_num;
};

#define TPO_LCD_CONTROLLER_INSTANCE(name, lcd_dev)   \
static lcd_controller_dev lcd_dev =                  \
{                                                    \
  {                                                  \
      ALT_LLIST_ENTRY,                               \
      name##_NAME,                                   \
      NULL, /* open */                               \
      NULL, /* close */                              \
      NULL, /* read */                               \
      NULL, /* write */                              \
      NULL, /* lseek */                              \
      NULL, /* fstat */                              \
      NULL, /* ioctl */                              \
   },                                                \
   name##_BASE,                                      \
   name##_IRQ,                                       \
}

#define TPO_LCD_CONTROLLER_INIT(name, lcd_dev)       \
    lcd_controller_dev_init (&lcd_dev )        

int lcd_controller_dev_init ( lcd_controller_dev* lcd_dev );

alt_dev* tpo_lcd_open(const char* name);

void tpo_lcd_isr (void* context, alt_u32 id) ;

alt_video_display* alt_video_display_init( char* lcd_name, 
                                           int width,
                                           int height,
                                           int color_depth,
                                           int buffer_location, 
                                           int dummy, 
                                           int num_buffers );
                         
void alt_video_display_close( alt_video_display* display, 
                              int buffer_location, 
                              int dummy );

inline void alt_video_display_clear_screen(alt_video_display* display, char color);

int alt_video_display_buffer_is_available( alt_video_display* display );

int alt_video_display_register_written_buffer( alt_video_display* display );

int alt_tpo_lcd_set_display_buffer( alt_video_display* display, void* buffer );

int alt_tpo_lcd_config(alt_video_display* display, int c);

int alt_tpo_lcd_write_serial_config_word( unsigned char addr, unsigned char data, alt_video_display* display );




#endif // __TPO_LCD_CONTROLLER_H__

