all:			dummyrepeater

CFLAGS :=		-I../Common $(CFLAGS)

dummyrepeater:	DummyRepeaterApp.o DummyRepeaterBleepSet.o DummyRepeaterCallsignSet.o DummyRepeaterControllerSet.o DummyRepeaterDongleSet.o \
				DummyRepeaterErrorEvent.o DummyRepeaterFrame.o DummyRepeaterHeaderEvent.o DummyRepeaterMessageEvent.o DummyRepeaterMessageSet.o DummyRepeaterNetworkSet.o \
				DummyRepeaterPreferences.o DummyRepeaterSlowDataEvent.o DummyRepeaterSoundcardSet.o DummyRepeaterStatusEvent.o DummyRepeaterThread.o \
				DummyRepeaterTimeoutSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dummyrepeater DummyRepeaterApp.o DummyRepeaterBleepSet.o DummyRepeaterCallsignSet.o DummyRepeaterControllerSet.o \
				DummyRepeaterDongleSet.o DummyRepeaterErrorEvent.o DummyRepeaterFrame.o DummyRepeaterHeaderEvent.o DummyRepeaterMessageEvent.o DummyRepeaterMessageSet.o \
				DummyRepeaterNetworkSet.o DummyRepeaterPreferences.o DummyRepeaterSlowDataEvent.o DummyRepeaterSoundcardSet.o DummyRepeaterStatusEvent.o \
				DummyRepeaterThread.o DummyRepeaterTimeoutSet.o ../Common/Common.a $(LIBS)

DummyRepeaterApp.o:	DummyRepeaterApp.cpp DummyRepeaterApp.h DummyRepeaterThread.h DummyRepeaterFrame.h DummyRepeaterDefs.h ../Common/MessageData.h \
					DummyRepeaterDongleSet.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/HeaderData.h ../Common/RepeaterProtocolHandler.h \
					../Common/SoundCardReaderWriter.h ../Common/SerialLineController.h ../Common/K8055Controller.h \
					../Common/URIUSBController.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/RaspberryController.h \
					../Common/ArduinoController.h ../Common/Version.h ../Common/DV3000Controller.h ../Common/AMBE3000Thread.h ../Common/DVDongleController.h \
					../Common/DVDongleThread.h
				$(CC) $(CFLAGS) -c DummyRepeaterApp.cpp

DummyRepeaterBleepSet.o:	DummyRepeaterBleepSet.cpp DummyRepeaterBleepSet.h
				$(CC) $(CFLAGS) -c DummyRepeaterBleepSet.cpp

DummyRepeaterCallsignSet.o:	DummyRepeaterCallsignSet.cpp DummyRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DummyRepeaterCallsignSet.cpp

DummyRepeaterControllerSet.o:	DummyRepeaterControllerSet.cpp DummyRepeaterControllerSet.h ../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c DummyRepeaterControllerSet.cpp

DummyRepeaterDongleSet.o:	DummyRepeaterDongleSet.cpp DummyRepeaterDongleSet.h DummyRepeaterDefs.h ../Common/SerialDataController.h \
						../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c DummyRepeaterDongleSet.cpp

DummyRepeaterErrorEvent.o:	DummyRepeaterErrorEvent.cpp DummyRepeaterErrorEvent.h
				$(CC) $(CFLAGS) -c DummyRepeaterErrorEvent.cpp

DummyRepeaterFrame.o:	DummyRepeaterFrame.cpp DummyRepeaterFrame.h DummyRepeaterDefs.h DummyRepeaterApp.h ../Common/MessageData.h \
						DummyRepeaterMessageEvent.h DummyRepeaterErrorEvent.h DummyRepeaterSlowDataEvent.h DummyRepeaterHeaderEvent.h \
						DummyRepeaterStatusEvent.h DummyRepeaterPreferences.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h \
						../Common/HeaderData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DummyRepeaterFrame.cpp

DummyRepeaterHeaderEvent.o:	DummyRepeaterHeaderEvent.cpp DummyRepeaterHeaderEvent.h ../Common/HeaderData.h
				$(CC) $(CFLAGS) -c DummyRepeaterHeaderEvent.cpp

DummyRepeaterMessageEvent.o:	DummyRepeaterMessageEvent.cpp DummyRepeaterMessageEvent.h ../Common/MessageData.h
				$(CC) $(CFLAGS) -c DummyRepeaterMessageEvent.cpp

DummyRepeaterMessageSet.o:	DummyRepeaterMessageSet.cpp DummyRepeaterMessageSet.h ../Common/MessageTextCtrl.h
				$(CC) $(CFLAGS) -c DummyRepeaterMessageSet.cpp

DummyRepeaterNetworkSet.o:	DummyRepeaterNetworkSet.cpp DummyRepeaterNetworkSet.h ../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
		$(CC) $(CFLAGS) -c DummyRepeaterNetworkSet.cpp

DummyRepeaterPreferences.o:	DummyRepeaterPreferences.cpp DummyRepeaterPreferences.h DummyRepeaterDefs.h DummyRepeaterSoundcardSet.h \
							DummyRepeaterCallsignSet.h DummyRepeaterControllerSet.h DummyRepeaterDongleSet.h DummyRepeaterMessageSet.h \
							DummyRepeaterTimeoutSet.h DummyRepeaterBleepSet.h DummyRepeaterNetworkSet.h
				$(CC) $(CFLAGS) -c DummyRepeaterPreferences.cpp

DummyRepeaterSlowDataEvent.o:	DummyRepeaterSlowDataEvent.cpp DummyRepeaterSlowDataEvent.h
				$(CC) $(CFLAGS) -c DummyRepeaterSlowDataEvent.cpp

DummyRepeaterSoundcardSet.o:	DummyRepeaterSoundcardSet.cpp DummyRepeaterSoundcardSet.h ../Common/SoundCardReaderWriter.h
				$(CC) $(CFLAGS) -c DummyRepeaterSoundcardSet.cpp

DummyRepeaterStatusEvent.o:	DummyRepeaterStatusEvent.cpp DummyRepeaterStatusEvent.h
				$(CC) $(CFLAGS) -c DummyRepeaterStatusEvent.cpp

DummyRepeaterThread.o:	DummyRepeaterThread.cpp DummyRepeaterThread.h DummyRepeaterDefs.h DummyRepeaterApp.h ../Common/MessageData.h \
						../Common/SlowDataDecoder.h ../Common/SlowDataEncoder.h ../Common/DongleThread.h ../Common/DStarDefines.h \
						../Common/SoundCardReaderWriter.h ../Common/HeaderData.h ../Common/AudioCallback.h \
						../Common/RingBuffer.h ../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/ExternalController.h \
						../Common/Version.h ../Common/EncodeCallback.h ../Common/DecodeCallback.h
				$(CC) $(CFLAGS) -c DummyRepeaterThread.cpp

DummyRepeaterTimeoutSet.o:	DummyRepeaterTimeoutSet.cpp DummyRepeaterTimeoutSet.h
				$(CC) $(CFLAGS) -c DummyRepeaterTimeoutSet.cpp

install:	all
				install -g bin -o root -m 0775 dummyrepeater $(BINDIR)

clean:
				rm -f core dummyrepeater *.o *~ *.bak
