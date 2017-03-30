VER = SDL_mixer-1.2.11

export ac_cv_path_SDL_CONFIG = $(CROSS_COMPILE)sdl-config

CONF_OPTS = \
	--disable-music-mod \
	--disable-music-timidity-midi \
	$(call USE_ENABLE,LIB_FLAC,music-flac) \
	$(call USE_ENABLE,LIB_LIBOGG,music-ogg) \
	--disable-music-mp3 \
	$(call USE_ENABLE,LIB_LIBMAD,music-mp3-mad-gpl)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libSDL_mixer-1.2.so.0 /usr/lib/libSDL_mixer-1.2.so.0
