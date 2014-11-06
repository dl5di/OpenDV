#!/bin/bash
#
# ircDDBGateway start script for GUI
# Hans-J. Barthen (DL5DI) - dl5di@gmx.de
#
# description: Starts the G4KLX ircDDBGateway in GUI mode and sets the LC_NUMERIC LOCALE
# to en_US.UTF-8 to make the configfile compatible for GUI and daemon mode.
# config: /etc/sysconfig/dstarrepeaterconfig
#

if [ $UID -ne 0 ]; then
	echo
	echo "ERROR:  This script must be run as the root user!"
	echo "        Please use 'su' or log in as root and try again."
	echo
	exit 1
fi

# start main task with correct LOCALE settings
LC_NUMERIC="en_US.UTF-8" /usr/local/bin/dstarrepeaterconfig -gui -logdir=/var/log/dstar
