VER = faad2-2.0
URL = http://downloads.sourceforge.net/faac/$(VER).tar.bz2

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libfaad.so.0 /usr/lib/libfaad.so.0
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libmp4ff.so.0 /usr/lib/libmp4ff.so.0
