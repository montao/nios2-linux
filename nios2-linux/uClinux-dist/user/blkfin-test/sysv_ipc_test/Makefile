CFLAGS ?= -O2 -g -pipe
CFLAGS += -Wall

EXEC = msgtool shmtool semtool

all: $(EXEC)

clean:
	rm -f *.o $(EXEC) *.gdb *~ *.elf

romfs:
	$(ROMFSINST) /bin/msgtool
	$(ROMFSINST) /bin/shmtool
	$(ROMFSINST) /bin/semtool
