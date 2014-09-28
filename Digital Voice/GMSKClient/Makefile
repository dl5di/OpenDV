all:			GMSKClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			GMSKClientApp.o GMSKClientCallsignSet.o GMSKClientFrame.o GMSKClientModemSet.o GMSKClientPreferences.o GMSKClientThread.o

GMSKClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o GMSKClient $(OBJS) ../Common/Common.a $(LIBS)

GMSKClientApp.o:	GMSKClientApp.cpp GMSKClientApp.h GMSKClientThread.h GMSKClientFrame.h GMSKClientDefs.h ../Common/SerialController.h \
				../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h ../Common/BusyData.h \
				../Common/Logger.h ../Common/Version.h ../Common/GMSKModem.h ../Common/GMSKModemLibUsb.h
				$(CC) $(CFLAGS) -c GMSKClientApp.cpp

GMSKClientCallsignSet.o:	GMSKClientCallsignSet.cpp GMSKClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c GMSKClientCallsignSet.cpp

GMSKClientFrame.o:	GMSKClientFrame.cpp GMSKClientFrame.h GMSKClientPreferences.h GMSKClientDefs.h GMSKClientApp.h GMSKClient.xpm \
				../Common/CallsignTextCtrl.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c GMSKClientFrame.cpp

GMSKClientModemSet.o:	GMSKClientModemSet.cpp GMSKClientModemSet.h GMSKClientDefs.h
				$(CC) $(CFLAGS) -c GMSKClientModemSet.cpp

GMSKClientPreferences.o:	GMSKClientPreferences.cpp GMSKClientPreferences.h GMSKClientCallsignSet.h GMSKClientModemSet.h GMSKClientDefs.h \
				../Common/SoundcardSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h  ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c GMSKClientPreferences.cpp

GMSKClientThread.o:	GMSKClientThread.cpp GMSKClientThread.h GMSKClientDefs.h GMSKClientApp.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
				../Common/GMSKModem.h ../Common/DVDongleController.h ../Common/DVTOOLFileWriter.h ../Common/SlowDataEncoder.h \
				../Common/SlowDataDecoder.h ../Common/AudioCallback.h ../Common/MessageData.h ../Common/RingBuffer.h ../Common/HeaderData.h \
				../Common/BusyData.h ../Common/Bleeper.h ../Common/DVDongleThread.h
				$(CC) $(CFLAGS) -c GMSKClientThread.cpp

install:		GMSKClient
				install -g bin -o root -m 0775 GMSKClient $(BINDIR)

clean:
				rm -f core GMSKClient *.o *~ *.bak
