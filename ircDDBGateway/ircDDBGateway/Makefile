all:			ircddbgateway ircddbgatewayd

CFLAGS :=		-I../ircDDB -I../Common $(CFLAGS)

ircddbgateway:	IRCDDBGatewayApp.o IRCDDBGatewayFrame.o IRCDDBGatewayLogRedirect.o IRCDDBGatewayStatusData.o IRCDDBGatewayThread.o IRCDDBGatewayThreadHelper.o \
				../ircDDB/libircDDB.a ../Common/Common.a
				$(CC) $(LDFLAGS) -o ircddbgateway IRCDDBGatewayApp.o IRCDDBGatewayFrame.o IRCDDBGatewayLogRedirect.o IRCDDBGatewayStatusData.o IRCDDBGatewayThread.o \
				IRCDDBGatewayThreadHelper.o ../ircDDB/libircDDB.a ../Common/Common.a $(LIBS)

ircddbgatewayd:	IRCDDBGatewayAppD.o IRCDDBGatewayStatusData.o IRCDDBGatewayThread.o ../ircDDB/libircDDB.a ../Common/Common.a
				$(CC) $(LDFLAGS) -o ircddbgatewayd IRCDDBGatewayAppD.o IRCDDBGatewayStatusData.o IRCDDBGatewayThread.o ../ircDDB/libircDDB.a ../Common/Common.a $(LIBS)

IRCDDBGatewayApp.o:		IRCDDBGatewayApp.cpp IRCDDBGatewayApp.h IRCDDBGatewayThread.h IRCDDBGatewayFrame.h IRCDDBGatewayLogRedirect.h IRCDDBGatewayStatusData.h \
				IRCDDBGatewayDefs.h ../Common/Defs.h ../Common/RepeaterProtocolHandler.h ../Common/HBRepeaterProtocolHandler.h IRCDDBGatewayThreadHelper.h \
				../Common/IcomRepeaterProtocolHandler.h ../Common/APRSWriter.h ../Common/Logger.h ../Common/Version.h ../Common/DummyRepeaterProtocolHandler.h \
				../Common/Utils.h ../Common/IRCDDBGatewayConfig.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayApp.cpp

IRCDDBGatewayAppD.o:	IRCDDBGatewayAppD.cpp IRCDDBGatewayAppD.h IRCDDBGatewayThread.h IRCDDBGatewayDefs.h ../Common/Defs.h ../Common/RepeaterProtocolHandler.h \
				../Common/HBRepeaterProtocolHandler.h ../Common/IcomRepeaterProtocolHandler.h ../Common/Logger.h ../Common/APRSWriter.h ../Common/Version.h \
				../Common/DummyRepeaterProtocolHandler.h ../Common/Utils.h ../Common/IRCDDBGatewayConfig.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayAppD.cpp

IRCDDBGatewayFrame.o:	IRCDDBGatewayFrame.cpp IRCDDBGatewayFrame.h IRCDDBGatewayStatusData.h IRCDDBGatewayApp.h ../Common/Defs.h ../Common/Version.h \
				../Common/LogEvent.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayFrame.cpp

IRCDDBGatewayLogRedirect.o:	IRCDDBGatewayLogRedirect.cpp IRCDDBGatewayLogRedirect.h IRCDDBGatewayApp.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayLogRedirect.cpp

IRCDDBGatewayStatusData.o:	IRCDDBGatewayStatusData.cpp IRCDDBGatewayStatusData.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayStatusData.cpp

IRCDDBGatewayThreadHelper.o:		IRCDDBGatewayThreadHelper.cpp IRCDDBGatewayThreadHelper.h IRCDDBGatewayThread.h IRCDDBGatewayStatusData.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayThreadHelper.cpp

IRCDDBGatewayThread.o:		IRCDDBGatewayThread.cpp IRCDDBGatewayThread.h IRCDDBGatewayStatusData.h IRCDDBGatewayApp.h ../Common/Defs.h \
				../Common/CacheManager.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/HostFile.h ../Common/RepeaterProtocolHandler.h \
				../Common/DCSProtocolHandler.h ../Common/DExtraProtocolHandler.h ../Common/DPlusProtocolHandler.h ../Common/G2ProtocolHandler.h \
				../Common/DCSHandler.h ../Common/DExtraHandler.h ../Common/DPlusHandler.h ../Common/ConnectData.h ../Common/DStarDefines.h \
				../Common/Utils.h ../Common/Timer.h ../Common/G2Handler.h ../Common/RepeaterHandler.h ../Common/PollData.h ../Common/HeaderLogger.h \
				../Common/APRSWriter.h ../Common/AudioUnit.h ../Common/StarNetHandler.h ../Common/DDData.h ../Common/RemoteHandler.h ../Common/DDHandler.h \
				../Common/HBRepeaterProtocolHandler.h ../Common/IcomRepeaterProtocolHandler.h ../Common/StatusData.h ../Common/CallsignServer.h \
				../Common/HeardData.h ../Common/DCSProtocolHandlerPool.h ../Common/DPlusProtocolHandlerPool.h ../Common/DummyRepeaterProtocolHandler.h \
				../Common/CCSHandler.h ../Common/CCSData.h ../Common/CallsignList.h ../Common/DExtraProtocolHandlerPool.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayThread.cpp

install:		ircddbgateway ircddbgatewayd
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 ircddbgateway $(BINDIR)
				install -g bin -o root -m 0775 ircddbgatewayd $(BINDIR)

clean:
				rm -f core ircddbgateway ircddbgatewayd *.o *~ *.bak
