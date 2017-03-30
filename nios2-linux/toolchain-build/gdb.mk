#
# GDB (host) Makefile
#

# Make-rules called from top-level
GDB_HOST	= gdb-host
GDB_HOST_CLEAN	= gdb-host-clean

# GDB on host
$(GDB_HOST): gcc $(INSTALLDIR)/bin/$(TARGET_NAME)-gdb
$(GDB_HOST_CLEAN):
	-$(MAKE) -C $(GDB_BUILD_HOST) clean
gdb-host-distclean:
	rm -rf $(GDB_BUILD_HOST)

$(GDB_BUILD_HOST)/.configured:
	mkdir -p $(GDB_BUILD_HOST)
	(cd $(GDB_BUILD_HOST); \
		gdb_cv_func_sigsetjmp=yes \
		$(GDB_SRC)/configure \
		--prefix=$(INSTALLDIR) \
		--target=$(TARGET_NAME) \
		--disable-nls \
		--without-included-gettext \
		--enable-threads \
	);
	touch $@

$(GDB_BUILD_HOST)/gdb/gdb: $(GDB_BUILD_HOST)/.configured
	$(MAKE) -C $(GDB_BUILD_HOST)
	strip $(GDB_BUILD_HOST)/gdb/gdb
	strip $(GDB_BUILD_HOST)/gdb/insight
	strip $(GDB_BUILD_HOST)/gdb/gdbtui

$(INSTALLDIR)/bin/$(TARGET_NAME)-gdb: $(GDB_BUILD_HOST)/gdb/gdb
	$(MAKE) -C $(GDB_BUILD_HOST) install
	mkdir -p $(INSTALLDIR)/libgui
	ln -sf ../share/redhat/gui $(INSTALLDIR)/libgui/library
	ln -snf ../../bin/$(TARGET_NAME)-gdb $(INSTALLDIR)/$(TARGET_NAME)/bin/gdb
	ln -snf $(TARGET_NAME)-gdb $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-gdb
	ln -snf ../../bin/$(TARGET_NAME)-insight $(INSTALLDIR)/$(TARGET_NAME)/bin/insight
	ln -snf $(TARGET_NAME)-insight $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-insight
	ln -snf ../../bin/$(TARGET_NAME)-gdbtui $(INSTALLDIR)/$(TARGET_NAME)/bin/gdbtui
	ln -snf $(TARGET_NAME)-gdbtui $(INSTALLDIR)/bin/$(TARGET_SHORT_NAME)-gdbtui

.PHONY: $(GDB_HOST) $(GDB_HOST_CLEAN) gdb-host-distclean
