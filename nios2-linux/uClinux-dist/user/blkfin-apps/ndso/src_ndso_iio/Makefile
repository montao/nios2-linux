CFLAGS += -Wall

EXEC = ndso
OBJS = ndso.o cgivars.o htmllib.o iio.o

#CFLAGS += -g

all: $(EXEC)

romfs:
	$(ROMFSINST) $(EXEC) /home/httpd/cgi-bin/ndso.cgi

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) -lm

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o

$(OBJS): cgivars.h htmllib.h
