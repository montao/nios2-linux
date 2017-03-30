ROOTDIR ?= $(PWD)/../../../
include $(ROOTDIR)/tools/kernel-module.mk

obj-m := cycles.o

all: user-cycles

user-cycles: CFLAGS += -Wall

romfs::
	$(ROMFSINST) /bin/user-cycles

clean::
	rm -f user-cycles
