VER = iperf-2.0.4

AUTOTOOLS_BUILD_HOST = true

include $(ROOTDIR)/tools/autotools.mk

host-build:
	$(MAKE) -C $(BUILDDIR_HOST)
romfs:
	$(ROMFSINST) -e CONFIG_USER_IPERF_IPERF build-$(VER)/src/iperf /bin/iperf
