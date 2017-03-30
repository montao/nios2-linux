VER = lowpan-tools-0.2-rc2

CONF_OPTS = --disable-manpages

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/izchat /usr/bin/izchat
	$(ROMFSINST) -d $(STAGEDIR)/usr/sbin/iz /usr/sbin/iz
	$(ROMFSINST) -d $(STAGEDIR)/usr/sbin/izattach /usr/sbin/izattach
	$(ROMFSINST) -d $(STAGEDIR)/usr/sbin/izcoordinator /usr/sbin/izcoordinator
