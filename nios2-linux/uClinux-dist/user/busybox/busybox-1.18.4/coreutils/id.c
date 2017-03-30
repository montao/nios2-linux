/* vi: set sw=4 ts=4: */
/*
 * Mini id implementation for busybox
 *
 * Copyright (C) 2000 by Randolph Chung <tausq@debian.org>
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

/* BB_AUDIT SUSv3 _NOT_ compliant -- option -G is not currently supported. */
/* Hacked by Tito Ragusa (C) 2004 to handle usernames of whatever length and to
 * be more similar to GNU id.
 * -Z option support: by Yuichi Nakamura <ynakam@hitachisoft.jp>
 */

#include "libbb.h"

#define PRINT_REAL        1
#define NAME_NOT_NUMBER   2
#define JUST_USER         4
#define JUST_GROUP        8

static int printf_full(unsigned int id, const char *arg, const char prefix)
{
	const char *fmt = "%cid=%u";
	int status = EXIT_FAILURE;

	if (arg) {
		fmt = "%cid=%u(%s)";
		status = EXIT_SUCCESS;
	}
	printf(fmt, prefix, id, arg);
	return status;
}

int id_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int id_main(int argc UNUSED_PARAM, char **argv)
{
	struct passwd *p;
	uid_t uid;
	gid_t gid;
	unsigned long flags;
	short status;

	/* Don't allow -n -r -nr -ug -rug -nug -rnug */
	/* Don't allow more than one username */
	opt_complementary = "?1:u--g:g--u:r?ug:n?ug";
	flags = getopt32(argv, "rnug");

	/* This values could be overwritten later */
	uid = geteuid();
	gid = getegid();
	if (flags & PRINT_REAL) {
		uid = getuid();
		gid = getgid();
	}

	if (argv[optind]) {
		p = getpwnam(argv[optind]);
		/* xuname2uid is needed because it exits on failure */
		uid = xuname2uid(argv[optind]);
		gid = p->pw_gid;
		/* in this case PRINT_REAL is the same */
	}

	if (flags & (JUST_GROUP | JUST_USER)) {
		/* JUST_GROUP and JUST_USER are mutually exclusive */
		if (flags & NAME_NOT_NUMBER) {
			/* bb_getXXXid(-1) exit on failure, puts cannot segfault */
			puts((flags & JUST_USER) ? xuid2uname(uid) : xgid2group(gid));
		} else {
			if (flags & JUST_USER) {
				printf("%u\n", uid);
			}
			if (flags & JUST_GROUP) {
				printf("%u\n", gid);
			}
		}

		/* exit */
		fflush_stdout_and_exit(EXIT_SUCCESS);
	}

	/* Print full info like GNU id */
	/* bb_getpwuid(0) doesn't exit on failure (returns NULL) */
	status = printf_full(uid, xuid2uname(uid), 'u');
	bb_putchar(' ');
	status |= printf_full(gid, xgid2group(gid), 'g');

	bb_putchar('\n');
	fflush_stdout_and_exit(status);
}
