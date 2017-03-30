CONFOPTS=--disable-shared --prefix=

all: build/build
	$(MAKE) -C build

build/build: makefile
	rm -rf build
	mkdir build
	(cd build; sh ../unix/configure $(CONFIGURE_OPTS) $(CONFOPTS))
	sed -i.tmp 's:-L/lib :-L$(ROOTDIR)/user/tcl/build :' build/tclConfig.sh
	touch build/build

clean:
	rm -rf build

romfs:
	$(ROMFSINST) -d library/init.tcl /lib/tcl8.3/init.tcl
