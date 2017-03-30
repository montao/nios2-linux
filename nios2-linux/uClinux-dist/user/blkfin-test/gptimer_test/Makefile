ROOTDIR ?= $(PWD)/../../../
LINUXDIR ?= linux-2.6.x
CC ?= bfin-uclinux-gcc

EXEC = simple_timer_test
OBJS = simple_timer_test.o

CFLAGS += -I$(ROOTDIR)/$(LINUXDIR)/arch/blackfin/include -I$(ROOTDIR)/$(LINUXDIR)/include

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

romfs::
	$(ROMFSINST) /bin/$(EXEC)

clean::
	-rm -f $(EXEC) *.elf *.gdb *.o
