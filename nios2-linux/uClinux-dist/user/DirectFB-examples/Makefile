VER = DirectFB-examples-1.2.0

include $(ROOTDIR)/tools/autotools.mk

romfs:
	for i in df_andi df_bltload df_drivertest df_cpuload df_databuffer df_dok df_fire \
		df_flip df_fonts df_input df_joystick df_layer df_matrix df_netload \
		df_dioload df_palette df_particle df_porter df_video df_video_particle \
		df_stress df_texture df_window pss; \
	do \
		$(ROMFSINST) -d build-$(VER)/src/$$i /usr/bin/$$i; \
	done
	$(ROMFSINST) -d build-$(VER)/src/df_knuckles/df_knuckles /usr/bin/df_knuckles
	$(ROMFSINST) -d build-$(VER)/src/spacedream/spacedream /usr/bin/spacedream
	$(ROMFSINST) -d build-$(VER)/src/df_neo/df_neo /usr/bin/df_neo
	$(ROMFSINST) -d $(VER)/data /usr/share/directfb-examples
	mkdir -p ${ROMFSDIR}/usr/share/directfb-examples/fonts
	mv ${ROMFSDIR}/usr/share/directfb-examples/decker.ttf ${ROMFSDIR}/usr/share/directfb-examples/fonts/decker.ttf
	mv ${ROMFSDIR}/usr/share/directfb-examples/df_neo/* ${ROMFSDIR}/usr/share/directfb-examples/
	rm -rf ${ROMFSDIR}/usr/share/directfb-examples/df_neo
