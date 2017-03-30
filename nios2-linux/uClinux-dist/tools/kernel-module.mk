ifeq ($(origin CC),default)
CC := bfin-uclinux-gcc
endif
MAKEARCH_KERNEL ?= $(MAKE) ARCH=blackfin CROSS_COMPILE=bfin-uclinux-
LINUXDIR ?= linux-2.6.x

# avoid infinite recursion
ifneq ($(LINUXDIR),)
MAKE_KERNEL = CFLAGS="" CPPFLAGS="" LDFLAGS="" \
	$(MAKEARCH_KERNEL) -C $(ROOTDIR)/$(LINUXDIR) SUBDIRS=$$PWD
else
MAKE_KERNEL = echo
endif

EXTRA_CFLAGS += -Wall

all: module

module:
	$(MAKE_KERNEL) modules

clean::
	rm -f *.o *.gdb
	$(MAKE_KERNEL) clean

ROMFS_MODULES ?= $(obj-m:.o=.ko)
romfs::
	for m in $(ROMFS_MODULES) ; do $(ROMFSINST) -d -M $$m misc/$$m || exit $$? ; done

.PHONY: all clean module romfs
