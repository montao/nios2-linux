VER = mediastreamer-2.2.4

CONF_OPTS = \
	--disable-strict \
	--enable-external-ortp \
	$(call USE_ENABLE,LIB_ALSA_LIB,alsa) \
	$(call USE_ENABLE,LIB_SPEEX,speex) \
	$(call USE_ENABLE,LIB_FFMPEG,video)

# disable generation of documentation
export ac_cv_path_DOXYGEN = false

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/libexec/mediastream /usr/libexec/mediastream
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libmediastreamer.so.0 /usr/lib/libmediastreamer.so.0
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libquickstream.so.0 /usr/lib/libquickstream.so.0
