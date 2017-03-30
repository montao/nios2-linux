CFLAGS ?= -g -O2
CFLAGS += -Wall
FLTFLAGS += -s 0x2000

all: event_test

romfs:
	$(ROMFSINST) /bin/event_test

clean:
	rm -f event_test *.elf *.gdb *.o

.PHONY: all clean romfs
