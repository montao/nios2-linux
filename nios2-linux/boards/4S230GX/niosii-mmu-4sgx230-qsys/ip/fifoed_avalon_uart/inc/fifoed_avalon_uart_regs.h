/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2009 Altera Corporation, San Jose, California, USA.           *
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

#ifndef __FIFOED_AVALON_UART_REGS_H__
#define __FIFOED_AVALON_UART_REGS_H__

#include <io.h>

#define IORD_FIFOED_AVALON_UART_RXDATA(base)         IORD(base, 0)
#define IOWR_FIFOED_AVALON_UART_RXDATA(base, data)   IOWR(base, 0, data)

#define FIFOED_AVALON_UART_RXDATA_DATA_MSK           (0x1ff)
#define FIFOED_AVALON_UART_RXDATA_DATA_OFST          (0)
#define FIFOED_AVALON_UART_RXDATA_PE_MSK             (0x400)
#define FIFOED_AVALON_UART_RXDATA_PE_OFST            (10)
#define FIFOED_AVALON_UART_RXDATA_FE_MSK             (0x800)
#define FIFOED_AVALON_UART_RXDATA_FE_OFST            (11)
#define FIFOED_AVALON_UART_RXDATA_BRK_MSK            (0x1000)
#define FIFOED_AVALON_UART_RXDATA_BRK_OFST           (12)
#define FIFOED_AVALON_UART_RXDATA_ROE_MSK            (0x2000)
#define FIFOED_AVALON_UART_RXDATA_ROE_OFST           (13)
#define FIFOED_AVALON_UART_RXDATA_GAP_MSK            (0x4000)
#define FIFOED_AVALON_UART_RXDATA_GAP_OFST           (14)
#define FIFOED_AVALON_UART_RXDATA_REMPTY_MSK         (0x8000)
#define FIFOED_AVALON_UART_RXDATA_REMPTY_OFST        (15)

#define IORD_FIFOED_AVALON_UART_TXDATA(base)         IORD(base, 1)
#define IOWR_FIFOED_AVALON_UART_TXDATA(base, data)   IOWR(base, 1, data)

#define IORD_FIFOED_AVALON_UART_STATUS(base)         IORD(base, 2)
#define IOWR_FIFOED_AVALON_UART_STATUS(base, data)   IOWR(base, 2, data)

#define FIFOED_AVALON_UART_STATUS_PE_MSK             (0x1)
#define FIFOED_AVALON_UART_STATUS_PE_OFST            (0)
#define FIFOED_AVALON_UART_STATUS_FE_MSK             (0x2)
#define FIFOED_AVALON_UART_STATUS_FE_OFST            (1)
#define FIFOED_AVALON_UART_STATUS_BRK_MSK            (0x4)
#define FIFOED_AVALON_UART_STATUS_BRK_OFST           (2)
#define FIFOED_AVALON_UART_STATUS_ROE_MSK            (0x8)
#define FIFOED_AVALON_UART_STATUS_ROE_OFST           (3)
#define FIFOED_AVALON_UART_STATUS_TOE_MSK            (0x10)
#define FIFOED_AVALON_UART_STATUS_TOE_OFST           (4)
#define FIFOED_AVALON_UART_STATUS_TMT_MSK            (0x20)
#define FIFOED_AVALON_UART_STATUS_TMT_OFST           (5)
#define FIFOED_AVALON_UART_STATUS_TRDY_MSK           (0x40)
#define FIFOED_AVALON_UART_STATUS_TRDY_OFST          (6)
#define FIFOED_AVALON_UART_STATUS_RRDY_MSK           (0x80)
#define FIFOED_AVALON_UART_STATUS_RRDY_OFST          (7)
#define FIFOED_AVALON_UART_STATUS_E_MSK              (0x100)
#define FIFOED_AVALON_UART_STATUS_E_OFST             (8)
#define FIFOED_AVALON_UART_STATUS_TBRK_MSK           (0x200)
#define FIFOED_AVALON_UART_STATUS_TBRK_OFST          (9)
#define FIFOED_AVALON_UART_STATUS_DCTS_MSK           (0x400)
#define FIFOED_AVALON_UART_STATUS_DCTS_OFST          (10)
#define FIFOED_AVALON_UART_STATUS_CTS_MSK            (0x800)
#define FIFOED_AVALON_UART_STATUS_CTS_OFST           (11)
#define FIFOED_AVALON_UART_STATUS_EOP_MSK            (0x1000)
#define FIFOED_AVALON_UART_STATUS_EOP_OFST           (12)
// these are for the rx_at threshold bit.
#define FIFOED_AVALON_UART_STATUS_RX_TH_MSK          (0x2000)
#define FIFOED_AVALON_UART_STATUS_RX_TH_OFST         (13)
#define FIFOED_AVALON_UART_STATUS_GAP_MSK            (0x4000)
#define FIFOED_AVALON_UART_STATUS_GAP_OFST           (14)

