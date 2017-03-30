VER = dosfstools-3.0.5

CPPFLAGS += -D_FILE_OFFSET_BITS=64

TARGETS-y :=
TARGETS-$(CONFIG_USER_DOSFSTOOLS_DOSFSCK) += dosfsck
TARGETS-$(CONFIG_USER_DOSFSTOOLS_MKDOSFS) += mkdosfs

all: build-$(VER)/Makefile
	$(MAKE) -C build-$(VER) OPTFLAGS= $(TARGETS-y)

build-$(VER)/Makefile:
	mkdir -p build-$(VER)
	sed s:@VER@:$(VER):g Makefile.out-of-tree > $@

clean:
	rm -rf build-$(VER)

romfs:
	$(ROMFSINST) -e CONFIG_USER_DOSFSTOOLS_MKDOSFS build-$(VER)/mkdosfs /sbin/mkdosfs
	$(ROMFSINST) -e CONFIG_USER_DOSFSTOOLS_MKDOSFS -s mkdosfs /sbin/mkfs.msdos
	$(ROMFSINST) -e CONFIG_USER_DOSFSTOOLS_MKDOSFS -s mkdosfs /sbin/mkfs.vfat

	$(ROMFSINST) -e CONFIG_USER_DOSFSTOOLS_DOSFSCK build-$(VER)/dosfsck /sbin/dosfsck
	$(ROMFSINST) -e CONFIG_USER_DOSFSTOOLS_DOSFSCK -s dosfsck /sbin/fsck.msdos
	$(ROMFSINST) -e CONFIG_USER_DOSFSTOOLS_DOSFSCK -s dosfsck /sbin/fsck.vfat

.PHONY: all clean romfs
