VER = madplay-0.15.2b

LDFLAGS += -L$(STAGEDIR)/usr/lib

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/madplay /usr/bin/madplay
