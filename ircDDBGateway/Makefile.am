DEFS += \
	-DDATA_DIR=\"${pkgdatadir}\" \
	-DLOG_DIR=\"${localstatedir}/log/opendv\" \
    -DCONF_DIR=\"${sysconfdir}\"
    
STARNETSERVER_COMMON = \
	StarNetServer/StarNetServerConfig.cpp \
	StarNetServer/StarNetServerThread.cpp
	
APRSTRANSMIT_COMMON = \
	APRSTransmit/APRSParser.cpp \
	APRSTransmit/APRSTransmit.cpp
	
TIMERCONTROL_COMMON = \
	TimerControl/TimerControlConfig.cpp \
	TimerControl/TimerControlItemFile.cpp \
	TimerControl/TimerControlRemoteControlHandler.cpp \
	TimerControl/TimerControlThread.cpp

TIMESERVER_COMMON = \
	TimeServer/TimeServerConfig.cpp \
	TimeServer/TimeServerThread.cpp

noinst_LIBRARIES = libCommon.a libircDDB.a libGUICommon.a

libCommon_a_SOURCES = \
	Common/AMBEData.cpp \
	Common/AnnouncementUnit.cpp \
	Common/APRSCollector.cpp \
	Common/APRSWriter.cpp \
	Common/APRSWriterThread.cpp \
	Common/AudioUnit.cpp \
	Common/CacheManager.cpp \
	Common/CallsignList.cpp \
	Common/CallsignServer.cpp \
	Common/CCITTChecksum.cpp \
	Common/CCSData.cpp \
	Common/CCSHandler.cpp \
	Common/CCSProtocolHandler.cpp \
	Common/ConnectData.cpp \
	Common/DCSHandler.cpp \
	Common/DCSProtocolHandler.cpp \
	Common/DCSProtocolHandlerPool.cpp \
	Common/DDData.cpp \
	Common/DDHandler.cpp \
	Common/DExtraHandler.cpp \
	Common/DExtraProtocolHandler.cpp \
	Common/DExtraProtocolHandlerPool.cpp \
	Common/DPlusAuthenticator.cpp \
	Common/DPlusHandler.cpp \
	Common/DPlusProtocolHandler.cpp \
	Common/DPlusProtocolHandlerPool.cpp \
	Common/DRATSServer.cpp \
	Common/DTMF.cpp \
	Common/DummyRepeaterProtocolHandler.cpp \
	Common/DVTOOLFileReader.cpp \
	Common/EchoUnit.cpp \
	Common/G2Handler.cpp \
	Common/G2ProtocolHandler.cpp \
	Common/GatewayCache.cpp \
	Common/HBRepeaterProtocolHandler.cpp \
	Common/HeaderData.cpp \
	Common/HeaderLogger.cpp \
	Common/HeardData.cpp \
	Common/HostFile.cpp \
	Common/IcomRepeaterProtocolHandler.cpp \
	Common/IRCDDBGatewayConfig.cpp \
	Common/LogEvent.cpp \
	Common/Logger.cpp \
	Common/PollData.cpp \
	Common/RemoteHandler.cpp \
	Common/RemoteLinkData.cpp \
	Common/RemoteProtocolHandler.cpp \
	Common/RemoteRepeaterData.cpp \
	Common/RemoteStarNetGroup.cpp \
	Common/RemoteStarNetUser.cpp \
	Common/RepeaterCache.cpp \
	Common/RepeaterHandler.cpp \
	Common/SHA256.cpp \
	Common/SlowDataEncoder.cpp \
	Common/StarNetHandler.cpp \
	Common/StatusData.cpp \
	Common/TCPReaderWriterClient.cpp \
	Common/TCPReaderWriterServer.cpp \
	Common/TextCollector.cpp \
	Common/TextData.cpp \
	Common/Timer.cpp \
	Common/UDPReaderWriter.cpp \
	Common/UserCache.cpp \
	Common/Utils.cpp \
	Common/VersionUnit.cpp 
libCommon_a_CPPFLAGS = -ICommon -IircDDB $(BASE_WX_CPPFLAGS)

libircDDB_a_SOURCES = \
	ircDDB/IRCClient.cpp \
	ircDDB/IRCDDB.cpp \
	ircDDB/IRCDDBClient.cpp \
	ircDDB/IRCDDBMultiClient.cpp \
	ircDDB/IRCDDBApp.cpp \
	ircDDB/IRCMessage.cpp \
	ircDDB/IRCMessageQueue.cpp \
	ircDDB/IRCProtocol.cpp \
	ircDDB/IRCReceiver.cpp \
	ircDDB/IRCutils.cpp
libircDDB_a_CPPFLAGS = -IircDDB $(BASE_WX_CPPFLAGS)

