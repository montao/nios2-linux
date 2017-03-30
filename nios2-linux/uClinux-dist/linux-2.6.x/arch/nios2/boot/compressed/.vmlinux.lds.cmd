cmd_arch/nios2/boot/compressed/vmlinux.lds := nios2-linux-gnu-gcc -E -Wp,-MD,arch/nios2/boot/compressed/.vmlinux.lds.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h -D__KERNEL__ -P -C -Unios2 -D__ASSEMBLY__ -DLINKER_SCRIPT -o arch/nios2/boot/compressed/vmlinux.lds /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/boot/compressed/vmlinux.lds.S

source_arch/nios2/boot/compressed/vmlinux.lds := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/boot/compressed/vmlinux.lds.S

deps_arch/nios2/boot/compressed/vmlinux.lds := \
    $(wildcard include/config/mem/base.h) \
    $(wildcard include/config/boot/link/offset.h) \
    $(wildcard include/config/kernel/region/base.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/asm-generic/vmlinux.lds.h \
    $(wildcard include/config/hotplug.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/ftrace/syscalls.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/pm/trace.h) \
    $(wildcard include/config/blk/dev/initrd.h) \

arch/nios2/boot/compressed/vmlinux.lds: $(deps_arch/nios2/boot/compressed/vmlinux.lds)

$(deps_arch/nios2/boot/compressed/vmlinux.lds):
