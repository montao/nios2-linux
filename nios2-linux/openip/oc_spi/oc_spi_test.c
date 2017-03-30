/* 
 * oc_spi_test.c
 * 
 * Simple test code to try out the opencores SPI core in SOPC Builder
 *
 * NOTES: 
 * - For writing driver
 *   > Check GO_BUSY bit - can only update regs when = 0
 *   > Read Rx reg first to clear it and to clear any interrupts
 */

#include <alt_types.h>
#include <io.h>
#include "sys/alt_stdio.h"
#include "system.h"

#define DEBUG_CONSOLE_EN  0    /* Set to 1 to allow alt_printf console I/O */

/* SPI core register addresses */
#define SPI_BASE_ADRS  OC_SPI_BASE   /* Assign base address from system.h */

#define SPI_RX_REG_OFFSET 0
#define SPI_RX_REG (SPI_BASE+ADRS + 0)       /* Read, offset 0 */
#define SPI_TX_REG_OFFSET 0
#define SPI_TX_REG (SPI_BASE+ADRS + 0)       /* Write, offset 0 */
#define SPI_CTRL_REG_OFFSET 0x10
#define SPI_CTRL_REG (SPI_BASE+ADRS + 0x10)
#define SPI_DIV_REG_OFFSET 0x14
#define SPI_DIV_REG (SPI_BASE+ADRS + 0x14)
#define SPI_SSEL_REG_OFFSET 0x18
#define SPI_SSEL_REG (SPI_BASE+ADRS + 0x18)

/* SPI core control register bits */
#define AUTO_SS_MSK     0x2000      /* 1=auto assertion of SS bit */
#define IE_MSK          0x1000      /* 1=interrupt enabled - xfr complete */
#define LSB_MSK         0x0800      /* 1=LSB xmitted first, 0=MSB first */
#define TX_NEG_MSK      0x0400      /* 1=MOSI signal changed on falling SCLK */
#define RX_NEG_MSK      0x0200      /* 1=MISO signal latched on falling SCLK */
#define GO_BUSY_MSK     0x0100      /* Write 1 to start xfr, 0 does nothing */
#define CHAR_LEN_MSK    0x007F      /* 7-bit value for xfr word length */

/* SPI core - other registers - bits */
#define SPI_DIVIDER_REG_MSK   0xFFFF    /* Divider: 16 LS-bits programmable */
#define SPI_SLAVE_SEL_REG_MSK 0x00FF    /* Slave select bits: 8 LS-bits, one per slave */

/* ----- Test Parameters ----- */
#define TEST_CHAR_LENGTH 16         /* SPI transfer length in bits */
#define TEST_CLK_DIVIDE  24         /* DIVIDER REG value, 24 gives divide-by-50 */

/* Test write data */
alt_u16 test_wrt_data[4] = {0x1111, 0xa5a5, 0x1234, 0x0f0f};

/*=================
 * Main
 * ================ */    
int main()
{ 
  alt_u16 data;
  alt_u32 i;
  alt_u32 control = 0;
  
  #if DEBUG_CONSOLE_EN
  alt_printf("OC SPI test\n");
  #endif
  
  /* --- Init SPI core */
  /* Read the Rx register: clears it and clears any interrupts */
  i = IORD_32DIRECT(SPI_BASE_ADRS, SPI_RX_REG_OFFSET);
  
  /* Setup the clock divider register */
  /* Calculation according to spec: SPI_CLK = CORE_CLK / ((DIV + 1)*2) */
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_DIV_REG_OFFSET, TEST_CLK_DIVIDE);
  
  /* --- Setup core for write transfer */
  /* General setup for the control register for:
   *   Auto Slave Select (auto asserts for slave selected in SS reg)
   *   Ints disabled
   *   MSB first
   *   Transmit data changed on rising edge
   *   Receive data latched on falling edge
   *   Word transfer size = 16 bits  */
  control |= ( AUTO_SS_MSK | RX_NEG_MSK | (CHAR_LEN_MSK & TEST_CHAR_LENGTH) );
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_CTRL_REG_OFFSET, control);
      
  /* Setup slave select register */         
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_SSEL_REG_OFFSET, 0x01);  /* Slave 0 */
  
  /* SPI write */
  data = test_wrt_data[0];
  #if DEBUG_CONSOLE_EN
  alt_printf("WRITE 16b: data = 0x%x\n", data);
  #endif
  
  control |= (alt_u32)(GO_BUSY_MSK);
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_TX_REG_OFFSET, (alt_u32)(data & 0xFFFF));
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_CTRL_REG_OFFSET, control);  /* GO */
  
  /* Wait for busy to clear - xfer complete (poor man's timeout loop) */
  i = 50000000;
  while (((IORD_32DIRECT(SPI_BASE_ADRS, SPI_CTRL_REG_OFFSET)) & GO_BUSY_MSK) != 0)
  {
      i--;
      if (i == 0)
      {
          #if DEBUG_CONSOLE_EN
          alt_printf("\n--TIME OUT waiting for xfr\n\n");
          #endif
          return 0;
      }      
  }
  
  /* SPI read */
  #if DEBUG_CONSOLE_EN
  alt_printf("READ 16b: data = 0x");   /* complete data output after read */
  #endif
  control = 0;          /* reset control word */
  
  /* --- Setup core for read transfer */
  /* General setup for the control register:
   *   Auto Slave Select (auto asserts for slave selected in SS reg)
   *   Ints disabled
   *   MSB first
   *   Transmit data changed on rising edge
   *   Receive data latched on falling edge
   *   Word transfer size = 16 bits  */
  control |= ( AUTO_SS_MSK | RX_NEG_MSK | (CHAR_LEN_MSK & TEST_CHAR_LENGTH) );
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_CTRL_REG_OFFSET, control);
   
  control |= (alt_u32)(GO_BUSY_MSK);
  IOWR_32DIRECT(SPI_BASE_ADRS, SPI_CTRL_REG_OFFSET, control);  /* GO */
  
  /* Wait for busy to clear - xfer complete  (poor man's timeout loop) */
  i = 50000000;
  while (((IORD_32DIRECT(SPI_BASE_ADRS, SPI_CTRL_REG_OFFSET)) & GO_BUSY_MSK) != 0)
  {
      i--;
      if (i == 0)
      {
          #if DEBUG_CONSOLE_EN
          alt_printf("\n--TIME OUT waiting for xfr\n\n");
          #endif
          return 0;
      }      
  }
  /* Read the Rx register */
  data = (alt_u16)((IORD_32DIRECT(SPI_BASE_ADRS, SPI_RX_REG_OFFSET)) & 0xFFFF); 
  #if DEBUG_CONSOLE_EN
  alt_printf("%x\n\n", data);  /* finish outputting read data */
  #endif

  return 0;
}
