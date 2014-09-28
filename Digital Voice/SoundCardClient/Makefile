all:			SoundCardClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			SoundCardClientApp.o SoundCardClientCallsignSet.o SoundCardClientControllerSet.o SoundCardClientFrame.o SoundCardClientPreferences.o SoundCardClientThread.o

SoundCardClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o SoundCardClient $(OBJS) ../Common/Common.a $(LIBS)

SoundCardClientApp.o:	SoundCardClientApp.cpp SoundCardClientApp.h SoundCardClientThread.h SoundCardClientFrame.h SoundCardClientDefs.h ../Common/SerialDataController.h \
				../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h \
				../Common/BusyData.h ../Common/Logger.h ../Common/Version.h ../Common/ExternalController.h ../Common/URIUSBController.h \
				../Common/K8055Controller.h ../Common/DummyController.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c SoundCardClientApp.cpp

SoundCardClientCallsignSet.o:	SoundCardClientCallsignSet.cpp SoundCardClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c SoundCardClientCallsignSet.cpp

SoundCardClientControllerSet.o:	SoundCardClientControllerSet.cpp SoundCardClientControllerSet.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c SoundCardClientControllerSet.cpp

SoundCardClientFrame.o:	SoundCardClientFrame.cpp SoundCardClientFrame.h SoundCardClientPreferences.h SoundCardClientDefs.h SoundCardClientApp.h SoundCardClient.xpm \
				../Common/CallsignTextCtrl.h ../Common/WAVFileReader.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c SoundCardClientFrame.cpp

SoundCardClientPreferences.o:	SoundCardClientPreferences.cpp SoundCardClientPreferences.h SoundCardClientCallsignSet.h SoundCardClientControllerSet.h SoundCardClientDefs.h ../Common/SoundcardSet.h \
				../Common/InversionSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h  ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c SoundCardClientPreferences.cpp

SoundCardClientThread.o:	SoundCardClientThread.cpp SoundCardClientThread.h SoundCardClientDefs.h SoundCardClientApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/LimitedPatternMatcher.h ../Common/LimitedLengthBuffer.h ../Common/DVDongleController.h \
				../Common/DStarGMSKDemodulator.h ../Common/DStarGMSKModulator.h ../Common/RadioHeaderDecoder.h ../Common/RadioHeaderEncoder.h \
				../Common/DVTOOLFileWriter.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/PatternMatcher.h \
				../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/WAVFileWriter.h ../Common/MessageData.h ../Common/RingBuffer.h \
				../Common/HeaderData.h ../Common/BusyData.h ../Common/Bleeper.h ../Common/DVDongleThread.h ../Common/ExternalController.h \
				../Common/PTTDelay.h
				$(CC) $(CFLAGS) -c SoundCardClientThread.cpp

install:		SoundCardClient
				install -g bin -o root -m 0775 SoundCardClient $(BINDIR)

clean:
				rm -f core SoundCardClient *.o *~ *.bak
