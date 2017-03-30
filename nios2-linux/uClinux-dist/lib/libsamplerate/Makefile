VER = libsamplerate-0.1.7

CONF_OPTS = \
	$(CONFIGURE_SHARED_ENABLE)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libsamplerate.so.0 /usr/lib/libsamplerate.so.0
