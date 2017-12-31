# Open Digital Voice software for Amateur Radio 
based on Jonathan Naylor's (G4KLX) "ircDDBGateway" and "PCRepeaterController" for D-Star

## How-To and Documenation
See the orginal documentation https://github.com/dl5di/OpenDV/tree/master/Documentation

Updated documentation is additionally here https://github.com/dl5di/OpenDV/wiki

## How-to Build Code
If you prefer to build debian packages see below.
OpenDV project is build with WX3 Widgets.
Install prerequisites :
    sudo apt install git build-essential automake debhelper libwxgtk3.0-dev libasound2-dev libusb-1.0-0-dev wiringpi fakeroot

    ./configure
    make
    sudo make install

./configure can be run with optional flags, see the OpenDV wiki for further details

    --with-gpio          to enable wiringPi on the RPi.
    --without-gui        no GUI build eliminating WX-GUI

Specific to DStarRepeater and DummyRepeater

    --with-stardv        flag to compile the Star*DV adapter in the STARDV directory
    --with-ambeserver    flag that compiles AMBEserver in the DV3000 directory.

## Build installable debian packages
    cd ~
    git clone https://github.com/dl5di/OpenDV.git
### ircDDBGateway
    cd ~/OpenDV/ircDDBGateway 
    dpkg-buildpackage -b -uc -j2

### dstarrepeater
    cd ~/OpenDV/DStarRepeater 
    dpkg-buildpackage -b -uc -j2

## Install packages
    cd ~/OpenDV/
    sudo dpkg -i opendv-base*.deb
    sudo dpkg -i ircddbgateway*.deb
    sudo dpkg -i dstarrepeater*.deb

## Enable daemons
    sudo systemctl enable ircddbgatewayd
    sudo systemctl enable dstarrepeaterd@1
If you have more than one repeater 
    sudo systemctl enable dstarrepeaterd@1

## Configuration and log files location
### Configuration files
	/etc/opendv/
### Log files
	/var/log/opendv/

## Support

Pre-compiled binary files are located in the Yahoo! groups including Windows binary files.

Please see the Yahoo! users group email reflectors for peer-support.

* [ircDDB Yahoo! group](https://groups.yahoo.com/neo/groups/ircDDBGateway/info)
* [PC Repeater Controller Yahoo! group](https://groups.yahoo.com/neo/groups/pcrepeatercontroller/info)
* [MMDVM Yahoo! group](https://groups.yahoo.com/neo/groups/mmdvm/info)

