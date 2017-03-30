#include <nios2.h>
#include "alt_up_ps2_port.h"

#define PS2_ACK 	(0xFA)

PS2_DEVICE get_mode()
{
	alt_u8 byte;
	//send the reset request, wait for ACK
	int status = write_data_byte_with_ack(0xff, DEFAULT_PS2_TIMEOUT_VAL);
	if (status == PS2_SUCCESS)
	{
		// reset succeed, now try to get the BAT result, AA means passed
		status = read_data_byte_with_timeout(&byte, DEFAULT_PS2_TIMEOUT_VAL);
		if (status == PS2_SUCCESS && byte == 0xAA)
		{
			//get the 2nd byte
			status = read_data_byte_with_timeout(&byte, DEFAULT_PS2_TIMEOUT_VAL);
			if (status == PS2_TIMEOUT)
			{
				//for keyboard, only 2 bytes are sent(ACK, PASS/FAIL), so timeout
				return PS2_KEYBOARD;
			}
			else if (status == PS2_SUCCESS && byte == 0x00)
			{
				//for mouse, it will sent out 0x00 after sending out ACK and PASS/FAIL.
				return PS2_MOUSE;
			}
		}
	}
	// when writing data to the PS/2 device, error occurs...
	return PS2_UNKNOWN;
}

void clear_FIFO()
{
	// The DATA byte of the data register will be automatically cleared after a read
	// So we simply keep reading it until there are no available bytes
	alt_u16 num = 0;
	alt_u32 data_reg = 0;
	do
	{
		// read the data register (the DATA byte is cleared)
		data_reg = read_data_reg();
		// get the number of available bytes from the RAVAIL part of data register
		num = read_num_bytes_available(data_reg);
	} while (num > 0);
}

////////////////////////////////////////////////////////////
// Control Register Operations
void write_ctrl_reg(alt_u32 ctrl_data)
{
	IOWR_ALT_UP_PS2_PORT_CONTROL(ALT_UP_PS2_BASE, ctrl_data);
}

alt_u32 read_ctrl_reg()
{
	alt_u32 ctrl_reg = IORD_ALT_UP_PS2_PORT_CONTROL(ALT_UP_PS2_BASE);
	return ctrl_reg;
}

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

////////////////////////////////////////////////////////////
// Data Register Operations

alt_u32 read_data_reg()
{
	alt_u32 data_reg = IORD_ALT_UP_PS2_PORT_DATA(ALT_UP_PS2_BASE);
	return data_reg;
}

/*
alt_u8 read_data_valid(alt_u32 data_reg)
{
	alt_u8 rvalid = (alt_u8)((data_reg & ALT_UP_PS2_PORT_DATA_REG_RVALID_MSK) >> ALT_UP_PS2_PORT_DATA_REG_RVALID_OFST);
	return rvalid;
}
*/


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

int write_data_byte(alt_u8 byte)
{
	//note: data are only located at the lower 8 bits
	//note: the software send command to the PS2 peripheral through the data
	//		register rather than the control register
	IOWR_ALT_UP_PS2_PORT_DATA(ALT_UP_PS2_BASE, byte);
	alt_u32 ctrl_reg = IORD_ALT_UP_PS2_PORT_DATA(ALT_UP_PS2_BASE);
	if (read_CE_bit(ctrl_reg))
	{
		//CE bit is set --> error occurs on sending commands
		return PS2_ERROR;
	}
	return PS2_SUCCESS;
}

int write_data_byte_with_ack(alt_u8 byte, unsigned timeout)
{
	//note: data are only located at the lower 8 bits
	//note: the software send command to the PS2 peripheral through the data
	//		register rather than the control register
	int send_status = write_data_byte(byte);
	if ( send_status != PS2_SUCCESS )
		// return on sending error
		return send_status;

	int ack_status = wait_for_ack(timeout);
	return ack_status;
}

int read_data_byte_with_timeout(alt_u8 *byte, alt_u32 time_out)
{
	alt_u32 data_reg = 0; 
	alt_u16 num = 0; 
	alt_u32 count = 0;
	do
	{
		count++;
		data_reg = read_data_reg();
		num = read_num_bytes_available(data_reg);
		if (num > 0)
		{
			*byte = read_data_byte(data_reg);
			return PS2_SUCCESS;
		}
		//timeout = 0 means to disable the timeout
		if ( time_out!= 0 && count > time_out)
		{
			return PS2_TIMEOUT;
		}
	} while (1);
	// return PS2_SUCCESS;
}

int wait_for_ack(unsigned timeout)
{
	alt_u8 ack = 0;
	alt_u8 data = 0;
	alt_u8 status = PS2_SUCCESS;
	do
	{
		status = read_data_byte_with_timeout(&data, timeout); 
		if ( status == PS2_SUCCESS )
		{
			if (data == PS2_ACK)
				return PS2_SUCCESS;
		}
		else 
		{
			return status;
		}
	} while(1);
	return PS2_TIMEOUT;
}
