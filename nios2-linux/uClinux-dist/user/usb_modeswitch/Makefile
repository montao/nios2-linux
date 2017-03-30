VER = usb-modeswitch-1.1.3
URL = http://www.draisberghof.de/usb_modeswitch/$(VER).tar.bz2

MAKE += CC=$(CC)

all:
	$(MAKE) -C $(VER)

clean:
	rm -f $(VER)/*.gdb $(VER)/usb_modeswitch

romfs:
	$(ROMFSINST) -d $(STAGEDIR)/usr/sbin/lsusb /usr/bin/lsusb
	$(ROMFSINST) -d $(STAGEDIR)/usr/share/usb.ids /usr/share/usb.ids
