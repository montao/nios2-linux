CC = bfin-uclinux-gcc

# we have to clobber a bunch of flags as they are normally geared towards
# running under linux, not a bare metal environment like core b

PWD:= $(shell pwd)
KERNEL_DIR ?= ${PWD}/../../../../linux-2.6.x
ICC_INCLUDE ?= $(PWD)/../icc_core
ICC_CFLAGS  := -I$(KERNEL_DIR)/drivers/staging/icc/include -I$(ICC_INCLUDE) -g

CFLAGS  = $(ICC_CFLAGS)

all: icc_loader

icc_loader: icc_loader.o
	$(CC) $(CFLAGS) icc_loader.o  -o $@ > $@.map
clean:
	rm -rf *.o *.map *.gdb icc_loader

.PHONY: all clean
