cmd_net/unix/unix.o := nios2-linux-gnu-ld  -mnios2elf   -r -o net/unix/unix.o net/unix/af_unix.o net/unix/garbage.o net/unix/sysctl_net_unix.o 
