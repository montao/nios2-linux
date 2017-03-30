ROOTDIR ?= $(PWD)/../../../
include $(ROOTDIR)/tools/kernel-module.mk

obj-m := timerbench.o

all: latency

latency: LDLIBS += -lm -lpthread -lrt

romfs::
	$(ROMFSINST) /bin/latency

clean::
	rm -f latency latency.gdb *.o
