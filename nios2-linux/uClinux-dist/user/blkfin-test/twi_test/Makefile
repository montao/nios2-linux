ROOTDIR  ?= $(PWD)/../../../
include $(ROOTDIR)/tools/kernel-module.mk

obj-m := twi_smbus_test.o

twi_test: CFLAGS += -Wall
all: twi_test

clean::
	rm -f twi_test

romfs::
	$(ROMFSINST) -e CONFIG_USER_BLACKFIN_TWI /bin/twi_test
