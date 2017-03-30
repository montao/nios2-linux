#ifndef __PS2_KEYBOARD_H__
#define __PS2_KEYBOARD_H__
#include "alt_up_ps2_port.h"

#define KB_RESET 0xFF
#define KB_SET_DEFAULT 0xF6
#define KB_DISABLE 0xF5
#define KB_ENABLE 0xF4
#define KB_SET_TYPE_RATE_DELAY 0xF3

/**
 * @brief The Enum type for the type of keyboard code received
 **/
typedef enum
{
	/** @brief --- Make Code that corresponds to an ASCII character. For example, the ASCII Make Code for letter <tt>A</tt> is 1C
	 */
	KB_ASCII_MAKE_CODE = 1, 
	/** @brief --- Make Code that corresponds to a non-ASCII character. For example, the Binary (Non-ASCII) Make Code for <tt>Left Alt</tt> is 11
	 */
	KB_BINARY_MAKE_CODE = 2,
	/** @brief --- Make Code that has two bytes (the first byte is E0). For example, the Long Binary Make Code for <tt>Right Alt</tt> is "E0 11"
	 */
	KB_LONG_BINARY_MAKE_CODE = 3,
	/** @brief --- Normal Break Code that has two bytes (the first byte is F0). For example, the Break Code for letter <tt>A</tt> is "F0 1C"
	 */
	KB_BREAK_CODE = 4,
	/** @brief --- Long Break Code that has three bytes (the first two bytes are E0, F0). For example, the Long Break Code for <tt>Right Alt</tt> is "E0 F0 11"
	 */
	KB_LONG_BREAK_CODE = 5,
	/** @brief --- Codes that the decode FSM cannot decode
	 */
	KB_INVALID_CODE = 6
} KB_CODE_TYPE;

/**
 * @brief Get the make code of the key when a key is pressed 
 *
 * @param decode_mode -- indicates which type of code (Make Code, Break Code, etc.) is received from the keyboard when the key is pressed 
 * @param buf  -- points to the location that stores the make code of the key pressed 
 * @note For \c KB_LONG_BINARY_MAKE_CODE and \c KB_BREAK_CODE, only the second byte is retured. For \c KB_LONG_BREAK_CODE, only the third byte is returned
 *
 * @return \c PS2_TIMEOUT on timeout, or \c PS2_ERROR on error, otherwise \c PS2_SUCCESS 
 **/
int read_make_code(KB_CODE_TYPE *decode_mode, alt_u8 *buf);

/**
 * @brief Set the repeat/delay rate of the keyboard
 *
 * @param rate -- an 8-bit number that represents the repeat/delay rate of the keyboard
 *
 * @return PS2_SUCCESS on success, otherwise PS2_ERROR
 **/
alt_u32 set_keyboard_rate(alt_u8 rate);

/**
 * @brief Send the reset command to the keyboard
 *
 * @return \c PS2_SUCCESS on passing the BAT (Basic Assurance Test), otherwise \c PS2_ERROR
 **/
alt_u32 reset_keyboard();

#endif
