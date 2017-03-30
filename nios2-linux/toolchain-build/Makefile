#
# Nios2 Toolchain Makefile
#

TOPDIR			?= $(PWD)
# Sources
KERNEL_SRC		?= $(TOPDIR)/../linux-2.6
UCLIBC_SRC		?= $(TOPDIR)/../uClibc
BINUTILS_SRC		?= $(TOPDIR)/../binutils
GCC_SRC			?= $(TOPDIR)/../gcc3
ELF2FLT_SRC		?= $(TOPDIR)/../elf2flt
GDB_SRC			?= $(TOPDIR)/../insight
UBOOT_SRC		?= $(TOPDIR)/../u-boot
# Build directories
BUILDDIR		?= $(TOPDIR)/build
SYSROOT			= $(BUILDDIR)/sysroot/
KERNEL_DIR		= $(BUILDDIR)/linux
BINUTILS_BUILD		= $(BUILDDIR)/binutils-build
GCC_BOOTSTRAP_BUILD	= $(BUILDDIR)/gcc-bootstrap-build
GCC_BUILD		= $(BUILDDIR)/gcc-build
GDB_BUILD_HOST		= $(BUILDDIR)/gdb-host
UBOOT_BUILD		= $(BUILDDIR)/u-boot
# Configs
UCLIBC_CONF		= $(TOPDIR)/config/uClibc.config
# Target defines
TARGET_NAME		= nios2-linux-uclibc
TARGET_SHORT_NAME	= nios2-linux
KERNEL_ARCH		= nios2
# Path
TARGET_PATH		= $(INSTALLDIR)/bin:$(PATH)

# Default toolchain installation directory
# Override with 'make INSTALLDIR=<toolchain full installation path>'
INSTALLDIR		?= $(BUILDDIR)/nios2

# Build everything by default
all: toolchain+extras

# U-boot
UBOOT_y=$(shell test -d $(UBOOT_SRC) && echo yes)
ifeq ($(UBOOT_y),yes)
include $(TOPDIR)/u-boot.mk
endif
# GDB
GDB_y=$(shell test -d $(GDB_SRC) && echo yes)
ifeq ($(GDB_y),yes)
include $(TOPDIR)/gdb.mk
endif

toolchain: gcc elf2flt
toolchain+extras: toolchain $(GDB_HOST) $(UBOOT_TOOLS)

# Kernel headers
kernel-headers: $(KERNEL_DIR)/.configured
kernel-headers-clean:
	rm -rf  $(KERNEL_DIR)

$(KERNEL_DIR)/.configured:
	mkdir -p $(BUILDDIR)/linux
	$(MAKE) -C $(KERNEL_SRC) ARCH=$(KERNEL_ARCH) CC=gcc INSTALL_HDR_PATH=$(KERNEL_DIR) headers_install
	touch $(KERNEL_DIR)/.configured

# uClibc
uClibc-headers: kernel-headers $(SYSROOT)/.configured
uClibc-headers-clean:
	rm -rf $(SYSROOT)
uClibc: gcc-bootstrap $(INSTALLDIR)/lib/libc.a
uClibc-clean:
	-$(MAKE) -C $(UCLIBC_SRC) clean
uClibc-distclean:
	-$(MAKE) -C $(UCLIBC_SRC) distclean
uClibc-menuconfig: $(UCLIBC_SRC)/.config
	$(MAKE) -C $(UCLIBC_SRC) \
		PREFIX=$(SYSROOT) \
		DEVEL_PREFIX=/usr/ \
		RUNTIME_PREFIX=$(SYSROOT) \
		KERNEL_HEADERS=$(KERNEL_DIR)/include \
		menuconfig
	touch $(UCLIBC_SRC)/.config

$(UCLIBC_SRC)/.config:
	cp -f $(UCLIBC_CONF) $(UCLIBC_SRC)/.config
	$(MAKE) -C $(UCLIBC_SRC) \
		PREFIX=$(SYSROOT) \
		DEVEL_PREFIX=/usr/ \
		RUNTIME_PREFIX=$(SYSROOT) \
		KERNEL_HEADERS=$(KERNEL_DIR)/include \
		oldconfig
	touch $(UCLIBC_SRC)/.config

$(SYSROOT)/.configured: $(UCLIBC_SRC)/.config
	mkdir -p $(SYSROOT)usr/include
	mkdir -p $(SYSROOT)usr/lib
	mkdir -p $(SYSROOT)lib
	$(MAKE) -C $(UCLIBC_SRC) \
		PREFIX=$(SYSROOT) \
		DEVEL_PREFIX=/usr/ \
		RUNTIME_PREFIX=$(SYSROOT) \
		KERNEL_HEADERS=$(KERNEL_DIR)/include \
		pregen install_headers
	cp -fpLR $(KERNEL_DIR)/include/asm $(SYSROOT)usr/include/
	cp -fpLR $(KERNEL_DIR)/include/asm-generic $(SYSROOT)usr/include/
	cp -fpLR $(KERNEL_DIR)/include/linux $(SYSROOT)usr/include/
	touch $(SYSROOT)/.configured

