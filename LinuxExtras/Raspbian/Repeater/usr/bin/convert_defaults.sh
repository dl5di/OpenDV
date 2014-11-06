#!/bin/bash
for i in {1..4}
do
    filename=repeater_${i}
    if [ -f /etc/default/$filename ]; then
        mv /etc/default/$filename /etc/default/$filename.bak
        cat /etc/default/$filename.bak | grep "^DAEMON_PATH" |  sed "s/DAEMON_PATH=\/usr\/bin\//DAEMON_NAME=/" > /etc/default/$filename
		echo "$filename entry DAEMON_NAME"
        cat /etc/default/$filename.bak | grep "^DAEMON_PATH" >> /etc/default/$filename
		echo "$filename entry DAEMON_PATH"
        cat /etc/default/$filename.bak | grep "^DAEMON_ARGS" >> /etc/default/$filename
		echo "$filename entry DAEMON_ARGS"
        cat /etc/default/$filename.bak | grep "^DAEMON_USER" >> /etc/default/$filename
		echo "$filename entry DAEMON_USER"
		echo "$filename done"
    fi
done
