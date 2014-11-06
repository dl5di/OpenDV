#!/bin/bash
#
# AnalogueRepeater start script for GUI
# Hans-J. Barthen (DL5DI) - dl5di@gmx.de
#
# description: Starts the G4KLX AnalogueRepeater in GUI mode and sets the LC_NUMERIC LOCALE
# to en_US.UTF-8 to make the configfile compatible for GUI and daemon modem.
# config: /etc/sysconfig/analoguerepeater
#

if [ $UID -ne 0 ]; then
	echo
	echo "ERROR:  This script must be run as the root user!"
	echo "        Please use 'su' or log in as root and try again."
	echo
	exit 1
fi

xhost -

# start AnalogueRepeater
su -c "/usr/bin/analoguerepeater -gui -logdir=/var/log/opendv" opendv
xhost +
