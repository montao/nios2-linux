#include "ps2_mouse.h"

alt_u8 reset_mouse()
{
	alt_u8 byte;

	//send the reset command
	int status = write_data_byte_with_ack(MOUSE_RESET, DEFAULT_PS2_TIMEOUT_VAL);
	if (status == PS2_SUCCESS) 
	{
		// get the ACK, now read the BAT result
		status = read_data_byte_with_timeout(&byte, DEFAULT_PS2_TIMEOUT_VAL);
		if ( status == PS2_SUCCESS && byte == 0xAA) 
		{
			// BAT is passed, now get the device ID
			status = read_data_byte_with_timeout(&byte, DEFAULT_PS2_TIMEOUT_VAL); 
			// mouse should return 0x00 for device ID
			if ( byte == 0x00)
				status = PS2_SUCCESS;
			else 
				status = PS2_ERROR;
		}
	}
	return status;
}

int set_mouse_mode(alt_u8 byte)
{
	return write_data_byte_with_ack(byte, DEFAULT_PS2_TIMEOUT_VAL);
}

