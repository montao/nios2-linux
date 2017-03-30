VER = expat-2.0.1

include $(ROOTDIR)/tools/autotools.mk

ifeq ($(CONFIG_USER_EXPAT_EXAMPLES),y)
post-build::
	$(MAKE) -C build-$(VER) DESTDIR=$(STAGEDIR) examples/elements examples/outline
endif

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libexpat.so.1 /usr/lib/libexpat.so.1
	$(ROMFSINST) -d -e CONFIG_USER_EXPAT_EXAMPLES build-$(VER)/examples/elements /usr/bin/elements
	$(ROMFSINST) -d -e CONFIG_USER_EXPAT_EXAMPLES build-$(VER)/examples/outline /usr/bin/outline