#define IORD_FIFOED_AVALON_UART_CONTROL(base)        IORD(base, 3)
#define IOWR_FIFOED_AVALON_UART_CONTROL(base, data)  IOWR(base, 3, data)

#define FIFOED_AVALON_UART_CONTROL_PE_MSK            (0x1)
#define FIFOED_AVALON_UART_CONTROL_PE_OFST           (0)
#define FIFOED_AVALON_UART_CONTROL_FE_MSK            (0x2)
#define FIFOED_AVALON_UART_CONTROL_FE_OFST           (1)
#define FIFOED_AVALON_UART_CONTROL_BRK_MSK           (0x4)
#define FIFOED_AVALON_UART_CONTROL_BRK_OFST          (2)
#define FIFOED_AVALON_UART_CONTROL_ROE_MSK           (0x8)
#define FIFOED_AVALON_UART_CONTROL_ROE_OFST          (3)
#define FIFOED_AVALON_UART_CONTROL_TOE_MSK           (0x10)
#define FIFOED_AVALON_UART_CONTROL_TOE_OFST          (4)
#define FIFOED_AVALON_UART_CONTROL_TMT_MSK           (0x20)
#define FIFOED_AVALON_UART_CONTROL_TMT_OFST          (5)
#define FIFOED_AVALON_UART_CONTROL_TRDY_MSK          (0x40)
#define FIFOED_AVALON_UART_CONTROL_TRDY_OFST         (6)
#define FIFOED_AVALON_UART_CONTROL_RRDY_MSK          (0x80)
#define FIFOED_AVALON_UART_CONTROL_RRDY_OFST         (7)
#define FIFOED_AVALON_UART_CONTROL_E_MSK             (0x100)
#define FIFOED_AVALON_UART_CONTROL_E_OFST            (8)
#define FIFOED_AVALON_UART_CONTROL_TBRK_MSK           (0x200)
#define FIFOED_AVALON_UART_CONTROL_TBRK_OFST          (9)
#define FIFOED_AVALON_UART_CONTROL_DCTS_MSK          (0x400)
#define FIFOED_AVALON_UART_CONTROL_DCTS_OFST         (10)
#define FIFOED_AVALON_UART_CONTROL_RTS_MSK           (0x800)
#define FIFOED_AVALON_UART_CONTROL_RTS_OFST          (11)
#define FIFOED_AVALON_UART_CONTROL_EOP_MSK           (0x1000)
#define FIFOED_AVALON_UART_CONTROL_EOP_OFST          (12)
#define FIFOED_AVALON_UART_CONTROL_GAP_MSK           (0x2000)
#define FIFOED_AVALON_UART_CONTROL_GAP_OFST          (13)

#define IORD_FIFOED_AVALON_UART_DIVISOR(base)        IORD(base, 4)
#define IOWR_FIFOED_AVALON_UART_DIVISOR(base, data)  IOWR(base, 4, data)

#define IORD_FIFOED_AVALON_UART_EOP(base)            IORD(base, 5)
#define IOWR_FIFOED_AVALON_UART_EOP(base, data)      IOWR(base, 5, data)

#define FIFOED_AVALON_UART_EOP_MSK                   (0xFF)
#define FIFOED_AVALON_UART_EOP_OFST                  (0)
// Read-only width depends on fifo depth log2(fifo_depth)+1 show the number of words in the receive FIFO
#define IORD_FIFOED_AVALON_UART_RX_FIFO_USED(base)			IORD(base, 6)
// Read-only width depends on fifo depth log2(fifo_depth)+1 show the number of words in the transmit FIFO
#define IORD_FIFOED_AVALON_UART_TX_FIFO_USED(base)			IORD(base, 7)

#define IORD_FIFOED_AVALON_UART_GAP(base)            IORD(base, 8)
#define IOWR_FIFOED_AVALON_UART_GAP(base, data)      IOWR(base, 8, data)

#define IORD_FIFOED_AVALON_UART_TIMESTAMP(base)            IORD(base, 9)

#endif /* __FIFOED_AVALON_UART_REGS_H__ */
