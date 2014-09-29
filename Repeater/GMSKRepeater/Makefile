all:			gmskrepeater gmskrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

gmskrepeater:	GMSKRepeaterApp.o GMSKRepeaterCallsignSet.o GMSKRepeaterConfig.o GMSKRepeaterControllerSet.o GMSKRepeaterFrame.o GMSKRepeaterLogger.o \
				GMSKRepeaterModemSet.o GMSKRepeaterPreferences.o GMSKRepeaterStatusData.o GMSKRepeaterTRXThread.o GMSKRepeaterTXRXThread.o \
				GMSKRepeaterTXThread.o GMSKRepeaterRXThread.o GMSKRepeaterTimesSet.o GMSKRepeaterThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o gmskrepeater GMSKRepeaterApp.o GMSKRepeaterCallsignSet.o GMSKRepeaterConfig.o GMSKRepeaterControllerSet.o \
				GMSKRepeaterFrame.o GMSKRepeaterLogger.o GMSKRepeaterModemSet.o GMSKRepeaterPreferences.o GMSKRepeaterStatusData.o \
				GMSKRepeaterTRXThread.o GMSKRepeaterTXRXThread.o GMSKRepeaterTXThread.o GMSKRepeaterRXThread.o GMSKRepeaterTimesSet.o \
				GMSKRepeaterThreadHelper.o ../Common/Common.a $(LIBS)

gmskrepeaterd:	GMSKRepeaterD.o GMSKRepeaterConfig.o GMSKRepeaterTRXThread.o GMSKRepeaterTXRXThread.o GMSKRepeaterTXThread.o GMSKRepeaterRXThread.o \
				GMSKRepeaterStatusData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o gmskrepeaterd GMSKRepeaterD.o GMSKRepeaterConfig.o GMSKRepeaterTRXThread.o GMSKRepeaterTXRXThread.o \
				GMSKRepeaterTXThread.o GMSKRepeaterRXThread.o GMSKRepeaterStatusData.o ../Common/Common.a $(LIBS)

GMSKRepeaterApp.o:	GMSKRepeaterApp.cpp GMSKRepeaterApp.h GMSKRepeaterThread.h GMSKRepeaterTRXThread.h GMSKRepeaterTXRXThread.h GMSKRepeaterTXThread.h \
					GMSKRepeaterRXThread.h GMSKRepeaterFrame.h GMSKRepeaterDefs.h GMSKRepeaterStatusData.h GMSKRepeaterLogger.h GMSKRepeaterConfig.h \
					GMSKRepeaterThreadHelper.h ../Common/GMSKModem.h ../Common/GMSKModemLibUsb.h ../Common/DStarDefines.h ../Common/Logger.h \
					../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h \
					../Common/K8055Controller.h ../Common/ArduinoController.h ../Common/RaspberryController.h
				$(CC) $(CFLAGS) -c GMSKRepeaterApp.cpp

GMSKRepeaterCallsignSet.o:	GMSKRepeaterCallsignSet.cpp GMSKRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c GMSKRepeaterCallsignSet.cpp

GMSKRepeaterConfig.o:	GMSKRepeaterConfig.cpp GMSKRepeaterConfig.h GMSKRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c GMSKRepeaterConfig.cpp

GMSKRepeaterControllerSet.o:	GMSKRepeaterControllerSet.cpp GMSKRepeaterControllerSet.h ../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c GMSKRepeaterControllerSet.cpp

GMSKRepeaterD.o:	GMSKRepeaterD.cpp GMSKRepeaterD.h GMSKRepeaterThread.h GMSKRepeaterTRXThread.h GMSKRepeaterTXRXThread.h GMSKRepeaterTXThread.h \
					GMSKRepeaterRXThread.h GMSKRepeaterConfig.h GMSKRepeaterDefs.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/GMSKModem.h \
					../Common/GMSKModemLibUsb.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h \
					../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/RaspberryController.h ../Common/ArduinoController.h
				$(CC) $(CFLAGS) -c GMSKRepeaterD.cpp

GMSKRepeaterFrame.o:	GMSKRepeaterFrame.cpp GMSKRepeaterFrame.h GMSKRepeaterDefs.h GMSKRepeaterApp.h GMSKRepeaterStatusData.h \
						GMSKRepeaterPreferences.h ../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c GMSKRepeaterFrame.cpp

