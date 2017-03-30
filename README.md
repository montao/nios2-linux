# nios2-linux
uClinux adapted for Altera DE2-115

I did the following changes:

To build this thing on 64-bit ubuntu I had to do the following:

$ LIBRARY_PATH=/usr/lib/x86_64-linux-gnu

append the toolchain-mmu to your path

$ PATH=$PATH:/home/developer/altera/nios2-linux/toolchain-mmu/x86-linux2/bin

edit line 373 in /home/developer/altera/nios2-linux/linux-2.6/kernel/timeconst.pl

and remove the define so that the conditions looks like:

if (!@val)

comment out line 2

//#define _POSIX_C_SOURCE 199309

in the file /home/developer/altera/nios2-linux/uClinux-dist/user/mtd-utils/36d8de81049c9c908740b690c664b5bd3703ccd6/serve_image.c

edit the file /home/developer/altera/nios2-linux/uClinux-dist/vendors/Altera/nios2/Makefile
and remove the references to syslog.conf and sc.cfg on line 12

Now the thing builds. 
