all:			Common.a

CFLAGS :=		-I../ircDDB -I../Common $(CFLAGS)

Common.a:		AddressTextCtrl.o AMBEData.o APRSCollector.o APRSWriter.o APRSWriterThread.o AudioUnit.o CacheManager.o CallsignList.o CallsignServer.o \
				CallsignTextCtrl.o CCITTChecksum.o CCSData.o CCSHandler.o CCSProtocolHandler.o ConnectData.o DescriptionTextCtrl.o DCSHandler.o DCSProtocolHandler.o \
				DCSProtocolHandlerPool.o DCSSet.o DDData.o DDHandler.o DExtraHandler.o DExtraProtocolHandler.o DExtraProtocolHandlerPool.o DExtraSet.o DPlusAuthenticator.o \
				DPlusHandler.o DPlusProtocolHandler.o DPlusProtocolHandlerPool.o DPlusSet.o DPRSSet.o DRATSServer.o DTMF.o DummyRepeaterProtocolHandler.o \
				DVTOOLFileReader.o EchoUnit.o G2Handler.o G2ProtocolHandler.o GatewayCache.o HBRepeaterProtocolHandler.o HeaderData.o HeaderLogger.o HeardData.o HostFile.o \
				IcomRepeaterProtocolHandler.o IRCDDBGatewayConfig.o LogEvent.o Logger.o PollData.o PortTextCtrl.o RemoteHandler.o RemoteLinkData.o RemoteProtocolHandler.o \
				RemoteRepeaterData.o RemoteSet.o RemoteStarNetGroup.o RemoteStarNetUser.o RepeaterCache.o RepeaterHandler.o RepeaterDataSet.o RepeaterInfoSet.o \
				RestrictedTextCtrl.o SHA256.o SlowDataEncoder.o StarNetHandler.o StarNetSet.o StatusData.o TCPReaderWriterClient.o TCPReaderWriterServer.o TextCollector.o \
				TextData.o Timer.o UDPReaderWriter.o UserCache.o Utils.o VersionUnit.o
				$(AR) rcs Common.a AddressTextCtrl.o AMBEData.o APRSCollector.o APRSWriter.o APRSWriterThread.o AudioUnit.o CacheManager.o CallsignList.o \
				CallsignServer.o CallsignTextCtrl.o CCITTChecksum.o CCSData.o CCSHandler.o CCSProtocolHandler.o ConnectData.o DescriptionTextCtrl.o DCSHandler.o \
				DCSProtocolHandler.o DCSProtocolHandlerPool.o DCSSet.o DDData.o DDHandler.o DExtraHandler.o DExtraProtocolHandler.o DExtraProtocolHandlerPool.o DExtraSet.o \
				DPlusAuthenticator.o DPlusHandler.o DPlusProtocolHandler.o DPlusProtocolHandlerPool.o DPlusSet.o DPRSSet.o DRATSServer.o DTMF.o \
				DummyRepeaterProtocolHandler.o DVTOOLFileReader.o EchoUnit.o G2Handler.o G2ProtocolHandler.o GatewayCache.o HBRepeaterProtocolHandler.o HeaderData.o \
				HeaderLogger.o HeardData.o HostFile.o IcomRepeaterProtocolHandler.o IRCDDBGatewayConfig.o LogEvent.o Logger.o PollData.o PortTextCtrl.o RemoteHandler.o \
				RemoteLinkData.o RemoteProtocolHandler.o RemoteRepeaterData.o RemoteSet.o RemoteStarNetGroup.o RemoteStarNetUser.o RepeaterCache.o RepeaterHandler.o \
				RepeaterDataSet.o RepeaterInfoSet.o RestrictedTextCtrl.o SHA256.o SlowDataEncoder.o StarNetHandler.o StarNetSet.o StatusData.o TCPReaderWriterClient.o \
				TCPReaderWriterServer.o TextCollector.o TextData.o Timer.o UDPReaderWriter.o UserCache.o Utils.o VersionUnit.o

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

AMBEData.o:		AMBEData.cpp AMBEData.h DStarDefines.h HeaderData.h Utils.h
				$(CC) $(CFLAGS) -c AMBEData.cpp

APRSWriter.o:	APRSWriter.cpp APRSWriter.h AMBEData.h APRSCollector.h DStarDefines.h Defs.h Timer.h APRSWriterThread.h
				$(CC) $(CFLAGS) -c APRSWriter.cpp

