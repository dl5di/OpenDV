all:			DExtraClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DExtraClientApp.o DExtraClientCallsignSet.o DExtraClientControllerSet.o DExtraClientFrame.o DExtraClientHostFile.o DExtraClientPanel.o \
				DExtraClientPreferences.o DExtraClientReflectorSet.o DExtraClientThread.o

DExtraClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o DExtraClient $(OBJS) ../Common/Common.a $(LIBS)

DExtraClientApp.o:	DExtraClientApp.cpp DExtraClientApp.h DExtraClientThread.h DExtraClientFrame.h DExtraClientDefs.h ../Common/SerialController.h \
				../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h ../Common/BusyData.h ../Common/Logger.h \
				../Common/Version.h ../Common/ExternalController.h ../Common/URIUSBController.h ../Common/K8055Controller.h ../Common/DummyController.h \
				../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DExtraClientApp.cpp

DExtraClientCallsignSet.o:	DExtraClientCallsignSet.cpp DExtraClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DExtraClientCallsignSet.cpp

DExtraClientControllerSet.o:	DExtraClientControllerSet.cpp DExtraClientControllerSet.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c DExtraClientControllerSet.cpp

DExtraClientFrame.o:	DExtraClientFrame.cpp DExtraClientFrame.h DExtraClientPreferences.h DExtraClientDefs.h DExtraClientApp.h DExtraClientPanel.h \
				DExtraClient.xpm ../Common/CallsignTextCtrl.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/MessageEvent.h \
				../Common/HeaderEvent.h ../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h \
				../Common/Version.h
				$(CC) $(CFLAGS) -c DExtraClientFrame.cpp

DExtraClientHostFile.o:	DExtraClientHostFile.cpp DExtraClientHostFile.h
				$(CC) $(CFLAGS) -c DExtraClientHostFile.cpp

DExtraClientPanel.o:	DExtraClientPanel.cpp DExtraClientPanel.h DExtraClientDefs.h DExtraClientApp.h DExtraClientHostFile.h ../Common/DStarDefines.h \
				../Common/MessageData.h ../Common/HeaderData.h ../Common/BusyData.h
				$(CC) $(CFLAGS) -c DExtraClientPanel.cpp

DExtraClientPreferences.o:	DExtraClientPreferences.cpp DExtraClientPreferences.h DExtraClientCallsignSet.h DExtraClientControllerSet.h DExtraClientDefs.h \
				DExtraClientReflectorSet.h ../Common/SoundcardSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DExtraClientPreferences.cpp

DExtraClientReflectorSet.o:	DExtraClientReflectorSet.cpp DExtraClientReflectorSet.h DExtraClientHostFile.h DExtraClientDefs.h
				$(CC) $(CFLAGS) -c DExtraClientReflectorSet.cpp

DExtraClientThread.o:	DExtraClientThread.cpp DExtraClientThread.h DExtraClientDefs.h DExtraClientApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/DVDongleController.h ../Common/DVDongleThread.h ../Common/DVTOOLFileWriter.h \
				../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/AudioCallback.h ../Common/MessageData.h ../Common/DExtraProtocolHandler.h \
				../Common/Timer.h ../Common/RingBuffer.h ../Common/HeaderData.h ../Common/BusyData.h ../Common/ExternalController.h ../Common/AudioDelay.h
				$(CC) $(CFLAGS) -c DExtraClientThread.cpp

install:		DExtraClient
				install -g bin -o root -m 0775 DExtraClient     $(BINDIR)
				install -g bin -o root -m 0664 DExtra_Hosts.txt $(DATADIR)

clean:
				rm -f core DExtraClient *.o *~ *.bak
