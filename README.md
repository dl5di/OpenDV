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

./configure can be run with optional flags, see the OpenDV wiki for further details

    --with-gpio          to enable wiringPi on the RPi.
    --without-gui        no GUI build eliminating WX-GUI

Specific to DStarRepeater and DummyRepeater

    --with-stardv        flag to compile the Star*DV adapter in the STARDV directory
    --with-ambeserver    flag that compiles AMBEserver in the DV3000 directory.


##Support

Pre-compiled binary files are located in the Yahoo! groups including Windows binary files.

Please see the Yahoo! users group email reflectors for peer-support.

* [ircDDB Yahoo! group](https://groups.yahoo.com/neo/groups/ircDDBGateway/info)
* [PC Repeater Controller Yahoo! group](https://groups.yahoo.com/neo/groups/pcrepeatercontroller/info)
* [MMDVM Yahoo! group](https://groups.yahoo.com/neo/groups/mmdvm/info)

