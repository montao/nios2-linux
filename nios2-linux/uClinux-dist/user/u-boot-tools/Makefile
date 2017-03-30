all clean romfs:
	$(MAKE) -C env $@

UBOOT_SRC = $(lastword $(wildcard /usr/local/src/blackfin/svn/u-boot/trunk/u-boot-* /usr/local/src/blackfin/git/u-boot/u-boot-*))
update:
	cp $(UBOOT_SRC)/tools/env/* env/
diff:
	diff -urp -x .svn -x Makefile -x '*.o' $(UBOOT_SRC)/tools/env env

.PHONY: all clean diff romfs update
