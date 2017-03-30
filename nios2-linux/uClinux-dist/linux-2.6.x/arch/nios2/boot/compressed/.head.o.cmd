cmd_arch/nios2/boot/compressed/head.o := nios2-linux-gnu-gcc -Wp,-MD,arch/nios2/boot/compressed/.head.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h -D__KERNEL__ -D__ASSEMBLY__   -c -o arch/nios2/boot/compressed/head.o /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/boot/compressed/head.S

source_arch/nios2/boot/compressed/head.o := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/boot/compressed/head.S

deps_arch/nios2/boot/compressed/head.o := \
    $(wildcard include/config/mem/base.h) \
    $(wildcard include/config/kernel/region/base.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cache.h \
    $(wildcard include/config/nios2/dcache/size.h) \
    $(wildcard include/config/nios2/icache/size.h) \
    $(wildcard include/config/nios2/dcache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/shift.h) \

arch/nios2/boot/compressed/head.o: $(deps_arch/nios2/boot/compressed/head.o)

$(deps_arch/nios2/boot/compressed/head.o):
