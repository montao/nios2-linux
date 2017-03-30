CFLAGS += -Wall

all: preempt_test 

romfs:
	$(ROMFSINST) -e CONFIG_USER_BLACKFIN_RTC /bin/preempt_test

clean:
	rm -f *.elf *.gdb *.o preempt_test

.PHONY: all clean romfs
