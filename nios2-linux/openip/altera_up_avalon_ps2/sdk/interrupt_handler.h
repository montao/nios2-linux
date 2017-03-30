#ifndef __INTERRUPT_HANDLER__H__
#define __INTERRUPT_HANDLER__H__

#include <alt_types.h>
#include "alt_up_ps2_port.h"

/**
 * @brief disable the interrupt by setting proper bits to the NIOS II processor and the PS2 port
 **/
void disable_interrupt();

/**
 * @brief initialize the interrupt by setting proper bits to the NIOS II processor and the PS2 port
 **/
void interrupt_init();

/**
 * @brief a wrapper function for the interrupt handlers
 **/
void interrupt_handler(void);

/**
 * @brief the subroutine to be placed at address 20 (interrupt handler)
 **/
void the_isr(void);

/**
 * @brief the interrupt handler for the PS2 port
 **/
void ps2_port_isr();

#endif