APRSWriterThread.o:	APRSWriterThread.cpp APRSWriterThread.h TCPReaderWriterClient.h DStarDefines.h Defs.h Utils.h RingBuffer.h
				$(CC) $(CFLAGS) -c APRSWriterThread.cpp

APRSCollector.o:	APRSCollector.cpp APRSCollector.h DStarDefines.h Utils.h Defs.h
				$(CC) $(CFLAGS) -c APRSCollector.cpp

AudioUnit.o:	AudioUnit.cpp AudioUnit.h RepeaterCallback.h HeaderData.h AMBEData.h Timer.h DStarDefines.h Defs.h SlowDataEncoder.h
				$(CC) $(CFLAGS) -c AudioUnit.cpp

CacheManager.o:	CacheManager.cpp CacheManager.h GatewayCache.h RepeaterCache.h UserCache.h DStarDefines.h
				$(CC) $(CFLAGS) -c CacheManager.cpp

CallsignList.o:	CallsignList.cpp CallsignList.h DStarDefines.h
				$(CC) $(CFLAGS) -c CallsignList.cpp

CallsignServer.o:	CallsignServer.cpp CallsignServer.h TCPReaderWriterClient.h Timer.h CacheManager.h Utils.h DStarDefines.h Defs.h
				$(CC) $(CFLAGS) -c CallsignServer.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksum.o:	CCITTChecksum.cpp CCITTChecksum.h Utils.h
				$(CC) $(CFLAGS) -c CCITTChecksum.cpp

CCSData.o:		CCSData.cpp CCSData.h DStarDefines.h Utils.h
				$(CC) $(CFLAGS) -c CCSData.cpp

CCSHandler.o:	CCSHandler.cpp CCSHandler.h DStarDefines.h RepeaterHandler.h Utils.h CCSProtocolHandler.h ConnectData.h AMBEData.h \
				PollData.h Timer.h Defs.h HeaderLogger.h CCSCallback.h HeaderData.h CCSData.h
				$(CC) $(CFLAGS) -c CCSHandler.cpp

CCSProtocolHandler.o:	CCSProtocolHandler.cpp CCSProtocolHandler.h AMBEData.h UDPReaderWriter.h DStarDefines.h Timer.h Utils.h ConnectData.h PollData.h \
				CCSData.h HeardData.h
				$(CC) $(CFLAGS) -c CCSProtocolHandler.cpp

ConnectData.o:	ConnectData.cpp ConnectData.h DStarDefines.h Version.h Utils.h Defs.h
				$(CC) $(CFLAGS) -c ConnectData.cpp

DescriptionTextCtrl.o:	DescriptionTextCtrl.cpp DescriptionTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c DescriptionTextCtrl.cpp

DCSHandler.o:	DCSHandler.cpp DCSHandler.h DStarDefines.h RepeaterHandler.h Utils.h DCSProtocolHandlerPool.h DCSProtocolHandler.h ConnectData.h AMBEData.h \
				PollData.h Timer.h Defs.h HeaderLogger.h ReflectorCallback.h HeaderData.h
				$(CC) $(CFLAGS) -c DCSHandler.cpp

DCSProtocolHandler.o:	DCSProtocolHandler.cpp DCSProtocolHandler.h AMBEData.h UDPReaderWriter.h DStarDefines.h Timer.h Utils.h ConnectData.h PollData.h
				$(CC) $(CFLAGS) -c DCSProtocolHandler.cpp

DCSProtocolHandlerPool.o:	DCSProtocolHandlerPool.cpp DCSProtocolHandlerPool.h DCSProtocolHandler.h
				$(CC) $(CFLAGS) -c DCSProtocolHandlerPool.cpp

DCSSet.o:		DCSSet.cpp DCSSet.h HostFile.h Defs.h
				$(CC) $(CFLAGS) -c DCSSet.cpp

DDData.o:		DDData.cpp DDData.h DStarDefines.h Utils.h HeaderData.h
				$(CC) $(CFLAGS) -c DDData.cpp

DDHandler.o:	DDHandler.cpp DDHandler.h DDData.h RepeaterHandler.h HeaderLogger.h Defs.h Timer.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c DDHandler.cpp