$(UCLIBC_SRC)/lib/libc.a: $(SYSROOT)/.configured
	PATH=$(TARGET_PATH) \
	$(MAKE) -C $(UCLIBC_SRC) CROSS=$(TARGET_NAME)- \
		PREFIX= \
		DEVEL_PREFIX=/ \
		RUNTIME_PREFIX=/ \
		KERNEL_HEADERS=$(KERNEL_DIR)/include \
		all
	touch -c $(UCLIBC_SRC)/lib/libc.a

$(INSTALLDIR)/lib/libc.a: $(UCLIBC_SRC)/lib/libc.a
	PATH=$(TARGET_PATH) \
	$(MAKE) -C $(UCLIBC_SRC) CROSS=$(TARGET_NAME)- \
		PREFIX= \
		DEVEL_PREFIX=$(INSTALLDIR)/ \
		RUNTIME_PREFIX=$(INSTALLDIR)/ \
		KERNEL_HEADERS=$(KERNEL_DIR)/include \
		install_runtime install_dev
	# Install kernel headers
	cp -fpLR $(KERNEL_DIR)/include/asm $(INSTALLDIR)/include/
	cp -fpLR $(KERNEL_DIR)/include/asm-generic $(INSTALLDIR)/include/
	cp -fpLR $(KERNEL_DIR)/include/linux $(INSTALLDIR)/include/
	# Include-fix
	ln -snf ../include $(INSTALLDIR)/$(TARGET_NAME)/sys-include
	# Lib-fix
	rm -rf  $(INSTALLDIR)/$(TARGET_NAME)/lib
	ln -snf ../lib $(INSTALLDIR)/$(TARGET_NAME)/lib
	touch -c $(INSTALLDIR)/lib/libc.a

# Binutils
binutils: uClibc-headers $(INSTALLDIR)/$(TARGET_NAME)/bin/ld
binutils-clean:
	rm -f $(INSTALLDIR)/bin/$(TARGET_NAME)*
	-$(MAKE) -C $(BINUTILS_BUILD) clean
binutils-distclean:
	rm -rf $(BINUTILS_BUILD)

$(BINUTILS_BUILD)/.configured:
	mkdir -p $(BINUTILS_BUILD)
	(cd  $(BINUTILS_BUILD); \
	$(BINUTILS_SRC)/configure \
		--prefix=$(INSTALLDIR) \
		--target=$(TARGET_NAME) \
		--with-build-sysroot=$(SYSROOT) \
		--with-sysroot=$(SYSROOT) \
		--disable-nls \
		--enable-multilib \
		--disable-werror );
	touch $(BINUTILS_BUILD)/.configured

$(BINUTILS_BUILD)/binutils/objdump: $(BINUTILS_BUILD)/.configured
	$(MAKE) -C $(BINUTILS_BUILD) all

$(INSTALLDIR)/$(TARGET_NAME)/bin/ld: $(BINUTILS_BUILD)/binutils/objdump
	mkdir -p $(INSTALLDIR)/lib
	mkdir -p $(INSTALLDIR)/$(TARGET_NAME)
	ln -snf ../lib $(INSTALLDIR)/$(TARGET_NAME)/lib
	$(MAKE) -C $(BINUTILS_BUILD) install

# GCC bootstrap
gcc-bootstrap: uClibc-headers binutils $(INSTALLDIR)/bin/$(TARGET_NAME)-gcc
gcc-bootstrap-clean:
	rm -rf $(GCC_BOOTSTRAP_BUILD)

$(GCC_BOOTSTRAP_BUILD)/.configured:
	mkdir -p $(GCC_BOOTSTRAP_BUILD)
	(cd $(GCC_BOOTSTRAP_BUILD); PATH=$(TARGET_PATH) \
	$(GCC_SRC)/configure \
		--prefix=$(INSTALLDIR) \
		--target=$(TARGET_NAME) \
		--enable-languages=c \
		--with-sysroot=$(SYSROOT) \
		--disable-__cxa_atexit \
		--enable-target-optspace \
		--with-gnu-ld \
		--disable-shared \
		--disable-nls \
		--enable-threads \
		--enable-multilib \
		--enable-cxx-flags=-static );
	touch  $(GCC_BOOTSTRAP_BUILD)/.configured

$(GCC_BOOTSTRAP_BUILD)/.compiled: $(GCC_BOOTSTRAP_BUILD)/.configured
	PATH=$(TARGET_PATH) $(MAKE) -C $(GCC_BOOTSTRAP_BUILD) all-gcc
	touch $(GCC_BOOTSTRAP_BUILD)/.compiled

