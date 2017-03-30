/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2006 Altera Corporation, San Jose, California, USA.           *
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

#include <errno.h>

#include <priv/alt_file.h>

#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_regs.h"

///////////////////////////////////////////////////////////////////////////
// Direct functions
alt_up_audio_dev* alt_up_audio_open_dev(const char* name)
{
  // find the device from the device list 
  // (see altera_hal/HAL/inc/priv/alt_file.h 
  // and altera_hal/HAL/src/alt_find_dev.c 
  // for details)
  alt_up_audio_dev *dev = (alt_up_audio_dev*)alt_find_dev(name, &alt_dev_list);

  return dev;
}


int alt_up_audio_enable_read_interrupt(alt_up_audio_dev *audio)
{
	alt_u32 ctrl_reg;
	ctrl_reg = IORD_ALT_UP_AUDIO_CONTROL(audio->base); 
	// set RE to 1 while maintaining other bits the same
	ctrl_reg |= ALT_UP_AUDIO_CONTROL_RE_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(audio->base, ctrl_reg);
	return 0;
}

int alt_up_audio_disable_read_interrupt(alt_up_audio_dev *audio)
{
	alt_u32 ctrl_reg;
	ctrl_reg = IORD_ALT_UP_AUDIO_CONTROL(audio->base); 
	// set RE to 0 while maintaining other bits the same
	ctrl_reg &= ~ALT_UP_AUDIO_CONTROL_RE_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(audio->base, ctrl_reg);
	return 0;
}

int alt_up_audio_reset_audio_core(alt_up_audio_dev *audio)
{
	alt_u32 ctrl_reg;
	ctrl_reg = IORD_ALT_UP_AUDIO_CONTROL(audio->base); 
	// set CR and CW to 1 while maintaining other bits the same
	ctrl_reg |= ALT_UP_AUDIO_CONTROL_CR_MSK;
	ctrl_reg |= ALT_UP_AUDIO_CONTROL_CW_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(audio->base, ctrl_reg);
	// set CR and CW to 0 while maintaining other bits the same
	ctrl_reg &= ~ALT_UP_AUDIO_CONTROL_CR_MSK;
	ctrl_reg &= ~ALT_UP_AUDIO_CONTROL_CW_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(audio->base, ctrl_reg);
	return 0;
}

int alt_up_audio_read_fifo(alt_up_audio_dev *audio, alt_u32 *buf, unsigned len)
{
	alt_u32 fifospace;
	unsigned count = 0;
	while ( count < len ) 
	{
		// read the whole fifospace register
		fifospace = IORD_ALT_UP_AUDIO_FIFOSPACE(audio->base);
		// extract the part for proper Channel Read Space
		fifospace = (audio->channel == LEFT) ? 
			(fifospace & ALT_UP_AUDIO_FIFOSPACE_RALC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_RALC_OFST 
			:
			(fifospace & ALT_UP_AUDIO_FIFOSPACE_RARC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_RARC_OFST;
		if (fifospace > 0) 
		{
			buf[count] = (audio->channel == LEFT) ? 
				IORD_ALT_UP_AUDIO_LEFTDATA(audio->base)
				:
				IORD_ALT_UP_AUDIO_RIGHTDATA(audio->base);
			count ++;
		}
		else
		{
			// no more data to read
			break;
		}
	}
	return count;
}

int alt_up_audio_write_fifo(alt_up_audio_dev *audio, alt_u32 *buf, unsigned len)
{
	alt_u32 fifospace;
	unsigned count = 0;
	while ( count < len ) 
	{
		// read the whole fifospace register
		fifospace = IORD_ALT_UP_AUDIO_FIFOSPACE(audio->base);
		// extract the part for Left Channel Write Space 
		fifospace = (audio->channel == LEFT) ? 
			(fifospace & ALT_UP_AUDIO_FIFOSPACE_WSLC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_WSLC_OFST
			:
			(fifospace & ALT_UP_AUDIO_FIFOSPACE_WSRC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_WSRC_OFST;
		if (fifospace > 0) 
		{
			if (audio->channel == LEFT) 
				IOWR_ALT_UP_AUDIO_LEFTDATA(audio->base, buf[count++]);
			else
				IOWR_ALT_UP_AUDIO_RIGHTDATA(audio->base, buf[count++]);
		}
		else
		{
			// no more space to write
			break;
		}
	}
	return count;
}

