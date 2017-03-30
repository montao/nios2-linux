/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2003 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
******************************************************************************/

#ifndef __ALT_UP_AUDIO_H__
#define __ALT_UP_AUDIO_H__

#include "alt_up_audio_regs.h"
#include "system.h"

/**
 * @brief Enable the read interrupts for Audio Core
 *
 * @return 0 for success
 **/
int alt_up_audio_enable_read_interrupt();

/**
 * @brief Disable the read interrupts for Audio Core
 *
 * @return 0 for success
 **/
int alt_up_audio_disable_read_interrupt();

/**
 * @brief Reset the Audio Core by clearing the Incoming and Outgoing FIFOs. 
 *
 * @return 0 for success
 **/
int alt_up_audio_reset_audio_core();

/**
 * @brief Read \em len number of data from the Left Channel Incoming FIFO, and store to where \em buf points
 *
 * @param buf  -- the pointer to the allocated memory for storing data
 * @param len  -- the number of data to read
 *
 * @return the total number of data read
 * @note The function will read the FIFO until \em len is reached or the FIFO is empty
 **/
int alt_up_audio_read_left_channel(alt_u32 *buf, unsigned len);

/**
 * @brief Read \em len number of data from the Right Channel Incoming FIFO, and store to where \em buf points
 *
 * @param buf  -- the pointer to the allocated memory for storing data
 * @param len  -- the number of data to read
 *
 * @return the total number of data read
 * @note The function will read the FIFO until \em len is reached or the FIFO is empty
 **/
int alt_up_audio_read_right_channel(alt_u32 *buf, unsigned len);

/**
 * @brief Write \em len number of data from \em buf to the Left Channel Outgoing FIFO
 *
 * @param buf  -- the pointer to the data to be written
 * @param len  -- the number of data to be written to the FIFO
 *
 * @return the total number of data written
 * @note The function will write to the FIFO until \em len is reached or the FIFO is full
 **/
int alt_up_audio_write_left_channel(alt_u32 *buf, unsigned len);

/**
 * @brief Write \em len number of data from \em buf to the Right Channel Outgoing FIFO
 *
 * @param buf  -- the pointer to the data to be written
 * @param len  -- the number of data to be written to the FIFO
 *
 * @return the total number of data written
 * @note The function will write to the FIFO until \em len is reached or the FIFO is full
 **/
int alt_up_audio_write_right_channel(alt_u32 *buf, unsigned len);

#endif /*__ALT_UP_AUDIO_H__*/
