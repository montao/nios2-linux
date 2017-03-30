#ifndef __FIFOED_AVALON_UART_H__
#define __FIFOED_AVALON_UART_H__

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

#include <stddef.h>
#include <sys/termios.h>

#include "sys/alt_dev.h"
#include "sys/alt_warning.h"

#include "os/alt_sem.h"
#include "os/alt_flag.h"
#include "alt_types.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined ALT_USE_SMALL_DRIVERS || defined FIFOED_AVALON_UART_SMALL

/*
 * The fifoed_avalon_uart_dev structure is used to hold device specific data. This
 * includes the transmit and receive buffers.
 *
 * An instance of this structure is created in the auto-generated 
 * alt_sys_init.c file for each UART listed in the systems PTF file. This is
 * done using the FIFOED_AVALON_UART_INSTANCE macro given below.
 */

typedef struct
{
  alt_dev        dev;
  int            base;
} fifoed_avalon_uart_dev;

/*
 * fifoed_avalon_uart_read() is called by the read() system call for all valid
 * attempts to read from an instance of this device.
 */

extern int fifoed_avalon_uart_read (alt_fd* fd, char* ptr, int len);

/*
 * fifoed_avalon_uart_write() is called by the write() system call for all valid
 * attempts to write to an instance of this device.
 */

extern int fifoed_avalon_uart_write (alt_fd* fd, const char* ptr, int len);

/*
 * The macro FIFOED_AVALON_UART_INSTANCE is used by the auto-generated file
 * alt_sys_init.c to create an instance of this device driver.
 */

#define FIFOED_AVALON_UART_INSTANCE(name, device)   \
  static fifoed_avalon_uart_dev device =               \
    {                                               \
      {                                             \
        ALT_LLIST_ENTRY,                            \
        name##_NAME,                                \
        NULL, /* open */                            \
        NULL, /* close */                           \
        fifoed_avalon_uart_read,                       \
        fifoed_avalon_uart_write,                      \
        NULL, /* lseek */                           \
        NULL, /* fstat */                           \
        NULL, /* ioctl */                           \
      },                                            \
      name##_BASE                                   \
    }

/*
 * The macro FIFOED_AVALON_UART_INIT is used by the auto-generated file
 * alt_sys_init.c to initialise an instance of the device driver.
 */

#define FIFOED_AVALON_UART_INIT(name, device) alt_dev_reg (&device.dev)

#else /* use fast version of the driver */

/*
 * FIFOED_AVALON_UART_BUF_LEN is the length of the circular buffers used to hold
 * pending transmit and receive data. This value must be a power of two.
 */

#define FIFOED_AVALON_UART_BUF_LEN (64)

/*
 * FIFOED_AVALON_UART_BUF_MSK is used as an internal convenience for detecting 
 * the end of the arrays used to implement the transmit and receive buffers.
 */

#define FIFOED_AVALON_UART_BUF_MSK (FIFOED_AVALON_UART_BUF_LEN - 1)

/*
 * This is somewhat of an ugly hack, but we need some mechanism for
 * representing the non-standard 9 bit mode provided by this UART. In this
 * case we abscond with the 5 bit mode setting. The value CS5 is defined in
 * termios.h.
 */

#define CS9 CS5

/*
 * The value FIFOED_AVALON_UART_FB is a value set in the devices flag field to
 * indicate that the device has a fixed baud rate; i.e. if this flag is set
 * software can not control the baud rate of the device.
 */

#define FIFOED_AVALON_UART_FB 0x1

/*
 * The value FIFOED_AVALON_UART_FC is a value set in the device flag field to
 * indicate the the device is using flow control, i.e. the driver must 
 * throttle on transmit if the nCTS pin is low.
 */

#define FIFOED_AVALON_UART_FC 0x2

/*
 * The fifoed_avalon_uart_dev structure is used to hold device specific data. This
 * includes the transmit and receive buffers.
 *
 * An instance of this structure is created in the auto-generated 
 * alt_sys_init.c file for each UART listed in the systems PTF file. This is
 * done using the FIFOED_AVALON_UART_INSTANCE macro given below.
 */

typedef struct
{
  alt_dev          dev;             /* The device callback structure */
  void*            base;            /* The base address of the device */
  alt_u32          ctrl;            /* Shadow value of the control register */
  alt_u32          rx_start;        /* Start of the pending receive data */
  volatile alt_u32 rx_end;          /* End of the pending receive data */
  volatile alt_u32 tx_start;        /* Start of the pending transmit data */
  alt_u32          tx_end;          /* End of the pending transmit data */
#ifdef FIFOED_AVALON_UART_USE_IOCTL
  struct termios termios;           /* Current device configuration */
  alt_u32          freq;            /* Current baud rate */
#endif
  alt_u32          flags;           /* Configuation flags */
  ALT_FLAG_GRP     (events)         /* Event flags used for 
                                     * foreground/background in mult-threaded
                                     * mode */
  ALT_SEM          (read_lock)      /* Semaphore used to control access to the 
                                     * read buffer in multi-threaded mode */
  ALT_SEM          (write_lock)     /* Semaphore used to control access to the
                                     * write buffer in multi-threaded mode */
  alt_u8           rx_buf[FIFOED_AVALON_UART_BUF_LEN]; /* The receive buffer */
  alt_u8           tx_buf[FIFOED_AVALON_UART_BUF_LEN]; /* The transmit buffer */
} fifoed_avalon_uart_dev;

/*
 * fifoed_avalon_uart_init() is called by the auto-generated function 
 * alt_sys_init() for each UART in the system. This is done using the 
 * FIFOED_AVALON_UART_INIT macro given below.
 *
 * This function is responsible for performing all the run time initilisation
 * for a device instance, i.e. registering the interrupt handler, and 
 * regestering the device with the system.
 */

