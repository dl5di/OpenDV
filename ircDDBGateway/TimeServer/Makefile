all:			timeserver timeserverd

CFLAGS :=		-I../Common $(CFLAGS)

timeserver:		TimeServerAnnouncementsSet.o TimeServerApp.o TimeServerConfig.o TimeServerFrame.o TimeServerGatewaySet.o TimeServerLogRedirect.o \
				TimeServerPreferences.o TimeServerThread.o TimeServerThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o timeserver TimeServerAnnouncementsSet.o TimeServerApp.o TimeServerConfig.o TimeServerFrame.o TimeServerGatewaySet.o \
				TimeServerLogRedirect.o TimeServerPreferences.o TimeServerThread.o TimeServerThreadHelper.o ../Common/Common.a $(LIBS)

timeserverd:	TimeServerD.o TimeServerConfig.o TimeServerThread.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o timeserverd TimeServerD.o TimeServerConfig.o TimeServerThread.o ../Common/Common.a $(LIBS)

TimeServerAnnouncementsSet.o:	TimeServerAnnouncementsSet.cpp TimeServerAnnouncementsSet.h TimeServerDefs.h
				$(CC) $(CFLAGS) -c TimeServerAnnouncementsSet.cpp

TimeServerApp.o:	TimeServerApp.cpp TimeServerApp.h TimeServerThread.h TimeServerFrame.h TimeServerLogRedirect.h TimeServerDefs.h TimeServerConfig.h \
					TimeServerThreadHelper.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c TimeServerApp.cpp

TimeServerD.o:	TimeServerD.cpp TimeServerD.h TimeServerThread.h TimeServerDefs.h TimeServerConfig.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c TimeServerD.cpp

TimeServerConfig.o:	TimeServerConfig.cpp TimeServerConfig.h TimeServerDefs.h
				$(CC) $(CFLAGS) -c TimeServerConfig.cpp

TimeServerFrame.o:	TimeServerFrame.cpp TimeServerFrame.h TimeServerPreferences.h TimeServerDefs.h TimeServerApp.h ../Common/Version.h
				$(CC) $(CFLAGS) -c TimeServerFrame.cpp

TimeServerGatewaySet.o:	TimeServerGatewaySet.cpp TimeServerGatewaySet.h TimeServerDefs.h ../Common/AddressTextCtrl.h ../Common/CallsignTextCtrl.h \
						../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c TimeServerGatewaySet.cpp

TimeServerLogRedirect.o:	TimeServerLogRedirect.cpp TimeServerLogRedirect.h TimeServerApp.h
				$(CC) $(CFLAGS) -c TimeServerLogRedirect.cpp

TimeServerPreferences.o:	TimeServerPreferences.cpp TimeServerPreferences.h TimeServerAnnouncementsSet.h TimeServerGatewaySet.h TimeServerDefs.h 
				$(CC) $(CFLAGS) -c TimeServerPreferences.cpp

TimeServerThreadHelper.o:	TimeServerThreadHelper.cpp TimeServerThreadHelper.h TimeServerThread.h
				$(CC) $(CFLAGS) -c TimeServerThreadHelper.cpp

TimeServerThread.o:	TimeServerThread.cpp TimeServerThread.h ../Common/DStarDefines.h ../Common/UDPReaderWriter.h ../Common/HeaderData.h \
					../Common/AMBEData.h ../Common/SlowDataEncoder.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c TimeServerThread.cpp

install:	timeserver timeserverd
			install -g bin -o root -m 0775 timeserver $(BINDIR)
			install -g bin -o root -m 0775 timeserverd $(BINDIR)

clean:
			rm -f core timeserver timeserverd *.o *~ *.bak
