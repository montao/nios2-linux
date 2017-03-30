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

#include <fcntl.h>

#include "sys/alt_dev.h"
#include "sys/alt_irq.h"
#include "sys/ioctl.h"
#include "sys/alt_errno.h"

#include "fifoed_avalon_uart.h"
#include "fifoed_avalon_uart_regs.h"

/*
 * FIFOED_AVALON_UART_READ_RDY and FIFOED_AVALON_UART_WRITE_RDY are the bitmasks 
 * that define uC/OS-II event flags that are releated to this device.
 *
 * FIFOED_AVALON_UART_READY_RDY indicates that there is read data in the buffer 
 * ready to be processed. ALT_UART_WRITE_RDY indicates that the transmitter is
 * ready for more data.
 */

#define ALT_UART_READ_RDY  0x1
#define ALT_UART_WRITE_RDY 0x2


#if defined ALT_USE_SMALL_DRIVERS || FIFOED_AVALON_UART_SMALL

/*
 * Implementation of the callback functions used for the
 * "small", i.e. polled mode, version of the altera_avalon_uart device
 * driver.
 */

/*
 * fifoed_avalon_uart_read() is called by the system read() function in order to
 * read a block of data from the UART. "len" is the maximum length of the data
 * to read, and "ptr" indicates the destination address. "fd" is the file
 * descriptor for the device to be read from.
 *
 * Permission checks are made before the call to fifoed_avalon_uart_read(), so we
 * know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * The return value is the number of bytes actually read.
 *
 * This implementation polls the device waiting for characters. At most it can
 * only return one character, regardless of how many are requested. If the 
 * device is being accessed in non-blocking mode then it is possible for this
 * function to return without reading any characters. In this case errno is
 * set to EWOULDBLOCK.
 */
 // thanks to tdudzik for the change in the driver. nov 18 2011

