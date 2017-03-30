VERS = httping-1.3.0

all:
	$(MAKE) -C $(VERS) -f Makefile.nossl

romfs:
	$(ROMFSINST) -d $(VERS)/httping /usr/bin/httping
	cp $(VERS)/httping.1 $(STAGEDIR)/usr/share/man/man1/httping.1

clean:
	$(MAKE) -C $(VERS) -f Makefile.nossl clean
	rm -f $(VERS)/*.gdb

