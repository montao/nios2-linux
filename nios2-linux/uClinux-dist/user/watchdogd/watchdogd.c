#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <signal.h>
#include <paths.h>
#include <sched.h>

int fd = -1;

/*
 * This function simply sends an IOCTL to the driver, which in turn ticks
 * the PC Watchdog card to reset its internal timer so it doesn't trigger
 * a computer reset.
 */
void keep_alive(void)
{
	int dummy;

	ioctl(fd, WDIOC_KEEPALIVE, &dummy);
}

void safe_exit()
{
	if (fd != -1) {
		write(fd, "V", 1);
		close(fd);
	}
	exit(0);
}

int set_wd_counter(int count)
{
	return ioctl(fd, WDIOC_SETTIMEOUT, &count);
}

int get_wd_counter()
{
	int count;
	int err;
	if ((err = ioctl(fd, WDIOC_GETTIMEOUT, &count))) {
		count = err;
	}
	return count;
}

static void usage(char *argv[])
{
	printf(
		"%s [-f] [-w <sec>] [-k <sec>] [-p <prio>] [-s] [-h|--help]\n"
		"A simple watchdog daemon that send WDIOC_KEEPALIVE ioctl every some\n"
		"\"heartbeat of keepalives\" seconds.\n"
		"Options:\n"
		"\t-f        start in foreground (background is default)\n"
		"\t-w <sec>  set the watchdog counter to <sec> in seconds\n"
		"\t-k <sec>  set the \"heartbeat of keepalives\" to <sec> in seconds\n"
		"\t-p <prio> set the schedule priority\n"
		"\t-s        safe exit (disable Watchdog) for CTRL-c and kill -SIGTERM signals\n"
		"\t--help|-h write this help message and exit\n",
		argv[0]);
}

/*
 * The main program.
 */
int main(int argc, char *argv[])
{
	int wd_count = 20;
	int real_wd_count = 0;
	int wd_keep_alive = wd_count / 2;
	struct sched_param sp = { .sched_priority = 1, };
	int background = 1;
	int opt;

	while ((opt = getopt(argc, argv, "fhk:p:sw:")) != -1) {
		switch (opt) {
			case 'f':
				background = 0;
				break;

			case 'h':
				usage(argv);
				return 0;

			case 'k':
				wd_keep_alive = atoi(optarg);
				break;

			case 'p':
				sp.sched_priority = atoi(optarg);
				break;

			case 's': {
				struct sigaction sa;
				memset(&sa, 0, sizeof(sa));

				sa.sa_handler = safe_exit;
				sigaction(SIGHUP, &sa, NULL);
				sigaction(SIGINT, &sa, NULL);
				sigaction(SIGTERM, &sa, NULL);
				break;
			}

			case 'w':
				wd_count = atoi(optarg);
				break;

			default:
				/* getopt() will output an error msg for us */
				return 1;
		}
	}

	if (background)
		daemon(0, 0);

	if (sched_setscheduler(0, SCHED_RR, &sp))
		perror("sched_setscheduler(SCHED_RR) failed");

	fd = open("/dev/watchdog", O_WRONLY);

	if (fd == -1) {
		perror("Watchdog device not enabled");
		fflush(stderr);
		exit(-1);
	}

	if (set_wd_counter(wd_count)) {
		fprintf(stderr, "-w switch: wrong value. Please look at kernel log for more dettails.\n Continue with the old value\n");
		fflush(stderr);
	}

	real_wd_count = get_wd_counter();
	if (real_wd_count < 0) {
		perror("Error while issue IOCTL WDIOC_GETTIMEOUT");
	} else {
		if (real_wd_count <= wd_keep_alive) {
			fprintf(stderr,
				"Warning watchdog counter less or equal to the heartbeat of keepalives: %d <= %d\n",
				real_wd_count, wd_keep_alive);
			fflush(stderr);
		}
	}

	while (1) {
		keep_alive();
		sleep(wd_keep_alive);
	}
}
