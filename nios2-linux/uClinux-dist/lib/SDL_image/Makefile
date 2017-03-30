VER = SDL_image-1.2.10

CONF_OPTS = \
	$(call USE_ENABLE,LIB_LIBJPEG,jpg) \
	$(call USE_ENABLE,LIB_LIBPNG,png) \
	$(call USE_ENABLE,LIB_TIFF,tif)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libSDL_image-1.2.so.0 /usr/lib/libSDL_image-1.2.so.0
