all:			dvapnode dvapnoded

CFLAGS :=		-I../Common $(CFLAGS)

dvapnode:	DVAPNodeApp.o DVAPNodeCallsignSet.o DVAPNodeConfig.o DVAPNodeFrame.o DVAPNodeLogger.o DVAPNodeDVAPSet.o DVAPNodePreferences.o \
			DVAPNodeStatusData.o DVAPNodeTRXThread.o DVAPNodeTXThread.o DVAPNodeRXThread.o DVAPNodeTimesSet.o DVAPNodeThreadHelper.o ../Common/Common.a
			$(CC) $(LDFLAGS) -o dvapnode DVAPNodeApp.o DVAPNodeCallsignSet.o DVAPNodeConfig.o DVAPNodeFrame.o DVAPNodeLogger.o DVAPNodeDVAPSet.o \
			DVAPNodePreferences.o DVAPNodeStatusData.o DVAPNodeTRXThread.o DVAPNodeTXThread.o DVAPNodeRXThread.o DVAPNodeTimesSet.o DVAPNodeThreadHelper.o \
			../Common/Common.a $(LIBS)

dvapnoded:	DVAPNodeD.o DVAPNodeConfig.o DVAPNodeTRXThread.o DVAPNodeTXThread.o DVAPNodeRXThread.o DVAPNodeStatusData.o ../Common/Common.a
			$(CC) $(LDFLAGS) -o dvapnoded DVAPNodeD.o DVAPNodeConfig.o DVAPNodeTRXThread.o DVAPNodeTXThread.o DVAPNodeRXThread.o DVAPNodeStatusData.o \
			../Common/Common.a $(LIBS)

DVAPNodeApp.o:	DVAPNodeApp.cpp DVAPNodeApp.h DVAPNodeThread.h DVAPNodeTRXThread.h DVAPNodeTXThread.h DVAPNodeRXThread.h DVAPNodeFrame.h DVAPNodeDefs.h \
				DVAPNodeStatusData.h DVAPNodeLogger.h DVAPNodeConfig.h DVAPNodeThreadHelper.h ../Common/DStarDefines.h ../Common/Logger.h \
				../Common/RepeaterProtocolHandler.h ../Common/DVAPController.h ../Common/Utils.h ../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c DVAPNodeApp.cpp

DVAPNodeCallsignSet.o:	DVAPNodeCallsignSet.cpp DVAPNodeCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVAPNodeCallsignSet.cpp

DVAPNodeConfig.o:	DVAPNodeConfig.cpp DVAPNodeConfig.h DVAPNodeDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVAPNodeConfig.cpp

DVAPNodeD.o:	DVAPNodeD.cpp DVAPNodeD.h DVAPNodeThread.h DVAPNodeTRXThread.h DVAPNodeTXThread.h DVAPNodeRXThread.h DVAPNodeConfig.h DVAPNodeDefs.h \
				../Common/DStarDefines.h ../Common/Logger.h ../Common/RepeaterProtocolHandler.h ../Common/DVAPController.h ../Common/Utils.h \
				../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c DVAPNodeD.cpp

DVAPNodeDVAPSet.o:	DVAPNodeDVAPSet.cpp DVAPNodeDVAPSet.h DVAPNodeDefs.h ../Common/DStarDefines.h ../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DVAPNodeDVAPSet.cpp

DVAPNodeFrame.o:	DVAPNodeFrame.cpp DVAPNodeFrame.h DVAPNodeDefs.h DVAPNodeApp.h DVAPNodeStatusData.h DVAPNodePreferences.h \
					../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVAPNodeFrame.cpp

DVAPNodeLogger.o:	DVAPNodeLogger.cpp DVAPNodeLogger.h DVAPNodeApp.h
				$(CC) $(CFLAGS) -c DVAPNodeLogger.cpp

DVAPNodePreferences.o:	DVAPNodePreferences.cpp DVAPNodePreferences.h DVAPNodeDefs.h DVAPNodeDVAPSet.h DVAPNodeApp.h DVAPNodeTimesSet.h \
						DVAPNodeCallsignSet.h ../Common/NetworkSet.h ../Common/BeaconSet.h ../Common/AnnouncementSet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVAPNodePreferences.cpp

DVAPNodeStatusData.o:	DVAPNodeStatusData.cpp DVAPNodeStatusData.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVAPNodeStatusData.cpp

DVAPNodeThreadHelper.o:	DVAPNodeThreadHelper.cpp DVAPNodeThreadHelper.h DVAPNodeThread.h DVAPNodeStatusData.h
				$(CC) $(CFLAGS) -c DVAPNodeThreadHelper.cpp

DVAPNodeTRXThread.o:	DVAPNodeTRXThread.cpp DVAPNodeTRXThread.h DVAPNodeThread.h DVAPNodeDefs.h DVAPNodeApp.h DVAPNodeStatusData.h ../Common/Utils.h \
					../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Timer.h ../Common/DVAPController.h \
					../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/OutputQueue.h ../Common/Version.h \
					../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/BeaconUnit.h ../Common/BeaconCallback.h ../Common/AnnouncementUnit.h \
					../Common/AnnouncementCallback.h
				$(CC) $(CFLAGS) -c DVAPNodeTRXThread.cpp

DVAPNodeTXThread.o:	DVAPNodeTXThread.cpp DVAPNodeTXThread.h DVAPNodeThread.h DVAPNodeDefs.h DVAPNodeApp.h DVAPNodeStatusData.h ../Common/Utils.h \
					../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Timer.h ../Common/DVAPController.h \
					../Common/RepeaterProtocolHandler.h ../Common/OutputQueue.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVAPNodeTXThread.cpp

DVAPNodeRXThread.o:	DVAPNodeRXThread.cpp DVAPNodeRXThread.h DVAPNodeThread.h DVAPNodeDefs.h DVAPNodeApp.h DVAPNodeStatusData.h ../Common/Utils.h \
					../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Timer.h ../Common/DVAPController.h \
					../Common/RepeaterProtocolHandler.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVAPNodeRXThread.cpp

DVAPNodeTimesSet.o:	DVAPNodeTimesSet.cpp DVAPNodeTimesSet.h
				$(CC) $(CFLAGS) -c DVAPNodeTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dvapnode $(BINDIR)
				install -g bin -o root -m 0775 dvapnoded $(BINDIR)

clean:
				rm -f core dvapnode dvapnoded *.o *~ *.bak

