CC      = bfin-uclinux-gcc
CFLAGS += -Wl,-elf2flt

SRCS    = process_a.c process_b.c
TARGETS = $(patsubst %.c,%,$(SRCS))

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
	rm -f $(wildcard *.gdb)
