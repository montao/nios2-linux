ICC_TOP_DIR := $(shell pwd)
all:    icc icc_loader libmcapi icc_example
icc_example: icc

libmcapi:
	$(MAKE) -C libmcapi_coreb

icc: icc_core
	$(MAKE) -C  icc_core

icc_loader: 
	$(MAKE) -C  $@

icc_example: libmcapi
	$(MAKE) -C example/task
	$(MAKE) -C example/test_app

INSTALL_TASK_SRC := $(notdir $(wildcard example/task/*.c))
INSTALL_TASK := $(patsubst %.c,%,$(INSTALL_TASK_SRC))

romfs: icc_core/icc icc_loader/icc_loader icc_example
	$(ROMFSINST) -d icc_core/icc /bin/icc
	$(ROMFSINST) -d icc_loader/icc_loader /bin/icc_loader
	@echo installing $(INSTALL_TASK)
	@for x in $(INSTALL_TASK); do \
		test -f example/task/$$x && $(ROMFSINST) -S -d example/task/$$x /bin/$$x; \
	done
	$(ROMFSINST) -d example/test_app/packet_test /bin/packet_test

clean:
	$(MAKE) -C icc_core clean
	$(MAKE) -C icc_loader clean
	$(MAKE) -C example/task clean
	$(MAKE) -C example/test_app clean
	$(MAKE) -C libmcapi_coreb clean

.PHONY: all clean romfs icc_loader
