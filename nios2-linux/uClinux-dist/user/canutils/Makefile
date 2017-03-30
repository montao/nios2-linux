VER = can-utils-rel-2.0.0-pre7

all: $(VER)/Makefile
	$(MAKE) -C $(VER)

clean:
	$(MAKE) -C $(VER) clean

romfs:
	$(ROMFSINST) -d $(VER)/cansend      /usr/bin/cansend
	$(ROMFSINST) -d $(VER)/canplayer    /usr/bin/canplayer
	$(ROMFSINST) -d $(VER)/cangen       /usr/bin/cangen
	$(ROMFSINST) -d $(VER)/cansniffer   /usr/bin/cansniffer
	$(ROMFSINST) -d $(VER)/candump      /usr/bin/candump
	$(ROMFSINST) -d $(VER)/slcan_attach /usr/bin/slcan_attach
	$(ROMFSINST) -d $(VER)/log2long     /usr/bin/log2long
	$(ROMFSINST) -d $(VER)/log2asc      /usr/bin/log2asc
	$(ROMFSINST) -d $(VER)/asc2log      /usr/bin/asc2log

.PHONY: all clean romfs
