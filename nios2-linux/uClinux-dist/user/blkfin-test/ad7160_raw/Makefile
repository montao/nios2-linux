CFLAGS += -Wall

EXEC = ad7160_raw

all: $(EXEC)

romfs:
	$(ROMFSINST) /bin/ad7160_raw
	
clean:
	rm -f $(EXEC) *.elf *.gdb *.o *~
