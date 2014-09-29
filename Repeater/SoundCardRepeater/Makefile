all:			soundcardrepeater soundcardrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

soundcardrepeater:	SoundCardRepeaterApp.o SoundCardRepeaterCallsignSet.o SoundCardRepeaterConfig.o SoundCardRepeaterFrame.o SoundCardRepeaterControllerSet.o \
				SoundCardRepeaterLogger.o SoundCardRepeaterPreferences.o SoundCardRepeaterRadioSet.o SoundCardRepeaterStatusData.o SoundCardRepeaterTRXThread.o \
				SoundCardRepeaterTXRXThread.o SoundCardRepeaterTXThread.o SoundCardRepeaterRXThread.o SoundCardRepeaterTimesSet.o SoundCardRepeaterThreadHelper.o \
				../Common/Common.a
				$(CC) $(LDFLAGS) -o soundcardrepeater SoundCardRepeaterApp.o SoundCardRepeaterCallsignSet.o SoundCardRepeaterConfig.o SoundCardRepeaterControllerSet.o \
				SoundCardRepeaterFrame.o SoundCardRepeaterLogger.o SoundCardRepeaterPreferences.o SoundCardRepeaterRadioSet.o SoundCardRepeaterStatusData.o \
				SoundCardRepeaterTRXThread.o SoundCardRepeaterTXRXThread.o SoundCardRepeaterTXThread.o SoundCardRepeaterRXThread.o SoundCardRepeaterTimesSet.o \
				SoundCardRepeaterThreadHelper.o ../Common/Common.a $(LIBS)

soundcardrepeaterd:	SoundCardRepeaterD.o SoundCardRepeaterConfig.o SoundCardRepeaterTRXThread.o SoundCardRepeaterTXRXThread.o SoundCardRepeaterTXThread.o \
				SoundCardRepeaterRXThread.o SoundCardRepeaterStatusData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o soundcardrepeaterd SoundCardRepeaterD.o SoundCardRepeaterConfig.o SoundCardRepeaterTRXThread.o SoundCardRepeaterTXRXThread.o \
				SoundCardRepeaterTXThread.o SoundCardRepeaterRXThread.o SoundCardRepeaterStatusData.o ../Common/Common.a $(LIBS)

SoundCardRepeaterApp.o:	SoundCardRepeaterApp.cpp SoundCardRepeaterApp.h SoundCardRepeaterThread.h SoundCardRepeaterTRXThread.h SoundCardRepeaterTXRXThread.h \
					SoundCardRepeaterTXThread.h SoundCardRepeaterRXThread.h SoundCardRepeaterFrame.h SoundCardRepeaterDefs.h SoundCardRepeaterStatusData.h \
					SoundCardRepeaterLogger.h SoundCardRepeaterConfig.h SoundCardRepeaterThreadHelper.h ../Common/WAVFileReader.h ../Common/DStarDefines.h \
					../Common/Logger.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/URIUSBController.h ../Common/SerialLineController.h \
					../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h ../Common/RaspberryController.h ../Common/ArduinoController.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterApp.cpp

SoundCardRepeaterCallsignSet.o:	SoundCardRepeaterCallsignSet.cpp SoundCardRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterCallsignSet.cpp

SoundCardRepeaterConfig.o:	SoundCardRepeaterConfig.cpp SoundCardRepeaterConfig.h SoundCardRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterConfig.cpp

SoundCardRepeaterControllerSet.o:	SoundCardRepeaterControllerSet.cpp SoundCardRepeaterControllerSet.h ../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterControllerSet.cpp

SoundCardRepeaterD.o:	SoundCardRepeaterD.cpp SoundCardRepeaterD.h SoundCardRepeaterThread.h SoundCardRepeaterTRXThread.h SoundCardRepeaterTXRXThread.h SoundCardRepeaterTXThread.h \
					SoundCardRepeaterRXThread.h SoundCardRepeaterConfig.h SoundCardRepeaterDefs.h ../Common/DStarDefines.h ../Common/Logger.h \
					../Common/DummyController.h ../Common/K8055Controller.h ../Common/URIUSBController.h ../Common/SerialLineController.h ../Common/RaspberryController.h \
					../Common/ArduinoController.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterD.cpp

SoundCardRepeaterFrame.o:	SoundCardRepeaterFrame.cpp SoundCardRepeaterFrame.h SoundCardRepeaterDefs.h SoundCardRepeaterApp.h SoundCardRepeaterStatusData.h \
						SoundCardRepeaterPreferences.h ../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterFrame.cpp

SoundCardRepeaterLogger.o:	SoundCardRepeaterLogger.cpp SoundCardRepeaterLogger.h SoundCardRepeaterApp.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterLogger.cpp

