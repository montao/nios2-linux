/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2007 Altera Corporation, San Jose, California, USA.           *
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
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
*                                                                             *
******************************************************************************/

#include <errno.h>

#include <priv/alt_file.h>

#include "altera_up_avalon_ps2.h"
#include "altera_up_avalon_ps2_regs.h"

#define PS2_ACK 	(0xFA)


//////////////////////////////////////////////////////////////////////////////////////////////
// Internal Functions
alt_u8 read_RI_bit(alt_u32 ctrl_reg)
{
	alt_u8 ri = (alt_u8) ((ctrl_reg & ALT_UP_PS2_PORT_CONTROL_RI_MSK) >> ALT_UP_PS2_PORT_CONTROL_RI_OFST);
	return ri;
}

alt_u8 read_RE_bit(alt_u32 ctrl_reg)
{
	alt_u8 re = (alt_u8) ((ctrl_reg & ALT_UP_PS2_PORT_CONTROL_RE_MSK) >> ALT_UP_PS2_PORT_CONTROL_RE_OFST);
	return re;
}

alt_u8 read_CE_bit(alt_u32 ctrl_reg)
{
	alt_u8 re = (alt_u8) ((ctrl_reg & ALT_UP_PS2_PORT_CONTROL_CE_MSK) >> ALT_UP_PS2_PORT_CONTROL_CE_OFST);
	return re;
}

alt_u16 read_num_bytes_available(alt_u32 data_reg)
{
	alt_u16 ravail = (alt_u16)((data_reg & ALT_UP_PS2_PORT_DATA_REG_RAVAIL_MSK ) >> ALT_UP_PS2_PORT_DATA_REG_RAVAIL_OFST);
	return ravail;
}

alt_u8 read_data_byte(alt_u32 data_reg)
{
	alt_u8 data = (alt_u8) ( (data_reg & ALT_UP_PS2_PORT_DATA_REG_DATA_MSK) >> ALT_UP_PS2_PORT_DATA_REG_DATA_OFST) ;
	return data;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// HAL Functions
void alt_up_ps2_init(alt_up_ps2_dev *ps2)
{
	// initialize the device
	alt_u8 byte;
	//send the reset request, wait for ACK
	int status = alt_up_ps2_write_data_byte_with_ack(ps2, 0xff);
	if (status == 0)
	{
		// reset succeed, now try to get the BAT result, AA means passed
		status = alt_up_ps2_read_data_byte(ps2, &byte);
		if (status == 0 && byte == 0xAA)
		{
			//get the 2nd byte
			status = alt_up_ps2_read_data_byte(ps2, &byte);
			if (status == -ETIMEDOUT)
			{
				//for keyboard, only 2 bytes are sent(ACK, PASS/FAIL), so timeout
				ps2->device_type = PS2_KEYBOARD;
			}
			else if (status == 0 && byte == 0x00)
			{
				//for mouse, it will sent out 0x00 after sending out ACK and PASS/FAIL.
				ps2->device_type = PS2_MOUSE;
			}
		}
	}
}

int alt_up_ps2_write_data_byte(alt_up_ps2_dev *ps2, alt_u8 byte)
{
	//note: data are only located at the lower 8 bits
	//note: the software send command to the PS2 peripheral through the data
	//		register rather than the control register
	IOWR_ALT_UP_PS2_PORT_DATA(ps2->base, byte);
	alt_u32 ctrl_reg = IORD_ALT_UP_PS2_PORT_DATA(ps2->base);
	if (read_CE_bit(ctrl_reg))
	{
		//CE bit is set --> error occurs on sending commands
		return -EIO;
	}
	return 0;
}

int alt_up_ps2_wait_for_ack(alt_up_ps2_dev *ps2)
{
	alt_u8 data = 0;
	alt_u8 status = 0;
	do
	{
		status = alt_up_ps2_read_data_byte(ps2, &data); 
		if ( status == 0)
		{
			if (data == PS2_ACK)
				return 0;
		}
		else 
		{
			return status;
		}
	} while(1);
	return -ETIMEDOUT;
}

int alt_up_ps2_write_data_byte_with_ack(alt_up_ps2_dev *ps2, alt_u8 byte)
{
	int send_status = alt_up_ps2_write_data_byte(ps2, byte);
	if ( send_status != 0)
		// return on sending error
		return send_status;

	int ack_status = alt_up_ps2_wait_for_ack(ps2);
	return ack_status;
}

int alt_up_ps2_read_data_byte(alt_up_ps2_dev *ps2, alt_u8 *byte)
{
	alt_u32 data_reg = 0; 
	alt_u16 num = 0; 
	alt_u32 count = 0;
	do {
		count++;
		data_reg = IORD_ALT_UP_PS2_PORT_DATA(ps2->base);
		num = read_num_bytes_available(data_reg);
		if (num > 0)
		{
			*byte = read_data_byte(data_reg);
			return 0;
		}
		//timeout = 0 means to disable the timeout
		if ( ps2->timeout != 0 && count > ps2->timeout)
		{
			return -ETIMEDOUT;
		}
	} while (1);
}

void alt_up_ps2_clear_fifo(alt_up_ps2_dev *ps2)
{
	// The DATA byte of the data register will be automatically cleared after a read
	// So we simply keep reading it until there are no available bytes
	alt_u16 num = 0;
	alt_u32 data_reg = 0;
	do
	{
		// read the data register (the DATA byte is cleared)
		data_reg = IORD_ALT_UP_PS2_PORT_DATA(ps2->base);
		// get the number of available bytes from the RAVAIL part of data register
		num = read_num_bytes_available(data_reg);
	} while (num > 0);
}

//////////////////////////////////////////////////////////////
// FD Functions
int alt_up_ps2_read_fd (alt_fd* fd, char* ptr, int len)
{
	alt_up_ps2_dev *ps2 = (alt_up_ps2_dev*) fd->dev;
	int status = 0;
	int count = 0;
	while (count < len);
	{
		status = alt_up_ps2_read_data_byte(ps2, ptr++);
		if (status!=0)
			return count;
		count++;
	} 
	return count;
}

int alt_up_ps2_write_fd (alt_fd* fd, const char* ptr, int len)
{
	alt_up_ps2_dev *ps2 = (alt_up_ps2_dev*) fd->dev;
	int status = 0;
	int count = 0;
	while (count < len)
	{
		status = alt_up_ps2_write_data_byte(ps2, *(ptr++) );
		if (status!=0)
			return count;
		count++;
	}
	return count;
}

alt_up_ps2_dev* alt_up_ps2_open_dev(const char* name)
{
  // find the device from the device list 
  // (see altera_hal/HAL/inc/priv/alt_file.h 
  // and altera_hal/HAL/src/alt_find_dev.c 
  // for details)
  alt_up_ps2_dev *dev = (alt_up_ps2_dev*)alt_find_dev(name, &alt_dev_list);

  return dev;
}

