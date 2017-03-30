/* (C) 2007 by folkert@vanheusden.com
 * The GPL (GNU public license) applies to this sourcecode.
 */
#include <errno.h>
#include <regex.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static void error_exit(char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	(void)vfprintf(stderr, format, ap);
	va_end(ap);
	if (errno)
		fprintf(stderr, "errno: %d=%s (if applicable)\n", errno, strerror(errno));

	exit(EXIT_FAILURE);
}

static void MEMLOG(char *s, ...)
{
        va_list ap;
        FILE *fh = fopen("log.log", "a+");
        if (!fh)
                error_exit("error logging\n");

        va_start(ap, s);
        vfprintf(fh, s, ap);
        va_end(ap);

        fclose(fh);
}

typedef struct
{
	void *p;
	char *descr;
	int size;
} memlist;
memlist *pm = NULL;
int n_pm = 0;

void dump_mem(int sig)
{
	int loop;

	signal(SIGHUP, dump_mem);

	if (sig != SIGHUP)
		error_exit("dump_mem: unexpected signal %d for dump_mem\n", sig);

	MEMLOG("%d elements of memory used\n", n_pm);
	for(loop=0; loop<n_pm; loop++)
	{
		MEMLOG("%06d] %p %d (%s)\n", loop, pm[loop].p, pm[loop].size, pm[loop].descr);
	}
	MEMLOG("--- finished memory dump\n");
}

static int remove_mem_element(void *p)
{
	int old_size = 0;

	if (p)
	{
		int loop;

		for(loop=0; loop<n_pm; loop++)
		{
			if (pm[loop].p == p)
			{
				int n_to_move;

				old_size = pm[loop].size;

				n_to_move = (n_pm - loop) - 1;
				if (n_to_move > 0)
					memmove(&pm[loop], &pm[loop + 1], n_to_move * sizeof(memlist));
				else if (n_to_move < 0)
					error_exit("remove_mem_element: n_to_move < 0!\n");
				n_pm--;
				loop=-1;

				break;
			}
		}

		if (loop != -1)
		{
			MEMLOG("remove_mem_element: pointer %p not found\n", p);
		}

		if (n_pm)
		{
			pm = (memlist *)realloc(pm, sizeof(memlist) * n_pm);
			if (!pm) error_exit("remove_mem_element: failed to shrink memorylist to %d elements\n", n_pm);
		}
		else
		{
			free(pm);
			pm = NULL;
		}
	}

	return old_size;
}
static void add_mem_element(void *p, int size, char *what)
{
	pm = (memlist *)realloc(pm, sizeof(memlist) * (n_pm + 1));
	if (!pm) error_exit("add_mem_element: failed to grow memorylist from %d elements\n", n_pm);
	pm[n_pm].p = p;
	pm[n_pm].size = size;
	pm[n_pm].descr = what;
	n_pm++;
}

void myfree(void *p)
{
	int old_size = remove_mem_element(p);

	MEMLOG("myfree: %p (%d bytes)\n", p, old_size);
	memset(p, 0xf3, old_size);

	free(p);
}

void * myrealloc(void *oldp, int new_size, char *what)
{
	int old_size;
	void *newp;

	if (new_size <= 0)
		error_exit("Tried to allocate %d bytes which is wrong.\n", new_size);

	newp = realloc(oldp, new_size);
	if (!newp)
		error_exit("Failed to reallocate a memory block to %d bytes.\n", new_size);
	old_size = remove_mem_element(oldp);
	add_mem_element(newp, new_size, what);
	signal(SIGHUP, dump_mem);

	if (new_size > old_size)
		memset(&((char *)newp)[old_size], 0xf1, new_size - old_size);

	return newp;
}


void * mymalloc(int size, char *what)
{
	return myrealloc(NULL, size, what);
}

char * mystrdup(char *in, char *what)
{
	int len = strlen(in) + 1;
	char *newp = (char *)mymalloc(len, what);

	memcpy(newp, in, len);

	return newp;
}
