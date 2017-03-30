#include <stdio.h>
int main()
{
	unsigned long c1, c2;
	unsigned long long c;
	asm volatile("%0 = cycles; %1 = cycles2;" : "=d"(c1), "=d"(c2));
	c = (((unsigned long long)c2) << 32) | c1;
	printf(
		"cycles1:0x%08lx %lu\n"
		"cycles2:0x%08lx %lu\n"
		"cycles: 0x%016llx %llu\n",
		c1, c1, c2, c2, c, c
	);
	return 0;
}
