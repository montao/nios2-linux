VER = SDL_ttf-2.0.9

export ac_cv_path_SDL_CONFIG = $(CROSS_COMPILE)sdl-config
export ac_cv_path_FREETYPE_CONFIG = $(CROSS_COMPILE)freetype-config

CONF_OPTS = --without-x

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libSDL_ttf-2.0.so.0 /usr/lib/libSDL_ttf-2.0.so.0
