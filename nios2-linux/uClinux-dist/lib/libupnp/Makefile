VER = libupnp-1.6.6

CONF_OPTS = --enable-samples

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libixml.so.2 /usr/lib/libixml.so.2
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libthreadutil.so.2 /usr/lib/libthreadutil.so.2
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libupnp.so.3 /usr/lib/libupnp.so.3
	$(ROMFSINST) -e CONFIG_USER_UPNP_SAMPLES build-$(VER)/upnp/sample/.libs/upnp_tv_ctrlpt /bin/
	$(ROMFSINST) -e CONFIG_USER_UPNP_SAMPLES build-$(VER)/upnp/sample/.libs/upnp_tv_device /bin/
	$(ROMFSINST) -e CONFIG_USER_UPNP_SAMPLES $(VER)/upnp/sample/web /web/
