VERS = iftop-0.17

# Can't build out of tree...

all: $(VERS)/Makefile
	$(MAKE) -C $(VERS) 
	
$(VERS)/Makefile:
	set -e ; \
	cd $(VERS); \
	./configure $(CONFIGURE_OPTS)

romfs:
	$(ROMFSINST) -d $(VERS)/iftop /sbin/iftop
	cp $(VERS)/iftop.8 $(STAGEDIR)/usr/share/man/man8/iftop.8

clean:
	$(MAKE) -C $(VERS) distclean
	rm -f $(VERS)/*.gdb

