VER = STLport-5.2.1

all: $(VER)/build/Makefiles/gmake/config.mak
	$(MAKE) -C $(VER) install DESTDIR=$(STAGEDIR)

CPPFLAGS += -D_STLP_NO_CWCHAR -D_STLP_NO_NATIVE_WIDE_FUNCTIONS -D_STLP_NO_NATIVE_WIDE_STREAMS
CONF_OPTS = \
	--prefix=/usr \
	--libdir=/usr/lib \
	--target=$(CONFIGURE_HOST) \
	--with-extra-cxxflags="${CXXFLAGS} ${CPPFLAGS}" \
	--with-extra-cflags="${CFLAGS} ${CPPFLAGS}" \
	--with-extra-ldflags="${LDFLAGS}"
# build system sucks and screws up CXX/target set at same time
ENV_OPTS = CXX="" CC=""
$(VER)/build/Makefiles/gmake/config.mak:
	cd $(VER) && env $(ENV_OPTS) ./configure $(CONF_OPTS)

clean:
	cd $(VER)/build && \
	rm -rf \
		Makefiles/gmake/config.mak \
		lib/Makefile lib/obj \
		test/*/Makefile

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(STAGEDIR)/usr/lib/libstlport.so.5.2 /usr/lib/libstlport.so.5.2

.PHONY: all clean romfs
