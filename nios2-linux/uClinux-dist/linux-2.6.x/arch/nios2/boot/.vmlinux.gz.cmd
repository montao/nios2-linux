cmd_arch/nios2/boot/vmlinux.gz := (cat arch/nios2/boot/vmlinux.bin | gzip -n -f -9 > arch/nios2/boot/vmlinux.gz) || (rm -f arch/nios2/boot/vmlinux.gz ; false)
