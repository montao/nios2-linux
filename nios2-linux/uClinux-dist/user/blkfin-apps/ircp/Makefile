VER = ircp-0.3

FLTFLAGS += -s 8192

ifeq ($(CONFIG_FMT_USE_FDPIC_ELF),y)
LDFLAGS += --shared
endif

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/ircp /usr/bin/ircp
