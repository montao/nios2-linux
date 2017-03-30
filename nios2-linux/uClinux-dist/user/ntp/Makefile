VER = ntp-4.2.6

# the ipv6 test passes compile but fails on link
export ac_cv_have_in6addr_any = no
export isc_cv_have_in6addr_any = no

export ac_cv_path_PATH_NET_SNMP_CONFIG = $(CONFIGURE_HOST)-net-snmp-config

CONF_OPTS = \
	--disable-debugging	\
	--disable-HOPFSERIAL --disable-HOPFPCI --disable-BANCOMM           \
	--disable-GPSVME --disable-SHM --disable-all-clocks                \
	--disable-ACTS --disable-ARBITER --disable-ARCRON-MSF              \
	--disable-ATOM --disable-AS2201 --disable-CHU --disable-AUDIO-CHU  \
	--disable-DATUM --disable-FG --disable-HEATH --disable-HPGPS       \
	--disable-IRIG --disable-JJY --disable-LEITCH                      \
	--disable-LOCAL-CLOCK --disable-MSFEES --disable-MX4200            \
	--disable-NMEA --disable-ONCORE --disable-PALISADE --disable-PST   \
	--disable-JUPITER --disable-PTBACTS --disable-TPRO --disable-TRAK  \
	--disable-CHRONOLOG --disable-DUMBCLOCK --disable-PCF              \
	--disable-SPECTRACOM --disable-TRUETIME --disable-ULINK            \
	--disable-WWV --disable-USNO --disable-parse-clocks                \
	--disable-COMPUTIME --disable-DCF7000 --disable-HOPF6021           \
	--disable-MEINBERG --disable-RAWDCF --disable-RCC8000              \
	--disable-SCHMID --disable-TRIMTAIP --disable-TRIMTSIP             \
	--disable-WHARTON --disable-VARITEXT --disable-kmem                \
	--without-openssl-libdir --without-openssl-incdir --without-crypto \
	--without-electricfence --without-sntp --without-ntpsnmpd          \
	$(call USE_WITH,USER_NTPD_NTPDATE,ntpdate) \
	$(call USE_WITH,USER_NTPD_NTPDC,ntpdc) \
	$(call USER_NTPD_NTPQ,ntpq)

include $(ROOTDIR)/tools/autotools.mk

romfs:
	$(ROMFSINST) -e CONFIG_USER_NTPD_NTPD build-$(VER)/ntpd/ntpd /bin/ntpd
	$(ROMFSINST) -e CONFIG_USER_NTPD_NTPDATE build-$(VER)/ntpdate/ntpdate /bin/ntpdate
	$(ROMFSINST) -e CONFIG_USER_NTPD_NTPDC build-$(VER)/ntpdc/ntpdc /bin/ntpdc
	$(ROMFSINST) -e CONFIG_USER_NTPD_NTPQ build-$(VER)/ntpq/ntpq /bin/ntpq
	$(ROMFSINST) -a "ntp	123/tcp" /etc/services
	$(ROMFSINST) -a "ntp	123/udp" /etc/services
