VER = lynx2-8-6.5

CONF_OPTS = \
	--with-screen=ncurses \
	--with-curses-dir=$(STAGEDIR)/usr

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/lynx /usr/bin/lynx
	$(ROMFSINST) -d $(STAGEDIR)/etc/lynx.cfg /etc/lynx.cfg
	$(ROMFSINST) -d $(STAGEDIR)/etc/lynx.lss /etc/lynx.lss
