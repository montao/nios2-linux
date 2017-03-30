CC := bfin-elf-gcc -g
LD := bfin-elf-ld

# we have to clobber a bunch of flags as they are normally geared towards
# running under linux, not a bare metal environment like core b
PWD:= $(shell pwd)
KERNEL_DIR ?= ${PWD}/../../../../linux-2.6.x
ICC_INCLUDE ?= ${PWD}/../include
ICC_MACHINE ?= bf561
ifeq ($(ICC_MACHINE), bf609)
CPU_TYPE := CONFIG_BF609
else
CPU_TYPE := CONFIG_BF561
endif
ICC_CFLAGS ?= -I${KERNEL_DIR}/arch/blackfin/include \
	-I${KERNEL_DIR}/include \
	-I${KERNEL_DIR}/arch/blackfin/mach-$(ICC_MACHINE)/include \
	-I${KERNEL_DIR}/arch/blackfin/include/generated \
	-I${KERNEL_DIR}/drivers/staging/icc/include


CFLAGS  = -I${ICC_INCLUDE} $(ICC_CFLAGS) -D$(CPU_TYPE) -D__KERNEL__ -mlong-calls
LDFLAGS  = $(CFLAGS)
ASFLAGS  = $(CFLAGS)
all: icc

coreb.lds: coreb.lds.S
	$(CC) -E -ansi $(CFLAGS) -D__ASSEMBLY__ -P $^ -o $@

ICC_OBJS = coreb_start.o test.o corebboot.o vsprintf.o mempool.o protocol.o platform.o
icc: $(ICC_OBJS) coreb.lds
	$(CC) -Wl,--verbose -Wl,-M -T coreb.lds -Wl,--nostdlib $(CFLAGS) $(ICC_OBJS) -o $@ > $@.map
	bfin-elf-objdump -x -D $@ > $@.dump

clean:
	rm -f *.map *.dump *.elf *.gdb *.o icc coreb.lds

.PHONY: all clean romfs
