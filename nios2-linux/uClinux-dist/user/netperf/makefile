VER = netperf-2.4.1

FLTFLAGS += -s 0x10000

export ac_cv_func_setpgrp_void = yes

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -e CONFIG_USER_NETPERF_NETPERF build-$(VER)/src/netperf /bin/netperf
	$(ROMFSINST) -e CONFIG_USER_NETPERF_NETSERVER build-$(VER)/src/netserver /bin/netserver
