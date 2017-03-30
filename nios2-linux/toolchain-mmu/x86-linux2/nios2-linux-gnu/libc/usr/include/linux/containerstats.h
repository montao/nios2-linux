/* containerstats.h - exporting per-container statistics
 *
 * Copyright IBM Corporation, 2007
 * Author Balbir Singh <balbir@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef _LINUX_CONTAINERSTATS_H
#define _LINUX_CONTAINERSTATS_H

#include <linux/taskstats.h>

/*
 * Data shared between user space and kernel space on a per container
 * basis. This data is shared using taskstats.
 *
 * Most of these states are derived by looking at the task->state value
 * For the nr_io_wait state, a flag in the delay accounting structure
 * indicates that the task is waiting on IO
 *
 * Each member is aligned to a 8 byte boundary.
 */
struct containerstats {
	__u64	nr_sleeping;		/* Number of tasks sleeping */
	__u64	nr_running;		/* Number of tasks running */
	__u64	nr_stopped;		/* Number of tasks in stopped state */
	__u64	nr_uninterruptible;	/* Number of tasks in uninterruptible */
					/* state */
	__u64	nr_io_wait;		/* Number of tasks waiting on IO */
};

/*
 * Commands sent from userspace
 * Not versioned. New commands should only be inserted at the enum's end
 * prior to __CONTAINERSTATS_CMD_MAX
 */

enum {
	CONTAINERSTATS_CMD_UNSPEC = __TASKSTATS_CMD_MAX,	/* Reserved */
	CONTAINERSTATS_CMD_GET,		/* user->kernel request/get-response */
	CONTAINERSTATS_CMD_NEW,		/* kernel->user event */
	__CONTAINERSTATS_CMD_MAX,
};

#define CONTAINERSTATS_CMD_MAX (__CONTAINERSTATS_CMD_MAX - 1)

enum {
	CONTAINERSTATS_TYPE_UNSPEC = 0,	/* Reserved */
	CONTAINERSTATS_TYPE_CONTAINER_STATS,	/* contains name + stats */
	__CONTAINERSTATS_TYPE_MAX,
};

#define CONTAINERSTATS_TYPE_MAX (__CONTAINERSTATS_TYPE_MAX - 1)

enum {
	CONTAINERSTATS_CMD_ATTR_UNSPEC = 0,
	CONTAINERSTATS_CMD_ATTR_FD,
	__CONTAINERSTATS_CMD_ATTR_MAX,
};

#define CONTAINERSTATS_CMD_ATTR_MAX (__CONTAINERSTATS_CMD_ATTR_MAX - 1)

#endif /* _LINUX_CONTAINERSTATS_H */
