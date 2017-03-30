ifeq (1,1)
VER = libnl-1.1

all: $(VER)/Makefile.opts
	$(MAKE) -C $(VER)
	$(MAKE) -C $(VER) install DESTDIR=$(STAGEDIR)

$(VER)/Makefile.opts:
	cd $(VER) && ./configure $(CONFIGURE_OPTS)

clean:
	-$(MAKE) -C $(VER) distclean

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libnl.so.1 /usr/lib/libnl.so.1

else

# This is for newer releases

GITWEB = http://git.kernel.org/?p=$(GITREPO).git
GITREPO = libs/netlink/libnl
VER = 8d2bb90726f75dfaf752de599886cb8bf15391bd

include $(ROOTDIR)/tools/autotools.mk
include $(ROOTDIR)/tools/gitweb.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libnl.so.2 /usr/lib/libnl.so.2

endif
