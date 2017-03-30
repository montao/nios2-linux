#include "ps2_keyboard.h"

#define SCAN_CODE_NUM  102

////////////////////////////////////////////////////////////////////
// Table of scan code, make code and their corresponding values 
// These data are useful for developing more features for the keyboard 
//
alt_u8 *key_table[SCAN_CODE_NUM] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "`", "-", "=", "\\", "BKSP", "SPACE", "TAB", "CAPS", "L SHFT", "L CTRL", "L GUI", "L ALT", "R SHFT", "R CTRL", "R GUI", "R ALT", "APPS", "ENTER", "ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "SCROLL", "[", "INSERT", "HOME", "PG UP", "DELETE", "END", "PG DN", "U ARROW", "L ARROW", "D ARROW", "R ARROW", "NUM", "KP /", "KP *", "KP -", "KP +", "KP ENTER", "KP .", "KP 0", "KP 1", "KP 2", "KP 3", "KP 4", "KP 5", "KP 6", "KP 7", "KP 8", "KP 9", "]", ";", "'", ",", ".", "/" };

alt_u8 ascii_codes[SCAN_CODE_NUM] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '`', '-', '=', 0, 0x08, 0, 0x09, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0A, 0x1B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '[', 0, 0, 0, 0x7F, 0, 0, 0, 0, 0, 0, 0, '/', '*', '-', '+', 0x0A, '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ']', ';', '\'', ',', '.', '/' };

alt_u8 single_byte_make_code[SCAN_CODE_NUM] = { 0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B, 0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A, 0x45, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x0E, 0x4E, 0x55, 0x5D, 0x66, 0x29, 0x0D, 0x58, 0x12, 0x14, 0, 0x11, 0x59, 0, 0, 0, 0, 0x5A, 0x76, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B, 0x83, 0x0A, 0x01, 0x09, 0x78, 0x07, 0x7E, 0x54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x77, 0, 0x7C, 0x7B, 0x79, 0, 0x71, 0x70, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C, 0x75, 0x7D, 0x5B, 0x4C, 0x52, 0x41, 0x49, 0x4A };

alt_u8 multi_byte_make_code[SCAN_CODE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1F, 0, 0, 0x14, 0x27, 0x11, 0x2F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70, 0x6C, 0x7D, 0x71, 0x69, 0x7A, 0x75, 0x6B, 0x72, 0x74, 0, 0x4A, 0, 0, 0, 0x5A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
////////////////////////////////////////////////////////////////////

// States for the Keyboard Decode FSM 
typedef enum
{
	STATE_INIT,
	STATE_LONG_BINARY_MAKE_CODE,
	STATE_BREAK_CODE ,
	STATE_DONE 
} DECODE_STATE;

//helper function for get_next_state
alt_u8 get_multi_byte_make_code_index(alt_u8 code)
{
	alt_u8 i;
	for (i = 0; i < SCAN_CODE_NUM; i++ )
	{
		if ( multi_byte_make_code[i] == code )
			return i;
	}
	return SCAN_CODE_NUM;
}

//helper function for get_next_state
alt_u8 get_single_byte_make_code_index(alt_u8 code)
{
	alt_u8 i;
	for (i = 0; i < SCAN_CODE_NUM; i++ )
	{
		if ( single_byte_make_code[i] == code )
			return i;
	}
	return SCAN_CODE_NUM;
}

//helper function for read_make_code
/* FSM Diagram (Main transitions)
 * Normal bytes: bytes that are not 0xF0 or 0xE0
  ______ 
  |     |                                   
  |     |
  |    INIT ------ 0xF0 ----> BREAK CODE
  |     |                     /  |
  |     |                    /   |
  |    0xE0                 /    |
 Normal |                  /    Normal
  |     |      ----0xF0--->      |
  |     V     /                  |
  |    LONG  /                   V
  | MAKE/BREAK --- Normal ----> DONE
  |    CODE                       ^
  X-------------------------------|

 */
DECODE_STATE get_next_state(DECODE_STATE state, alt_u8 byte, KB_CODE_TYPE *decode_mode, alt_u8 *buf)
{
	DECODE_STATE next_state = STATE_INIT;
	alt_u16 idx = SCAN_CODE_NUM;
	switch (state)
	{
		case STATE_INIT:
			if ( byte == 0xE0 )
			{
				next_state = STATE_LONG_BINARY_MAKE_CODE;
			}
			else if (byte == 0xF0)
			{
				next_state = STATE_BREAK_CODE;
			}
			else
			{
				idx = get_single_byte_make_code_index(byte);
				if ( (idx < 40 || idx == 68 || idx > 79) && ( idx != SCAN_CODE_NUM ) )
				{
					*decode_mode = KB_ASCII_MAKE_CODE;
					*buf= ascii_codes[idx];
				}
				else 
				{
					*decode_mode = KB_BINARY_MAKE_CODE;
					*buf = byte;
				}
				next_state = STATE_DONE;
			}
			break;
		case STATE_LONG_BINARY_MAKE_CODE:
			if ( byte != 0xF0 && byte!= 0xE0)
			{
				*decode_mode = KB_LONG_BINARY_MAKE_CODE;
				*buf = byte;
				next_state = STATE_DONE;
			}
			else
			{
				next_state = STATE_BREAK_CODE;
			}
			break;
		case STATE_BREAK_CODE:
			if ( byte != 0xF0 && byte != 0xE0)
			{
				*decode_mode = KB_BREAK_CODE;
				*buf = byte;
				next_state = STATE_DONE;
			}
			else
			{
				next_state = STATE_BREAK_CODE;
			}
			break;
		default:
			*decode_mode = KB_INVALID_CODE;
			next_state = STATE_INIT;
	}
	return next_state;
}

int read_make_code(KB_CODE_TYPE *decode_mode, alt_u8 *buf)
{
	alt_u8 byte = 0;
	int status_read =0;
	*decode_mode = KB_INVALID_CODE;
	DECODE_STATE state = STATE_INIT;
	do
	{
		status_read = read_data_byte_with_timeout(&byte, 0);
		//FIXME: When the user press the keyboard extremely fast, data may get
		//occasionally get lost 

		if (status_read == PS2_ERROR)
			return PS2_ERROR;

		state = get_next_state(state, byte, decode_mode, buf);
	} while (state != STATE_DONE);

	return PS2_SUCCESS;
}

alt_u32 set_keyboard_rate(alt_u8 rate)
{
	alt_u8 byte;
	// send the set keyboard rate command
	int status_send = write_data_byte_with_ack(0xF3, DEFAULT_PS2_TIMEOUT_VAL);
	if ( status_send == PS2_SUCCESS )
	{
		// we received ACK, so send out the desired rate now
		status_send = write_data_byte_with_ack(rate & 0x1F, DEFAULT_PS2_TIMEOUT_VAL);
	}
	return status_send;
}

alt_u32 reset_keyboard()
{
	alt_u8 byte;
	// send out the reset command
	int status = write_data_byte_with_ack(0xff, DEFAULT_PS2_TIMEOUT_VAL); 
	if ( status == PS2_SUCCESS)
	{
		// received the ACK for reset, now check the BAT result
		status = read_data_byte_with_timeout(&byte, DEFAULT_PS2_TIMEOUT_VAL);
		if (status == PS2_SUCCESS && byte == 0xAA)
		{
			// BAT succeed
		}
		else
		{
			// BAT failed
			status == PS2_ERROR;
		}
	}	
	return status;
}