extern void fifoed_avalon_uart_init (fifoed_avalon_uart_dev* dev,
                                  void*                base,
                                  alt_u32              irq);

/*
 * fifoed_avalon_uart_read() is called by the read() system call for all valid
 * attempts to read from an instance of this device.
 */

extern int fifoed_avalon_uart_read (alt_fd* fd, char* ptr, int len);

/*
 * fifoed_avalon_uart_write() is called by the write() system call for all valid
 * attempts to write to an instance of this device.
 */

extern int fifoed_avalon_uart_write (alt_fd* fd, const char* ptr, int len);

/*
 * fifoed_avalon_uart_ioctl() is called by the ioctl() system call for all ioctl
 * calls for instances of this device that the ioctl() system call cannot
 * handle itself.
 *
 * In order to reduce the footprint of this device driver, this feature is
 * only enabled if the macro FIFOED_AVALON_UART_USE_IOCTL is defined.
 */

extern int fifoed_avalon_uart_ioctl (alt_fd* fd, int req, void* arg);

/*
 * Conditionally define the data structures used to process ioctl requests.
 * The following macros are defined for use in creating a device instance:
 *
 * FIFOED_AVALON_UART_TERMIOS - Initialise the termios structure used to
 *                              describe the UART configuration.
 * FIFOED_AVALON_UART_FREQ    - Initialise the 'freq' field of the device
 *                              structure, if the field exists.
 * FIFOED_AVALON_UART_IOCTL   - Initialise the 'ioctl' field of the device
 *                              callback structure, if ioctls are enabled.
 */

#ifdef FIFOED_AVALON_UART_USE_IOCTL

#define FIFOED_AVALON_UART_TERMIOS(stop_bits,               \
                                   parity,                  \
                                   odd_parity,              \
                                   data_bits,               \
                                   ctsrts,                  \
                                   baud)                    \
{                                                           \
  0,                                                        \
  0,                                                        \
  ((stop_bits == 2) ? CSTOPB: 0)      |                     \
    ((parity) ? PARENB: 0)            |                     \
    ((odd_parity) ? PAODD: 0)         |                     \
    ((data_bits == 7) ? CS7: (data_bits == 9) ? CS9: CS8) | \
    ((ctsrts) ? CRTSCTS : 0),                               \
  0,                                                        \
  0,                                                        \
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},                  \
  baud,                                                     \
  baud                                                      \
},
#define FIFOED_AVALON_UART_FREQ(name) name##_FREQ,
#define FIFOED_AVALON_UART_IOCTL fifoed_avalon_uart_ioctl

#else 

#define FIFOED_AVALON_UART_TERMIOS(stop_bits,  \
                                   parity,     \
                                   odd_parity, \
                                   data_bits,  \
                                   ctsrts,     \
                                   baud)
#define FIFOED_AVALON_UART_FREQ(name)
#define FIFOED_AVALON_UART_IOCTL NULL

#endif /* FIFOED_AVALON_UART_USE_IOCTL */

/*
 * The macro FIFOED_AVALON_UART_INSTANCE is used by the auto-generated file
 * alt_sys_init.c to create an instance of this device driver.
 */

#define FIFOED_AVALON_UART_INSTANCE(name, dev)         \
  static fifoed_avalon_uart_dev dev =                     \
   {                                                   \
     {                                                 \
       ALT_LLIST_ENTRY,                                \
       name##_NAME,                                    \
       NULL, /* open */                                \
       NULL, /* close */                               \
       fifoed_avalon_uart_read,                           \
       fifoed_avalon_uart_write,                          \
       NULL, /* lseek */                               \
       NULL, /* fstat */                               \
       FIFOED_AVALON_UART_IOCTL,                       \
     },                                                \
     (void*) name##_BASE,                              \
     0,                                                \
     0,                                                \
     0,                                                \
     0,                                                \
     0,                                                \
     FIFOED_AVALON_UART_TERMIOS(name##_STOP_BITS,      \
                               (name##_PARITY == 'N'), \
                               (name##_PARITY == 'O'), \
                               name##_DATA_BITS,       \
                               name##_USE_CTS_RTS,     \
                               name##_BAUD)            \
     FIFOED_AVALON_UART_FREQ(name)                     \
     (name##_FIXED_BAUD ? FIFOED_AVALON_UART_FB : 0) |    \
       (name##_USE_CTS_RTS ? FIFOED_AVALON_UART_FC : 0)   \
   }

/*
 * The macro FIFOED_AVALON_UART_INIT is used by the auto-generated file
 * alt_sys_init.c to initialise an instance of the device driver. 
 *
 * This macro performs a sanity check to ensure that the interrupt has been
 * connected for this device. If not, then an apropriate error message is 
 * generated at build time.
 */

#define FIFOED_AVALON_UART_INIT(name, dev)                                 \
  if (name##_IRQ == ALT_IRQ_NOT_CONNECTED)                                 \
  {                                                                        \
    ALT_LINK_ERROR ("Error: Interrupt not connected for " #dev ". "        \
                    "You have selected the interrupt driven version of "   \
                    "the ALTERA Avalon UART driver, but the interrupt is " \
                    "not connected for this device. You can select a "     \
                    "polled mode driver by checking the 'small driver' "   \
                    "option in the HAL configuration window, or by "       \
                    "using the -DFIFOED_AVALON_UART_SMALL preprocessor "   \
                    "flag.");                                              \
  }                                                                        \
  else                                                                     \
  {                                                                        \
    fifoed_avalon_uart_init (&dev, (void*) name##_BASE, name##_IRQ);          \
  }

/*
 *
 */

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FIFOED_AVALON_UART_H__ */
