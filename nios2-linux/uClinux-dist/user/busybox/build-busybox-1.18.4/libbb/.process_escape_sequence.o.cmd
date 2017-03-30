cmd_libbb/process_escape_sequence.o := nios2-linux-gnu-gcc -Wp,-MD,libbb/.process_escape_sequence.o.d  -DEMBED -I/home/developer/altera/nios2-linux/uClinux-dist   -mhw-mul -mno-hw-mulx -mno-hw-div -std=gnu99 -Iinclude -Ilibbb -Iinclude2 -I/home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include -I/home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.18.4)" -DBB_BT=AUTOCONF_TIMESTAMP -I/home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb -Ilibbb -pipe -Wall -g -O2 -mhw-mul -mno-hw-mulx -mno-hw-div -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(process_escape_sequence)"  -D"KBUILD_MODNAME=KBUILD_STR(process_escape_sequence)" -c -o libbb/process_escape_sequence.o /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb/process_escape_sequence.c

deps_libbb/process_escape_sequence.o := \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb/process_escape_sequence.c \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/pmap.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/feature/devfs.h) \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/big/endian.h) \
    $(wildcard include/config/little/endian.h) \
    $(wildcard include/config/nommu.h) \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include-fixed/limits.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include-fixed/syslimits.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/limits.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/features.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/predefs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/cdefs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/wordsize.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/gnu/stubs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/posix1_lim.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/local_lim.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/linux/limits.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/posix2_lim.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/xopen_lim.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/stdio_lim.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/byteswap.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/byteswap.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/endian.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/endian.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include/stdbool.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/ctype.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/types.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include/stddef.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/typesizes.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/xlocale.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/dirent.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/dirent.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/linux/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm-generic/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm-generic/errno-base.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/fcntl.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/fcntl.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/types.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/time.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/select.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/select.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sigset.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/time.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/sysmacros.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/pthreadtypes.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/uio.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/stat.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/stat.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/inttypes.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/stdint.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/wchar.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/netdb.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/netinet/in.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/socket.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/uio.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/socket.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sockaddr.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/socket.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/sockios.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/in.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/rpc/netdb.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/siginfo.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/netdb.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/setjmp.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/setjmp.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/signal.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/signum.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sigaction.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sigcontext.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/sigcontext.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/ptrace.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sigstack.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/ucontext.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sigthread.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/stdio.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/libio.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/_G_config.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/wchar.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/gconv.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include/stdarg.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/sys_errlist.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/stdlib.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/waitflags.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/waitstatus.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/alloca.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/string.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/poll.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/poll.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/ioctl.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/ioctls.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/ioctls.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/ioctl.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/ioctl-types.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/ttydefaults.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/mman.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/mman.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/time.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/wait.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/resource.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/resource.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/termios.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/termios.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/unistd.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/posix_opt.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/environments.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/confname.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/getopt.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/param.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/linux/param.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/param.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/mntent.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/paths.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/statfs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/statfs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/pwd.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/grp.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/arpa/inet.h \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include/pwd_.h \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include/grp_.h \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include/xatonum.h \

libbb/process_escape_sequence.o: $(deps_libbb/process_escape_sequence.o)

$(deps_libbb/process_escape_sequence.o):
