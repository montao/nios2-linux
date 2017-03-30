cmd_arch/nios2/kernel/syscalltable.o := nios2-linux-gnu-gcc -Wp,-MD,arch/nios2/kernel/.syscalltable.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h -D__KERNEL__ -D__ASSEMBLY__   -c -o arch/nios2/kernel/syscalltable.o /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/kernel/syscalltable.S

source_arch/nios2/kernel/syscalltable.o := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/kernel/syscalltable.S

deps_arch/nios2/kernel/syscalltable.o := \
    $(wildcard include/config/mmu.h) \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/sys.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/linkage.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/unistd.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include/asm/asm-macros.h \

arch/nios2/kernel/syscalltable.o: $(deps_arch/nios2/kernel/syscalltable.o)

$(deps_arch/nios2/kernel/syscalltable.o):
