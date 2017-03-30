VER = libid3tag-0.15.1b

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libid3tag.so.0 /usr/lib/libid3tag.so.0
