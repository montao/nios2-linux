GITWEB = http://git.ffmpeg.org/?p=$(GITREPO);a=summary
GITREPO = ffmpeg
VER = ffmpeg-svn-20344

all: build/Makefile
	$(MAKE) -C build
	$(MAKE) -C build DESTDIR=$(STAGEDIR) install

include $(ROOTDIR)/tools/gitweb.mk

build/Makefile:
	find $(VER) -type f -print0 | xargs -0 touch -r $(VER)/configure
	set -e ; \
	rm -rf build ; \
	mkdir build ; \
	cd build ; \
	SDL_CONFIG=$(STAGEDIR)/usr/bin/sdl-config \
	../$(VER)/configure \
		--prefix=/usr \
		--source-path=$$PWD/../$(VER) \
		--target-os=linux \
		--arch=$(MACHINE) \
		--sysinclude="$(STAGEDIR)/usr/include" \
		--extra-ldflags="$(LDFLAGS)" \
		--enable-static \
		$(CONFIGURE_SHARED_ENABLE) \
		--enable-gpl \
		--enable-postproc \
		--disable-ipv6 \
		--disable-stripping \
		--cross-prefix=$(CONFIGURE_HOST)-

clean:
	rm -rf build

romfs:
	set -e ; \
	cd $(STAGEDIR)/usr/lib ; \
	for l in libavcodec.so.?? libavdevice.so.?? libavformat.so.?? libavutil.so.?? libpostproc.so.?? libswscale.so.? ; do \
		$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $$l /usr/lib/$$l ; \
	done
	$(ROMFSINST) -d -e CONFIG_USER_FFMPEG_APPS $(STAGEDIR)/usr/bin/ffmpeg /usr/bin/ffmpeg
	$(ROMFSINST) -d -e CONFIG_USER_FFMPEG_APPS $(STAGEDIR)/usr/bin/ffserver /usr/bin/ffserver
	$(ROMFSINST) -d -e CONFIG_USER_FFMPEG_APPS -e CONFIG_LIB_LIBSDL $(STAGEDIR)/usr/bin/ffplay /usr/bin/ffplay

.PHONY: all clean romfs
