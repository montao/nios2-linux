CC := bfin-elf-gcc -g
LD := bfin-elf-ld

# we have to clobber a bunch of flags as they are normally geared towards
# running under linux, not a bare metal environment like core b
PWD:= $(shell pwd)
KERNEL_DIR ?= ${PWD}/../../../../../linux-2.6.x
ICC_INCLUDE ?= ${PWD}/../../include
MCAPI_INCLUDE ?= ${PWD}/../../../../../staging/usr/include/mcapi-2.0_coreb/
MCAPI_TEST_INCLUDE ?= ${PWD}/../../../../../staging/usr/include/mcapi-2.0/
ICC_CORE := ../../icc_core/icc
LIBMCAPI_COREB ?= ${PWD}/../../../../../staging/usr/lib/libmcapi_coreb.a

ifneq ($(ICC_MACHINE),bf609)
TASK_LOAD_BASE := 0x3c00000
CFLAGS  := -mcpu=bf561 -mmulticore -mcoreb -mlong-calls -I${ICC_INCLUDE} -I${KERNEL_DIR}/include -I${KERNEL_DIR}/arch/blackfin/include -I${KERNEL_DIR}/arch/blackfin/include/generated  -I${KERNEL_DIR}/arch/blackfin/mach-bf561/include -I${KERNEL_DIR}/drivers/staging/icc/include -I${MCAPI_INCLUDE} -I${MCAPI_TEST_INCLUDE} -DCONFIG_BF561 -D__KERNEL__
endif

ifeq ($(ICC_MACHINE),bf609)
TASK_LOAD_BASE := 0x7c00000
CFLAGS = -I$(ICC_INCLUDE) -I$(MCAPI_INCLUDE) -I$(MCAPI_TEST_INCLUDE) -I$(KERNEL_DIR)/include \
		-I$(KERNEL_DIR)/arch/blackfin/include \
		-I$(KERNEL_DIR)/arch/blackfin/include/generated \
		-I$(KERNEL_DIR)/arch/blackfin/mach-$(ICC_MACHINE)/include \
		-I$(KERNEL_DIR)/drivers/staging/icc/include \
		-DCONFIG_BF609 -D__KERNEL__ -mlong-calls

endif

LDFLAGS  =
ASFLAGS  =

INSTALL_TASK_SRC := $(notdir $(wildcard mcapi_*.c))
INSTALL_TASK := $(patsubst %.c,%,$(INSTALL_TASK_SRC))

all: task1 task2 scalar icc_latency $(INSTALL_TASK)

coreb_task.lds: coreb_task.lds.S
	$(CC) -E -ansi $(CFLAGS) -D__ASSEMBLY__ -P $^ -o $@

$(INSTALL_TASK): %:%.o $(ICC_CORE) $(LIBMCAPI_COREB) coreb_task.lds
	$(LD) -static $(LDFLAGS) -o $@ -T coreb_task.lds --just-symbol $(ICC_CORE) $< $(LIBMCAPI_COREB)  -Ttext $(TASK_LOAD_BASE)

task1: task1.o $(ICC_CORE) coreb_task.lds
	$(LD) $(LDFLAGS) -o task1 -T coreb_task.lds --just-symbol $(ICC_CORE) task1.o -Ttext $(TASK_LOAD_BASE)

task2: task2.o $(ICC_CORE) coreb_task.lds
	$(LD) $(LDFLAGS) -o task2 -T coreb_task.lds --just-symbol $(ICC_CORE) task2.o -Ttext $(TASK_LOAD_BASE)

scalar: scalar.o $(ICC_CORE) coreb_task.lds
	$(LD) $(LDFLAGS) -o scalar -T coreb_task.lds --just-symbol $(ICC_CORE) scalar.o -Ttext $(TASK_LOAD_BASE)

icc_latency: icc_latency.o $(ICC_CORE) coreb_task.lds
	$(LD) $(LDFLAGS) -o icc_latency -T coreb_task.lds --just-symbol $(ICC_CORE) icc_latency.o -Ttext $(TASK_LOAD_BASE)

clean:
	rm -rf *.o task1 task2 scalar $(INSTALL_TASK) coreb_task.lds

.PHONY: all clean
