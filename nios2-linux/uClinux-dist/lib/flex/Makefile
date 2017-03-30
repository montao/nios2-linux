VER = flex-2.5.35

include $(ROOTDIR)/tools/autotools.mk

# disable the `flex` binary as we only want the library.  the flex binary
# is used to generate code which needs to be compiled, and it needs fork,
# so it doesn't make much sense to build for the target.
MAKE += bin_PROGRAMS=

romfs:
