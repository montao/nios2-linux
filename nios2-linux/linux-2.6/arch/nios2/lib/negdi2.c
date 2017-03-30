#include <linux/module.h>

#include "libgcc.h"

long long __negdi2(long long u)
{
	const DWunion uu = { .ll = u };
	const DWunion w = {
		{ .low = -uu.s.low,
		  .high = -uu.s.high - ((unsigned int) -uu.s.low > 0)
		} };

	return w.ll;
}
EXPORT_SYMBOL(__negdi2);
