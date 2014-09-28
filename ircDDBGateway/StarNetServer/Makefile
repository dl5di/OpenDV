all:			starnetserver starnetserverd

CFLAGS :=		-I../ircDDB -I../Common $(CFLAGS)

starnetserver:	StarNetServerApp.o StarNetServerCallsignSet.o StarNetServerConfig.o StarNetServerFrame.o StarNetServerIrcDDBSet.o StarNetServerLogRedirect.o \
				StarNetServerMiscellaneousSet.o StarNetServerPreferences.o StarNetServerThread.o StarNetServerThreadHelper.o ../ircDDB/libircDDB.a ../Common/Common.a
				$(CC) $(LDFLAGS) -o starnetserver StarNetServerApp.o StarNetServerCallsignSet.o StarNetServerConfig.o StarNetServerFrame.o StarNetServerIrcDDBSet.o \
				StarNetServerLogRedirect.o StarNetServerMiscellaneousSet.o StarNetServerPreferences.o StarNetServerThread.o StarNetServerThreadHelper.o \
				../ircDDB/libircDDB.a ../Common/Common.a $(LIBS)

starnetserverd:	StarNetServerAppD.o StarNetServerConfig.o StarNetServerThread.o ../ircDDB/libircDDB.a ../Common/Common.a
				$(CC) $(LDFLAGS) -o starnetserverd StarNetServerAppD.o StarNetServerConfig.o StarNetServerThread.o ../ircDDB/libircDDB.a ../Common/Common.a \
				$(LIBS)

StarNetServerApp.o:		StarNetServerApp.cpp StarNetServerApp.h StarNetServerThread.h StarNetServerFrame.h StarNetServerConfig.h StarNetServerLogRedirect.h \
						StarNetServerDefs.h StarNetServerThreadHelper.h ../Common/Defs.h ../Common/Logger.h ../Common/Version.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c StarNetServerApp.cpp

StarNetServerAppD.o:	StarNetServerAppD.cpp StarNetServerAppD.h StarNetServerThread.h StarNetServerConfig.h StarNetServerDefs.h ../Common/Defs.h \
						../Common/Logger.h ../Common/Version.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c StarNetServerAppD.cpp

StarNetServerConfig.o:		StarNetServerConfig.cpp StarNetServerConfig.h StarNetServerDefs.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c StarNetServerConfig.cpp

StarNetServerFrame.o:	StarNetServerFrame.cpp StarNetServerFrame.h StarNetServerPreferences.h StarNetServerApp.h ../Common/Version.h \
						../Common/LogEvent.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c StarNetServerFrame.cpp

StarNetServerCallsignSet.o:	StarNetServerCallsignSet.cpp StarNetServerCallsignSet.h ../Common/DStarDefines.h ../Common/CallsignTextCtrl.h \
							../Common/AddressTextCtrl.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c StarNetServerCallsignSet.cpp

StarNetServerIrcDDBSet.o:	StarNetServerIrcDDBSet.cpp StarNetServerIrcDDBSet.h
				$(CC) $(CFLAGS) -c StarNetServerIrcDDBSet.cpp

StarNetServerLogRedirect.o:	StarNetServerLogRedirect.cpp StarNetServerLogRedirect.h StarNetServerApp.h
				$(CC) $(CFLAGS) -c StarNetServerLogRedirect.cpp

StarNetServerMiscellaneousSet.o:	StarNetServerMiscellaneousSet.cpp StarNetServerMiscellaneousSet.h ../Common/Defs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c StarNetServerMiscellaneousSet.cpp

StarNetServerPreferences.o:	StarNetServerPreferences.cpp StarNetServerPreferences.h StarNetServerMiscellaneousSet.h StarNetServerCallsignSet.h \
							StarNetServerIrcDDBSet.h ../Common/Defs.h ../Common/RemoteSet.h ../Common/StarNetSet.h
				$(CC) $(CFLAGS) -c StarNetServerPreferences.cpp

StarNetServerThreadHelper.o:		StarNetServerThreadHelper.cpp StarNetServerThreadHelper.h StarNetServerThread.h
				$(CC) $(CFLAGS) -c StarNetServerThreadHelper.cpp

StarNetServerThread.o:		StarNetServerThread.cpp StarNetServerThread.h StarNetServerApp.h ../Common/Defs.h ../Common/CacheManager.h \
							../Common/HeaderData.h ../Common/AMBEData.h ../Common/G2ProtocolHandler.h ../Common/DStarDefines.h ../Common/Utils.h \
							../Common/Timer.h ../Common/G2Handler.h ../Common/HeaderLogger.h ../Common/StarNetHandler.h ../Common/RemoteHandler.h \
							../Common/DExtraProtocolHandlerPool.h ../Common/DCSProtocolHandlerPool.h ../ircDDB/IRCDDB.h
				$(CC) $(CFLAGS) -c StarNetServerThread.cpp

install:	starnetserver starnetserverd
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 starnetserver $(BINDIR)
				install -g bin -o root -m 0775 starnetserverd $(BINDIR)

clean:
				rm -f core starnetserver starnetserverd *.o *~ *.bak