int fifoed_avalon_uart_wide_read (fifoed_avalon_uart_state* sp, short* ptr, int len, int flags)
{
  int block;
  unsigned int status;

  block = !(flags & O_NONBLOCK);
  int i=0;
  do
  {
    while ((status = IORD_FIFOED_AVALON_UART_STATUS(sp->base)& FIFOED_AVALON_UART_CONTROL_RRDY_MSK)&& i<len)
    {
      IOWR_FIFOED_AVALON_UART_STATUS(sp->base, 0);  // clear flags
      ptr[i++] = IORD_FIFOED_AVALON_UART_RXDATA(sp->base);
    }
    if( i>0)  // we have gotten something return it
      return i;
  while (block);

  ALT_ERRNO = EWOULDBLOCK;

  return 0;
}

/*
 * fifoed_avalon_uart_write() is called by the system write() function in order to
 * write a block of data to the UART. "len" is the length of the data to write,
 * and "ptr" indicates the source address. "fd" is the file descriptor for the 
 * device to be read from.
 *
 * Permission checks are made before the call to fifoed_avalon_uart_write(), so we
 * know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * The return value is the number of bytes actually written.
 *
 * This function will block on the devices transmit register, until all 
 * characters have been transmitted. This is unless the device is being 
 * accessed in non-blocking mode. In this case this function will return as 
 * soon as the device reports that it is not ready to transmit.
 *
 * Since this is the small footprint version of the UART driver, the value of 
 * CTS is ignored.
 */

int fifoed_avalon_uart_wide_write (fifoed_avalon_uart_state* sp, const short* ptr, int len, int flags)
{
  int block;
  unsigned int status;
  int count;

  block = !(flags & O_NONBLOCK);
  count = len;

  do
  {
    status = IORD_FIFOED_AVALON_UART_STATUS(sp->base);
   
    if (status & FIFOED_AVALON_UART_STATUS_TRDY_MSK)
    {
      IOWR_FIFOED_AVALON_UART_TXDATA(sp->base, *ptr++);
      count--;
    }
  }
  while (block && count);

  if (count)
  {
    ALT_ERRNO = EWOULDBLOCK;
  }

  return (len - count);
}



#else /* Using the "fast" version of the driver */

/*
 * Implementation of the callback functions used for the
 * "fast", i.e. interrupt driven, version of the altera_avalon_uart device
 * driver.
 */


/*
 * fifoed_avalon_uart_read() is called by the system read() function in order to
 * read a block of data from the UART. "len" is the maximum length of the data
 * to read, and "ptr" indicates the destination address. "fd" is the file
 * descriptor for the device to be read from.
 *
 * Permission checks are made before the call to fifoed_avalon_uart_read(), so we
 * know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * The return value is the number of bytes actually read.
 *
 * This function does not communicate with the device directly. Instead data is
 * transfered from a circular buffer. The interrupt handler is then responsible
 * for copying data from the device into this buffer.
 */

int fifoed_avalon_uart_wide_read (fifoed_avalon_uart_state* sp, short* ptr, int len, int flags)
{
  alt_irq_context context;
  int             block;
  alt_u32         next;

  int count                = 0;

  /* 
   * Construct a flag to indicate whether the device is being accessed in
   * blocking or non-blocking mode.
   */

  block = !(flags & O_NONBLOCK);

  /*
   * When running in a multi threaded environment, obtain the "read_lock"
   * semaphore. This ensures that reading from the device is thread-safe.
   */

  ALT_SEM_PEND (sp->read_lock, 0);

  /*
   * Calculate which slot in the circular buffer is the next one to read
   * data from.
   */

  next = (sp->rx_start + 1) & FIFOED_AVALON_UART_BUF_MSK;

  /*
   * Loop, copying data from the circular buffer to the destination address
   * supplied in "ptr". This loop is terminated when the required number of
   * bytes have been read. If the circular buffer is empty, and no data has
   * been read, then the loop will block (when in blocking mode).
   *
   * If the circular buffer is empty, and some data has already been 
   * transferred, or the device is being accessed in non-blocking mode, then
   * the loop terminates without necessarily reading all the requested data.
   */

  do
  {
    /*
     * Read the required amount of data, until the circular buffer runs
     * empty
     */

    while ((count < len) && (sp->rx_start != sp->rx_end))
    {
      count++;
      *ptr++ = sp->rx_buf[sp->rx_start];
      
      sp->rx_start = (++sp->rx_start) & FIFOED_AVALON_UART_BUF_MSK;
    }

    /*
     * If no data has been transferred, the circular buffer is empty, and
     * this is not a non-blocking access, block waiting for data to arrive.
     */

    if (!count && (sp->rx_start == sp->rx_end))
    {
      if (!block)
      {
        /* Set errno to indicate the reason we're not returning any data */

        ALT_ERRNO = EWOULDBLOCK;
        break;
      }
      else
      {
       /* Block waiting for some data to arrive */

       /* First, ensure read interrupts are enabled to avoid deadlock */

       context = alt_irq_disable_all ();
       sp->ctrl |= FIFOED_AVALON_UART_CONTROL_RRDY_MSK;
       IOWR_FIFOED_AVALON_UART_CONTROL(sp->base, sp->ctrl);
       alt_irq_enable_all (context);

       /*
        * When running in a multi-threaded mode, we pend on the read event 
        * flag set in the interrupt service routine. This avoids wasting CPU
        * cycles waiting in this thread, when we could be doing something more 
        * profitable elsewhere.
        */

       ALT_FLAG_PEND (sp->events,
                      ALT_UART_READ_RDY,
                      OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
                      0);
      }
    }
  }
  while (!count && len);

  /*
   * Now that access to the circular buffer is complete, release the read
   * semaphore so that other threads can access the buffer.
   */

  ALT_SEM_POST (sp->read_lock);

  /*
   * Ensure that interrupts are enabled, so that the circular buffer can
   * re-fill.
   */

  context = alt_irq_disable_all ();
  sp->ctrl |= FIFOED_AVALON_UART_CONTROL_RRDY_MSK;
  IOWR_FIFOED_AVALON_UART_CONTROL(sp->base, sp->ctrl);
  alt_irq_enable_all (context);

  /* Return the number of bytes read */

  return count;
}

/*
 * fifoed_avalon_uart_write() is called by the system write() function in order to
 * write a block of data to the UART. "len" is the length of the data to write,
 * and "ptr" indicates the source address. "fd" is the file descriptor for the 
 * device to be read from.
 *
 * Permission checks are made before the call to fifoed_avalon_uart_write(), so we
 * know that the file descriptor has been opened with the correct permissions
 * for this operation.
 *
 * The return value is the number of bytes actually written.
 *
 * This function does not communicate with the device directly. Instead data is
 * transfered to a circular buffer. The interrupt handler is then responsible
 * for copying data from this buffer into the device.
 */

int fifoed_avalon_uart_wide_write (fifoed_avalon_uart_state* sp, const short* ptr, int len, int flags)
{
  alt_irq_context context;
  int             no_block;
  alt_u32         next;
  int count                = len;

  /* 
   * Construct a flag to indicate whether the device is being accessed in
   * blocking or non-blocking mode.
   */

  no_block = (flags & O_NONBLOCK);

  /*
   * When running in a multi threaded environment, obtain the "write_lock"
   * semaphore. This ensures that writing to the device is thread-safe.
   */

  ALT_SEM_PEND (sp->write_lock, 0);

  /*
   * Loop transferring data from the input buffer to the transmit circular
   * buffer. The loop is terminated once all the data has been transferred,
   * or, (if in non-blocking mode) the buffer becomes full.
   */

  while (count)
  {
    /* Determine the next slot in the buffer to access */

    next = (sp->tx_end + 1) & FIFOED_AVALON_UART_BUF_MSK;

    /* block waiting for space if necessary */

    if (next == sp->tx_start)
    {
      if (no_block)
      {
        /* Set errno to indicate why this function returned early */
 
        ALT_ERRNO = EWOULDBLOCK;
        break;
      }
      else
      {
        /* Block waiting for space in the circular buffer */

        /* First, ensure transmit interrupts are enabled to avoid deadlock */

        context = alt_irq_disable_all ();
        sp->ctrl |= (FIFOED_AVALON_UART_CONTROL_TRDY_MSK |
                        FIFOED_AVALON_UART_CONTROL_DCTS_MSK);
        IOWR_FIFOED_AVALON_UART_CONTROL(sp->base, sp->ctrl);
        alt_irq_enable_all (context);

        /* wait for space to come free */

        do
        {
          /*
           * When running in a multi-threaded mode, we pend on the write event 
           * flag set in the interrupt service routine. This avoids wasting CPU
           * cycles waiting in this thread, when we could be doing something
           * more profitable elsewhere.
           */

          ALT_FLAG_PEND (sp->events,
                         ALT_UART_WRITE_RDY,
                         OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
                         0);
        }
        while ((next == sp->tx_start));
      }
    }

    count--;

    /* Add the next character to the transmit buffer */

    sp->tx_buf[sp->tx_end] = *ptr++;
    sp->tx_end = next;
  }

  /*
   * Now that access to the circular buffer is complete, release the write
   * semaphore so that other threads can access the buffer.
   */

  ALT_SEM_POST (sp->write_lock);

  /* 
   * Ensure that interrupts are enabled, so that the circular buffer can 
   * drain.
   */

  context = alt_irq_disable_all ();
  sp->ctrl |= FIFOED_AVALON_UART_CONTROL_TRDY_MSK |
                 FIFOED_AVALON_UART_CONTROL_DCTS_MSK;
  IOWR_FIFOED_AVALON_UART_CONTROL(sp->base, sp->ctrl);
  alt_irq_enable_all (context);

  /* return the number of bytes written */

  return (len - count);
}



#endif /* defined ALT_USE_SMALL_DRIVERS || FIFOED_AVALON_UART_SMALL */
