all:			analoguerepeater analoguerepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

analoguerepeater:	AnalogueRepeaterAckSet.o AnalogueRepeaterApp.o AnalogueRepeaterAPRSSet.o AnalogueRepeaterCallsignSet.o AnalogueRepeaterConfig.o \
					AnalogueRepeaterControllerSet.o AnalogueRepeaterDTMF1Set.o AnalogueRepeaterDTMF2Set.o AnalogueRepeaterExternalSet.o AnalogueRepeaterFeelSet.o \
					AnalogueRepeaterFrame.o AnalogueRepeaterLogger.o AnalogueRepeaterPreferences.o AnalogueRepeaterRadioSet.o AnalogueRepeaterStatusData.o \
					AnalogueRepeaterThread.o AnalogueRepeaterTimesSet.o AnalogueRepeaterTonesSet.o AnalogueRepeaterThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o analoguerepeater AnalogueRepeaterAckSet.o AnalogueRepeaterApp.o AnalogueRepeaterAPRSSet.o AnalogueRepeaterCallsignSet.o \
				AnalogueRepeaterConfig.o AnalogueRepeaterControllerSet.o AnalogueRepeaterDTMF1Set.o AnalogueRepeaterDTMF2Set.o AnalogueRepeaterExternalSet.o \
				AnalogueRepeaterFeelSet.o AnalogueRepeaterFrame.o AnalogueRepeaterLogger.o AnalogueRepeaterPreferences.o AnalogueRepeaterStatusData.o \
				AnalogueRepeaterRadioSet.o AnalogueRepeaterThread.o AnalogueRepeaterTimesSet.o AnalogueRepeaterTonesSet.o AnalogueRepeaterThreadHelper.o ../Common/Common.a $(LIBS)

analoguerepeaterd:	AnalogueRepeaterD.o AnalogueRepeaterConfig.o AnalogueRepeaterThread.o AnalogueRepeaterStatusData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o analoguerepeaterd AnalogueRepeaterD.o AnalogueRepeaterConfig.o AnalogueRepeaterThread.o AnalogueRepeaterStatusData.o \
				../Common/Common.a $(LIBS)

AnalogueRepeaterApp.o:	AnalogueRepeaterApp.cpp AnalogueRepeaterApp.h AnalogueRepeaterThread.h AnalogueRepeaterFrame.h AnalogueRepeaterDefs.h \
						AnalogueRepeaterStatusData.h AnalogueRepeaterLogger.h AnalogueRepeaterConfig.h AnalogueRepeaterThreadHelper.h ../Common/ExternalController.h \
						../Common/DummyController.h ../Common/K8055Controller.h ../Common/SerialLineController.h ../Common/RaspberryController.h \
						../Common/ArduinoController.h ../Common/Logger.h ../Common/AnalogueDefines.h ../Common/Version.h ../Common/CWKeyer.h ../Common/WAVFileStore.h \
						../Common/APRSTX.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterApp.cpp

AnalogueRepeaterD.o:	AnalogueRepeaterD.cpp AnalogueRepeaterD.h AnalogueRepeaterThread.h AnalogueRepeaterConfig.h AnalogueRepeaterDefs.h \
						../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/RaspberryController.h \
						../Common/SerialLineController.h ../Common/ArduinoController.h ../Common/Logger.h ../Common/AnalogueDefines.h ../Common/Version.h ../Common/CWKeyer.h \
						../Common/WAVFileStore.h ../Common/APRSTX.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterD.cpp

AnalogueRepeaterAckSet.o:	AnalogueRepeaterAckSet.cpp AnalogueRepeaterAckSet.h AnalogueRepeaterApp.h ../Common/CWTextCtrl.h \
							../Common/AnalogueDefines.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterAckSet.cpp

AnalogueRepeaterAPRSSet.o:	AnalogueRepeaterAPRSSet.cpp AnalogueRepeaterAPRSSet.h ../Common/CallsignTextCtrl.h ../Common/AnalogueDefines.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterAPRSSet.cpp

AnalogueRepeaterCallsignSet.o:	AnalogueRepeaterCallsignSet.cpp AnalogueRepeaterCallsignSet.h AnalogueRepeaterApp.h ../Common/CWTextCtrl.h \
								../Common/AnalogueDefines.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterCallsignSet.cpp

AnalogueRepeaterConfig.o:	AnalogueRepeaterConfig.cpp AnalogueRepeaterConfig.h AnalogueRepeaterDefs.h ../Common/AnalogueDefines.h \
							../Common/TimeoutTones.h ../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterConfig.cpp

