#include <linux/module.h>

#include "libgcc.h"

long __mulsi3(long a, long b)
{
	long res = 0;
	unsigned long cnt = a;

	while (cnt) {
		if (cnt & 1)
			res += b;
		b <<= 1;
		cnt >>= 1;
	}

	return res;
}
EXPORT_SYMBOL(__mulsi3);
