ROOTDIR  ?= $(PWD)/../../../
include $(ROOTDIR)/tools/kernel-module.mk

obj-m := string_test.o

# need to test the kernel string funcs directly rather
# that what gcc decides to optimize with
EXTRA_CFLAGS += -fno-builtin
