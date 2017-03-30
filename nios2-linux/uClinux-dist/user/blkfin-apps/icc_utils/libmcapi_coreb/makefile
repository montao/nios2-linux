VER = libmcapi-2.0
CONFIGURE_OPTS := \
	--host=bfin-elf \
	--prefix=/usr \
	--sysconfdir=/etc \
	--datadir=/usr/share \
	--mandir=/usr/share/man \
	--infodir=/usr/share/info \
	--localstatedir=/var/lib \
	--disable-dependency-tracking \
	--enable-fast-install
ifeq ($(ICC_MACHINE),bf609)
CC := "bfin-elf-gcc -mcpu=bf609-0.0"
CFLAGS := "-DCONFIG_BF609 -Wall -mlong-calls -I$(KERNEL_DIR)/drivers/staging/icc/include -I${KERNEL_DIR}/arch/blackfin/include -I$(ICC_INCLUDE)"
else
CC := bfin-elf-gcc -mcpu=bf561
CFLAGS := -Wall -mcpu=bf561 -mmulticore -mcoreb -mlong-calls -DCONFIG_BF561 -I$(ROOTDIR)/linux-2.6.x/drivers/staging/icc/include -I$(ROOTDIR)/linux-2.6.x/arch/blackfin/mach-bf561/include/mach -I$(ROOTDIR)/user/blkfin-apps/icc_utils/include
endif

LDFLAGS :=
ifeq ($(ICC_BUILDROOT),y)
all: build install

build:
	cd $(VER) && ./configure $(CONFIGURE_OPTS) CC=$(CC) CFLAGS=$(CFLAGS)
	$(MAKE) -C $(VER)

install:
	$(MAKE) -C $(VER) DESTDIR=$(DESTDIR) install

else
include $(ROOTDIR)/tools/autotools.mk
endif
