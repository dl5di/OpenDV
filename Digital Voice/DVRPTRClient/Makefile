all:			DVRPTRClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DVRPTRClientApp.o DVRPTRClientCallsignSet.o DVRPTRClientModemSet.o DVRPTRClientFrame.o DVRPTRClientPreferences.o DVRPTRClientThread.o

DVRPTRClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o DVRPTRClient $(OBJS) ../Common/Common.a $(LIBS)

DVRPTRClientApp.o:	DVRPTRClientApp.cpp DVRPTRClientApp.h DVRPTRClientThread.h DVRPTRClientFrame.h DVRPTRClientDefs.h ../Common/SerialDataController.h \
				../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h ../Common/BusyData.h \
				../Common/Logger.h ../Common/Version.h ../Common/DVRPTRController.h ../Common/DVRPTRControllerV1.h ../Common/DVRPTRControllerV2.h
				$(CC) $(CFLAGS) -c DVRPTRClientApp.cpp

DVRPTRClientCallsignSet.o:	DVRPTRClientCallsignSet.cpp DVRPTRClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DVRPTRClientCallsignSet.cpp

DVRPTRClientFrame.o:	DVRPTRClientFrame.cpp DVRPTRClientFrame.h DVRPTRClientPreferences.h DVRPTRClientDefs.h DVRPTRClientApp.h DVRPTRClient.xpm \
				../Common/CallsignTextCtrl.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVRPTRClientFrame.cpp

DVRPTRClientModemSet.o:	DVRPTRClientModemSet.cpp DVRPTRClientModemSet.h DVRPTRClientDefs.h ../Common/DStarDefines.h ../Common/DVRPTRControllerV1.h \
				../Common/DVRPTRControllerV2.h
				$(CC) $(CFLAGS) -c DVRPTRClientModemSet.cpp

DVRPTRClientPreferences.o:	DVRPTRClientPreferences.cpp DVRPTRClientPreferences.h DVRPTRClientCallsignSet.h DVRPTRClientModemSet.h DVRPTRClientDefs.h \
				../Common/SoundcardSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h  ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DVRPTRClientPreferences.cpp

DVRPTRClientThread.o:	DVRPTRClientThread.cpp DVRPTRClientThread.h DVRPTRClientDefs.h DVRPTRClientApp.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
				../Common/DVRPTRController.h ../Common/DVDongleController.h ../Common/DVTOOLFileWriter.h ../Common/SlowDataEncoder.h \
				../Common/SlowDataDecoder.h ../Common/AudioCallback.h ../Common/MessageData.h ../Common/RingBuffer.h ../Common/HeaderData.h \
				../Common/BusyData.h ../Common/Bleeper.h ../Common/DVDongleThread.h
				$(CC) $(CFLAGS) -c DVRPTRClientThread.cpp

install:		DVRPTRClient
				install -g bin -o root -m 0775 DVRPTRClient $(BINDIR)

clean:
				rm -f core DVRPTRClient *.o *~ *.bak
