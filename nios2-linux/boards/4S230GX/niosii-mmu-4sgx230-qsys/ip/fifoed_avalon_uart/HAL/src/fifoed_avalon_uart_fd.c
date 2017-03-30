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

#include "alt_types.h"
#include "sys/alt_dev.h"
#include "fifoed_avalon_uart.h"

extern int fifoed_avalon_uart_read(fifoed_avalon_uart_state* sp,
  char* buffer, int space, int flags);
extern int fifoed_avalon_uart_write(fifoed_avalon_uart_state* sp,
  const char* ptr, int count, int flags);
extern int fifoed_avalon_uart_ioctl(fifoed_avalon_uart_state* sp,
  int req, void* arg);

int fifoed_avalon_uart_classic_read(fifoed_avalon_uart_state* sp,
  char* buffer, int space, int flags)
  {
     return fifoed_avalon_uart_read( sp,
  buffer, space,  flags);
  }

  int fifoed_avalon_uart_classic_write(fifoed_avalon_uart_state* sp,
  const char* ptr, int count, int flags)
  {
     return fifoed_avalon_uart_write( sp,
  ptr, count,  flags);
  }

/* ----------------------------------------------------------------------- */
/* --------------------- WRAPPERS FOR ALT FD SUPPORT --------------------- */
/*
 *
 */

int
fifoed_avalon_uart_read_fd(alt_fd* fd, char* buffer, int space)
{
    fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

    return fifoed_avalon_uart_read(&dev->state, buffer, space,
      fd->fd_flags);
}

int
fifoed_avalon_uart_write_fd(alt_fd* fd, const char* buffer, int space)
{
    fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

    return fifoed_avalon_uart_write(&dev->state, buffer, space,
      fd->fd_flags);
}
int
 fifoed_avalon_uart_classic_read_fd(alt_fd* fd, char* buffer, int space)
{
    fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

    return fifoed_avalon_uart_read(&dev->state, buffer, space,
      fd->fd_flags);
}

int
fifoed_avalon_uart_classic_write_fd(alt_fd* fd, const char* buffer, int space)
{
    fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

    return fifoed_avalon_uart_write(&dev->state, buffer, space,
      fd->fd_flags);
}
#if !defined(ALT_USE_SMALL_DRIVERS) && !defined(FIFOED_AVALON_UART_SMALL)

/*
 * Fast driver
 */

/*
 * To reduce the code footprint of this driver, the ioctl() function is not
 * included by default. If you wish to use the ioctl features provided
 * below, you can do so by adding the option : -DFIFOED_AVALON_UART_USE_IOCTL
 * to CPPFLAGS in the Makefile (or through the Eclipse IDE).
 */

#ifdef FIFOED_AVALON_UART_USE_IOCTL

int
fifoed_avalon_uart_ioctl_fd(alt_fd* fd, int req, void* arg)
{
    fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

    return fifoed_avalon_uart_ioctl(&dev->state, req, arg);
}
fifoed_avalon_uart_classic_ioctl_fd(alt_fd* fd, int req, void* arg)
{
    fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

    return fifoed_avalon_uart_ioctl(&dev->state, req, arg);
}

#endif /* FIFOED_AVALON_UART_USE_IOCTL */

#endif /* fast driver */
