# Makefile
#
# Build the ircDDB Gateway software on Linux
#

include settings.mk

all:	ircDDBGateway/ircddbgateway ircDDBGatewayConfig/ircddbgatewayconfig RemoteControl/remotecontrol StarNetServer/starnetserver TextTransmit/texttransmit \
			APRSTransmit/aprstransmit TimerControl/timercontrol TimeServer/timeserver VoiceTransmit/voicetransmit

ircDDBGateway/ircddbgateway:	ircDDB/libircDDB.a Common/Common.a
		make -C ircDDBGateway

ircDDBGatewayConfig/ircddbgatewayconfig:	Common/Common.a GUICommon/GUICommon.a
		make -C ircDDBGatewayConfig

RemoteControl/remotecontrol:	Common/Common.a GUICommon/GUICommon.a
		make -C RemoteControl

StarNetServer/starnetserver:	ircDDB/libircDDB.a Common/Common.a GUICommon/GUICommon.a
		make -C StarNetServer

TextTransmit/texttransmit:	Common/Common.a
		make -C TextTransmit

APRSTransmit/aprstransmit:	Common/Common.a
		make -C APRSTransmit

TimerControl/timercontrol:	Common/Common.a GUICommon/GUICommon.a
		make -C TimerControl

TimeServer/timeserver:	Common/Common.a GUICommon/GUICommon.a
		make -C TimeServer

VoiceTransmit/voicetransmit:	Common/Common.a
		make -C VoiceTransmit

Common/Common.a:
		make -C Common

GUICommon/GUICommon.a:
		make -C GUICommon

ircDDB/libircDDB.a:
		make -C ircDDB

install:	all
		make -C ircDDBGateway install
		make -C ircDDBGatewayConfig install
		make -C RemoteControl install
		make -C StarNetServer install
		make -C TextTransmit install
		make -C APRSTransmit install
		make -C TimerControl install
		make -C TimeServer install
		make -C VoiceTransmit install
		make -C Data install

clean:
		rm -f core *~ *.bak
		make -C ircDDB clean
		make -C Common clean
		make -C GUICommon clean
		make -C ircDDBGateway clean
		make -C ircDDBGatewayConfig clean
		make -C RemoteControl clean
		make -C StarNetServer clean
		make -C TextTransmit clean
		make -C APRSTransmit clean
		make -C TimerControl clean
		make -C TimeServer clean
		make -C VoiceTransmit clean
		
include package.mk
