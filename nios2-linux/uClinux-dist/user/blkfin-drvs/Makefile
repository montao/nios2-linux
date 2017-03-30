dir_y =
dir_n =
dir_  =
dir_all = $(dir_y) $(dir_n) $(dir_)

dir_$(CONFIG_USER_BLACKFIN_DRV_TEA5764)          += tea5764
dir_$(CONFIG_USER_BLACKFIN_DRV_TIMER_LATENCY)    += timer_latency
dir_$(CONFIG_USER_BLACKFIN_DRV_PREEMPT_LATENCY)  += preempt_latency
dir_$(CONFIG_USER_BLACKFIN_DRV_SPI_ADC)          += bfin_spi_adc

all romfs:
	for i in $(dir_y)   ; do $(MAKE) -C $$i $@ || exit $$? ; done

clean:
	for i in $(dir_all) ; do $(MAKE) -C $$i $@ || exit $$? ; done

.PHONY: all clean romfs
