cmd_arch/nios2/boot/linked_dtb.o := nios2-linux-gnu-gcc -Wp,-MD,arch/nios2/boot/.linked_dtb.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h -D__KERNEL__ -D__ASSEMBLY__   -c -o arch/nios2/boot/linked_dtb.o /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/boot/linked_dtb.S

source_arch/nios2/boot/linked_dtb.o := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/boot/linked_dtb.S

deps_arch/nios2/boot/linked_dtb.o := \

arch/nios2/boot/linked_dtb.o: $(deps_arch/nios2/boot/linked_dtb.o)

$(deps_arch/nios2/boot/linked_dtb.o):
