DEFS += -DDATA_DIR=\"${pkgdatadir}\"
    
noinst_LIBRARIES = libCommon.a libircDDB.a

libCommon_a_SOURCES = \
 	Common/AMBEData.cpp \
 	Common/APRSCollector.cpp \
 	Common/APRSWriter.cpp \
 	Common/APRSWriterThread.cpp \
 	Common/AudioUnit.cpp \
 	Common/CacheManager.cpp \
 	Common/CallsignServer.cpp \
	Common/CCITTChecksum.cpp \
 	Common/ConnectData.cpp \
 	Common/DDData.cpp \
 	Common/DDHandler.cpp \
 	Common/DPlusAuthenticator.cpp \
 	Common/DPlusHandler.cpp \
 	Common/DPlusProtocolHandler.cpp \
 	Common/DPlusProtocolHandlerPool.cpp \
 	Common/DRATSServer.cpp \
 	Common/DTMF.cpp \
 	Common/DummyRepeaterProtocolHandler.cpp \
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
 	Common/LogEvent.cpp \
 	Common/Logger.cpp \
 	Common/PollData.cpp \
 	Common/RemoteHandler.cpp \
	Common/RemoteLinkData.cpp \
 	Common/RemoteProtocolHandler.cpp \
 	Common/RemoteRepeaterData.cpp \
 	Common/RemoteReflectorData.cpp \
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
 	Common/VersionUnit.cpp \
 	Common/DExtraHandler.cpp \
 	Common/DExtraProtocolHandler.cpp
libCommon_a_CPPFLAGS = -ICommon -IircDDB $(BASE_WX_CPPFLAGS)

libircDDB_a_SOURCES = \
	ircDDB/IRCClient.cpp \
 	ircDDB/IRCDDB.cpp \
 	ircDDB/IRCDDBApp.cpp \
 	ircDDB/IRCMessage.cpp \
 	ircDDB/IRCMessageQueue.cpp \
 	ircDDB/IRCProtocol.cpp \
 	ircDDB/IRCReceiver.cpp \
 	ircDDB/IRCutils.cpp
libircDDB_a_CPPFLAGS = -IircDDB $(BASE_WX_CPPFLAGS)

sbin_PROGRAMS = xreflectord

xreflectord_SOURCES = \
	XReflector/XReflectorAppD.cpp \
 	XReflector/XReflectorConfig.cpp \
 	XReflector/XReflectorDPlusHandler.cpp \
 	XReflector/XReflectorReflectorHandler.cpp \
 	XReflector/XReflectorThread.cpp \
 	XReflector/XReflectorUserLog.cpp \
 	XReflector/XReflectorDExtraHandler.cpp
xreflectord_LDADD = $(BASE_WX_LIBS) libCommon.a libircDDB.a
xreflectord_CPPFLAGS = -ICommon -IircDDB $(BASE_WX_CPPFLAGS)

if WITH_DEXTRA
DEFS += -DDEXTRA_LINK
endif

if WITH_DCS
libCommon_a_SOURCES += \
 	Common/DCSHandler.cpp \
 	Common/DCSProtocolHandler.cpp \
 	Common/DCSProtocolHandlerPool.cpp \
 	Common/CCSHandler.cpp \
 	Common/CCSProtocolHandler.cpp
DEFS += -DDCS_LINK
endif

if WITH_GUI
bin_PROGRAMS = xreflector

xreflector_SOURCES = \
	Common/AddressTextCtrl.cpp \
	Common/CallsignTextCtrl.cpp \
	Common/DPlusSet.cpp \
	Common/DPRSSet.cpp \
	Common/PortTextCtrl.cpp \
	Common/RestrictedTextCtrl.cpp \
	Common/RemoteSet.cpp \
	Common/RepeaterDataSet.cpp \
	Common/RepeaterInfoSet.cpp \
	Common/StarNetSet.cpp \
	XReflector/XReflectorApp.cpp \
 	XReflector/XReflectorConfig.cpp \
 	XReflector/XReflectorDExtraHandler.cpp \
 	XReflector/XReflectorDPlusHandler.cpp \
 	XReflector/XReflectorFrame.cpp \
 	XReflector/XReflectorLogRedirect.cpp \
 	XReflector/XReflectorMiscellaneousSet.cpp \
 	XReflector/XReflectorPreferences.cpp \
 	XReflector/XReflectorReflectorHandler.cpp \
 	XReflector/XReflectorReflectorSet.cpp \
 	XReflector/XReflectorThread.cpp \
 	XReflector/XReflectorDPlusSet.cpp \
 	XReflector/XReflectorUserLog.cpp \
 	XReflector/XReflectorThreadHelper.cpp
xreflector_LDADD = $(GUI_WX_LIBS) libCommon.a libircDDB.a
xreflector_CPPFLAGS = -ICommon -IircDDB $(GUI_WX_CPPFLAGS)

if WITH_DCS
xreflector_SOURCES += \
	Common/DCSSet.cpp
endif

if WITH_DEXTRA
xreflector_SOURCES += \
	Common/DExtraSet.cpp
endif

endif
