CFLAGS ?= -O2 -pipe
CFLAGS += -Wall

all: sport_test

romfs:
	$(ROMFSINST) /bin/sport_test
	$(ROMFSINST) /sample.wav

clean:
	rm -f sport_test *.elf *.gdb *.o
