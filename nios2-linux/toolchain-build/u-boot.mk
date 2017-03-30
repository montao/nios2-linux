#
# U-boot Makefile
#

# Board config
UBOOT_BOARD	?=

# Misc.
UBOOT_ARCH	= nios2
ifeq ($(UBOOT_BOARD),)
$(warning Board not defined! Using EP1S10 as default.)
UBOOT_BOARD	= EP1S10
endif
UBOOT_CONFIG	= $(UBOOT_BOARD)_config
# Make-rules called from top-level
UBOOT_TOOLS	= u-boot-tools
UBOOT_TOOLS_CLEAN = u-boot-clean

# U-boot
u-boot_config: $(UBOOT_BUILD)/.configured
$(UBOOT_BUILD)/.configured:
	PATH=$(TARGET_PATH):$(PATH) \
	$(MAKE) O=$(UBOOT_BUILD) -C $(UBOOT_SRC) CROSS_COMPILE=$(TARGET_NAME)- ARCH=$(UBOOT_ARCH) $(UBOOT_CONFIG)
	touch $@

u-boot: $(UBOOT_BUILD)/u-boot.bin
$(UBOOT_BUILD)/u-boot.bin: $(UBOOT_BUILD)/.configured
	PATH=$(TARGET_PATH):$(PATH) \
	$(MAKE) O=$(UBOOT_BUILD) -C $(UBOOT_SRC) CROSS_COMPILE=$(TARGET_NAME)- ARCH=$(UBOOT_ARCH)
	touch $@

# Tools
$(UBOOT_BUILD)/.tools_configured:
	PATH=$(TARGET_PATH):$(PATH) \
	$(MAKE) O=$(UBOOT_BUILD) -C $(UBOOT_SRC) CROSS_COMPILE= ARCH=$(UBOOT_ARCH) $(UBOOT_CONFIG)
	touch $@

$(UBOOT_TOOLS): $(UBOOT_BUILD)/tools/mkimage
	cp -f  $(UBOOT_BUILD)/tools/mkimage $(INSTALLDIR)/bin/mkimage

$(UBOOT_BUILD)/tools/mkimage: $(UBOOT_BUILD)/.tools_configured
	PATH=$(TARGET_PATH):$(PATH) \
	$(MAKE) O=$(UBOOT_BUILD) -C $(UBOOT_SRC) tools
	touch $@

# Clean
$(UBOOT_TOOLS_CLEAN):
	PATH=$(TARGET_PATH):$(PATH) \
	$(MAKE) O=$(UBOOT_BUILD) -C $(UBOOT_SRC) ARCH=$(UBOOT_ARCH) clean
	rm -f $(UBOOT_BUILD)/.tools_configured $(UBOOT_BUILD)/.configured

u-boot-distclean:
	rm -rf $(UBOOT_BUILD)

.PHONY: u-boot_config u-boot $(UBOOT_TOOLS) $(UBOOT_TOOLS_CLEAN) u-boot-distclean
