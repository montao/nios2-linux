VER = thttpd-2.25b

ifneq (,$(CONFIG_READ_TIMEOUT)$(READ_TIMEOUT))
CFLAGS += -DCONFIG_READ_TIMEOUT
endif

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) $(STAGEDIR)/usr/sbin/thttpd /bin/thttpd