AnalogueRepeaterControllerSet.o:	AnalogueRepeaterControllerSet.cpp AnalogueRepeaterControllerSet.h ../Common/SerialDataController.h \
							../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterControllerSet.cpp

AnalogueRepeaterDTMF1Set.o:	AnalogueRepeaterDTMF1Set.cpp AnalogueRepeaterDTMF1Set.h AnalogueRepeaterApp.h ../Common/DTMFTextCtrl.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterDTMF1Set.cpp

AnalogueRepeaterDTMF2Set.o:	AnalogueRepeaterDTMF2Set.cpp AnalogueRepeaterDTMF2Set.h ../Common/DTMFTextCtrl.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterDTMF2Set.cpp

AnalogueRepeaterExternalSet.o:	AnalogueRepeaterExternalSet.cpp AnalogueRepeaterExternalSet.h ../Common/AnalogueDefines.h ../Common/SoundCardReaderWriter.h \
								../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterExternalSet.cpp

AnalogueRepeaterFeelSet.o:	AnalogueRepeaterFeelSet.cpp AnalogueRepeaterFeelSet.h ../Common/AnalogueDefines.h ../Common/TimeoutTones.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterFeelSet.cpp

AnalogueRepeaterFrame.o:	AnalogueRepeaterFrame.cpp AnalogueRepeaterFrame.h AnalogueRepeaterApp.h AnalogueRepeaterPreferences.h AnalogueRepeaterDefs.h \
							AnalogueRepeaterStatusData.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterFrame.cpp

AnalogueRepeaterLogger.o:	AnalogueRepeaterLogger.cpp AnalogueRepeaterLogger.h AnalogueRepeaterApp.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterLogger.cpp

AnalogueRepeaterPreferences.o:	AnalogueRepeaterPreferences.cpp AnalogueRepeaterPreferences.h AnalogueRepeaterRadioSet.h AnalogueRepeaterDTMF1Set.h \
								AnalogueRepeaterDTMF2Set.h AnalogueRepeaterControllerSet.h AnalogueRepeaterCallsignSet.h AnalogueRepeaterExternalSet.h \
								AnalogueRepeaterTonesSet.h AnalogueRepeaterTimesSet.h AnalogueRepeaterFeelSet.h AnalogueRepeaterAckSet.h AnalogueRepeaterAPRSSet.h \
								../Common/ActiveHangSet.h ../Common/AnalogueDefines.h 
				$(CC) $(CFLAGS) -c AnalogueRepeaterPreferences.cpp

AnalogueRepeaterRadioSet.o:	AnalogueRepeaterRadioSet.cpp AnalogueRepeaterRadioSet.h ../Common/SoundCardReaderWriter.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterRadioSet.cpp

AnalogueRepeaterStatusData.o:	AnalogueRepeaterStatusData.cpp AnalogueRepeaterStatusData.h ../Common/AnalogueDefines.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterStatusData.cpp

AnalogueRepeaterThreadHelper.o:	AnalogueRepeaterThreadHelper.cpp AnalogueRepeaterThreadHelper.h AnalogueRepeaterThread.h AnalogueRepeaterStatusData.h \
							../Common/WAVFileReader.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterThreadHelper.cpp

AnalogueRepeaterThread.o:	AnalogueRepeaterThread.cpp AnalogueRepeaterThread.h AnalogueRepeaterDefs.h AnalogueRepeaterApp.h AnalogueRepeaterStatusData.h \
							../Common/FIRFilter.h ../Common/SoundCardReaderWriter.h ../Common/ExternalController.h ../Common/NetworkController.h \
							../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/TimeoutTones.h ../Common/RingBuffer.h ../Common/Goertzel.h \
							../Common/FixedAudioSource.h ../Common/Timer.h ../Common/NCO.h ../Common/VOGAD.h \
							../Common/FirstOrderIIR.h ../Common/AudioDelay.h ../Common/PTTDelay.h ../Common/AnalogueDefines.h \
							../Common/DTMFController.h ../Common/APRSTX.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterThread.cpp

AnalogueRepeaterTimesSet.o:	AnalogueRepeaterTimesSet.cpp AnalogueRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterTimesSet.cpp

AnalogueRepeaterTonesSet.o:	AnalogueRepeaterTonesSet.cpp AnalogueRepeaterTonesSet.h AnalogueRepeaterApp.h ../Common/AnalogueDefines.h
				$(CC) $(CFLAGS) -c AnalogueRepeaterTonesSet.cpp

install:	all
				install -g bin -o root -m 0775 analoguerepeater $(BINDIR)
				install -g bin -o root -m 0775 analoguerepeaterd $(BINDIR)

clean:
				rm -f core analoguerepeater analoguerepeaterd *.o *~ *.bak
