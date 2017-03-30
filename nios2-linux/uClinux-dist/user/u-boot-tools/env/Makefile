CPPFLAGS += -DUSE_HOSTCC

all: fw_env

fw_env: crc32.o fw_env.o fw_env_main.o

clean:
	rm -f *.o fw_env *.gdb

romfs:
	$(ROMFSINST) -d /bin/fw_env
	test -e $(ROMFSDIR)/etc/fw_env.config || $(ROMFSINST) -d /etc/fw_env.config
	$(ROMFSINST) -s fw_env /bin/fw_printenv
	$(ROMFSINST) -s fw_env /bin/fw_setenv

.PHONY: all clean romfs
