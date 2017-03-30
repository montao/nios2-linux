# version is based on current git snapshot from here (since releases are not
# really made anymore by the mtd guys):
GITWEB = http://git.infradead.org/?p=$(GITREPO).git
GITREPO = mtd-utils
VER = 36d8de81049c9c908740b690c664b5bd3703ccd6

# Host based builds needed for constructing host images
BUILDTARGETS-m :=
BUILDTARGETS-y :=
BUILDTARGETS-$(CONFIG_JFFS_FS)  += mkfs.jffs
BUILDTARGETS-$(CONFIG_JFFS2_FS) += mkfs.jffs2
BUILDTARGETS-$(CONFIG_JFFS2_SUMMARY) += sumtool
BUILDTARGETS = $(BUILDTARGETS-m) $(BUILDTARGETS-y)

UBI_UTILS_BUILDTARGETS-m :=
UBI_UTILS_BUILDTARGETS-y :=
UBI_UTILS_BUILDTARGETS-$(CONFIG_UBIFS_FS) += mkfs.ubifs
UBI_UTILS_BUILDTARGETS = $(UBI_UTILS_BUILDTARGETS-m) $(UBI_UTILS_BUILDTARGETS-y)

define MAKE_MTD
	$(MAKE) -C $(VER)/$(2) WITHOUT_XATTR=1 BUILDDIR=$$PWD/build-$(VER)$(4)/$(2) $(1) $(3)
endef
define _MAKE_HOST_MTD
	unset CC CFLAGS CPPFLAGS CROSS LDFLAGS; $(call MAKE_MTD,,$(2),$(3),-host)
endef
define MAKE_HOST_MTD
	$(call _MAKE_HOST_MTD,$(1),$(2),$(3))
endef
define MAKE_HOST_LINK
	for f in $(1); do ln -sf build-$(VER)-host/$(2)/$$f $$f ; done
endef
all:
	$(call MAKE_HOST_MTD)
	$(call MAKE_HOST_LINK, $(BUILDTARGETS))
	$(call MAKE_HOST_LINK, $(UBI_UTILS_BUILDTARGETS),mkfs.ubifs)
	$(call MAKE_MTD)

clean:
	rm -rf build-* mkfs.jffs mkfs.jffs2 mkfs.ubifs sumtool

romfs:
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_ERASE        build-$(VER)/flash_erase /bin/flash_erase
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_FLASHCP      build-$(VER)/flashcp /bin/flashcp
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_LOCK         build-$(VER)/flash_lock /bin/flash_lock
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UNLOCK       build-$(VER)/flash_unlock /bin/flash_unlock
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_FTL_CHECK    build-$(VER)/ftl_check /bin/ftl_check
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_FTL_FORMAT   build-$(VER)/ftl_format /bin/ftl_format
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_MKFSJFFS     build-$(VER)/mkfs.jffs /bin/mkfs.jffs
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_MKFSJFFS2    build-$(VER)/mkfs.jffs2 /bin/mkfs.jffs2
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_NFTLDUMP     build-$(VER)/nftldump /bin/nftldump
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_NFTL_FORMAT  build-$(VER)/nftl_format /bin/nftl_format
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_NANDDUMP     build-$(VER)/nanddump /bin/nanddump
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_NANDTEST     build-$(VER)/nandtest /bin/nandtest
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_NANDWRITE    build-$(VER)/nandwrite /bin/nandwrite
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_DOC_LOADBIOS build-$(VER)/doc_loadbios /bin/doc_loadbios
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_DOC_LOADIPL  build-$(VER)/doc_loadipl /bin/doc_loadipl
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubiformat /bin/ubiformat
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubiattach /bin/ubiattach
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubidetach /bin/ubidetach
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubinfo /bin/ubinfo
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubimkvol /bin/ubimkvol
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubirmvol /bin/ubirmvol
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubiupdatevol /bin/ubiupdatevol
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubinize /bin/ubinize
	$(ROMFSINST) -e CONFIG_USER_MTDUTILS_UBIUTILS     build-$(VER)/ubi-utils/ubirename /bin/ubirename

include $(ROOTDIR)/tools/gitweb.mk

.PHONY: all clean romfs
