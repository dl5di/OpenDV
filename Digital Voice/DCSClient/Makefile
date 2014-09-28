all:			DCSClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DCSClientApp.o DCSClientCallsignSet.o DCSClientControllerSet.o DCSClientFrame.o DCSClientHostFile.o DCSClientPanel.o \
				DCSClientPreferences.o DCSClientReflectorSet.o DCSClientThread.o

DCSClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o DCSClient $(OBJS) ../Common/Common.a $(LIBS)

DCSClientApp.o:	DCSClientApp.cpp DCSClientApp.h DCSClientThread.h DCSClientFrame.h DCSClientDefs.h ../Common/SerialController.h \
				../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h ../Common/BusyData.h ../Common/Logger.h \
				../Common/Version.h ../Common/ExternalController.h ../Common/URIUSBController.h ../Common/K8055Controller.h ../Common/DummyController.h \
				../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DCSClientApp.cpp

DCSClientCallsignSet.o:	DCSClientCallsignSet.cpp DCSClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DCSClientCallsignSet.cpp

DCSClientControllerSet.o:	DCSClientControllerSet.cpp DCSClientControllerSet.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c DCSClientControllerSet.cpp

DCSClientFrame.o:	DCSClientFrame.cpp DCSClientFrame.h DCSClientPreferences.h DCSClientDefs.h DCSClientApp.h DCSClientPanel.h \
				DCSClient.xpm ../Common/CallsignTextCtrl.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/MessageEvent.h \
				../Common/HeaderEvent.h ../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h \
				../Common/Version.h
				$(CC) $(CFLAGS) -c DCSClientFrame.cpp

DCSClientHostFile.o:	DCSClientHostFile.cpp DCSClientHostFile.h
				$(CC) $(CFLAGS) -c DCSClientHostFile.cpp

DCSClientPanel.o:	DCSClientPanel.cpp DCSClientPanel.h DCSClientDefs.h DCSClientApp.h DCSClientHostFile.h ../Common/DStarDefines.h \
				../Common/MessageData.h ../Common/HeaderData.h ../Common/BusyData.h
				$(CC) $(CFLAGS) -c DCSClientPanel.cpp

DCSClientPreferences.o:	DCSClientPreferences.cpp DCSClientPreferences.h DCSClientCallsignSet.h DCSClientControllerSet.h DCSClientDefs.h \
				DCSClientReflectorSet.h ../Common/SoundcardSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DCSClientPreferences.cpp

DCSClientReflectorSet.o:	DCSClientReflectorSet.cpp DCSClientReflectorSet.h DCSClientHostFile.h DCSClientDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DCSClientReflectorSet.cpp

DCSClientThread.o:	DCSClientThread.cpp DCSClientThread.h DCSClientDefs.h DCSClientApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/DVDongleController.h ../Common/DVDongleThread.h ../Common/DVTOOLFileWriter.h \
				../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/AudioCallback.h ../Common/MessageData.h ../Common/DCSProtocolHandler.h \
				../Common/Timer.h ../Common/RingBuffer.h ../Common/HeaderData.h ../Common/BusyData.h ../Common/ExternalController.h ../Common/AudioDelay.h
				$(CC) $(CFLAGS) -c DCSClientThread.cpp

install:		DCSClient
				install -g bin -o root -m 0775 DCSClient     $(BINDIR)
				install -g bin -o root -m 0664 DCS_Hosts.txt $(DATADIR)

clean:
				rm -f core DCSClient *.o *~ *.bak
