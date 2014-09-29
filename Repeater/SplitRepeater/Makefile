all:			splitrepeater splitrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

splitrepeater:	SplitRepeaterAddressSet.o SplitRepeaterAMBEData.o SplitRepeaterApp.o SplitRepeaterCallsignSet.o SplitRepeaterConfig.o \
				SplitRepeaterControl1Set.o SplitRepeaterControl2Set.o SplitRepeaterFrame.o SplitRepeaterHeaderData.o SplitRepeaterLogger.o \
				SplitRepeaterPreferences.o SplitRepeaterProtocolHandler.o SplitRepeaterStatusData.o SplitRepeaterThread.o SplitRepeaterTimesSet.o \
				SplitRepeaterUDPReaderWriter.o SplitRepeaterThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o splitrepeater SplitRepeaterAddressSet.o SplitRepeaterAMBEData.o SplitRepeaterApp.o SplitRepeaterCallsignSet.o \
				SplitRepeaterConfig.o SplitRepeaterControl1Set.o SplitRepeaterControl2Set.o SplitRepeaterFrame.o SplitRepeaterHeaderData.o \
				SplitRepeaterLogger.o SplitRepeaterPreferences.o SplitRepeaterProtocolHandler.o SplitRepeaterStatusData.o SplitRepeaterThread.o \
				SplitRepeaterTimesSet.o SplitRepeaterUDPReaderWriter.o SplitRepeaterThreadHelper.o ../Common/Common.a $(LIBS)

splitrepeaterd:	SplitRepeaterAMBEData.o SplitRepeaterD.o SplitRepeaterConfig.o SplitRepeaterHeaderData.o SplitRepeaterProtocolHandler.o \
				SplitRepeaterStatusData.o SplitRepeaterThread.o SplitRepeaterUDPReaderWriter.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o splitrepeaterd SplitRepeaterAMBEData.o SplitRepeaterD.o SplitRepeaterConfig.o SplitRepeaterHeaderData.o \
				SplitRepeaterProtocolHandler.o SplitRepeaterStatusData.o SplitRepeaterThread.o SplitRepeaterUDPReaderWriter.o \
				../Common/Common.a $(LIBS)

SplitRepeaterAddressSet.o:	SplitRepeaterAddressSet.cpp SplitRepeaterAddressSet.h ../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c SplitRepeaterAddressSet.cpp

SplitRepeaterAMBEData.o:	SplitRepeaterAMBEData.cpp SplitRepeaterAMBEData.h ../Common/DStarDefines.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c SplitRepeaterAMBEData.cpp

SplitRepeaterApp.o:	SplitRepeaterApp.cpp SplitRepeaterApp.h SplitRepeaterThread.h SplitRepeaterFrame.h SplitRepeaterDefs.h SplitRepeaterStatusData.h \
					SplitRepeaterLogger.h SplitRepeaterConfig.h SplitRepeaterProtocolHandler.h SplitRepeaterThreadHelper.h ../Common/DStarDefines.h ../Common/Logger.h \
					../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c SplitRepeaterApp.cpp

SplitRepeaterCallsignSet.o:	SplitRepeaterCallsignSet.cpp SplitRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SplitRepeaterCallsignSet.cpp

SplitRepeaterConfig.o:	SplitRepeaterConfig.cpp SplitRepeaterConfig.h SplitRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SplitRepeaterConfig.cpp

SplitRepeaterControl1Set.o:	SplitRepeaterControl1Set.cpp SplitRepeaterControl1Set.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SplitRepeaterControl1Set.cpp

SplitRepeaterControl2Set.o:	SplitRepeaterControl2Set.cpp SplitRepeaterControl2Set.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SplitRepeaterControl2Set.cpp

SplitRepeaterD.o:	SplitRepeaterD.cpp SplitRepeaterD.h SplitRepeaterThread.h SplitRepeaterProtocolHandler.h SplitRepeaterConfig.h SplitRepeaterDefs.h \
					../Common/DStarDefines.h ../Common/Logger.h ../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c SplitRepeaterD.cpp

SplitRepeaterFrame.o:	SplitRepeaterFrame.cpp SplitRepeaterFrame.h SplitRepeaterDefs.h SplitRepeaterApp.h SplitRepeaterStatusData.h \
						SplitRepeaterPreferences.h ../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c SplitRepeaterFrame.cpp

SplitRepeaterHeaderData.o:	SplitRepeaterHeaderData.cpp SplitRepeaterHeaderData.h ../Common/CCITTChecksumReverse.h ../Common/DStarDefines.h \
							../Common/HeaderData.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c SplitRepeaterHeaderData.cpp

SplitRepeaterLogger.o:	SplitRepeaterLogger.cpp SplitRepeaterLogger.h SplitRepeaterApp.h
				$(CC) $(CFLAGS) -c SplitRepeaterLogger.cpp

SplitRepeaterPreferences.o:	SplitRepeaterPreferences.cpp SplitRepeaterPreferences.h SplitRepeaterDefs.h SplitRepeaterControl1Set.h \
							SplitRepeaterControl2Set.h SplitRepeaterAddressSet.h SplitRepeaterApp.h SplitRepeaterTimesSet.h SplitRepeaterCallsignSet.h \
							../Common/NetworkSet.h ../Common/BeaconSet.h ../Common/AnnouncementSet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SplitRepeaterPreferences.cpp

SplitRepeaterProtocolHandler.o:	SplitRepeaterProtocolHandler.cpp SplitRepeaterProtocolHandler.h SplitRepeaterUDPReaderWriter.h \
								SplitRepeaterAMBEData.h SplitRepeaterHeaderData.h ../Common/DStarDefines.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c SplitRepeaterProtocolHandler.cpp

SplitRepeaterStatusData.o:	SplitRepeaterStatusData.cpp SplitRepeaterStatusData.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c SplitRepeaterStatusData.cpp

SplitRepeaterThreadHelper.o:	SplitRepeaterThreadHelper.cpp SplitRepeaterThreadHelper.h SplitRepeaterThread.h SplitRepeaterStatusData.h
				$(CC) $(CFLAGS) -c SplitRepeaterThreadHelper.cpp

SplitRepeaterThread.o:	SplitRepeaterThread.cpp SplitRepeaterThread.h SplitRepeaterDefs.h SplitRepeaterApp.h SplitRepeaterAMBEData.h \
						SplitRepeaterHeaderData.h SplitRepeaterStatusData.h SplitRepeaterProtocolHandler.h ../Common/DStarDefines.h ../Common/Timer.h \
						../Common/SlowDataEncoder.h ../Common/Version.h ../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/BeaconUnit.h \
						../Common/BeaconCallback.h ../Common/AnnouncementUnit.h ../Common/AnnouncementCallback.h ../Common/HeaderData.h
				$(CC) $(CFLAGS) -c SplitRepeaterThread.cpp

SplitRepeaterTimesSet.o:	SplitRepeaterTimesSet.cpp SplitRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c SplitRepeaterTimesSet.cpp

SplitRepeaterUDPReaderWriter.o:	SplitRepeaterUDPReaderWriter.cpp SplitRepeaterUDPReaderWriter.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c SplitRepeaterUDPReaderWriter.cpp

install:	all
				install -g bin -o root -m 0775 splitrepeater $(BINDIR)
				install -g bin -o root -m 0775 splitrepeaterd $(BINDIR)

clean:
				rm -f core splitrepeater splitrepeaterd *.o *~ *.bak