DExtraHandler.o:	DExtraHandler.cpp DExtraHandler.h DStarDefines.h RepeaterHandler.h Utils.h DExtraProtocolHandlerPool.h DExtraProtocolHandler.h \
					ConnectData.h HeaderData.h AMBEData.h PollData.h Timer.h Defs.h HeaderLogger.h ReflectorCallback.h
				$(CC) $(CFLAGS) -c DExtraHandler.cpp

DExtraProtocolHandler.o:	DExtraProtocolHandler.cpp DExtraProtocolHandler.h HeaderData.h AMBEData.h UDPReaderWriter.h DStarDefines.h Timer.h Utils.h \
							ConnectData.h PollData.h
				$(CC) $(CFLAGS) -c DExtraProtocolHandler.cpp

DExtraProtocolHandlerPool.o:	DExtraProtocolHandlerPool.cpp DExtraProtocolHandlerPool.h DExtraProtocolHandler.h
				$(CC) $(CFLAGS) -c DExtraProtocolHandlerPool.cpp

DExtraSet.o:	DExtraSet.cpp DExtraSet.h
				$(CC) $(CFLAGS) -c DExtraSet.cpp

DPlusAuthenticator.o:	DPlusAuthenticator.cpp DPlusAuthenticator.h UDPReaderWriter.h TCPReaderWriterClient.h Timer.h CacheManager.h Utils.h DStarDefines.h \
						Defs.h
				$(CC) $(CFLAGS) -c DPlusAuthenticator.cpp

DPlusHandler.o:	DPlusHandler.cpp DPlusHandler.h DStarDefines.h RepeaterHandler.h Utils.h DPlusProtocolHandlerPool.h DPlusProtocolHandler.h ConnectData.h \
						HeaderData.h AMBEData.h PollData.h Timer.h Defs.h HeaderLogger.h DPlusAuthenticator.h CacheManager.h ReflectorCallback.h
				$(CC) $(CFLAGS) -c DPlusHandler.cpp

DPlusProtocolHandler.o:	DPlusProtocolHandler.cpp DPlusProtocolHandler.h HeaderData.h AMBEData.h UDPReaderWriter.h DStarDefines.h Timer.h Utils.h \
						ConnectData.h PollData.h
				$(CC) $(CFLAGS) -c DPlusProtocolHandler.cpp

DPlusProtocolHandlerPool.o:	DPlusProtocolHandlerPool.cpp DPlusProtocolHandlerPool.h DPlusProtocolHandler.h
				$(CC) $(CFLAGS) -c DPlusProtocolHandlerPool.cpp

DPlusSet.o:		DPlusSet.cpp DPlusSet.h Defs.h CallsignTextCtrl.h DStarDefines.h
				$(CC) $(CFLAGS) -c DPlusSet.cpp

DPRSSet.o:		DPRSSet.cpp DPRSSet.h PortTextCtrl.h
				$(CC) $(CFLAGS) -c DPRSSet.cpp

DRATSServer.o:		DRATSServer.cpp DRATSServer.h TCPReaderWriterServer.h RepeaterCallback.h HeaderData.h AMBEData.h Utils.h DStarDefines.h Defs.h
				$(CC) $(CFLAGS) -c DRATSServer.cpp

DTMF.o:			DTMF.cpp DTMF.h
				$(CC) $(CFLAGS) -c DTMF.cpp

DummyRepeaterProtocolHandler.o:	DummyRepeaterProtocolHandler.cpp DummyRepeaterProtocolHandler.h RepeaterProtocolHandler.h HeaderData.h AMBEData.h \
								DStarDefines.h Utils.h TextData.h StatusData.h DDData.h HeardData.h PollData.h
				$(CC) $(CFLAGS) -c DummyRepeaterProtocolHandler.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h HeaderData.h AMBEData.h DStarDefines.h
				$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

EchoUnit.o:		EchoUnit.cpp EchoUnit.h RepeaterCallback.h HeaderData.h AMBEData.h Timer.h DStarDefines.h Defs.h
				$(CC) $(CFLAGS) -c EchoUnit.cpp

G2Handler.o:	G2Handler.cpp G2Handler.h G2ProtocolHandler.h DStarDefines.h HeaderData.h AMBEData.h Timer.h RepeaterHandler.h Utils.h Defs.h HeaderLogger.h \
				StarNetHandler.h
				$(CC) $(CFLAGS) -c G2Handler.cpp

