CFLAGS += -Wall

all: xip_test

romfs:
	$(ROMFSINST) /bin/xip_test

clean:
	rm -f xip_test *.elf *.gdb *.o *~

.PHONY: all clean romfs
