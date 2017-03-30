VER = dnsmasq-2.50

all: build/Makefile
	$(MAKE) -C build CFLAGS="$(CFLAGS) $(CPPFLAGS) -U__uClinux__ -I."

build/Makefile:
	mkdir -p build
	sed "s:@VER@:$(VER):g" Makefile.subdir > $@

clean:
	rm -rf build

romfs:
	$(ROMFSINST) build/dnsmasq /bin/dnsmasq

.PHONY: all clean romfs
