VER = SDL_net-1.2.7

export ac_cv_path_SDL_CONFIG = $(CROSS_COMPILE)sdl-config

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libSDL_net-1.2.so.0 /usr/lib/libSDL_net-1.2.so.0
