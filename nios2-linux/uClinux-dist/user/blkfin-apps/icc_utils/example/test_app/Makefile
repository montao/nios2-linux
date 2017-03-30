CC := bfin-uclinux-gcc

# we have to clobber a bunch of flags as they are normally geared towards
# running under linux, not a bare metal environment like core b

KERNEL_DIR ?= ${PWD}/../../../../../linux-2.6.x
ICC_INCLUDE ?= ${pwd}/../icc_core
CFLAGS  ?=
CFLAGS  += -I$(KERNEL_DIR)/arch/blackfin/include -I$(KERNEL_DIR)/arch/blackfin/mach-$(ICC_MACHINE)/include -I$(KERNEL_DIR)/drivers/staging/icc/include -I$(ICC_INCLUDE) -g
LDFLAGS  = $(CFLAGS)
ASFLAGS  = $(CFLAGS)
PWD:= $(shell pwd)

all: scalar_test packet_test session_packet_test icc_latency_test

packet_test: packet.o
	$(CC) $(CFLAGS) $< -o $@ > $@.map

scalar_test: scalar.o
	$(CC) $(CFLAGS) $< -o $@ > $@.map

icc_latency_test: icc_latency.o
	$(CC) $(CFLAGS) $< -o $@ > $@.map

mcapi_cmd_test: mcapi_cmd.o
	$(CC) $(CFLAGS) $< -o $@ > $@.map

packet_wrap_test: wrap_test.o icc_wrap.o
	$(CC) $(CFLAGS) $? -o $@ > $@.map

session_packet_test: session_packet.o
	$(CC) $(CFLAGS) $< -o $@ > $@.map

clean:
	rm -rf *.o *.map *.gdb packet_test session_packet_test scalar_test

.PHONY: all clean
