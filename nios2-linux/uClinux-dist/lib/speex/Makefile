GITWEB = http://git.xiph.org/?p=$(GITREPO).git;a=summary
GITREPO = speex
VER = a6d05eb5ff9d5062852cdf7df574bec728921ef9

CONF_OPTS = \
	--without-ogg \
	--enable-blackfin-asm \
	--enable-fixed-point \
	--disable-vbr

include $(ROOTDIR)/tools/autotools.mk
include $(ROOTDIR)/tools/gitweb.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libspeex.so.1 /usr/lib/libspeex.so.1
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libspeexdsp.so.1 /usr/lib/libspeexdsp.so.1