libGUICommon_a_SOURCES = \
	GUICommon/AddressTextCtrl.cpp \
	GUICommon/CallsignTextCtrl.cpp \
	GUICommon/DCSSet.cpp \
	GUICommon/DescriptionTextCtrl.cpp \
	GUICommon/DExtraSet.cpp \
	GUICommon/DPlusSet.cpp \
	GUICommon/DPRSSet.cpp \
	GUICommon/PortTextCtrl.cpp \
	GUICommon/RemoteSet.cpp \
	GUICommon/RepeaterDataSet.cpp \
    GUICommon/RepeaterInfoSet.cpp \
    GUICommon/RestrictedTextCtrl.cpp \
    GUICommon/StarNetSet.cpp
libGUICommon_a_CPPFLAGS = -ICommon $(GUI_WX_CPPFLAGS)

sbin_PROGRAMS = ircddbgatewayd starnetserverd aprstransmitd timercontrold timeserverd
bin_PROGRAMS = texttransmit voicetransmit

ircddbgatewayd_SOURCES = \
	ircDDBGateway/IRCDDBGatewayAppD.cpp \
	ircDDBGateway/IRCDDBGatewayStatusData.cpp \
	ircDDBGateway/IRCDDBGatewayThread.cpp
ircddbgatewayd_LDADD = $(BASE_WX_LIBS) libCommon.a libircDDB.a
ircddbgatewayd_CPPFLAGS = -ICommon -IircDDB $(BASE_WX_CPPFLAGS)

starnetserverd_SOURCES = \
	StarNetServer/StarNetServerAppD.cpp \
	$(STARNETSERVER_COMMON)
starnetserverd_LDADD = $(BASE_WX_LIBS) libCommon.a libircDDB.a
starnetserverd_CPPFLAGS = -ICommon -IircDDB $(BASE_WX_CPPFLAGS)

aprstransmitd_SOURCES = \
	APRSTransmit/APRSTransmitAppD.cpp \
	$(APRSTRANSMIT_COMMON)
aprstransmitd_LDADD = $(BASE_WX_LIBS) libCommon.a
aprstransmitd_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

timercontrold_SOURCES = \
	TimerControl/TimerControlAppD.cpp \
	$(TIMERCONTROL_COMMON)
timercontrold_LDADD = $(BASE_WX_LIBS) libCommon.a
timercontrold_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

timeserverd_SOURCES = \
	TimeServer/TimeServerD.cpp \
	$(TIMESERVER_COMMON)
timeserverd_LDADD = $(BASE_WX_LIBS) libCommon.a
timeserverd_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

texttransmit_SOURCES = \
	TextTransmit/TextTransmit.cpp
texttransmit_LDADD = $(BASE_WX_LIBS) libCommon.a
texttransmit_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

voicetransmit_SOURCES = \
	VoiceTransmit/VoiceTransmit.cpp \
	VoiceTransmit/VoiceStore.cpp
voicetransmit_LDADD = $(BASE_WX_LIBS) libCommon.a
voicetransmit_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

pkgdata_DATA = \
	Data/CCS_Hosts.txt \
	Data/DCS_Hosts.txt \
	Data/de_DE.ambe \
	Data/de_DE.indx \
	Data/DExtra_Hosts.txt \
	Data/dk_DK.ambe \
	Data/dk_DK.indx \
	Data/DPlus_Hosts.txt \
	Data/en_GB.ambe \
	Data/en_GB.indx \
	Data/en_US.ambe \
	Data/en_US.indx \
	Data/es_ES.ambe \
	Data/es_ES.indx \
	Data/fr_FR.ambe \
	Data/fr_FR.indx \
	Data/it_IT.ambe \
	Data/it_IT.indx \
	Data/no_NO.ambe \
	Data/no_NO.indx \
	Data/pl_PL.ambe \
	Data/pl_PL.indx \
	Data/se_SE.ambe \
	Data/se_SE.indx \
	Data/TIME_de_DE.ambe \
	Data/TIME_de_DE.indx \
	Data/TIME_en_GB.ambe \
	Data/TIME_en_GB.indx \
	Data/TIME_en_US.ambe \
	Data/TIME_en_US.indx \
	Data/TIME_fr_FR.ambe \
	Data/TIME_fr_FR.indx \
	Data/TIME_se_SE.ambe \
	Data/TIME_se_SE.indx

if WITH_GUI
bin_PROGRAMS += ircddbgateway ircddbgatewayconfig remotecontrol starnetserver \
                aprstransmit timercontrol timeserver

ircddbgateway_SOURCES = \
	ircDDBGateway/IRCDDBGatewayApp.cpp \
    ircDDBGateway/IRCDDBGatewayFrame.cpp \
    ircDDBGateway/IRCDDBGatewayLogRedirect.cpp \
    ircDDBGateway/IRCDDBGatewayThreadHelper.cpp \
    ircDDBGateway/IRCDDBGatewayStatusData.cpp \
	ircDDBGateway/IRCDDBGatewayThread.cpp
