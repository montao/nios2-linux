cmd_arch/nios2/lib/libgcc.o := nios2-linux-gnu-gcc -Wp,-MD,arch/nios2/lib/.libgcc.o.d  -nostdinc -isystem /home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin/../lib/gcc/nios2-linux-gnu/4.1.2/include -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/include -Iarch/nios2/include/generated -Iinclude  -I/home/developer/altera/nios2-linux/linux-2.6/include -include /home/developer/altera/nios2-linux/linux-2.6/include/linux/kconfig.h  -I/home/developer/altera/nios2-linux/linux-2.6/arch/nios2/lib -Iarch/nios2/lib -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -pipe -D__linux__ -D__ELF__ -mhw-mul -mno-hw-mulx -mno-hw-div -fno-optimize-sibling-calls -DUTS_SYSNAME=\"Linux\" -fno-builtin -G 0 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(libgcc)"  -D"KBUILD_MODNAME=KBUILD_STR(libgcc)" -c -o arch/nios2/lib/libgcc.o /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/lib/libgcc.c

source_arch/nios2/lib/libgcc.o := /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/lib/libgcc.c

deps_arch/nios2/lib/libgcc.o := \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/lib/libgcc.h \
  /home/developer/altera/nios2-linux/linux-2.6/arch/nios2/lib/longlong.h \

arch/nios2/lib/libgcc.o: $(deps_arch/nios2/lib/libgcc.o)

$(deps_arch/nios2/lib/libgcc.o):
