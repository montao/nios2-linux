/*
 * nios2_oprofile.c - Nios II oprofile code
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/oprofile.h>
#include <linux/init.h>

int __init oprofile_arch_init(struct oprofile_operations *ops)
{
	return -1;
}

void oprofile_arch_exit(void)
{
}
