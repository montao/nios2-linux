VER = iproute2-2.6.35

ARGS =
ifeq ($(CONFIGURE_SHARED_WITH),--without-shared)
ARGS += SHARED_LIBS=n
endif

all: $(VER)/Config
	$(MAKE) -C $(VER) CC="$(CC)" LIBUTIL= $(ARGS)

$(VER)/Config: Config
	cp $< $@

clean:
	$(MAKE) -C $(VER) distclean

romfs:
	$(ROMFSINST) $(VER)/etc /etc
	$(ROMFSINST) $(VER)/ip/ip /bin/ip
	$(ROMFSINST) $(VER)/misc/ss /bin/ss
	$(ROMFSINST) $(VER)/tc/tc /bin/tc

.PHONY: all clean romfs
