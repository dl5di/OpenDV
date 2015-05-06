This is command line program for Linux that lets the Star*DV dongle
emulate a NW Digital DV3000 for use with DummyRepeater.

It also can be used with a customized version DummyRepeater to make
use of the speaker/mic jack and ptt instead of PC audio.

There are two command line options: -d followed by the Star*DV 
device number.  Default is 0.  Use -p to change the network
UDP port used by DummyRepeater. Default is 2460.

You will need to install the FTD2XX libraries before compiling.
http://www.ftdichip.com/Drivers/D2XX.htm

Once the libraries are installed simply type:
make
sudo make install

The program name is dvemu.  Start the program then start
DummyRepeater and select either DV3000 Network or either
of the Star*DV selections if using the customized version
depending on whether you wishto use the PC audio or the
speaker/mic.

73,
Rick Schnicker KD0OSS

