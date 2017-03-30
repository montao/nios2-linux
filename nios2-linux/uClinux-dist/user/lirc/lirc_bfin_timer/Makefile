ROOTDIR  ?= $(PWD)/../../../
include $(ROOTDIR)/tools/kernel-module.mk

obj-m := lirc_bfin_timer.o

EXTRA_CFLAGS += -I$(PWD)/../lirc-0.8.6 -g -O0

IP = bfin
RSH = rsh -l root $(IP)
rcp: module
	rcp lirc_bfin_timer.ko root@$(IP):/
	-$(RSH) rmmod lirc_bfin_timer
	$(RSH) modprobe lirc_dev
	$(RSH) dmesg -c -n 8 > /dev/null
	$(RSH) insmod /lirc_bfin_timer.ko
	$(RSH) dmesg -c -n 4 > /dev/null
