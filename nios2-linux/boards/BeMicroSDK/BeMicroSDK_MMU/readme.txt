This is embedded Linux example design for BeMicro-SDK.

How to use
1.Configure the BeMicro-SDK with BeMicroSDK_Linux.sof
	Open NIOS2 command shell and navigate to your working folder which contains this text file. and type
	nios2-configure-sof '--cable=USB-Blaster on localhost [USB-0]'

2.Download zImage.initramfs.gz into the BeMicro-SDK
	From the NIOS2 command shell, type
	nios2-downlaod -g zImage.initramfs.gz; nios2-terminal

You'll see download progress and will see the Linux boots up.
Welcome to
          ____ _  _
         /  __| ||_|
    _   _| |  | | _ ____  _   _  _  _
   | | | | |  | || |  _ \| | | |\ \/ /
   | |_| | |__| || | | | | |_| |/    \
   |  ___\____|_||_|_| |_|\____|\_/\_/
   | |
   |_|

For further information check:
http://www.uclinux.org/



3.How to use...

How to know the IP address that is given to the BeMicro??
	By default, this Linux example is using DHCP to get IP address
	after Linux loaded, connect to board by using nios2-terminal
	Type ifconfig
	Then, you'll see something like this
------------------------------------------------------------------------------
root:/etc> ifconfig
eth0      Link encap:Ethernet  HWaddr 00:70:ED:11:12:12
          inet addr:137.57.185.88  Bcast:137.57.185.255  Mask:255.255.255.0
          UP BROADCAST NOTRAILERS RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:4351 errors:0 dropped:2622 overruns:0 frame:0
          TX packets:675 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000
          RX bytes:343254 (335.2 KiB)  TX bytes:79554 (77.6 KiB)
          Base address:0x2000

lo        Link encap:Local Loopback
          inet addr:127.0.0.1  Mask:255.0.0.0
          UP LOOPBACK RUNNING  MTU:16436  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)

root:/etc>
------------------------------------------------------------------------------
In this example, the IP address of 137.57.185.88 is the given IP address for BeMicro-SDK



How to know the hostname???
	use nios2-terminal and connect to the board
	type "hostname"
	by default, the host name is "bem"
-------------------------
root:/etc> hostname
bem
root:/etc>
-------------------------

How to change the IP??
	For example, if you want to change the IP to be 192.168.1.10 then
	"ifconfig eth0 192.168.1.10"


Telnet : open windows command shell(DOS window) and type "telnet bem"
or type "telnet ***.***.***.***" the *** is the IP address that given to the BeMicro-SDK.
No ID/password should be required

FTP : open windows commans shell and type "ftp bem" or IP address.
the ID is root and password is root as well


SSH : Please use SSH client software to connect. The famous one is putty.
the ID is root and password is root as well


Web server : you should be able to any kind of web browser to connect.
type the IP address or the host name "bem" in the address.
The ID is admin    password is password

You should be able to see
	IP address
	Hostname
	System Uptime
	FPGA Build version






****************************************************************
For more info, please visit 
Altera wiki : http://www.alterawiki.com/wiki/Main_Page
or
Altera forum : http://www.alteraforum.com/

