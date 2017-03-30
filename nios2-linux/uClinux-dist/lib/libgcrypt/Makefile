# http://www.gnupg.org/download/index.en.html#libgcrypt

VER = libgcrypt-1.4.5

CONF_OPTS = --disable-ld-version-script

export GPG_ERROR_CONFIG = $(STAGEDIR)/usr/bin/gpg-error-config

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libgcrypt.so.11.5.3 /usr/lib/
