#ifndef __PS2_INTERFACE_H__	
#define __PS2_INTERFACE_H__	

#include <io.h>
#include <alt_types.h>
#include "system.h"
#include "alt_up_ps2_port_regs.h"

/**
 * @brief The Enum type for PS/2 device type
 **/
typedef enum{
	// @brief Indicate that the device is a PS/2 Mouse.
	PS2_MOUSE = 0,
	// @brief Indicate that the device is a PS/2 Keyboard.
	PS2_KEYBOARD = 1,
	// @brief The program cannot determine what type the device is.
	PS2_UNKNOWN = 2
} PS2_DEVICE;

//when we dont' do anything else durng polling the keyboard, the timeout value need to be large
#define DEFAULT_PS2_TIMEOUT_VAL 	(0x001fffff)

#define PS2_SUCCESS (0)
#define PS2_TIMEOUT (-1)
#define PS2_ERROR 	(-2)

#define PS2_ACK 	(0xFA)

////////////////////////////////////////////////////////////
// Control Register Operations

/**
 * @brief Read the contents of the Control register for the PS/2 port.
 *
 * @return Register contents (32 bits, bits 10, 8, and 0 are used for CE, RI and RE respectively. Other bits are reserved).
 **/
alt_u32 read_ctrl_reg();

/**
 * @brief Set the contents of the Control register.
 *
 * @param ctrl_data -- contents to be written into the Control register.
 *
 **/
void write_ctrl_reg(alt_u32 ctrl_data);

/**
 * @brief Extract the RI (Read Interrupt) bit from the Control register.
 *
 * @param ctrl_reg -- the Control register.
 *
 * @return 8-bit number, where bit 0 is the value of the RI bit.
 **/
alt_u8 read_RI_bit(alt_u32 ctrl_reg);

/**
 * @brief Extract the RE (Read Interrupt Enable) bit from the Control register.
 *
 * @param ctrl_reg -- the Control register.
 *
 * @return 8-bit number, where bit 0 is the value of the RE bit.
 **/
alt_u8 read_RE_bit(alt_u32 ctrl_reg);

/**
 * @brief Extract the CE (Command Error) bit from the Control register.
 *
 * @param ctrl_reg -- the Control register.
 *
 * @return 8-bit number, where bit 0 is the value of the CE bit.
 **/
alt_u8 read_CE_bit(alt_u32 ctrl_reg);

////////////////////////////////////////////////////////////
// Data Register Operations

/**
 * @brief Read the contents of the Data register.
 *
 * @return 32 bits of the Data register. Bits 31-16 indicate the number of available bytes in the FIFO (RAVAIL), bits 7-0 are the data received from the PS/2 device.
 *
 **/
alt_u32 read_data_reg();

/**
 * @brief Read the DATA byte from the Data register.
 *
 * @param data_reg  -- Data register.
 *
 * @return Bits 7-0 of the Data register.
 **/
alt_u8 read_data_byte(alt_u32 data_reg);

/**
 * @brief Find the number of bytes available to read from the FIFO buffer of the PS/2 port.
 *
 * @param data_reg  -- the Data register.
 *
 * @return The number represented by bits 31-16 of the Data register.
 **/
alt_u16 read_num_bytes_available(alt_u32 data_reg);

////////////////////////////////////////////////////////////
// Actions

/**
 * @brief Check the PS/2 peripheral's mode (whether it is a keyboard or a mouse).
 *
 * @return PS2_MOUSE for mouse, PS2_KEYBOARD for keyboard, or PS2_UNKNOWN if undetermined.
 *
 * @note This operation will \b reset the PS/2 peripheral. Usually,  it should be used only at the beginning of a program.
 **/
PS2_DEVICE get_mode();

/**
 * @brief Clear the FIFO's contents.
 **/
void clear_FIFO();

/**
 * @brief Wait for the acknowledge byte (0xFA) from the PS/2 peripheral.
 *
 * @param timeout -- the number of cycles to allow before timeout.
 *
 * @return \c PS2_SUCCESS on receving ACK signal, or \c PS2_TIMEOUT on timeout.
 **/
int wait_for_ack(unsigned timeout);

/**
 * @brief Send a one-byte command to the PS/2 peripheral.
 *
 * @param byte -- the one-byte command to be sent.
 *
 * @return \c PS2_ERROR if the CE bit of the Control register is set to 1, otherwise \c PS2_SUCCESS.
 **/
int write_data_byte(alt_u8 byte);

/**
 * @brief Send a one-byte command to the PS/2 peripheral and wait for the ACK signal.
 *
 * @param byte -- the one-byte command to be sent. See <tt> alt_up_ps2_port_regs.h </tt> in the sdk directory or any reference for the PS/2 protocol for details.
 *
 * @return \c PS2_ERROR if the CE bit of the Control register is set to 1, or \c PS2_TIMEOUT on timeout, or \c PS2_SUCCESS if the ACK signal is received before timeout.
 **/
int write_data_byte_with_ack(alt_u8 byte, unsigned timeout);

/**
 * @brief Read the DATA byte from the PS/2 FIFO, using a user-defined timeout value.
 *
 * @param byte  -- the byte read from the FIFO for the PS/2 Core.
 * @param time_out  -- the user-defined timeout value. Setting \em time_out to 0 will disable the time-out mechanism.
 *
 * @return \c PS2_SUCCESS on reading data, or \c PS2_TIMEOUT on timeout.
 **/
int read_data_byte_with_timeout(alt_u8 *byte, alt_u32 time_out);

#endif
