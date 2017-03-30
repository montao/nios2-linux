#ignore CONFIG_USER_SQUASHFS_MKSQUASHFS7Z for squashfs 4.0
VER = squashfs4.0

all:

build-y :=
build-$(CONFIG_USER_SQUASHFS_MKSQUASHFS) += mksquashfs
#build-$(CONFIG_USER_SQUASHFS_MKSQUASHFS7Z) += mksquashfs7z
ifneq ($(build-y),)
all: build
endif

build-host-y :=
build-host-$(CONFIG_SQUASHFS) += mksquashfs
ifneq ($(build-host-y),)
all: build build-host
endif

env_build =
env_build-host = CFLAGS= CPPFLAGS= LDFLAGS= CC=cc
build build-host:
	mkdir -p $@
	cp -p Makefile.subdir $@/Makefile
	$(env_$@) $(MAKE) -C $@ VER=$(VER) $($@-y)

clean:
	rm -rf build*

romfs:
	$(ROMFSINST) -e CONFIG_USER_SQUASHFS_MKSQUASHFS build/mksquashfs /bin/mksquashfs
#	$(ROMFSINST) -e CONFIG_USER_SQUASHFS_MKSQUASHFS7Z build-host/mksquashfs7z /bin/mksquashfs7z

.PHONY: all clean romfs build build-host
