all:			dvrptrrepeater dvrptrrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

dvrptrrepeater:	DVRPTRRepeaterApp.o DVRPTRRepeaterCallsignSet.o DVRPTRRepeaterConfig.o DVRPTRRepeaterControllerSet.o DVRPTRRepeaterFrame.o \
				DVRPTRRepeaterLogger.o DVRPTRRepeaterModemSet.o DVRPTRRepeaterPreferences.o DVRPTRRepeaterStatusData.o DVRPTRRepeaterTRXThread.o \
				DVRPTRRepeaterTXRXThread.o DVRPTRRepeaterTXThread.o DVRPTRRepeaterRXThread.o DVRPTRRepeaterTimesSet.o DVRPTRRepeaterThreadHelper.o \
				../Common/Common.a
				$(CC) $(LDFLAGS) -o dvrptrrepeater DVRPTRRepeaterApp.o DVRPTRRepeaterCallsignSet.o DVRPTRRepeaterConfig.o DVRPTRRepeaterControllerSet.o \
				DVRPTRRepeaterFrame.o DVRPTRRepeaterLogger.o DVRPTRRepeaterModemSet.o DVRPTRRepeaterPreferences.o DVRPTRRepeaterStatusData.o \
				DVRPTRRepeaterTRXThread.o DVRPTRRepeaterTXRXThread.o DVRPTRRepeaterTXThread.o DVRPTRRepeaterRXThread.o DVRPTRRepeaterTimesSet.o \
				DVRPTRRepeaterThreadHelper.o ../Common/Common.a $(LIBS)

dvrptrrepeaterd:	DVRPTRRepeaterD.o DVRPTRRepeaterConfig.o DVRPTRRepeaterTRXThread.o DVRPTRRepeaterTXRXThread.o DVRPTRRepeaterTXThread.o \
					DVRPTRRepeaterRXThread.o DVRPTRRepeaterStatusData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dvrptrrepeaterd DVRPTRRepeaterD.o DVRPTRRepeaterConfig.o DVRPTRRepeaterTRXThread.o DVRPTRRepeaterTXRXThread.o \
				DVRPTRRepeaterTXThread.o DVRPTRRepeaterRXThread.o DVRPTRRepeaterStatusData.o ../Common/Common.a $(LIBS)

DVRPTRRepeaterApp.o:	DVRPTRRepeaterApp.cpp DVRPTRRepeaterApp.h DVRPTRRepeaterThread.h DVRPTRRepeaterTRXThread.h DVRPTRRepeaterTXRXThread.h \
						DVRPTRRepeaterTXThread.h DVRPTRRepeaterRXThread.h DVRPTRRepeaterFrame.h DVRPTRRepeaterDefs.h DVRPTRRepeaterStatusData.h \
						DVRPTRRepeaterLogger.h DVRPTRRepeaterConfig.h DVRPTRRepeaterThreadHelper.h ../Common/DVRPTRController.h ../Common/DVRPTRControllerV1.h \
						../Common/DVRPTRControllerV2.h ../Common/DVRPTRControllerV3.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/ExternalController.h \
						../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h ../Common/DummyController.h ../Common/K8055Controller.h \
						../Common/ArduinoController.h ../Common/RaspberryController.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterApp.cpp

DVRPTRRepeaterCallsignSet.o:	DVRPTRRepeaterCallsignSet.cpp DVRPTRRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterCallsignSet.cpp

DVRPTRRepeaterConfig.o:	DVRPTRRepeaterConfig.cpp DVRPTRRepeaterConfig.h DVRPTRRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterConfig.cpp

DVRPTRRepeaterControllerSet.o:	DVRPTRRepeaterControllerSet.cpp DVRPTRRepeaterControllerSet.h ../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterControllerSet.cpp

DVRPTRRepeaterD.o:	DVRPTRRepeaterD.cpp DVRPTRRepeaterD.h DVRPTRRepeaterThread.h DVRPTRRepeaterTRXThread.h DVRPTRRepeaterTXRXThread.h \
					DVRPTRRepeaterTXThread.h DVRPTRRepeaterRXThread.h DVRPTRRepeaterConfig.h DVRPTRRepeaterDefs.h ../Common/DStarDefines.h \
					../Common/Logger.h ../Common/DVRPTRController.h ../Common/DVRPTRControllerV1.h ../Common/DVRPTRControllerV2.h ../Common/DVRPTRControllerV3.h \
					../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h \
					../Common/K8055Controller.h ../Common/RaspberryController.h ../Common/RaspberryController.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterD.cpp

