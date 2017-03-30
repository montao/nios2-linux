VER = strace-4.6

FLTFLAGS += -s 0x5000

AUTOTOOLS_ALL_DEPS = .check

include $(ROOTDIR)/tools/autotools.mk

# don't fail in a release, just in a scm
.check: $(ROOTDIR)/$(LINUXDIR)/Makefile
	if ! CPP="$(CC) -E" BUILD_CC=gcc ./check-entities.sh $(ROOTDIR)/$(LINUXDIR) $(ARCH) $(VER) $(MACHINE) ; then [ -d .svn ] && exit 1 ; fi
	touch .check

romfs:
	$(ROMFSINST) build-$(VER)/strace /bin/strace
	$(ROMFSINST) -e CONFIG_USER_PERL_PERL build-$(VER)/strace-graph /bin/strace-graph
