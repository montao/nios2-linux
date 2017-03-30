VER = jpeg-7

ifeq ($(ARCH),blackfin)
CPPFLAGS += -DDCT_IFAST_SUPPORTED_ARCH
endif
CONF_OPTS = \
	--disable-ld-version-script

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libjpeg.so.7 /usr/lib/libjpeg.so.7
