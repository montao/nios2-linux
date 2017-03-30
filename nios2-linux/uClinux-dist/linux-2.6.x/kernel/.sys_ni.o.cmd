cmd_kernel/sys_ni.o := nios2-linux-gnu-gcc -Wp,-MD,kernel/.sys_ni.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h  -I/home/developer/altera/nios2-linux/linux-2.6/kernel -Ikernel -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -pipe -D__linux__ -D__ELF__ -mhw-mul -mno-hw-mulx -mno-hw-div -fno-optimize-sibling-calls -DUTS_SYSNAME=\"Linux\" -fno-builtin -G 0 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(sys_ni)"  -D"KBUILD_MODNAME=KBUILD_STR(sys_ni)" -c -o kernel/sys_ni.o /home/developer/altera/nios2-linux/linux-2.6/kernel/sys_ni.c

source_kernel/sys_ni.o := /home/developer/altera/nios2-linux/linux-2.6/kernel/sys_ni.c

deps_kernel/sys_ni.o := \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler-gcc4.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/errno.h \
  arch/nios2/include/generated/asm/errno.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/errno.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/errno-base.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/unistd.h \

kernel/sys_ni.o: $(deps_kernel/sys_ni.o)

$(deps_kernel/sys_ni.o):
