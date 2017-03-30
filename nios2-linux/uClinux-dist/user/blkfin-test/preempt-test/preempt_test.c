/*
 *      Preempt latency Test Program
 *	usage ./preempt_test runtime(in minutes)
 *
 *      Copyright (C) 2010, Bob Liu.
 *
 *      Released under the GNU General Public License, version 2,
 *      included herein by reference.
 *
 */
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define BFIN_LATENCYTEST_IOCTL_MAGIC 'r'
#define BFIN_LATENCYTEST_START  _IO (BFIN_LATENCYTEST_IOCTL_MAGIC, \
								6)
#define BFIN_LATENCYTEST_STOP   _IO (BFIN_LATENCYTEST_IOCTL_MAGIC, \
								8)
#define HISTSIZE 		25
#define HIST_USEC 		10/* Microseconds per slot */
#define SLOT_BEGIN      	50

static int latency_counter[HISTSIZE];
static unsigned int max_delay;
static unsigned int min_delay = 100000;
static const char default_path[] = "/dev/bfin_latencytest";
static int fd;

static int set_realtime_priority(void)
{
	struct sched_param schp;

	/*
	 * set the process to realtime privs
	 */
	memset(&schp, 0, sizeof(schp));
	schp.sched_priority = sched_get_priority_max(SCHED_FIFO);

	if (sched_setscheduler(0, SCHED_FIFO, &schp) != 0) {
		perror("sched_setscheduler");
		exit(1);
	}
	return 0;
}

static void sigio_handler(int sig)
{
	struct timeval data,now;
	unsigned int delay;
	int retval, index;

	gettimeofday(&now,NULL);

	retval = read(fd, &data, sizeof(struct timeval));
	if (retval == -1) {
		perror("read");
		exit(errno);
	}

	delay = 1000000*(now.tv_sec - data.tv_sec)  +
		(now.tv_usec - data.tv_usec);
	if(delay < min_delay)
		min_delay = delay;
	if(delay > max_delay)
		max_delay = delay;

	if(delay >= SLOT_BEGIN ) {
		index = (delay - SLOT_BEGIN) / HIST_USEC;
		if(index > HISTSIZE)
			printf("bump: a %d delay happed\n", delay);
		latency_counter[index]  ;
	}
}

int main(int argc, char **argv)
{
	int i, oflags, retval;
	const char *path = default_path;
	int runtime;

	switch (argc) {
		case 2:
			runtime = atoi(argv[1]);
			break;
		default:
			printf("usage:  preempt_test [runtime(in minutes)]\n");
			return 1;
	}

	set_realtime_priority();
	if (mlockall(MCL_CURRENT|MCL_FUTURE) != 0) {
		perror("mlockall");
		exit(1);
	}

	fd = open(path, O_RDONLY);
	if (fd ==  -1) {
		perror(path);
		exit(errno);
	}

	printf("\n\t\t\tPreempt Latency Test.\n\n");
	if (fcntl(fd, F_SETOWN, getpid()) < 0) {
		perror ("fcntl setown: ");
		return -1;
	}

	if ((oflags = fcntl(fd, F_GETFL)) < 0) {
		perror ("fcntl getfl: ");
		return -1;
	}

	if (fcntl(fd, F_SETFL, oflags | FASYNC) < 0) {
		perror ("fcntl setfl: ");
		return -1;
	}

	/* Turn on update interrupts (one per second) */
	retval = ioctl(fd, BFIN_LATENCYTEST_START, 0);
	if (retval == -1) {
		if (errno == ENOTTY) {
			printf("\nerror..Update IRQs not supported.\n");
			goto done;
		}
		perror("BFIN_LATENCYTEST_START ioctl");
		exit(errno);
	}

	signal(SIGIO, sigio_handler);
	for (i = 0; i < 60 * runtime; i  )
	{
		sleep(10000);
	}

	/* Turn off update interrupts */
	retval = ioctl(fd, BFIN_LATENCYTEST_STOP, 0);
	if (retval == -1) {
		perror("BFIN_LATENCYTEST_STOP ioctl");
		exit(errno);
	}
	close(fd);

	printf("min_delay: %d, max_delay : %d\n", min_delay, max_delay);
	for(i = 0; i< HISTSIZE; i  ){
		printf("latency_counter[%3d---%3dus] = %d\n",
				i * HIST_USEC + SLOT_BEGIN,
				(i + 1) * HIST_USEC + SLOT_BEGIN,
				latency_counter[i]);
	}
	printf("\n\n\t\t\t *** Test complete ***\n");
done:
	return 0;
}
