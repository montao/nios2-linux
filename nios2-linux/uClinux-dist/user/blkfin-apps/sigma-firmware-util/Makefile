EXEC = adau1761.bin

HELPERS = read_firmware gen_firmware

all: $(EXEC) $(HELPERS)

$(EXEC): gen_firmware
	./gen_firmware

romfs:
	$(ROMFSINST) -d /lib/firmware/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o $(HELPERS)

BUILD_CC ?= gcc
BUILD_CFLAGS ?= -O2 -g
BUILD_CFLAGS += -Wall
BUILD_COMPILE = $(BUILD_CC) $(BUILD_CFLAGS) $(BUILD_LDFLAGS) -o $@ $^

gen_firmware: gen_firmware.c crc32.c ; $(BUILD_COMPILE)
read_firmware: read_firmware.c ; $(BUILD_COMPILE)
