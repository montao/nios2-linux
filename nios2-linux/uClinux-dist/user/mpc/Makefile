VER = mpc-0.12.1
URL = http://downloads.sourceforge.net/musicpd/$(VER).tar.bz2

CONF_OPTS = \
	--disable-iconv

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/mpc /usr/bin/mpc
