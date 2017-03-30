VER = libuio-0.2.1

include $(ROOTDIR)/tools/autotools.mk

$(BUILDDIR)/Makefile: $(VER)/configure

$(VER)/configure:
	cd $(VER) && ./autogen.sh
romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libuio.so.1 /usr/lib/libuio.so.1
	$(ROMFSINST) -d -e CONFIG_USER_LSUIO $(STAGEDIR)/usr/bin/lsuio /usr/bin/lsuio