SoundCardRepeaterPreferences.o:	SoundCardRepeaterPreferences.cpp SoundCardRepeaterPreferences.h SoundCardRepeaterDefs.h SoundCardRepeaterRadioSet.h SoundCardRepeaterApp.h \
							SoundCardRepeaterControllerSet.h SoundCardRepeaterTimesSet.h SoundCardRepeaterCallsignSet.h ../Common/NetworkSet.h \
							../Common/BeaconSet.h ../Common/AnnouncementSet.h ../Common/DStarDefines.h ../Common/ActiveHangSet.h ../Common/Control1Set.h ../Common/Control2Set.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterPreferences.cpp

SoundCardRepeaterRadioSet.o:	SoundCardRepeaterRadioSet.cpp SoundCardRepeaterRadioSet.h SoundCardRepeaterApp.h SoundCardRepeaterDefs.h ../Common/SoundCardReaderWriter.h \
							../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterRadioSet.cpp

SoundCardRepeaterStatusData.o:	SoundCardRepeaterStatusData.cpp SoundCardRepeaterStatusData.h SoundCardRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterStatusData.cpp

SoundCardRepeaterThreadHelper.o:	SoundCardRepeaterThreadHelper.cpp SoundCardRepeaterThreadHelper.h SoundCardRepeaterThread.h SoundCardRepeaterStatusData.h \
							../Common/WAVFileReader.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterThreadHelper.cpp

SoundCardRepeaterTRXThread.o:	SoundCardRepeaterTRXThread.cpp SoundCardRepeaterTRXThread.h SoundCardRepeaterThread.h SoundCardRepeaterDefs.h SoundCardRepeaterApp.h \
							SoundCardRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/LimitedLengthBuffer.h ../Common/DStarGMSKDemodulator.h ../Common/DStarGMSKModulator.h \
							../Common/RadioHeaderDecoder.h ../Common/RadioHeaderEncoder.h ../Common/PatternMatcher.h ../Common/HeaderData.h \
							../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/RingBuffer.h ../Common/Timer.h ../Common/ExternalController.h \
							../Common/PTTDelay.h ../Common/Utils.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataDecoder.h \
							../Common/SlowDataEncoder.h ../Common/AudioDelay.h ../Common/Version.h ../Common/Goertzel.h ../Common/DVTOOLFileWriter.h \
							../Common/CallsignList.h ../Common/BeaconUnit.h ../Common/BeaconCallback.h ../Common/AnnouncementUnit.h ../Common/AnnouncementCallback.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterTRXThread.cpp

SoundCardRepeaterTXRXThread.o:	SoundCardRepeaterTXRXThread.cpp SoundCardRepeaterTXRXThread.h SoundCardRepeaterThread.h SoundCardRepeaterDefs.h SoundCardRepeaterApp.h \
							SoundCardRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/LimitedLengthBuffer.h ../Common/DStarGMSKDemodulator.h ../Common/DStarGMSKModulator.h \
							../Common/RadioHeaderDecoder.h ../Common/RadioHeaderEncoder.h ../Common/PatternMatcher.h ../Common/HeaderData.h \
							../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/RingBuffer.h ../Common/Timer.h ../Common/ExternalController.h \
							../Common/PTTDelay.h ../Common/Utils.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataDecoder.h \
							../Common/SlowDataEncoder.h ../Common/AudioDelay.h ../Common/Version.h ../Common/Goertzel.h ../Common/DVTOOLFileWriter.h \
							../Common/CallsignList.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterTXRXThread.cpp

SoundCardRepeaterTXThread.o:	SoundCardRepeaterTXThread.cpp SoundCardRepeaterTXThread.h SoundCardRepeaterThread.h SoundCardRepeaterDefs.h SoundCardRepeaterApp.h \
							SoundCardRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/DStarGMSKModulator.h ../Common/RadioHeaderEncoder.h ../Common/HeaderData.h ../Common/AudioCallback.h \
							../Common/RingBuffer.h ../Common/Timer.h ../Common/ExternalController.h ../Common/PTTDelay.h ../Common/Utils.h \
							../Common/RepeaterProtocolHandler.h ../Common/AudioDelay.h ../Common/Version.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterTXThread.cpp

SoundCardRepeaterRXThread.o:	SoundCardRepeaterRXThread.cpp SoundCardRepeaterRXThread.h SoundCardRepeaterThread.h SoundCardRepeaterDefs.h SoundCardRepeaterApp.h \
							SoundCardRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/LimitedLengthBuffer.h ../Common/DStarGMSKDemodulator.h ../Common/RadioHeaderDecoder.h ../Common/PatternMatcher.h \
							../Common/HeaderData.h ../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/RingBuffer.h ../Common/Timer.h \
							../Common/ExternalController.h ../Common/Utils.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h \
							../Common/Goertzel.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterRXThread.cpp

SoundCardRepeaterTimesSet.o:	SoundCardRepeaterTimesSet.cpp SoundCardRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c SoundCardRepeaterTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 soundcardrepeater $(BINDIR)
				install -g bin -o root -m 0775 soundcardrepeaterd $(BINDIR)

clean:
				rm -f core soundcardrepeater soundcardrepeaterd *.o *~ *.bak

