all:			DExtraBridge

CFLAGS :=		-I../DVCommon $(CFLAGS)

OBJS :=			DExtraClientApp.o DExtraClientCallsignSet.o DExtraClientControllerSet.o DExtraClientFrame.o DExtraClientHostFile.o DExtraClientPanel.o \
				DExtraClientPreferences.o DExtraClientReflectorSet.o DExtraClientThread.o

DExtraBridge:	$(OBJS)
				$(CC) $(LDFLAGS) -o DExtraBridge $(OBJS) ../DVCommon/Common.a $(LIBS)

DExtraClientApp.o:	DExtraClientApp.cpp DExtraClientApp.h DExtraClientThread.h DExtraClientFrame.h DExtraClientDefs.h ../DVCommon/SerialController.h \
				../DVCommon/DStarDefines.h ../DVCommon/MessageData.h ../DVCommon/DVDongleSet.h ../DVCommon/HeaderData.h ../DVCommon/BusyData.h ../DVCommon/Logger.h \
				../DVCommon/Version.h ../DVCommon/ExternalController.h ../DVCommon/URIUSBController.h ../DVCommon/K8055Controller.h ../DVCommon/DummyController.h \
				../DVCommon/SerialDataController.h
				$(CC) $(CFLAGS) -c DExtraClientApp.cpp

DExtraClientCallsignSet.o:	DExtraClientCallsignSet.cpp DExtraClientCallsignSet.h ../DVCommon/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DExtraClientCallsignSet.cpp

DExtraClientControllerSet.o:	DExtraClientControllerSet.cpp DExtraClientControllerSet.h ../DVCommon/SerialController.h
				$(CC) $(CFLAGS) -c DExtraClientControllerSet.cpp

DExtraClientFrame.o:	DExtraClientFrame.cpp DExtraClientFrame.h DExtraClientPreferences.h DExtraClientDefs.h DExtraClientApp.h DExtraClientPanel.h \
				DExtraClient.xpm ../DVCommon/CallsignTextCtrl.h ../DVCommon/MessageEvent.h ../DVCommon/DStarDefines.h ../DVCommon/MessageEvent.h \
				../DVCommon/HeaderEvent.h ../DVCommon/MessageData.h ../DVCommon/ErrorEvent.h ../DVCommon/HeaderData.h ../DVCommon/BusyEvent.h ../DVCommon/BusyData.h \
				../DVCommon/Version.h
				$(CC) $(CFLAGS) -c DExtraClientFrame.cpp

DExtraClientHostFile.o:	DExtraClientHostFile.cpp DExtraClientHostFile.h
				$(CC) $(CFLAGS) -c DExtraClientHostFile.cpp

DExtraClientPanel.o:	DExtraClientPanel.cpp DExtraClientPanel.h DExtraClientDefs.h DExtraClientApp.h DExtraClientHostFile.h ../DVCommon/DStarDefines.h \
				../DVCommon/MessageData.h ../DVCommon/HeaderData.h ../DVCommon/BusyData.h
				$(CC) $(CFLAGS) -c DExtraClientPanel.cpp

DExtraClientPreferences.o:	DExtraClientPreferences.cpp DExtraClientPreferences.h DExtraClientCallsignSet.h DExtraClientControllerSet.h DExtraClientDefs.h \
				DExtraClientReflectorSet.h ../DVCommon/SoundcardSet.h ../DVCommon/DVDongleSet.h ../DVCommon/MessageSet.h ../DVCommon/BleepSet.h
				$(CC) $(CFLAGS) -c DExtraClientPreferences.cpp

DExtraClientReflectorSet.o:	DExtraClientReflectorSet.cpp DExtraClientReflectorSet.h DExtraClientHostFile.h DExtraClientDefs.h
				$(CC) $(CFLAGS) -c DExtraClientReflectorSet.cpp

DExtraClientThread.o:	DExtraClientThread.cpp DExtraClientThread.h DExtraClientDefs.h DExtraClientApp.h ../DVCommon/DStarDefines.h \
				../DVCommon/SoundCardReaderWriter.h ../DVCommon/DVDongleController.h ../DVCommon/DVDongleThread.h ../DVCommon/DVTOOLFileWriter.h \
				../DVCommon/SlowDataEncoder.h ../DVCommon/SlowDataDecoder.h ../DVCommon/AudioCallback.h ../DVCommon/MessageData.h ../DVCommon/DExtraProtocolHandler.h \
				../DVCommon/Timer.h ../DVCommon/RingBuffer.h ../DVCommon/HeaderData.h ../DVCommon/BusyData.h ../DVCommon/ExternalController.h ../DVCommon/AudioDelay.h
				$(CC) $(CFLAGS) -c DExtraClientThread.cpp

install:		DExtraBridge
				install -g bin -o root -m 0775 DExtraBridge     $(BINDIR)
				install -g bin -o root -m 0664 DExtra_Hosts.txt $(DATADIR)

clean:
				rm -f core DExtraBridge *.o *~ *.bak
