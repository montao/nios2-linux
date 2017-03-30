#ifndef __PS2_MOUSE_H__
#define __PS2_MOUSE_H__

#include "alt_up_ps2_port.h"

//mouse comamnds
#define MOUSE_SET_SAMPLE_RATE 0xF3
#define MOUSE_RESET 0xFF
#define MOUSE_SET_DEFAULT 0xF6
#define MOUSE_DISABLE_DATA_REPORTING 0xF5
#define MOUSE_ENABLE_DATA_REPORTING 0xF4
#define MOUSE_SET_SAMPLE_RATE 0xF3
#define MOUSE_READ_DATA 0xEB
#define MOUSE_SET_STREAM_MODE 0xEA
#define MOUSE_REQUEST_STATUS 0xE9
#define MOUSE_SET_RESOLUTION 0xE8

//Mouse modes (incomplete list)
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_REMOTE_MODE 0xF0

/**
 * @brief Reset the mouse
 *
 * @return \c PS2_SUCCESS on BAT is passed, otherwise \c PS2_ERROR
 **/
alt_u8 reset_mouse();

/**
 * @brief Set the operation mode of the mouse
 *
 * @param byte -- the byte representing the mode (see macro definitions for details)
 * @sa PS/2 Mouse document 
 *
 * @return \c PS2_SUCCESS on receiving acknowledgment
 **/
int set_mouse_mode(alt_u8 byte);

#endif
