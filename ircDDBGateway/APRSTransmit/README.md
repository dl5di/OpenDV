
# APRSTransmit and APRSTransmitD
APRSTransmit and APRSTransmitD allow both to transmit APRS data over DStar i.e. turning it to DPRS. Newer radios like ID51+, ID5100 and ID4100 have extended DPRS capability and are able to handle various data types such as weather and objects. Yet not all APRS formats are supported by Icom : Mic-E, Compressed. When possible the APRS data is converted so that it can be parsed by Icom radios.

# APRSTransmit
APRSTransmit sends one single APRS frame to the specified repeater. The frame can be sent directly on the command line, be sure to escape it into single quotes.
```sh
aprstransmit N1RPT__C -aprs 'N0CALL>APRS:!3842.78N/00759.76E>208/023/Hello'
```
It can also read data from a single file. For example, this comes handy if you run a weather station with weewx and want to broadcast it over DPRS
```sh
aprstransmit N1RPT__C -file /path/to/your/file/containing/one/line/of/aprs/data
```

# APRSTransmitD
APRSTransmitD is a daemon that runs in the background and takes data from APRSIS and broadcasts as DPRS to the specified DStar repeater. It can take any [server side APRS filter](http://www.aprs-is.net/javaprsfilter.aspx) and will pass the data received to the repeater.
## Running from command line
The line below will run aprstransmitd in the console, staying in front. This can be useful to see what is being taken from APRSIS over to your repeater.
The example below forwards aprs data within a radius of 100km to n1rpt__c.
```sh
sudo aprstransmitd n1rpt__c -host rotate.aprs2.net -port 14580 -filter 'm/100'
```
To run it as a background daemon, add -daemon switch e.g.:
```sh
sudo aprstransmitd n1rpt__c -host rotate.aprs2.net -port 14580 -filter 'm/100' -daemon
```
# Runnning as a systemd service
To run as a systemd service you must have installed aprstransmitd through the debian packages. Once, this is done configuration works as follow. 
Lets assume we want to send APRS data on the n1rpt__c repeater.
Copy file /etc/opendv/aprstransmitd.conf to /etc/opendv/aprstransmid_n1rpt__c.conf
```sh
sudo cp /etc/opendv/aprstransmid.conf /etc/opendv/aprstransmid_n1rpt__c.conf
```
Edit the file /etc/opendv/aprstransmid_n1rpt__c.conf and change parameters to suite your needs.

Now enable the service 
```sh
sudo systemctl enable aprstransmitd@n1rtpt__c
```
And start it
```sh
sudo service aprstransmitd@n1rpt__c start
```sh

You can repeat above steps if you have a second repeater running.
