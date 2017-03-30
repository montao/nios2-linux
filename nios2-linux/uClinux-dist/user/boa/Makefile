VER = boa-0.94.14rc21

FLTFLAGS += -s 65536

include $(ROOTDIR)/tools/autotools.mk

# boa doesnt like to be "installed"
pre-build::
	$(MAKE) -C $(BUILDDIR)

romfs:
	$(ROMFSINST) $(BUILDDIR)/src/boa /bin/boa
