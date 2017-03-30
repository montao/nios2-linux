VER = libsndfile-1.0.21

CONF_OPTS = \
	$(CONFIGURE_SHARED_ENABLE)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libsndfile.so.1 /usr/lib/libsndfile.so.1
