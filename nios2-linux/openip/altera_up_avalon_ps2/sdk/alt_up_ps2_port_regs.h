#ifndef __ALT_UP_PS2_PORT_REGS_H__
#define __ALT_UP_PS2_PORT_REGS_H__

/* 
 * Data Register 
 */
#define ALT_UP_PS2_PORT_DATA_REG                  0
#define IOADDR_ALT_UP_PS2_PORT_DATA(base)         \
        __IO_CALC_ADDRESS_NATIVE(base, ALT_UP_PS2_PORT_DATA_REG)
#define IORD_ALT_UP_PS2_PORT_DATA(base)           \
        IORD(base, ALT_UP_PS2_PORT_DATA_REG) 
#define IOWR_ALT_UP_PS2_PORT_DATA(base, data)     \
        IOWR(base, ALT_UP_PS2_PORT_DATA_REG, data)

#define ALT_UP_PS2_PORT_DATA_REG_DATA_MSK	     (0x000000FF)
#define ALT_UP_PS2_PORT_DATA_REG_DATA_OFST	     (0)
#define ALT_UP_PS2_PORT_DATA_REG_RVALID_MSK	 	 (0x00008000)
#define ALT_UP_PS2_PORT_DATA_REG_RVALID_OFST	 (15)
#define ALT_UP_PS2_PORT_DATA_REG_RAVAIL_MSK		 (0xFFFF0000)
#define ALT_UP_PS2_PORT_DATA_REG_RAVAIL_OFST	 (16)

/*
 * Control Register 
 */
#define ALT_UP_PS2_PORT_CONTROL_REG               1
#define IOADDR_ALT_UP_PS2_PORT_CONTROL(base)      \
        __IO_CALC_ADDRESS_NATIVE(base, ALT_UP_PS2_PORT_CONTROL_REG)
#define IORD_ALT_UP_PS2_PORT_CONTROL(base)        \
        IORD(base, ALT_UP_PS2_PORT_CONTROL_REG)
#define IOWR_ALT_UP_PS2_PORT_CONTROL(base, data)  \
        IOWR(base, ALT_UP_PS2_PORT_CONTROL_REG, data)

#define ALT_UP_PS2_PORT_CONTROL_RE_MSK            (0x00000001)
#define ALT_UP_PS2_PORT_CONTROL_RE_OFST           (0)
#define ALT_UP_PS2_PORT_CONTROL_RI_MSK            (0x00000100)
#define ALT_UP_PS2_PORT_CONTROL_RI_OFST           (8)
#define ALT_UP_PS2_PORT_CONTROL_CE_MSK 		  	  (0x00000400)
#define ALT_UP_PS2_PORT_CONTROL_CE_OFST 		  (10)

#endif
