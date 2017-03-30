CFLAGS ?= -O2 -pipe
CFLAGS += -Wall

all: bfin_crc_test

romfs:
	$(ROMFSINST) /bin/bfin_crc_test

clean:
	rm -f bfin_crc_test *.elf *.gdb *.o
