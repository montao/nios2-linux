VER = mpeg2dec-0.4.1

CONF_OPTS = \
	$(CONFIGURE_SHARED_ENABLE) \
	--enable-static \
	--disable-sdl

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libmpeg2.so.0 /usr/lib/libmpeg2.so.0
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libmpeg2convert.so.0 /usr/lib/libmpeg2convert.so.0
