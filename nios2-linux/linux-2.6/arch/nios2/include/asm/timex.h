#ifndef _ASM_NIOS2_TIMEX_H
#define _ASM_NIOS2_TIMEX_H

/* Supply dummy tick-rate. Real value will be read from devicetree */
#define CLOCK_TICK_RATE		(HZ * 100000UL)

#include <asm-generic/timex.h>

#endif
