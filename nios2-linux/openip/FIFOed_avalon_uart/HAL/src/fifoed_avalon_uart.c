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

int fifoed_avalon_uart_read (alt_fd* fd, char* ptr, int len)
{
  int block;
  unsigned int status;

  fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev; 

  block = !(fd->fd_flags & O_NONBLOCK);

  do
  {
    status = IORD_FIFOED_AVALON_UART_STATUS(dev->base);

    /* clear any error flags */

    IOWR_FIFOED_AVALON_UART_STATUS(dev->base, 0);

    if (status & FIFOED_AVALON_UART_CONTROL_RRDY_MSK)
    {
      ptr[0] = IORD_FIFOED_AVALON_UART_RXDATA(dev->base);

      if (!(status & (FIFOED_AVALON_UART_STATUS_PE_MSK | 
      FIFOED_AVALON_UART_STATUS_FE_MSK)))
      {
        return 1;
      }
    }
  }
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

int fifoed_avalon_uart_write (alt_fd* fd, const char* ptr, int len)
{
  int block;
  unsigned int status;
  int count;

  fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;

  block = !(fd->fd_flags & O_NONBLOCK);
  count = len;

  do
  {
    status = IORD_FIFOED_AVALON_UART_STATUS(dev->base);
   
    if (status & FIFOED_AVALON_UART_STATUS_TRDY_MSK)
    {
      IOWR_FIFOED_AVALON_UART_TXDATA(dev->base, *ptr++);
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

int fifoed_avalon_uart_read (alt_fd* fd, char* ptr, int len)
{
  alt_irq_context context;
  int             block;
  alt_u32         next;

  fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;
  int count                = 0;

  /* 
   * Construct a flag to indicate whether the device is being accessed in
   * blocking or non-blocking mode.
   */

  block = !(fd->fd_flags & O_NONBLOCK);

  /*
   * When running in a multi threaded environment, obtain the "read_lock"
   * semaphore. This ensures that reading from the device is thread-safe.
   */

  ALT_SEM_PEND (dev->read_lock, 0);

  /*
   * Calculate which slot in the circular buffer is the next one to read
   * data from.
   */

  next = (dev->rx_start + 1) & FIFOED_AVALON_UART_BUF_MSK;

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

    while ((count < len) && (dev->rx_start != dev->rx_end))
    {
      count++;
      *ptr++ = dev->rx_buf[dev->rx_start];
      
      dev->rx_start = (++dev->rx_start) & FIFOED_AVALON_UART_BUF_MSK;
    }

    /*
     * If no data has been transferred, the circular buffer is empty, and
     * this is not a non-blocking access, block waiting for data to arrive.
     */

    if (!count && (dev->rx_start == dev->rx_end))
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
       dev->ctrl |= FIFOED_AVALON_UART_CONTROL_RRDY_MSK;
       IOWR_FIFOED_AVALON_UART_CONTROL(dev->base, dev->ctrl);
       alt_irq_enable_all (context);

       /*
        * When running in a multi-threaded mode, we pend on the read event 
        * flag set in the interrupt service routine. This avoids wasting CPU
        * cycles waiting in this thread, when we could be doing something more 
        * profitable elsewhere.
        */

       ALT_FLAG_PEND (dev->events, 
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

  ALT_SEM_POST (dev->read_lock);

  /*
   * Ensure that interrupts are enabled, so that the circular buffer can
   * re-fill.
   */

  context = alt_irq_disable_all ();
  dev->ctrl |= FIFOED_AVALON_UART_CONTROL_RRDY_MSK;
  IOWR_FIFOED_AVALON_UART_CONTROL(dev->base, dev->ctrl);
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

int fifoed_avalon_uart_write (alt_fd* fd, const char* ptr, int len)
{
  alt_irq_context context;
  int             no_block;
  alt_u32         next;

  fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) fd->dev;
  int count                = len;

  /* 
   * Construct a flag to indicate whether the device is being accessed in
   * blocking or non-blocking mode.
   */

  no_block = (fd->fd_flags & O_NONBLOCK);

  /*
   * When running in a multi threaded environment, obtain the "write_lock"
   * semaphore. This ensures that writing to the device is thread-safe.
   */

  ALT_SEM_PEND (dev->write_lock, 0);

  /*
   * Loop transferring data from the input buffer to the transmit circular
   * buffer. The loop is terminated once all the data has been transferred,
   * or, (if in non-blocking mode) the buffer becomes full.
   */

  while (count)
  {
    /* Determine the next slot in the buffer to access */

    next = (dev->tx_end + 1) & FIFOED_AVALON_UART_BUF_MSK;

    /* block waiting for space if necessary */

    if (next == dev->tx_start)
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
        dev->ctrl |= (FIFOED_AVALON_UART_CONTROL_TRDY_MSK |
                        FIFOED_AVALON_UART_CONTROL_DCTS_MSK);
        IOWR_FIFOED_AVALON_UART_CONTROL(dev->base, dev->ctrl);
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

          ALT_FLAG_PEND (dev->events, 
                         ALT_UART_WRITE_RDY,
                         OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME,
                         0);
        }
        while ((next == dev->tx_start));
      }
    }

    count--;

    /* Add the next character to the transmit buffer */

    dev->tx_buf[dev->tx_end] = *ptr++;
    dev->tx_end = next;
  }

  /*
   * Now that access to the circular buffer is complete, release the write
   * semaphore so that other threads can access the buffer.
   */

  ALT_SEM_POST (dev->write_lock);

  /* 
   * Ensure that interrupts are enabled, so that the circular buffer can 
   * drain.
   */

  context = alt_irq_disable_all ();
  dev->ctrl |= FIFOED_AVALON_UART_CONTROL_TRDY_MSK |
                 FIFOED_AVALON_UART_CONTROL_DCTS_MSK;
  IOWR_FIFOED_AVALON_UART_CONTROL(dev->base, dev->ctrl);
  alt_irq_enable_all (context);

  /* return the number of bytes written */

  return (len - count);
}

/*
 * fifoed_avalon_uart_rxirq() is called by fifoed_avalon_uart_irq() to process a
 * receive interrupt. It transfers the incoming character into the receive
 * circular buffer, and sets the apropriate flags to indicate that there is
 * dat ready to be processed.
 */

static void fifoed_avalon_uart_rxirq (fifoed_avalon_uart_dev* dev,
                                   alt_u32              status)
{
  alt_u32 next;

  /*
   * In a multi-threaded environment, set the read event flag to indicate
   * that there is data ready. This is only done if the circular buffer was
   * previously empty.
   */

  if (dev->rx_end == dev->rx_start)
  {
    ALT_FLAG_POST (dev->events, ALT_UART_READ_RDY, OS_FLAG_SET);
  }

  /* Determine which slot to use next in the circular buffer */

  next = (dev->rx_end + 1) & FIFOED_AVALON_UART_BUF_MSK;

  /* Transfer data from the device to the circular buffer */

  dev->rx_buf[dev->rx_end] = IORD_FIFOED_AVALON_UART_RXDATA(dev->base);

  /* If there was an error, discard the data */

  if (status & (FIFOED_AVALON_UART_STATUS_PE_MSK | 
                  FIFOED_AVALON_UART_STATUS_FE_MSK))
  {
    return;
  }

  dev->rx_end = next;

  next = (dev->rx_end + 1) & FIFOED_AVALON_UART_BUF_MSK;

  /*
   * If the cicular buffer was full, disable interrupts. Interrupts will be
   * re-enabled when data is removed from the buffer.
   */

  if (next == dev->rx_start)
  {
    dev->ctrl &= ~FIFOED_AVALON_UART_CONTROL_RRDY_MSK;
    IOWR_FIFOED_AVALON_UART_CONTROL(dev->base, dev->ctrl); 
  }   
}

/*
 * fifoed_avalon_uart_txirq() is called by fifoed_avalon_uart_irq() to process a
 * transmit interrupt. It transfers data from the transmit buffer to the
 * device, and sets the apropriate flags to indicate that there is
 * data ready to be processed.
 */

static void fifoed_avalon_uart_txirq (fifoed_avalon_uart_dev* dev,
                                   alt_u32              status)
{
  /* Transfer data if there is some ready to be transfered */

  if (dev->tx_start != dev->tx_end)
  {
    /* 
     * If the device is using flow control (i.e. RTS/CTS), then the
     * transmitter is required to throttle if CTS is high.
     */

    if (!(dev->flags & FIFOED_AVALON_UART_FC) ||
      (status & FIFOED_AVALON_UART_STATUS_CTS_MSK))
    { 

      /*
       * In a multi-threaded environment, set the write event flag to indicate
       * that there is space in the circular buffer. This is only done if the
       * buffer was previously empty.
       */

      if (dev->tx_start == ((dev->tx_end + 1) & FIFOED_AVALON_UART_BUF_MSK))
      { 
        ALT_FLAG_POST (dev->events, 
                       ALT_UART_WRITE_RDY,
                       OS_FLAG_SET);
      }

      /* Write the data to the device */
      // updated to allow mutiple writes here if the fifos are enabled.
       while ((dev->tx_start != dev->tx_end) && 
          (IORD_FIFOED_AVALON_UART_STATUS(dev->base) & FIFOED_AVALON_UART_STATUS_TRDY_MSK))
          {
              IOWR_FIFOED_AVALON_UART_TXDATA(dev->base, dev->tx_buf[dev->tx_start]);

              dev->tx_start = (++dev->tx_start) & FIFOED_AVALON_UART_BUF_MSK;
          }

      /*
       * In case the tranmit interrupt had previously been disabled by 
       * detecting a low value on CTS, it is reenabled here.
       */ 

      dev->ctrl |= FIFOED_AVALON_UART_CONTROL_TRDY_MSK;
    }
    else
    {
      /*
       * CTS is low and we are using flow control, so disable the transmit
       * interrupt while we wait for CTS to go high again. This will be 
       * detected using the DCTS interrupt.
       *
       * There is a race condition here. "status" may indicate that 
       * CTS is low, but it actually went high before DCTS was cleared on 
       * the last write to the status register. To avoid this resulting in
       * deadlock, it's necessary to re-check the status register here
       * before throttling.
       */
 
      status = IORD_FIFOED_AVALON_UART_STATUS(dev->base); 

      if (!(status & FIFOED_AVALON_UART_STATUS_CTS_MSK))
      {
        dev->ctrl &= ~FIFOED_AVALON_UART_CONTROL_TRDY_MSK;
      }
    }
  }

  /*
   * If the circular buffer is empty, disable the interrupt. This will be
   * re-enabled when new data is placed in the buffer.
   */

  if (dev->tx_start == dev->tx_end)
  {
    dev->ctrl &= ~(FIFOED_AVALON_UART_CONTROL_TRDY_MSK |
                    FIFOED_AVALON_UART_CONTROL_DCTS_MSK);
  }

  IOWR_FIFOED_AVALON_UART_CONTROL(dev->base, dev->ctrl);
}

/*
 * fifoed_avalon_uart_irq() is the interrupt handler registered at configuration
 * time for processing UART interrupts. It vectors interrupt requests to
 * either fifoed_avalon_uart_rxirq() (for incoming data), or
 * fifoed_avalon_uart_txirq() (for outgoing data).
 */

static void fifoed_avalon_uart_irq (void* context, alt_u32 id)
{
  alt_u32 status;

  fifoed_avalon_uart_dev* dev = (fifoed_avalon_uart_dev*) context;
  void* base               = dev->base;

  /*
   * Read the status register in order to determine the cause of the
   * interrupt.
   */

  status = IORD_FIFOED_AVALON_UART_STATUS(base);

  /* Clear any error flags set at the device */

  IOWR_FIFOED_AVALON_UART_STATUS(base, 0);

  /* process a read irq */
 
  if (status & FIFOED_AVALON_UART_STATUS_RRDY_MSK)
  {
    fifoed_avalon_uart_rxirq (dev, status);
  }

  /* process a write irq */

  if (status & (FIFOED_AVALON_UART_STATUS_TRDY_MSK | 
                  FIFOED_AVALON_UART_STATUS_DCTS_MSK))
  {
    fifoed_avalon_uart_txirq (dev, status);
  }
}

/*
 * fifoed_avalon_uart_init() is called by the auto-generated function 
 * alt_sys_init() in order to initialise a particular instance of this device.
 * It is responsible for configuring the device and associated software 
 * constructs.
 *
 * If no errors occur, then the device is register as available to the system
 * through a call to alt_dev_reg().
 */

void fifoed_avalon_uart_init (fifoed_avalon_uart_dev* dev, void* base, alt_u32 irq)
{
  int error;

  /* 
   * Initialise the read and write flags and the semaphores used to 
   * protect access to the circular buffers when running in a multi-threaded
   * environment.
   */

  error = ALT_FLAG_CREATE (&dev->events, 0)    || 
          ALT_SEM_CREATE (&dev->read_lock, 1)   ||
          ALT_SEM_CREATE (&dev->write_lock, 1);

  if (!error)
  {
    /* enable interrupts at the device */

    dev->ctrl = FIFOED_AVALON_UART_CONTROL_RTS_MSK  |
                FIFOED_AVALON_UART_CONTROL_RRDY_MSK |
                FIFOED_AVALON_UART_CONTROL_DCTS_MSK;

    IOWR_FIFOED_AVALON_UART_CONTROL(base, dev->ctrl); 
  
    /* register the interrupt handler */

    if (alt_irq_register (irq, dev, fifoed_avalon_uart_irq) >= 0)
    {
      /* make the device available to the system */

      alt_dev_reg (&dev->dev);
    }
  }
}

/*
 * To reduce the code footprint of this driver, the ioctl() function is not
 * included by default. If you wish to use the ioctl features provided 
 * below, you can do so by adding the option : -DFIFOED_AVALON_UART_USE_IOCTL
 * to CPPFLAGS in the Makefile (or through the Eclipse IDE).
 */

#ifdef FIFOED_AVALON_UART_USE_IOCTL

/*
 * fifoed_avalon_uart_tiocmget() is used by fifoed_avalon_uart_ioctl() to fill in
 * the input termios structure with the current device configuration. 
 *
 * See termios.h for further details on the contents of the termios structure.
 */

static int fifoed_avalon_uart_tiocmget (fifoed_avalon_uart_dev* dev,
                                     struct termios*      term)
{
  memcpy (term, &dev->termios, sizeof (struct termios));
  return 0;
}

/*
 * fifoed_avalon_uart_tiocmset() is used by fifoed_avalon_uart_ioctl() to configure
 * the device according to the settings in the input termios structure. In
 * practice the only configuration that can be changed is the baud rate, and
 * then only if the hardware is configured to have a writable baud register.
 */

static int fifoed_avalon_uart_tiocmset (fifoed_avalon_uart_dev* dev,
                                     struct termios*      term)
{
  alt_u32 divisor;
  speed_t speed;

  speed = dev->termios.c_ispeed;

  /* Update the settings if the hardware supports it */

  if (!(dev->flags & FIFOED_AVALON_UART_FB))
  {
    dev->termios.c_ispeed = dev->termios.c_ospeed = term->c_ispeed;
  }
  /* 
   * If the request was for an unsupported setting, return an error.
   */

  if (memcmp(term, &dev->termios, sizeof (struct termios)))
  {
    dev->termios.c_ispeed = dev->termios.c_ospeed = speed;
    return -EIO;
  }

  /*
   * Otherwise, update the hardware.
   */
  
  IOWR_FIFOED_AVALON_UART_DIVISOR(dev->base, ((dev->freq/speed) - 1));

  return 0;
}

/*
 * fifoed_avalon_uart_ioctl() is called by the system ioctl() function to handle
 * ioctl requests for the UART. The only ioctl requests supported are TIOCMGET
 * and TIOCMSET.
 *
 * TIOCMGET returns a termios structure that describes the current device
 * configuration.
 *
 * TIOCMSET sets the device (if possible) to match the requested configuration.
 * The requested configuration is described using a termios structure passed
 * through the input argument "arg".
 */

int fifoed_avalon_uart_ioctl (alt_fd* fd, int req, void* arg)
{
  int rc = -ENOTTY;

  switch (req)
  {
  case TIOCMGET:
    rc = fifoed_avalon_uart_tiocmget ((fifoed_avalon_uart_dev*) fd->dev,
                                   (struct termios*) arg);
    break;
  case TIOCMSET:
    rc = fifoed_avalon_uart_tiocmset ((fifoed_avalon_uart_dev*) fd->dev,
                                   (struct termios*) arg);
    break;
  default:
    break;
  }
  return rc;
}

#endif /* FIFOED_AVALON_UART_USE_IOCTL */

#endif /* defined ALT_USE_SMALL_DRIVERS || FIFOED_AVALON_UART_SMALL */
