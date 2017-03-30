/*
 * Test sanity of sram_alloc() and sram_free() system calls.
 * We assume a generally idle system where other code isn't attempting to
 * allocate and/or free the SRAM regions at the same time.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <bfin_sram.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

typedef struct {
	const char *sflag;
	unsigned long flag;
} sram_test;
static const sram_test const sram_tests[] = {
	{ "L1_INST_SRAM",     L1_INST_SRAM,   },
	{ "L1_DATA_A_SRAM",   L1_DATA_A_SRAM, },
	{ "L1_DATA_B_SRAM",   L1_DATA_B_SRAM, },
	{ "L1_DATA_SRAM",     L1_DATA_SRAM,   },
	{ "L1_DATA_SRAM+A+B", L1_DATA_SRAM | L1_DATA_A_SRAM | L1_DATA_B_SRAM, },
	{ "L2_SRAM",          L2_SRAM,        },
};

static int tret;
static const char *passfail(int t)
{
	if (t)
		return "PASS";

	tret = 1;
	return "FAIL";
}

static size_t all_len;
static char **all;
int run_tests(int cpu)
{
	int i, j, ret;
	char *p, *start;

	tret = 0;

	for (i = 0; i < ARRAY_SIZE(sram_tests); ++i) {
		const sram_test *t = &sram_tests[i];

		printf("\nTesting %s on cpu %i ...\n", t->sflag, cpu);

		/* get a valid pointer */
		printf("sram_alloc(1, %s) = ", t->sflag);
		p = sram_alloc(1, t->flag);
		if (!p && !(t->flag & L1_INST_SRAM)) {
			/* hrm, have to assume the part doesn't have any available */
			printf("<not available?>: SKIP\n");
			continue;
		}
		printf("%p: %s\n", p, passfail(p != NULL));
		start = p;

		/* free the valid pointer */
		printf("sram_free(%p) = ", p);
		ret = sram_free(p);
		printf("%i: %s\n", ret, passfail(ret == 0));

		/* now see if the same pointer is available (look for mem leak) */
		printf("sram_alloc(1, %s) = ", t->sflag);
		p = sram_alloc(1, t->flag);
		printf("%p: %s\n", p, passfail(p == start));

		printf("sram_free(%p) = ", p);
		ret = sram_free(p);
		printf("%i: %s\n", ret, passfail(ret == 0));

		/* free an invalid pointer */
		printf("sram_free(%p) = ", p);
		ret = sram_free(p);
		printf("%i: %s\n", ret, passfail(ret == -1));

		/* try to allocate too large an area */
		printf("sram_alloc(0x1000000, %s) = ", t->sflag);
		p = sram_alloc(0x1000000, t->flag);
		printf("%p: %s\n", p, passfail(!p));

		/* now alloc all of the memory */
		printf("sram_alloc(<all>, %s) = ", t->sflag);
		j = 0;
		while (1) {
			if (j >= all_len) {
				all_len += 0x100;
				all = realloc(all, sizeof(all[0]) * all_len);
			}
			all[j] = sram_alloc(1, t->flag);
			if (!all[j])
				break;
			++j;
		}
		printf("<%i allocs>: PASS\n", j);
		printf("sram_free(<all>) = ");
		while (j--) {
			ret = sram_free(all[j]);
			if (ret) {
				printf("<iteration %i>: %s\n", ret, passfail(ret == 0));
				break;
			}
		}
		if (j == -1)
			printf("0: PASS\n");

		/* now see if the same pointer is available (look for mem leak) */
		printf("sram_alloc(1, %s) = ", t->sflag);
		p = sram_alloc(1, t->flag);
		printf("%p: %s\n", p, passfail(p == start));

		printf("sram_free(%p) = ", p);
		ret = sram_free(p);
		printf("%i: %s\n", ret, passfail(ret == 0));
	}

	printf("\nGeneral tests on cpu %i ...\n", cpu);

	/* These should all fail */
	ret = 0;
	printf("sram_free(bad pointer) = ");
	for (p = 0; p <= (char *)0xff000000; p += 512 * 1024)
		if (sram_free(p) != -1) {
			printf("(%p) ", p);
			ret = 1;
			break;
		}
	printf("%i: %s\n", ret, passfail(ret == 0));

	return tret;
}

int main(int argc, char *argv[])
{
	cpu_set_t cset;
	int ret, i, cpu, cpumax;

	setbuf(stdout, NULL);

	if (argc == 2 && !strcmp(argv[1], "--child")) {
		for (i = 0; i < 10000; ++i)
			sram_free(sram_alloc(1, L1_INST_SRAM));
		return 0;
	}

	/* Run all of the single thread tests */
	ret = 0;
	cpu = 0;
	while (1) {
		CPU_ZERO(&cset);
		CPU_SET(cpu, &cset);
		if (sched_setaffinity(0, sizeof(cset), &cset) == -1)
			break;
		ret |= run_tests(cpu);
		++cpu;
	}
	cpumax = cpu;

	/* Then thrash in parallel */
	printf("\nParallel tests on all cpus ...\n");
	for (i = 0; i < 10; ++i) {
		pid_t pid;

		cpu = i % cpumax;
		CPU_ZERO(&cset);
		CPU_SET(cpu, &cset);
		if (sched_setaffinity(0, sizeof(cset), &cset) == -1) {
			ret |= 1;
			printf("sched_setaffinity(%i %% %i -> %i) FAIL\n", i, cpumax, cpu);
			break;
		}

		pid = vfork();
		switch (pid) {
		case -1:
			ret |= 1;
			printf("vfork() FAIL\n");
			break;
		case 0:
			_exit(execlp(argv[0], argv[0], "--child", NULL));
		default:
			printf("\tpid %i on cpu %i\n", pid, cpu);
		}
	}
	while (wait(&i) != -1)
		if (!WIFEXITED(i) || WEXITSTATUS(i)) {
			printf("child exited with %i: FAIL\n", i);
			ret |= 1;
		}

	/* All done! */
	printf("\nTest summary: %s\n\n", ret ? "some tests FAIL" : "all tests PASS");

	return ret;
}
