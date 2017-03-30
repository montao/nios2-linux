VERS = htpdate-1.0.4

all:
	$(MAKE) -e -C $(VERS)

romfs:
	$(ROMFSINST) -e CONFIG_USER_HTPDATE $(VERS)/htpdate /bin/htpdate

clean:
	$(MAKE) -C $(VERS) clean
	rm -f $(VERS)/*.gdb

