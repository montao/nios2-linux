VER = openssl-0.9.8k

ifeq ($(CONFIG_FMT_USE_FDPIC_ELF),y)
SSL_SHARED := shared
SSL_SHARED_CMD :=
else
SSL_SHARED := no-dso
SSL_SHARED_CMD := sed -i -e 's: -ldl::' $(VER)/Makefile
endif

SSL_MAKE = \
	$(MAKE) -C $(VER) -j1 \
	DIRS="crypto ssl engines" \
	CC=$(CONFIGURE_HOST)-gcc

all: $(VER)/.configure
	$(SSL_MAKE)
	$(SSL_MAKE) install_sw INSTALL_PREFIX=$(STAGEDIR)

$(VER)/.configure:
	set -e ; \
	cd $(VER) ; \
	./Configure \
		linux-generic32 -DL_ENDIAN \
		--prefix=/usr \
		--openssldir=/etc/ssl \
		$(SSL_SHARED) threads
	$(SSL_SHARED_CMD)
	touch $(VER)/.configure

clean:
	rm -f $(VER)/.configure
	$(MAKE) -C $(VER) clean

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libssl.so.0.9.8 /usr/lib/libssl.so.0.9.8
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libcrypto.so.0.9.8 /usr/lib/libcrypto.so.0.9.8

.PHONY: all clean romfs