$(INSTALLDIR)/bin/$(TARGET_NAME)-gcc: $(GCC_BOOTSTRAP_BUILD)/.compiled
	PATH=$(TARGET_PATH) $(MAKE) -C $(GCC_BOOTSTRAP_BUILD) install-gcc

# GCC final
gcc: uClibc-headers binutils gcc-bootstrap uClibc $(GCC_BUILD)/.installed
gcc-clean:
	rm -rf  $(GCC_BUILD)
	for prog in cpp gcc gcc-[0-9]* g++ c++ gcov gccbug cc; do \
		rm -f $(INSTALLDIR)/bin/$(TARGET_NAME)-$$prog \
		rm -f $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-$$prog; \
	done
gcc-configured: $(GCC_BUILD)/.configured

$(GCC_BUILD)/.configured:
	mkdir -p $(GCC_BUILD)
	(cd $(GCC_BUILD); PATH=$(TARGET_PATH) \
	$(GCC_SRC)/configure \
		--prefix=$(INSTALLDIR) \
		--target=$(TARGET_NAME) \
		--enable-languages=c,c++ \
		--disable-__cxa_atexit \
		--enable-target-optspace \
		--with-gnu-ld \
		--disable-shared \
		--disable-nls \
		--enable-threads \
		--enable-multilib \
		--enable-cxx-flags=-static );
	touch  $(GCC_BUILD)/.configured

$(GCC_BUILD)/.compiled: $(GCC_BUILD)/.configured
	PATH=$(TARGET_PATH) $(MAKE) -C $(GCC_BUILD) all
	touch $(GCC_BUILD)/.compiled

$(GCC_BUILD)/.installed: $(GCC_BUILD)/.compiled
	PATH=$(TARGET_PATH) $(MAKE) -C $(GCC_BUILD) install
	# Make sure we have 'cc'
	ln -snf gcc $(INSTALLDIR)/$(TARGET_NAME)/bin/cc
	ln -snf $(TARGET_NAME)-gcc $(INSTALLDIR)/bin/$(TARGET_NAME)-cc
	# Create nios2-linux-* symlinks
	(cd $(INSTALLDIR); \
		ln -snf $(TARGET_NAME) $(TARGET_SHORT_NAME); \
		cd bin; \
		for app in $(TARGET_NAME)-* ; do \
			ln -snf $${app} \
		   	$(TARGET_SHORT_NAME)$${app##$(TARGET_NAME)}; \
		done; \
	);
	touch  $(GCC_BUILD)/.installed

# Elf2flt
elf2flt: uClibc-headers binutils gcc $(ELF2FLT_SRC)/elf2flt
elf2flt-clean:
	-$(MAKE) -C $(ELF2FLT_SRC) clean
elf2flt-distclean:
	-$(MAKE) -C $(ELF2FLT_SRC) distclean
	rm -rf $(ELF2FLT_SRC)/.configured

$(ELF2FLT_SRC)/.configured:
	(cd $(ELF2FLT_SRC); rm -rf config.cache; \
	$(ELF2FLT_SRC)/configure \
		--with-bfd-include-dir=$(BINUTILS_BUILD)/bfd \
		--with-binutils-include-dir=$(BINUTILS_SRC)/include \
		--target=$(TARGET_NAME) \
		--with-libbfd=$(BINUTILS_BUILD)/bfd/libbfd.a \
		--with-libiberty=$(BINUTILS_BUILD)/libiberty/libiberty.a \
		--prefix=$(INSTALLDIR) \
		--disable-emit-relocs)
	touch $(ELF2FLT_SRC)/.configured

$(ELF2FLT_SRC)/elf2flt: $(ELF2FLT_SRC)/.configured $(INSTALLDIR)/$(TARGET_NAME)/bin/ld
	$(MAKE) -C $(ELF2FLT_SRC) all
	$(MAKE) -C $(ELF2FLT_SRC) install
	# Create nios2-linux-* symlinks
	ln -snf $(TARGET_NAME)-elf2flt $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-elf2flt
	ln -snf $(TARGET_NAME)-flthdr  $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-flthdr
	ln -snf $(TARGET_NAME)-ld.real $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-ld.real
	touch -c $(ELF2FLT_SRC)/elf2flt

# Clean
clean: elf2flt-clean gcc-clean gcc-bootstrap-clean uClibc-clean uClibc-headers-clean \
	kernel-headers-clean $(GDB_HOST_CLEAN) $(UBOOT_TOOLS_CLEAN)

distclean: elf2flt-distclean uClibc-distclean
	rm -rf $(BUILDDIR)

.PHONY: toolchain toolchain+extras kernel-headers uClibc-headers uClibc uClibc-menuconfig \
	binutils gcc-bootstrap gcc gcc-configured elf2flt clean distclean
