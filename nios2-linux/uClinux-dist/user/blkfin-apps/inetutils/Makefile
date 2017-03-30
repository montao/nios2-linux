VER = inetutils-1.6

FLTFLAGS += -s 0x6000

CONF_OPTS = \
	--without-included-regex \
	--disable-libls \
	--disable-servers --disable-clients \
	--with-ncurses-include-dir=$(STAGEDIR) \
	$(call USE_ENABLE,USER_INETUTILS_RCP,rcp) \
	$(call USE_ENABLE,USER_INETUTILS_RSH,rsh) \
	$(call USE_ENABLE,USER_INETUTILS_RSHD,rshd) \
	$(call USE_ENABLE,USER_INETUTILS_TELNET,telnet) \
	$(call USE_ENABLE,USER_INETUTILS_FTPD,ftpd) \
	$(call USE_ENABLE,USER_INETUTILS_FTP,ftp)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_RCP        build-$(VER)/rcp/rcp /bin/rcp
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_RSH        build-$(VER)/rsh/rsh /bin/rsh
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_RSHD       build-$(VER)/rshd/rshd /bin/rshd
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_RSHD \
	    -a "shell   stream tcp nowait root /bin/rshd" /etc/inetd.conf
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_RSHD \
	    -a "+ +" /root/.rhosts
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_TELNET     build-$(VER)/telnet/telnet /bin/telnet
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_FTPD       build-$(VER)/ftpd/ftpd /bin/ftpd
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_FTPD \
	    -a "ftp     stream tcp nowait root /bin/ftpd -l" /etc/inetd.conf
	$(ROMFSINST) -e CONFIG_USER_INETUTILS_FTP        build-$(VER)/ftp/ftp /bin/ftp
