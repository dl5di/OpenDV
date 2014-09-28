all:			DVAPClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DVAPClientApp.o DVAPClientCallsignSet.o DVAPClientModemSet.o DVAPClientFrame.o DVAPClientPreferences.o DVAPClientThread.o

DVAPClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o DVAPClient $(OBJS) ../Common/Common.a $(LIBS)

DVAPClientApp.o:	DVAPClientApp.cpp DVAPClientApp.h DVAPClientThread.h DVAPClientFrame.h DVAPClientDefs.h ../Common/SerialDataController.h \
				../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h ../Common/BusyData.h \
				../Common/Logger.h ../Common/Version.h ../Common/DVAPController.h
				$(CC) $(CFLAGS) -c DVAPClientApp.cpp

DVAPClientCallsignSet.o:	DVAPClientCallsignSet.cpp DVAPClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DVAPClientCallsignSet.cpp

DVAPClientFrame.o:	DVAPClientFrame.cpp DVAPClientFrame.h DVAPClientPreferences.h DVAPClientDefs.h DVAPClientApp.h DVAPClient.xpm \
				../Common/CallsignTextCtrl.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVAPClientFrame.cpp

DVAPClientModemSet.o:	DVAPClientModemSet.cpp DVAPClientModemSet.h DVAPClientDefs.h ../Common/DStarDefines.h ../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DVAPClientModemSet.cpp

DVAPClientPreferences.o:	DVAPClientPreferences.cpp DVAPClientPreferences.h DVAPClientCallsignSet.h DVAPClientModemSet.h DVAPClientDefs.h \
				../Common/SoundcardSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h  ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DVAPClientPreferences.cpp

DVAPClientThread.o:	DVAPClientThread.cpp DVAPClientThread.h DVAPClientDefs.h DVAPClientApp.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
				../Common/DVAPController.h ../Common/DVDongleController.h ../Common/DVTOOLFileWriter.h ../Common/SlowDataEncoder.h \
				../Common/SlowDataDecoder.h ../Common/AudioCallback.h ../Common/MessageData.h ../Common/RingBuffer.h ../Common/HeaderData.h \
				../Common/BusyData.h ../Common/Bleeper.h ../Common/DVDongleThread.h ../Common/Timer.h
				$(CC) $(CFLAGS) -c DVAPClientThread.cpp

install:		DVAPClient
				install -g bin -o root -m 0775 DVAPClient $(BINDIR)

clean:
				rm -f core DVAPClient *.o *~ *.bak
