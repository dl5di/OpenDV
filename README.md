#Open Digital Voice software for Amateur Radio 
based on Jonathan Naylor's (G4KLX) "ircDDBGateway" and "PCRepeaterController" for D-Star

##How-To and Documenation
See the orginal documentation https://github.com/dl5di/OpenDV/tree/master/Documentation

Updated documentation is additionally here https://github.com/dl5di/OpenDV/wiki

##How-to Build Code
OpenDV project is build with WX3 Widgets see more details at
https://github.com/dl5di/OpenDV/wiki/ircDDB-Gateway-Installation

    ./configure
    make
    sudo make install

./configure can be ran with optional flags, see OpenDV wiki for further details

    --with-gpio to enable wiringPi on the RPi.
    --without-gui no GUI build eliminating WX-GUI

Spicific to DStarRepeater and DummyRepeater

    --with-stardv        flag to compile the Star*DV adapter in the STARDV directory
    --with-ambeserver        flag that compiles AMBEserver in the DV3000 directory.


##Support
```
pre-compiled binary files are located in the Yahoo! groups including windows binary files.

please see the Yahoo! users group email reflectors. for peer-support.
ircDDB Yahoo! group, pcrepeatercontroller Yahoo! group, and the mmdvm Yahoo! group.

https://groups.yahoo.com/neo/groups/ircDDBGateway/info
https://groups.yahoo.com/neo/groups/pcrepeatercontroller/info
https://groups.yahoo.com/neo/groups/mmdvm/info
```
