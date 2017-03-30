VERS = matrixssl-1-8-6-open

CFLAGS  += -I./ -DLINUX 
LDFLAGS += -lpthread

all:
ifeq ($(CONFIG_FMT_USE_FDPIC_ELF),y)
	$(MAKE) -C $(VERS)/src -e 
	cp $(VERS)/src/libmatrixssl.so $(STAGEDIR)/usr/lib/libmatrixssl.so
	$(MAKE) -C $(VERS)/examples -e
else
	$(MAKE) -C $(VERS)/src -e libmatrixsslstatic.a
	cp $(VERS)/src/libmatrixsslstatic.a $(STAGEDIR)/usr/lib/libmatrixssl.a
endif

romfs:
	$(ROMFSINST) -d -e CONFIG_FMT_USE_FDPIC_ELF $(VERS)/libmatrixssl.so /usr/lib/libmatrixssl.so

clean:
	$(MAKE) -C $(VERS)/src  clean
	$(MAKE) -C $(VERS)/examples clean
	rm -f $(VERS)/*.gdb