DVRPTRRepeaterFrame.o:	DVRPTRRepeaterFrame.cpp DVRPTRRepeaterFrame.h DVRPTRRepeaterDefs.h DVRPTRRepeaterApp.h DVRPTRRepeaterStatusData.h \
						DVRPTRRepeaterPreferences.h ../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterFrame.cpp

DVRPTRRepeaterLogger.o:	DVRPTRRepeaterLogger.cpp DVRPTRRepeaterLogger.h DVRPTRRepeaterApp.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterLogger.cpp

DVRPTRRepeaterModemSet.o:	DVRPTRRepeaterModemSet.cpp DVRPTRRepeaterModemSet.h DVRPTRRepeaterDefs.h ../Common/DVRPTRControllerV1.h ../Common/DVRPTRControllerV2.h \
						../Common/DVRPTRControllerV3.h ../Common/DStarDefines.h ../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterModemSet.cpp

DVRPTRRepeaterPreferences.o:	DVRPTRRepeaterPreferences.cpp DVRPTRRepeaterPreferences.h DVRPTRRepeaterDefs.h DVRPTRRepeaterControllerSet.h \
								DVRPTRRepeaterModemSet.h DVRPTRRepeaterApp.h DVRPTRRepeaterTimesSet.h DVRPTRRepeaterCallsignSet.h ../Common/NetworkSet.h ../Common/BeaconSet.h \
								../Common/AnnouncementSet.h ../Common/DStarDefines.h ../Common/Control1Set.h ../Common/Control2Set.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterPreferences.cpp

DVRPTRRepeaterStatusData.o:	DVRPTRRepeaterStatusData.cpp DVRPTRRepeaterStatusData.h DVRPTRRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterStatusData.cpp

DVRPTRRepeaterTRXThread.o:	DVRPTRRepeaterTRXThread.cpp DVRPTRRepeaterTRXThread.h DVRPTRRepeaterThread.h DVRPTRRepeaterDefs.h DVRPTRRepeaterApp.h \
						DVRPTRRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h \
						../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h \
						../Common/OutputQueue.h ../Common/Version.h ../Common/DVTOOLFileWriter.h ../Common/CallsignList.h \
						../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/BeaconUnit.h \
						../Common/BeaconCallback.h ../Common/AnnouncementUnit.h ../Common/AnnouncementCallback.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterTRXThread.cpp

DVRPTRRepeaterTXRXThread.o:	DVRPTRRepeaterTXRXThread.cpp DVRPTRRepeaterTXRXThread.h DVRPTRRepeaterThread.h DVRPTRRepeaterDefs.h DVRPTRRepeaterApp.h \
						DVRPTRRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h \
						../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h \
						../Common/OutputQueue.h ../Common/Version.h ../Common/DVTOOLFileWriter.h ../Common/CallsignList.h \
						../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterTXRXThread.cpp

DVRPTRRepeaterTXThread.o:	DVRPTRRepeaterTXThread.cpp DVRPTRRepeaterTXThread.h DVRPTRRepeaterThread.h DVRPTRRepeaterDefs.h DVRPTRRepeaterApp.h \
						DVRPTRRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h \
						../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/OutputQueue.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterTXThread.cpp

DVRPTRRepeaterRXThread.o:	DVRPTRRepeaterRXThread.cpp DVRPTRRepeaterRXThread.h DVRPTRRepeaterThread.h DVRPTRRepeaterDefs.h DVRPTRRepeaterApp.h \
						DVRPTRRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h \
						../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataDecoder.h ../Common/Version.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterRXThread.cpp

DVRPTRRepeaterThreadHelper.o:	DVRPTRRepeaterThreadHelper.cpp DVRPTRRepeaterThreadHelper.h DVRPTRRepeaterThread.h DVRPTRRepeaterStatusData.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterThreadHelper.cpp

DVRPTRRepeaterTimesSet.o:	DVRPTRRepeaterTimesSet.cpp DVRPTRRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c DVRPTRRepeaterTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dvrptrrepeater $(BINDIR)
				install -g bin -o root -m 0775 dvrptrrepeaterd $(BINDIR)

clean:
				rm -f core dvrptrrepeater dvrptrrepeaterd *.o *~ *.bak
