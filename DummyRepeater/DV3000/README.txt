
To build the daemon programs

On Raspberry Pi machines
make clean
make
sudo make install
sudo make init-install

This will build new versions of 

AMBEserver      for DV3000U
AMBEserverGPIO  for DV3000
dv3000d         deprecated alternative to AMBEserverGPIO

It will also install to /usr/bin

AMBEserver
AMBEserverGPIO

As well as start scripts in /etc/init.d

AMBEserver
AMBEserverGPIO

For other Linux / Mac OS X machines

make clean
make

Install for other Linux

sudo install AMBEserver /usr/bin
sudo install init.d/AMBEserver /etc/init.d
 
For Windows, this daemon can also be built under Cygwin and run by installing the cygwin1.dll and the generated .exe file after

make AMBEserver

The python scripts

AMBEtest2.py  (deprecated, strictly for DV3000)
AMBEtest3.py -i /dev/ttyXXX (new, works for DV3000U and DV3000 [if reset])