ircddbgateway_LDADD = $(GUI_WX_LIBS) libCommon.a libircDDB.a
ircddbgateway_CPPFLAGS = -ICommon -IircDDB $(GUI_WX_CPPFLAGS)

ircddbgatewayconfig_SOURCES = \
	ircDDBGatewayConfig/IRCDDBGatewayConfigApp.cpp \
	ircDDBGatewayConfig/IRCDDBGatewayConfigFrame.cpp \
	ircDDBGatewayConfig/IRCDDBGatewayConfigGatewaySet.cpp \
	ircDDBGatewayConfig/IRCDDBGatewayConfigIrcDDBSet.cpp \
	ircDDBGatewayConfig/IRCDDBGatewayConfigMiscellaneousSet.cpp
ircddbgatewayconfig_LDADD = $(GUI_WX_LIBS) libGUICommon.a libCommon.a
ircddbgatewayconfig_CPPFLAGS = -ICommon -IGUICommon $(GUI_WX_CPPFLAGS)

remotecontrol_SOURCES = \
	RemoteControl/RemoteControlApp.cpp \
	RemoteControl/RemoteControlCallsignData.cpp \
	RemoteControl/RemoteControlConfig.cpp \
	RemoteControl/RemoteControlFrame.cpp \
	RemoteControl/RemoteControlLinkData.cpp \
	RemoteControl/RemoteControlPreferences.cpp \
	RemoteControl/RemoteControlRemoteControlHandler.cpp \
	RemoteControl/RemoteControlRemoteSet.cpp \
	RemoteControl/RemoteControlRepeaterData.cpp \
	RemoteControl/RemoteControlRepeaterPanel.cpp \
	RemoteControl/RemoteControlStarNetGroup.cpp \
	RemoteControl/RemoteControlStarNetPanel.cpp \
	RemoteControl/RemoteControlStarNetUser.cpp 
remotecontrol_LDADD = $(GUI_WX_LIBS) libCommon.a libGUICommon.a
remotecontrol_CPPFLAGS = -ICommon -IGUICommon $(GUI_WX_CPPFLAGS)

starnetserver_SOURCES = \
	StarNetServer/StarNetServerApp.cpp \
	StarNetServer/StarNetServerFrame.cpp \
	StarNetServer/StarNetServerCallsignSet.cpp \
	StarNetServer/StarNetServerIrcDDBSet.cpp \
	StarNetServer/StarNetServerLogRedirect.cpp \
	StarNetServer/StarNetServerMiscellaneousSet.cpp \
	StarNetServer/StarNetServerPreferences.cpp \
	StarNetServer/StarNetServerThreadHelper.cpp \
	$(STARNETSERVER_COMMON)
starnetserver_LDADD = $(GUI_WX_LIBS) libCommon.a libircDDB.a libGUICommon.a
starnetserver_CPPFLAGS = -ICommon -IircDDB -IGUICommon $(GUI_WX_CPPFLAGS)

aprstransmit_SOURCES = \
	APRSTransmit/APRSTransmitApp.cpp \
	$(APRSTRANSMIT_COMMON)
aprstransmit_LDADD = $(GUI_WX_LIBS) libCommon.a
aprstransmit_CPPFLAGS = -ICommon $(GUI_WX_CPPFLAGS)

timercontrol_SOURCES = \
	TimerControl/TimerControlApp.cpp \
	TimerControl/TimerControlFrame.cpp \
	TimerControl/TimerControlPreferences.cpp \
	TimerControl/TimerControlRemoteSet.cpp \
	TimerControl/TimerControlRepeaterPanel.cpp \
	TimerControl/TimerControlThreadHelper.cpp \
	$(TIMERCONTROL_COMMON)
timercontrol_LDADD = $(GUI_WX_LIBS) libCommon.a libGUICommon.a
timercontrol_CPPFLAGS = -ICommon -IGUICommon $(GUI_WX_CPPFLAGS)

timeserver_SOURCES = \
	TimeServer/TimeServerApp.cpp \
	TimeServer/TimeServerAnnouncementsSet.cpp \
	TimeServer/TimeServerFrame.cpp \
	TimeServer/TimeServerGatewaySet.cpp \
	TimeServer/TimeServerLogRedirect.cpp \
	TimeServer/TimeServerThreadHelper.cpp \
	TimeServer/TimeServerPreferences.cpp \
	$(TIMESERVER_COMMON)
timeserver_LDADD = $(GUI_WX_LIBS) libCommon.a libGUICommon.a
timeserver_CPPFLAGS = -ICommon -IGUICommon $(GUI_WX_CPPFLAGS)

endif