GMSKRepeaterLogger.o:	GMSKRepeaterLogger.cpp GMSKRepeaterLogger.h GMSKRepeaterApp.h
				$(CC) $(CFLAGS) -c GMSKRepeaterLogger.cpp

GMSKRepeaterModemSet.o:	GMSKRepeaterModemSet.cpp GMSKRepeaterModemSet.h GMSKRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c GMSKRepeaterModemSet.cpp

GMSKRepeaterPreferences.o:	GMSKRepeaterPreferences.cpp GMSKRepeaterPreferences.h GMSKRepeaterDefs.h GMSKRepeaterControllerSet.h \
							GMSKRepeaterModemSet.h GMSKRepeaterApp.h GMSKRepeaterTimesSet.h GMSKRepeaterCallsignSet.h ../Common/NetworkSet.h \
							../Common/BeaconSet.h ../Common/AnnouncementSet.h ../Common/DStarDefines.h ../Common/Control1Set.h ../Common/Control2Set.h
				$(CC) $(CFLAGS) -c GMSKRepeaterPreferences.cpp

GMSKRepeaterStatusData.o:	GMSKRepeaterStatusData.cpp GMSKRepeaterStatusData.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c GMSKRepeaterStatusData.cpp

GMSKRepeaterThreadHelper.o:	GMSKRepeaterThreadHelper.cpp GMSKRepeaterThreadHelper.h GMSKRepeaterThread.h GMSKRepeaterStatusData.h
				$(CC) $(CFLAGS) -c GMSKRepeaterThreadHelper.cpp

GMSKRepeaterTRXThread.o:	GMSKRepeaterTRXThread.cpp GMSKRepeaterTRXThread.h GMSKRepeaterThread.h GMSKRepeaterDefs.h GMSKRepeaterApp.h \
							GMSKRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/GMSKModem.h ../Common/HeaderData.h \
							../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/OutputQueue.h \
							../Common/Version.h ../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/ExternalController.h \
							../Common/DummyController.h ../Common/K8055Controller.h ../Common/BeaconUnit.h ../Common/BeaconCallback.h \
							../Common/AnnouncementUnit.h ../Common/AnnouncementCallback.h
				$(CC) $(CFLAGS) -c GMSKRepeaterTRXThread.cpp

GMSKRepeaterTXRXThread.o:	GMSKRepeaterTXRXThread.cpp GMSKRepeaterTXRXThread.h GMSKRepeaterThread.h GMSKRepeaterDefs.h GMSKRepeaterApp.h \
							GMSKRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/GMSKModem.h ../Common/HeaderData.h \
							../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/OutputQueue.h \
							../Common/Version.h ../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/ExternalController.h \
							../Common/DummyController.h ../Common/K8055Controller.h
				$(CC) $(CFLAGS) -c GMSKRepeaterTXRXThread.cpp

GMSKRepeaterTXThread.o:		GMSKRepeaterTXThread.cpp GMSKRepeaterTXThread.h GMSKRepeaterThread.h GMSKRepeaterDefs.h GMSKRepeaterApp.h \
							GMSKRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/GMSKModem.h ../Common/HeaderData.h \
							../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/OutputQueue.h ../Common/Version.h \
							../Common/DVTOOLFileWriter.h
				$(CC) $(CFLAGS) -c GMSKRepeaterTXThread.cpp

GMSKRepeaterRXThread.o:		GMSKRepeaterRXThread.cpp GMSKRepeaterRXThread.h GMSKRepeaterThread.h GMSKRepeaterDefs.h GMSKRepeaterApp.h \
							GMSKRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/GMSKModem.h ../Common/HeaderData.h \
							../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h
				$(CC) $(CFLAGS) -c GMSKRepeaterRXThread.cpp

GMSKRepeaterTimesSet.o:	GMSKRepeaterTimesSet.cpp GMSKRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c GMSKRepeaterTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 gmskrepeater $(BINDIR)
				install -g bin -o root -m 0775 gmskrepeaterd $(BINDIR)

clean:
				rm -f core gmskrepeater gmskrepeaterd *.o *~ *.bak
