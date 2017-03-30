#include "alt_up_audio.h"

int alt_up_audio_enable_read_interrupt()
{
	alt_u32 ctrl_reg;
	ctrl_reg = IORD_ALT_UP_AUDIO_CONTROL(ALT_UP_AUDIO_BASE); 
	// set RE to 1 while maintaining other bits the same
	ctrl_reg |= ALT_UP_AUDIO_CONTROL_RE_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(ALT_UP_AUDIO_BASE, ctrl_reg);
	return 0;
}

int alt_up_audio_disable_read_interrupt()
{
	alt_u32 ctrl_reg;
	ctrl_reg = IORD_ALT_UP_AUDIO_CONTROL(ALT_UP_AUDIO_BASE); 
	// set RE to 0 while maintaining other bits the same
	ctrl_reg &= ~ALT_UP_AUDIO_CONTROL_RE_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(ALT_UP_AUDIO_BASE, ctrl_reg);
	return 0;
}

int alt_up_audio_reset_audio_core()
{
	alt_u32 ctrl_reg;
	ctrl_reg = IORD_ALT_UP_AUDIO_CONTROL(ALT_UP_AUDIO_BASE); 
	// set CR and CW to 1 while maintaining other bits the same
	ctrl_reg |= ALT_UP_AUDIO_CONTROL_CR_MSK;
	ctrl_reg |= ALT_UP_AUDIO_CONTROL_CW_MSK;
	// set CR and CW to 0 while maintaining other bits the same
	ctrl_reg &= ~ALT_UP_AUDIO_CONTROL_CR_MSK;
	ctrl_reg &= ~ALT_UP_AUDIO_CONTROL_CW_MSK;
	IOWR_ALT_UP_AUDIO_CONTROL(ALT_UP_AUDIO_BASE, ctrl_reg);
	return 0;
}

int alt_up_audio_read_left_channel(alt_u32 *buf, unsigned len)
{
	alt_u32 fifospace;
	unsigned count = 0;
	while ( count < len ) 
	{
		// read the whole fifospace register
		fifospace = IORD_ALT_UP_AUDIO_FIFOSPACE(ALT_UP_AUDIO_BASE);
		// extract the part for Left Channel Read Space
		fifospace = (fifospace & ALT_UP_AUDIO_FIFOSPACE_RALC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_RALC_OFST;
		if (fifospace > 0) 
		{
			buf[count] = IORD_ALT_UP_AUDIO_LEFTDATA(ALT_UP_AUDIO_BASE);
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

int alt_up_audio_read_right_channel(alt_u32 *buf, unsigned len)
{
	alt_u32 fifospace;
	unsigned count = 0;
	while ( count < len ) 
	{
		// read the whole fifospace register
		fifospace = IORD_ALT_UP_AUDIO_FIFOSPACE(ALT_UP_AUDIO_BASE);
		// extract the part for Right Channel Read Space
		fifospace = (fifospace & ALT_UP_AUDIO_FIFOSPACE_RARC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_RARC_OFST;
		if (fifospace > 0) 
		{
			buf[count++] = IORD_ALT_UP_AUDIO_RIGHTDATA(ALT_UP_AUDIO_BASE);
		}
		else
		{
			// no more data to read
			break;
		}
	}
	return count;
}

int alt_up_audio_write_left_channel(alt_u32 *buf, unsigned len)
{
	alt_u32 fifospace;
	unsigned count = 0;
	while ( count < len ) 
	{
		// read the whole fifospace register
		fifospace = IORD_ALT_UP_AUDIO_FIFOSPACE(ALT_UP_AUDIO_BASE);
		// extract the part for Left Channel Write Space 
		fifospace = (fifospace & ALT_UP_AUDIO_FIFOSPACE_WSLC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_WSLC_OFST;
		if (fifospace > 0) 
		{
			IOWR_ALT_UP_AUDIO_LEFTDATA(ALT_UP_AUDIO_BASE, buf[count++]);
		}
		else
		{
			// no more space to write
			break;
		}
	}
	return count;
}

int alt_up_audio_write_right_channel(alt_u32 *buf, unsigned len)
{
	alt_u32 fifospace;
	unsigned count = 0;
	while ( count < len ) 
	{
		// read the whole fifospace register
		fifospace = IORD_ALT_UP_AUDIO_FIFOSPACE(ALT_UP_AUDIO_BASE);
		// extract the part for Right Channel Write Space
		fifospace = (fifospace & ALT_UP_AUDIO_FIFOSPACE_WSRC_MSK) >> ALT_UP_AUDIO_FIFOSPACE_WSRC_OFST;
		if (fifospace > 0) 
		{
			IOWR_ALT_UP_AUDIO_RIGHTDATA(ALT_UP_AUDIO_BASE, buf[count++]);
		}
		else
		{
			// no more space to write
			break;
		}
	}
	return count;
}

