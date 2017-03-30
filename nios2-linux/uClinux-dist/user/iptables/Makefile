VER = iptables-1.4.5

CONF_OPTS = \
	--sbindir=/sbin \
	--libexecdir=/lib \
	--libdir=/lib \
	$(CONFIGURE_SHARED_ENABLE) \
	$(call USE_ENABLE,USER_IPTABLES_IP6TABLES,ipv6)

include $(ROOTDIR)/tools/autotools.mk

# the install target lacks proper dependencies
pre-build::
	$(MAKE) -C build-$(VER)

romfs:
	$(ROMFSINST) $(STAGEDIR)/sbin/iptables-multi /bin/iptables
	$(ROMFSINST) -e CONFIG_USER_IPTABLES_IP6TABLES $(STAGEDIR)/sbin/ip6tables /bin/ip6tables
	$(ROMFSINST) -l /bin/iptables /bin/iptables-save
	$(ROMFSINST) -l /bin/iptables /bin/iptables-restore
	$(ROMFSINST) -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/lib/libip4tc.so.0 /lib/libip4tc.so.0
	$(ROMFSINST) -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/lib/libxtables.so.2 /lib/libxtables.so.2
	$(ROMFSINST) -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/lib/xtables /lib/xtables
