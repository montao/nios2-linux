all: src/Makefile
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean

romfs:
	$(ROMFSINST) src/ptpd /bin/ptpd

.PHONY: all clean romfs
