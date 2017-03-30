/*
 * simple test case to make sure a FLAT app's stack is in L1 scratchpad
 *
 * Compile:
 *    bfin-uclinux-gcc helloworld.c -o helloworld
 * Set stack up:
 *    bfin-uclinux-flthdr -u -s 3000 helloworld
 * Run it and see all PASSes
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE	/* needed for "environ" */
#endif
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TEST_SCRATCHPAD_LOW      0xffb00000
#define TEST_SCRATCHPAD_HIGH     0xffb01000

static int in_scratch(unsigned long addr)
{
	return addr >= TEST_SCRATCHPAD_LOW && addr < TEST_SCRATCHPAD_HIGH;
}
#define in_scratch(addr) in_scratch((unsigned long)(addr))

#define pad 15

static int ret = 0;
#define passfail(p, e) \
	do { \
		if ((e) == in_scratch(p)) { \
			printf("PASS   "); \
		} else \
			++ret, printf("FAIL!  "); \
	} while (0)

#define check_pointer(p, e1, e2) \
	do { \
		passfail(&(p), e1); \
		printf("&%-*s: %p\n", pad, #p, &(p)); \
		passfail(p, e2); \
		printf(" %-*s: %p\n", pad, #p, (p)); \
	} while (0)

#define check_string(s, e1, e2) \
	do { \
		check_pointer(s, e1, e2); \
		printf("....    %-*s: \"%s\"\n", pad, #s"[...]", (s)); \
	} while (0)

int main(int argc, char *argv[])
{
	char *str = "Hello world!";

	/* str variable should be in L1, but not the string */
	check_string(str, 1, 0);
	puts("");

	/* argv variable and the array should be in L1, but not the strings */
	check_pointer(argv, 1, 1);
	check_string(argv[0], 1, 0);
	if (!strstr(argv[0], "a.out") && !strstr(argv[0], "helloworld"))
		puts("FAIL:  test app should be named 'helloworld'");
	puts("");

	/* the pointers should be in L1, but not the environ symbol or
	 * the strings themselves */
	check_pointer(environ, 0, 1);
	check_string(environ[0], 1, 0);
	puts("");

	if (ret)
		printf("%i TESTS FAILED\n", ret);
	else
		printf("ALL TESTS PASS\n");

	return ret;
}