G2ProtocolHandler.o:	G2ProtocolHandler.cpp G2ProtocolHandler.h DStarDefines.h Utils.h UDPReaderWriter.h HeaderData.h AMBEData.h
				$(CC) $(CFLAGS) -c G2ProtocolHandler.cpp

GatewayCache.o:	GatewayCache.cpp GatewayCache.h Defs.h DStarDefines.h
				$(CC) $(CFLAGS) -c GatewayCache.cpp

HBRepeaterProtocolHandler.o:	HBRepeaterProtocolHandler.cpp HBRepeaterProtocolHandler.h RepeaterProtocolHandler.h HeaderData.h AMBEData.h \
								UDPReaderWriter.h DStarDefines.h CCITTChecksum.h Utils.h TextData.h StatusData.h DDData.h HeardData.h PollData.h
				$(CC) $(CFLAGS) -c HBRepeaterProtocolHandler.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h CCITTChecksum.h DStarDefines.h Utils.h
				$(CC) $(CFLAGS) -c HeaderData.cpp

HeaderLogger.o:	HeaderLogger.cpp HeaderLogger.h HeaderData.h DDData.h Defs.h
				$(CC) $(CFLAGS) -c HeaderLogger.cpp

HeardData.o:	HeardData.cpp HeardData.h HeaderData.h DStarDefines.h
				$(CC) $(CFLAGS) -c HeardData.cpp

HostFile.o:		HostFile.cpp HostFile.h DStarDefines.h
				$(CC) $(CFLAGS) -c HostFile.cpp

IcomRepeaterProtocolHandler.o:	IcomRepeaterProtocolHandler.cpp IcomRepeaterProtocolHandler.h RepeaterProtocolHandler.h HeaderData.h AMBEData.h \
								UDPReaderWriter.h DStarDefines.h CCITTChecksum.h Utils.h TextData.h RingBuffer.h Timer.h StatusData.h DDData.h \
								HeardData.h PollData.h
				$(CC) $(CFLAGS) -c IcomRepeaterProtocolHandler.cpp

IRCDDBGatewayConfig.o:	IRCDDBGatewayConfig.cpp IRCDDBGatewayConfig.h Defs.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfig.cpp

LogEvent.o:		LogEvent.cpp LogEvent.h
				$(CC) $(CFLAGS) -c LogEvent.cpp

Logger.o:		Logger.cpp Logger.h
				$(CC) $(CFLAGS) -c Logger.cpp

PollData.o:		PollData.cpp PollData.h DStarDefines.h Defs.h Utils.h
				$(CC) $(CFLAGS) -c PollData.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RemoteHandler.o:	RemoteHandler.cpp RemoteHandler.h RemoteProtocolHandler.h Timer.h RepeaterHandler.h StarNetHandler.h DExtraHandler.h DPlusHandler.h \
					DCSHandler.h DStarDefines.h StatusData.h
				$(CC) $(CFLAGS) -c RemoteHandler.cpp

RemoteLinkData.o:	RemoteLinkData.cpp RemoteLinkData.h Defs.h
				$(CC) $(CFLAGS) -c RemoteLinkData.cpp

RemoteProtocolHandler.o:	RemoteProtocolHandler.cpp RemoteProtocolHandler.h RemoteRepeaterData.h RemoteStarNetGroup.h UDPReaderWriter.h DStarDefines.h \
							SHA256.h Utils.h Defs.h
				$(CC) $(CFLAGS) -c RemoteProtocolHandler.cpp

RemoteRepeaterData.o:	RemoteRepeaterData.cpp RemoteRepeaterData.h RemoteLinkData.h

				$(CC) $(CFLAGS) -c RemoteRepeaterData.cpp

RemoteSet.o:	RemoteSet.cpp RemoteSet.h PortTextCtrl.h DStarDefines.h
				$(CC) $(CFLAGS) -c RemoteSet.cpp

RemoteStarNetGroup.o:	RemoteStarNetGroup.cpp RemoteStarNetGroup.h RemoteStarNetUser.h
				$(CC) $(CFLAGS) -c RemoteStarNetGroup.cpp

