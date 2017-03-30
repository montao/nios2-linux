VER = alsa-lib-1.0.24

CONF_OPTS = \
	--with-softfloat \
	--with-versioned=no \
	--enable-static \
	--disable-alisp \
	--disable-python \
	$(CONFIGURE_SHARED_ENABLE) CFLAGS="$(CFLAGS) -lm"

# install target is not parallel safe
.NOTPARALLEL:
include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/share/alsa /usr/share/alsa
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libasound.so.2 /usr/lib/libasound.so.2
# ALSA installs every card by default, so rm them, and only install the ones we have drivers for
# if you feel this list is not complete - please send patches
	rm -rf $(ROMFSDIR)/usr/share/alsa/cards/*.conf
	$(ROMFSINST) $(VER)/src/conf/cards/aliases.conf /usr/share/alsa/cards/aliases.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_AD1836 $(VER)/src/conf/cards/bfin-ad1836.conf /usr/share/alsa/cards/bfin-ad1836.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_AD193X $(VER)/src/conf/cards/bfin-ad193x.conf /usr/share/alsa/cards/bfin-ad193x.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_AD73311 $(VER)/src/conf/cards/bfin-ad73311.conf /usr/share/alsa/cards/bfin-ad73311.conf
	$(ROMFSINST) -e CONFIG_SND_BFIN_AD73322 $(VER)/src/conf/cards/AD73322.conf /usr/share/alsa/cards/AD73322.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_AC97 -e CONFIG_SND_BF5XX_MMAP_SUPPORT $(VER)/src/conf/cards/AD1980.conf /usr/share/alsa/cards/AD1980.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_AC97 -E CONFIG_SND_BF5XX_MMAP_SUPPORT $(VER)/src/conf/cards/AD1980-no-mmap.conf /usr/share/alsa/cards/AD1980.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_SSM2602 $(VER)/src/conf/cards/bfin-ssm2602.conf /usr/share/alsa/cards/bfin-ssm2602.conf
	$(ROMFSINST) -e CONFIG_SND_USB_AUDIO $(VER)/src/conf/cards/USB-Audio.conf /usr/share/alsa/cards/USB-Audio.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_ADAU1371 $(VER)/src/conf/cards/ADAU1371.conf /usr/share/alsa/cards/ADAU1371.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_ADAU1361 $(VER)/src/conf/cards/ADAU1361.conf /usr/share/alsa/cards/ADAU1361.conf
	$(ROMFSINST) -e CONFIG_SND_BF5XX_SOC_ADAU1373 $(VER)/src/conf/cards/ADAU1373.conf /usr/share/alsa/cards/ADAU1373.conf
