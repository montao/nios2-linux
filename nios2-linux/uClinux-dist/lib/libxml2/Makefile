VER = libxml2-2.7.4

CONF_OPTS = \
	--without-python \
	$(call USE_WITH,LIB_LIBXML2_MINIMUM,minimum) \
	$(call USE_WITH,LIB_ZLIB,zlib) \
	$(call USE_WITH,LIB_READLINE,readline) \
	$(call USE_WITH,LIB_READLINE,history)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libxml2.so.2 /usr/lib/libxml2.so.2
