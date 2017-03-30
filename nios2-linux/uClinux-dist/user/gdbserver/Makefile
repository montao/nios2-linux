VER = gdb-6.5

FLTFLAGS += -s 0x10000

include $(ROOTDIR)/tools/autotools.mk
CONFIGURE = gdbserver/configure

# install target has broken deps
pre-build::
	$(MAKE) -C $(BUILDDIR)

romfs:
	$(ROMFSINST) -e CONFIG_USER_GDBSERVER_GDBSERVER	$(STAGEDIR)/usr/bin/gdbserver /bin/gdbserver
	$(ROMFSINST) -e CONFIG_USER_GDBSERVER_GDBREPLAY	$(BUILDDIR)/gdbreplay /bin/gdbreplay
