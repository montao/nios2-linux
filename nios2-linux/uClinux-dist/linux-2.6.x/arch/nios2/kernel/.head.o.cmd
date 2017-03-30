cmd_arch/nios2/kernel/head.o := nios2-linux-gnu-gcc -Wp,-MD,arch/nios2/kernel/.head.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h -D__KERNEL__ -D__ASSEMBLY__   -c -o arch/nios2/kernel/head.o /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/kernel/head.S

source_arch/nios2/kernel/head.o := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/kernel/head.S

deps_arch/nios2/kernel/head.o := \
    $(wildcard include/config/mmu.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  arch/nios2/include/generated/asm/types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/types.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/int-ll64.h \
  arch/nios2/include/generated/asm/bitsperlong.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/bitsperlong.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/thread_info.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/processor.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/ptrace.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/registers.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/cache.h \
    $(wildcard include/config/nios2/dcache/size.h) \
    $(wildcard include/config/nios2/icache/size.h) \
    $(wildcard include/config/nios2/dcache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/size.h) \
    $(wildcard include/config/nios2/icache/line/shift.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/page.h \
    $(wildcard include/config/mem/base.h) \
    $(wildcard include/config/kernel/region/base.h) \
    $(wildcard include/config/io/region/base.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/pfn.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/getorder.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/asm-macros.h \

arch/nios2/kernel/head.o: $(deps_arch/nios2/kernel/head.o)

$(deps_arch/nios2/kernel/head.o):
