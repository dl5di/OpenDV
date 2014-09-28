dv3000d is a small program which runs on a Raspberry Pi and provides access to
the NWDR DV3000 AMBE board. It makes the AMBE3000R chip on it available via the
network using UDP frames. Details of the chip are beyond the scope of this
document, but sufficient to say that it is capable of vocoding audio for D-Star,
DMR, and P.25 and maybe more.

In order to use it a few preliminaries need to be done.

1. Prepare the DV3000 board. You need to ensure that the board is running at
   230400 baud and to do this, ensure that only one jumper is fitted on the
   board, BR2, and it is the one between pins 1 and 2. This is the one at the
   end nearest to the connector to the Pi.

2. Prepare the Raspberry Pi. This consists of (a) increasing the clock to the
   RPi UART, (b) disabling the getty (to allow terminal login) running on
   the RPi serial port, and (c) disable the console on the serial port.

   a. Edit the file /boot/config.txt and add the line:
   
   init_uart_clock=3686400
   
   b. Edit the file /etc/inittab and replace the line:
   
   T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100
   
   with:
   
   #T0:23:respawn:/sbin/getty -L ttyAMA0 115200 vt100

   or delete the line entirely.
   
   c. Edit the file /boot/cmdline.txt and replace the line:
   
   dwc_otg.lpm_enable=0 console=ttyAMA0,115200 kgdboc=ttyAMA0,115200 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait
   
   with:
   
   dwc_otg.lpm_enable=0 console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait

3. Fit the DV3000 board and reboot your Raspberry Pi.

The dv3000d program then needs to be built and run on the Pi. Take the files
from this directory and copy them to somewhere suitable on the Pi, once there,
and assuming that you have a set of compilers on your Pi, type in the following
command:

make

This should build a new file named dv3000d. This can be copied somewhere more
convenient.

To run the program you need to use the command:

sudo dv3000d

which will run the program in the current window or terminal. However if you
want it to run in the background use the command:

sudo dv3000d -d

By default dv3000d uses UDP port 2460 for communicating with programs that need
its services, if this isn't convenient then it can be changed with the -p option.
This can be seen in copies of the above commands, using port 24600 now, below:

sudo dv3000d -p 24600
sudo dv3000d -p 24600 -d

That is all there is to running the dv3000d program. By itself it does very
little but allows other programs to access the DV3000 on your Pi from anywhere
on your network, or even the Internet.
