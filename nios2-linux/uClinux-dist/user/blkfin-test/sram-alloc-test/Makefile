ifeq ($(origin CC),default)
CC := bfin-uclinux-gcc
endif

CFLAGS ?= -O2 -g
CFLAGS += -Wall

EXEC = sram_alloc

all: $(EXEC)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	rm -f $(EXEC) *.elf *.gdb *.o
 
.PHONY: all clean romfs
