VER = nmap-5.00
URL = http://nmap.org/dist/$(VER).tar.bz2

export ac_cv_path_STRIP = true

CONF_OPTS =	\
	--without-zenmap \
	--without-ndiff \
	--disable-ipv6 \
	--datadir=/etc \
	--with-libpcap \
	--with-libpcre \
	$(call USE_WITH,LIB_LIBSSL,openssl) \
	$(call USE_WITH,USER_LUA_LUA,liblua)

include $(ROOTDIR)/tools/autotools.mk

# install doesn't depend on built stuff
pre-build::
	$(MAKE) -C build-$(VER)

romfs:
	$(ROMFSINST) $(STAGEDIR)/etc/nmap /etc/nmap
	$(ROMFSINST) -d $(STAGEDIR)/usr/bin/nmap /usr/bin/nmap
