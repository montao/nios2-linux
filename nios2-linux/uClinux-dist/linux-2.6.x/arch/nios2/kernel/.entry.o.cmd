cmd_arch/nios2/kernel/entry.o := nios2-linux-gnu-gcc -Wp,-MD,arch/nios2/kernel/.entry.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h -D__KERNEL__ -D__ASSEMBLY__   -c -o arch/nios2/kernel/entry.o /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/kernel/entry.S

source_arch/nios2/kernel/entry.o := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/kernel/entry.S

deps_arch/nios2/kernel/entry.o := \
    $(wildcard include/config/kgdb.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/alignment/trap.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sys.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/asm-macros.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/thread_info.h \
    $(wildcard include/config/mmu.h) \
  arch/nios2/include/generated/asm/errno.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/errno.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/errno-base.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/setup.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/setup.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/entry.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/processor.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/ptrace.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/registers.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/unistd.h \

arch/nios2/kernel/entry.o: $(deps_arch/nios2/kernel/entry.o)

$(deps_arch/nios2/kernel/entry.o):
