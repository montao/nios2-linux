#ifndef _ASM_NIOS2_UNISTD_H_
#define _ASM_NIOS2_UNISTD_H_

/*--------------------------------------------------------------------
 *
 * include/asm-nios2/unistd.h
 *
 * Derived from various works, Alpha, ix86, M68K, Sparc, ...et al
 *
 * Copyright (C) 2004   Microtronix Datacom Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * This file was copied from nios2 and reworked.
 *
 * Dec/12/2008    frma/ivho     Ported to nios2 with mmu.
 *
 ---------------------------------------------------------------------*/


#include <asm/traps.h>

#if defined(__ASSEMBLY__) || defined(ASSEMBLER)

#define UR ERROR

#else

extern int unresolved_symbol;
#define UR unresolved_symbol

#endif

/* TRAP 0 isr expects the the syscall # in r3, and arguments in r4, r5, ...
 * Return value in r2 and error/ok flag in r7.
 */
#define __NR_restart_syscall            0
#define __NR_exit                       1
#define __NR_fork                       2
#define __NR_read                       3
#define __NR_write                      4
#define __NR_open                       5
#define __NR_close                      6
/* waitpid */
#define __NR_creat                      8
#define __NR_link                       9
#define __NR_unlink                     10
#define __NR_execve                     11
#define __NR_chdir                      12
#define __NR_time                       13
#define __NR_mknod                      14
#define __NR_chmod                      15
#define __NR_lchown                      182
/* sys_break */
/* sys_stat */
#define __NR_lseek                      19
#define __NR_getpid                     20
#define __NR_mount                      21
#define __NR_umount                     22
#define __NR_setuid                     23
#define __NR_getuid                     24
#define __NR_stime                      25
#define __NR_ptrace                     26
#define __NR_alarm                      27
/* sys_fstat */
#define __NR_pause                      29
#define __NR_utime                      30
/* sys_stty */
/* sys_gtty */
#define __NR_access                     33
#define __NR_nice                       34
/* sys_ftime */
#define __NR_sync                       36
#define __NR_kill                       37
#define __NR_rename                     38
#define __NR_mkdir                      39
#define __NR_rmdir                      40
#define __NR_dup                        41
#define __NR_pipe                       42
#define __NR_times                      43
/* sys_prof */
#define __NR_brk                        45
#define __NR_setgid                     46
#define __NR_getgid                     47
/* sys_signal */
#define __NR_geteuid                    49
#define __NR_getegid                    50
#define __NR_acct                       51
#define __NR_umount2                    52
/* sys_lock */
#define __NR_ioctl                      54
#define __NR_fcntl                      55
/* sys_mpx */
#define __NR_setpgid                    57
/* sys_ulimit */
/* sys_olduname */
#define __NR_umask                      60
#define __NR_chroot                     61
#define __NR_ustat                      62
#define __NR_dup2                       63
#define __NR_getppid                    64
#define __NR_getpgrp                    65
#define __NR_setsid                     66
#define __NR_sigaction                  67
/* sys_sgetmask */
/* sys_ssetmask */
#define __NR_setreuid                   70
#define __NR_setregid                   71
#define __NR_sigsuspend                 72
#define __NR_sigpending                 73
#define __NR_sethostname                74
#define __NR_setrlimit                  75
/* get_rlimit */
#define __NR_getrusage                  77
#define __NR_gettimeofday               78
#define __NR_settimeofday               79
#define __NR_getgroups                  80
#define __NR_setgroups                  81
/* sys_select */
#define __NR_symlink                    83
/* sys_lstat */
#define __NR_readlink                   85
#define __NR_uselib                     86
#define __NR_swapon                     87
#define __NR_reboot                     88
/* sys_readdir */
#define __NR_mmap                       90
#define __NR_munmap                     91
#define __NR_truncate                   92
#define __NR_ftruncate                  93
#define __NR_fchmod                     94
#define __NR_fchown                     95
#define __NR_getpriority                96
#define __NR_setpriority                97
/* sys_profil */
#define __NR_statfs                     99
#define __NR_fstatfs                    100
/* sys_ioperm */
#define __NR_socketcall                 102
#define __NR_syslog                     103
#define __NR_setitimer                  104
#define __NR_getitimer                  105
#define __NR_stat                       106
#define __NR_lstat                      107
#define __NR_fstat                      108
/* sys_uname */
/* sys_iopl */
#define __NR_vhangup                    111
/* sys_idle */
/* sys_syscall */
#define __NR_wait4                      114
#define __NR_swapoff                    115
#define __NR_sysinfo                    116
#define __NR_ipc                        117
#define __NR_fsync                      118
#define __NR_sigreturn                  119
#define __NR_clone                      120
#define __NR_setdomainname              121
#define __NR_uname                      122
#define __NR_cacheflush                 123
#define __NR_adjtimex                   124
#define __NR_mprotect                   125
#define __NR_sigprocmask                126
/* sys_creat_module */
#define __NR_init_module                128
#define __NR_delete_module              129
/* sys_get_kernel_syms */
#define __NR_quotactl                   131
#define __NR_getpgid                    132
#define __NR_fchdir                     133
#define __NR_bdflush                    134
#define __NR_sysfs                      135
#define __NR_personality                136
/* sys_afs_syscall */
#define __NR_setfsuid                   138
#define __NR_setfsgid                   139
#define __NR__llseek                    140
#define __NR_getdents                   141
#define __NR__newselect                 142
#define __NR_flock                      143
#define __NR_msync                      144
#define __NR_readv                      145
#define __NR_writev                     146
#define __NR_getsid                     147
#define __NR_fdatasync                  148
#define __NR__sysctl                    149
#define __NR_mlock                      150
#define __NR_munlock                    151
#define __NR_mlockall                   152
#define __NR_munlockall                 153
#define __NR_sched_setparam             154
#define __NR_sched_getparam             155
#define __NR_sched_setscheduler         156
#define __NR_sched_getscheduler         157
#define __NR_sched_yield                158
#define __NR_sched_get_priority_max     159
#define __NR_sched_get_priority_min     160
#define __NR_sched_rr_get_interval      161
#define __NR_nanosleep                  162
#define __NR_mremap                     163
#define __NR_setresuid                  164
#define __NR_getresuid                  165
#define __NR_getpagesize                166
/* sys_query_module */
#define __NR_poll                       168
#define __NR_nfsservctl                 169
#define __NR_setresgid                  170
#define __NR_getresgid                  171
#define __NR_prctl                      172
#define __NR_rt_sigreturn               173
#define __NR_rt_sigaction               174
#define __NR_rt_sigprocmask             175
#define __NR_rt_sigpending              176
#define __NR_rt_sigtimedwait            177
#define __NR_rt_sigqueueinfo            178
#define __NR_rt_sigsuspend              179
#define __NR_pread64                    320
#define __NR_pwrite64                   321
#define __NR_chown                      16
#define __NR_getcwd                     183
#define __NR_capget                     184
#define __NR_capset                     185
#define __NR_sigaltstack                186
#define __NR_sendfile                   187
/* sys_getpmsg */
/* sys_putpmsg */
#define __NR_vfork                      190
#define __NR_ugetrlimit                 191
/* mmap2 */
#define __NR_truncate64                 193
#define __NR_ftruncate64                194
#define __NR_stat64                     195
#define __NR_lstat64                    196
#define __NR_fstat64                    197
#define __NR_lchown32                   198
#define __NR_getuid32                   199
#define __NR_getgid32                   200
#define __NR_geteuid32                  201
#define __NR_getegid32                  202
#define __NR_setreuid32                 203
#define __NR_setregid32                 204
#define __NR_getgroups32                205
#define __NR_setgroups32                206
#define __NR_fchown32                   207
#define __NR_setresuid32                208
#define __NR_getresuid32                209
#define __NR_setresgid32                210
#define __NR_getresgid32                211
#define __NR_chown32                    212
#define __NR_setuid32                   213
#define __NR_setgid32                   214
#define __NR_setfsuid32                 215
#define __NR_setfsgid32                 216
#define __NR_getdents64                 220
#define __NR_pivot_root                 217
#define __NR_mincore                    237
#define __NR_madvise                    238
#define __NR_fcntl64                    239
#define __NR_gettid                     221
#define __NR_readahead                  240
#define __NR_setxattr                   223
#define __NR_lsetxattr                  224
#define __NR_fsetxattr                  225
#define __NR_getxattr                   226
#define __NR_lgetxattr                  227
#define __NR_fgetxattr                  228
#define __NR_listxattr                  229
#define __NR_llistxattr                 230
#define __NR_flistxattr                 231
#define __NR_removexattr                232
#define __NR_lremovexattr               233
#define __NR_fremovexattr               234
#define __NR_tkill                      222
#define __NR_sendfile64                 236
#define __NR_futex                      235
#define __NR_sched_setaffinity          311
#define __NR_sched_getaffinity          312
#define __NR_io_setup                   241
#define __NR_io_destroy                 242
#define __NR_io_getevents               243
#define __NR_io_submit                  244
#define __NR_io_cancel                  245
#define __NR_exit_group                 247
#define __NR_lookup_dcookie             248
#define __NR_epoll_create               249
#define __NR_epoll_ctl                  250
#define __NR_epoll_wait                 251
#define __NR_remap_file_pages           252
#define __NR_set_tid_address            253
#define __NR_timer_create               254
#define __NR_timer_settime              255
#define __NR_timer_gettime              256
#define __NR_timer_getoverrun           257
#define __NR_timer_delete               258
#define __NR_clock_settime              259
#define __NR_clock_gettime              260
#define __NR_clock_getres               261
#define __NR_clock_nanosleep            262
#define __NR_statfs64                   263
#define __NR_fstatfs64                  264
#define __NR_tgkill                     265
#define __NR_utimes                     266
#define __NR_fadvise64                  246
#define __NR_fadvise64_64               267
#define __NR_mq_open                    271
#define __NR_mq_unlink                  272
#define __NR_mq_timedsend               273
#define __NR_mq_timedreceive            274
#define __NR_mq_notify                  275
#define __NR_mq_getsetattr              276
#define __NR_waitid                     277
#define __NR_ioprio_set                 282
#define __NR_ioprio_get                 283
#define __NR_inotify_init               284
#define __NR_inotify_add_watch          285
#define __NR_inotify_rm_watch           286
#define __NR_mbind                      268
#define __NR_get_mempolicy              269
#define __NR_set_mempolicy              270
/* migrate_pages */
#define __NR_openat                     288
#define __NR_mkdirat                    289
#define __NR_mknodat                    290
#define __NR_fchownat                   291
#define __NR_futimesat                  292
#define __NR_fstatat64                  293
#define __NR_unlinkat                   294
#define __NR_renameat                   295
#define __NR_linkat                     296
#define __NR_symlinkat                  297
#define __NR_readlinkat                 298
#define __NR_fchmodat                   299
#define __NR_faccessat                  300
/* sys_pselect */
/* sys_ppoll */
#define __NR_unshare                    303
#define __NR_set_robust_list            304
#define __NR_get_robust_list            305
#define __NR_splice                     306
#define __NR_sync_file_range            307
#define __NR_tee                        308
#define __NR_vmsplice                   309
#define __NR_move_pages                 310
#define __NR_getcpu                     314
#define __NR_kexec_load                 313

#define __NR_nios2cmpxchg               113

#define NR_syscalls                     323


#endif  /* _ASM_NIOS2_UNISTD_H_ */