RemoteStarNetUser.o:	RemoteStarNetUser.cpp RemoteStarNetUser.h
				$(CC) $(CFLAGS) -c RemoteStarNetUser.cpp

RepeaterCache.o:	RepeaterCache.cpp RepeaterCache.h
				$(CC) $(CFLAGS) -c RepeaterCache.cpp

RepeaterHandler.o:		RepeaterHandler.cpp RepeaterHandler.h RepeaterProtocolHandler.h DCSProtocolHandler.h DExtraProtocolHandler.h \
						DPlusProtocolHandler.h G2ProtocolHandler.h CacheManager.h Timer.h DExtraHandler.h DPlusHandler.h DStarDefines.h HeaderData.h \
						AMBEData.h Utils.h Defs.h HeaderLogger.h APRSWriter.h TextCollector.h EchoUnit.h RepeaterCallback.h AudioUnit.h DRATSServer.h \
						StarNetHandler.h ReflectorCallback.h DDData.h DDHandler.h RemoteRepeaterData.h StatusData.h DTMF.h HeardData.h VersionUnit.h \
						PollData.h DCSHandler.h CCSHandler.h CCSCallback.h CallsignList.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c RepeaterHandler.cpp

RepeaterDataSet.o:	RepeaterDataSet.cpp RepeaterDataSet.h Defs.h DStarDefines.h AddressTextCtrl.h PortTextCtrl.h HostFile.h
				$(CC) $(CFLAGS) -c RepeaterDataSet.cpp

RepeaterInfoSet.o:	RepeaterInfoSet.cpp RepeaterInfoSet.h Defs.h DStarDefines.h DescriptionTextCtrl.h
				$(CC) $(CFLAGS) -c RepeaterInfoSet.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SHA256.o:		SHA256.cpp SHA256.h
				$(CC) $(CFLAGS) -c SHA256.cpp

SlowDataEncoder.o:	SlowDataEncoder.cpp SlowDataEncoder.h HeaderData.h CCITTChecksum.h DStarDefines.h
				$(CC) $(CFLAGS) -c SlowDataEncoder.cpp

StarNetHandler.o:	StarNetHandler.cpp StarNetHandler.h TextCollector.h SlowDataEncoder.h G2ProtocolHandler.h CacheManager.h HeaderData.h AMBEData.h \
					Timer.h DStarDefines.h RepeaterHandler.h RepeaterCallback.h RemoteStarNetGroup.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c StarNetHandler.cpp

StarNetSet.o:	StarNetSet.cpp StarNetSet.h CallsignTextCtrl.h DStarDefines.h Defs.h
				$(CC) $(CFLAGS) -c StarNetSet.cpp

StatusData.o:	StatusData.cpp StatusData.h DStarDefines.h Utils.h
				$(CC) $(CFLAGS) -c StatusData.cpp

TCPReaderWriterClient.o:	TCPReaderWriterClient.cpp TCPReaderWriterClient.h UDPReaderWriter.h
				$(CC) $(CFLAGS) -c TCPReaderWriterClient.cpp

TCPReaderWriterServer.o:	TCPReaderWriterServer.cpp TCPReaderWriterServer.h TCPReaderWriterClient.h
				$(CC) $(CFLAGS) -c TCPReaderWriterServer.cpp

TextCollector.o:	TextCollector.cpp TextCollector.h AMBEData.h DStarDefines.h Defs.h Utils.h
				$(CC) $(CFLAGS) -c TextCollector.cpp

TextData.o:		TextData.cpp TextData.h DStarDefines.h Utils.h Defs.h
				$(CC) $(CFLAGS) -c TextData.cpp

Timer.o:		Timer.cpp Timer.h
				$(CC) $(CFLAGS) -c Timer.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h
				$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

UserCache.o:	UserCache.cpp UserCache.h
				$(CC) $(CFLAGS) -c UserCache.cpp

Utils.o:		Utils.cpp Utils.h
				$(CC) $(CFLAGS) -c Utils.cpp

VersionUnit.o:	VersionUnit.cpp VersionUnit.h RepeaterCallback.h HeaderData.h AMBEData.h Timer.h DStarDefines.h Defs.h SlowDataEncoder.h Version.h
				$(CC) $(CFLAGS) -c VersionUnit.cpp

clean:
				rm -f core Common.a *.o *~ *.bak
