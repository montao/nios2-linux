VER = libmcapi-2.0

CFLAGS += -I$(ROOTDIR)/linux-2.6.x/drivers/staging/icc/include -I$(ROOTDIR)/linux-2.6.x/arch/blackfin/mach-bf561/include/mach

.NOTPARALLEL:
include $(ROOTDIR)/tools/autotools.mk

$(BUILDDIR)/Makefile: $(VER)/configure

$(VER)/configure: $(VER)/aclocal.m4 $(VER)/configure.ac
	cd $(VER) && ./autogen.sh

INSTALL_TEST_SRC := $(notdir $(wildcard $(VER)/tests/*.c))
INSTALL_TEST_PROGRAMS := $(patsubst %.c,%,$(INSTALL_TEST_SRC))

romfs:
	@echo installing $(INSTALL_TEST_PROGRAMS)
	@for x in $(INSTALL_TEST_PROGRAMS); do \
		$(ROMFSINST) -d $(STAGEDIR)/usr/bin/$$x /usr/bin/$$x; \
	done

