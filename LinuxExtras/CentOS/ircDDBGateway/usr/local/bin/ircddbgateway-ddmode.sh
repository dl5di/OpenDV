#!/bin/bash
#
# ircDDBGateway start script for GUI
# Hans-J. Barthen (DL5DI) - dl5di@gmx.de
#
# description: Starts the G4KLX ircDDBGateway in GUI mode and sets the LC_NUMERIC LOCALE
# to en_US.UTF-8 to make the configfile compatible for GUI and daemon mode.
# config: /etc/sysconfig/ircddbgateway
#

if [ $UID -ne 0 ]; then
	echo
	echo "ERROR:  This script must be run as the root user!"
	echo "        Please use 'su' or log in as root and try again."
	echo
	exit 1
fi

NAME=ircddbgateway

# Take care to start necessary services for DD-mode if enabled

# Read configuration file if it is present
[ -r /etc/sysconfig/$NAME ] && . /etc/sysconfig/$NAME || echo "Configfile /etc/sysconfig/$NAME is missing"

if [ "${IRCDDBGATEWAY_DDENABLED}" = "yes" ];then

# give ircddbgateway time to come up and create the tapi device, no need to hurry
    sleep 60
    echo " - configure dd-mode network device $IRCDDBGATEWAY_DDDEV "
    /sbin/ifconfig $IRCDDBGATEWAY_DDDEV $IRCDDBGATEWAY_DDIP netmask $IRCDDBGATEWAY_DDMASK
    echo 1 > /proc/sys/net/ipv4/ip_forward
    if [ "${IRCDDBGATEWAY_DDDHCP}" = "yes" ];then
        echo " - start dhcpd "
        /etc/init.d/dhcpd start
    fi
fi
                                        