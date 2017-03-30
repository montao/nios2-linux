cmd_libbb/ptr_to_globals.o := nios2-linux-gnu-gcc -Wp,-MD,libbb/.ptr_to_globals.o.d  -DEMBED -I/home/developer/altera/nios2-linux/uClinux-dist   -mhw-mul -mno-hw-mulx -mno-hw-div -std=gnu99 -Iinclude -Ilibbb -Iinclude2 -I/home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/include -I/home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.18.4)" -DBB_BT=AUTOCONF_TIMESTAMP -I/home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb -Ilibbb -pipe -Wall -g -O2 -mhw-mul -mno-hw-mulx -mno-hw-div -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ptr_to_globals)"  -D"KBUILD_MODNAME=KBUILD_STR(ptr_to_globals)" -c -o libbb/ptr_to_globals.o /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb/ptr_to_globals.c

deps_libbb/ptr_to_globals.o := \
  /home/developer/altera/nios2-linux/uClinux-dist/user/busybox/busybox-1.18.4/libbb/ptr_to_globals.c \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/features.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/predefs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/sys/cdefs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/wordsize.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/gnu/stubs.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/bits/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/linux/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm-generic/errno.h \
  /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../nios2-linux-gnu/libc/usr/include/asm-generic/errno-base.h \

libbb/ptr_to_globals.o: $(deps_libbb/ptr_to_globals.o)

$(deps_libbb/ptr_to_globals.o):
