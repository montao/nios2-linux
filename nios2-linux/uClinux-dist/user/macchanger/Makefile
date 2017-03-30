VER = macchanger-1.5.0

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/macchanger /usr/bin/macchanger
	$(ROMFSINST) -d $(STAGEDIR)/usr/share/macchanger/OUI.list /usr/share/macchanger/OUI.list
	$(ROMFSINST) -d $(STAGEDIR)/usr/share/macchanger/wireless.list /usr/share/macchanger/wireless.list
