all:			dcsgateway dcsgatewayd

CFLAGS :=		-I../Common $(CFLAGS)

dcsgateway:		DCSGatewayApp.o DCSGatewayAMBEData.o DCSGatewayAudioUnit.o DCSGatewayConfig.o DCSGatewayFrame.o DCSGatewayEchoUnit.o \
				DCSGatewayHeaderData.o DCSGatewayRepeaterProtocolHandler.o DCSGatewayDCSProtocolHandler.o DCSGatewayRepeaterSet.o \
				DCSGatewayReflectorSet.o DCSGatewayPreferences.o DCSGatewayHostFile.o DCSGatewayStatusData.o DCSGatewayTextCollector.o DCSGatewayThread.o \
				../Common/Common.a
				$(CC) $(LDFLAGS) -o dcsgateway DCSGatewayApp.o DCSGatewayAMBEData.o DCSGatewayAudioUnit.o DCSGatewayConfig.o \
				DCSGatewayEchoUnit.o DCSGatewayFrame.o DCSGatewayHeaderData.o DCSGatewayRepeaterProtocolHandler.o \
				DCSGatewayDCSProtocolHandler.o DCSGatewayRepeaterSet.o DCSGatewayReflectorSet.o DCSGatewayPreferences.o \
				DCSGatewayHostFile.o DCSGatewayStatusData.o DCSGatewayTextCollector.o DCSGatewayThread.o ../Common/Common.a $(LIBS)

dcsgatewayd:	DCSGatewayD.o DCSGatewayAMBEData.o DCSGatewayAudioUnit.o DCSGatewayConfig.o DCSGatewayEchoUnit.o \
				DCSGatewayRepeaterProtocolHandler.o DCSGatewayDCSProtocolHandler.o DCSGatewayStatusData.o DCSGatewayThread.o \
				DCSGatewayHostFile.o DCSGatewayHeaderData.o DCSGatewayTextCollector.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dcsgatewayd DCSGatewayD.o DCSGatewayAMBEData.o DCSGatewayAudioUnit.o DCSGatewayConfig.o \
				DCSGatewayEchoUnit.o DCSGatewayRepeaterProtocolHandler.o DCSGatewayDCSProtocolHandler.o DCSGatewayStatusData.o \
				DCSGatewayThread.o DCSGatewayHostFile.o DCSGatewayHeaderData.o DCSGatewayTextCollector.o ../Common/Common.a $(LIBS)

DCSGatewayAMBEData.o:	DCSGatewayAMBEData.cpp DCSGatewayAMBEData.h ../Common/DStarDefines.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DCSGatewayAMBEData.cpp

DCSGatewayApp.o:	DCSGatewayApp.cpp DCSGatewayApp.h DCSGatewayThread.h DCSGatewayFrame.h DCSGatewayDefs.h DCSGatewayStatusData.h \
					DCSGatewayConfig.h DCSGatewayRepeaterProtocolHandler.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DCSGatewayApp.cpp

DCSGatewayAudioUnit.o:	DCSGatewayAudioUnit.cpp DCSGatewayAudioUnit.h DCSGatewayCallback.h DCSGatewayHeaderData.h \
							DCSGatewayAMBEData.h DCSGatewayDefs.h ../Common/Timer.h ../Common/DStarDefines.h ../Common/SlowDataEncoder.h
				$(CC) $(CFLAGS) -c DCSGatewayAudioUnit.cpp

DCSGatewayD.o:	DCSGatewayD.cpp DCSGatewayD.h DCSGatewayThread.h DCSGatewayDefs.h DCSGatewayConfig.h \
					DCSGatewayRepeaterProtocolHandler.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DCSGatewayD.cpp

DCSGatewayConfig.o:	DCSGatewayConfig.cpp DCSGatewayConfig.h DCSGatewayDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DCSGatewayConfig.cpp

DCSGatewayDCSProtocolHandler.o:	DCSGatewayDCSProtocolHandler.cpp DCSGatewayDCSProtocolHandler.h DCSGatewayHeaderData.h \
						DCSGatewayAMBEData.h ../Common/UDPReaderWriter.h ../Common/DStarDefines.h ../Common/Timer.h ../Common/Version.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DCSGatewayDCSProtocolHandler.cpp

DCSGatewayEchoUnit.o:	DCSGatewayEchoUnit.cpp DCSGatewayEchoUnit.h DCSGatewayCallback.h DCSGatewayHeaderData.h DCSGatewayAMBEData.h \
							../Common/Timer.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DCSGatewayEchoUnit.cpp

DCSGatewayFrame.o:	DCSGatewayFrame.cpp DCSGatewayFrame.h DCSGatewayPreferences.h DCSGatewayDefs.h DCSGatewayApp.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DCSGatewayFrame.cpp

DCSGatewayHeaderData.o:	DCSGatewayHeaderData.cpp DCSGatewayHeaderData.h ../Common/CCITTChecksumReverse.h ../Common/DStarDefines.h \
							../Common/Utils.h
				$(CC) $(CFLAGS) -c DCSGatewayHeaderData.cpp

DCSGatewayHostFile.o:	DCSGatewayHostFile.cpp DCSGatewayHostFile.h
				$(CC) $(CFLAGS) -c DCSGatewayHostFile.cpp

DCSGatewayPreferences.o:	DCSGatewayPreferences.cpp DCSGatewayPreferences.h DCSGatewayDefs.h DCSGatewayRepeaterSet.h \
							DCSGatewayReflectorSet.h
				$(CC) $(CFLAGS) -c DCSGatewayPreferences.cpp

DCSGatewayReflectorSet.o:	DCSGatewayReflectorSet.cpp DCSGatewayReflectorSet.h DCSGatewayHostFile.h DCSGatewayDefs.h \
						../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DCSGatewayReflectorSet.cpp

DCSGatewayRepeaterProtocolHandler.o:	DCSGatewayRepeaterProtocolHandler.cpp DCSGatewayRepeaterProtocolHandler.h DCSGatewayHeaderData.h \
						DCSGatewayAMBEData.h ../Common/UDPReaderWriter.h ../Common/DStarDefines.h ../Common/CCITTChecksumReverse.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DCSGatewayRepeaterProtocolHandler.cpp

DCSGatewayRepeaterSet.o:	DCSGatewayRepeaterSet.cpp DCSGatewayRepeaterSet.h DCSGatewayDefs.h ../Common/DStarDefines.h \
						../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DCSGatewayRepeaterSet.cpp

DCSGatewayStatusData.o:	DCSGatewayStatusData.cpp DCSGatewayStatusData.h DCSGatewayDefs.h
				$(CC) $(CFLAGS) -c DCSGatewayStatusData.cpp

DCSGatewayTextCollector.o:	DCSGatewayTextCollector.cpp DCSGatewayTextCollector.h DCSGatewayAMBEData.h DCSGatewayDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DCSGatewayTextCollector.cpp

DCSGatewayThread.o:	DCSGatewayThread.cpp DCSGatewayThread.h DCSGatewayDefs.h DCSGatewayApp.h DCSGatewayStatusData.h \
						DCSGatewayHeaderData.h DCSGatewayAMBEData.h DCSGatewayHostFile.h DCSGatewayRepeaterProtocolHandler.h \
						DCSGatewayDCSProtocolHandler.h DCSGatewayAudioUnit.h DCSGatewayEchoUnit.h DCSGatewayCallback.h DCSGatewayTextCollector.h \
						../Common/DStarDefines.h ../Common/Utils.h ../Common/Timer.h
				$(CC) $(CFLAGS) -c DCSGatewayThread.cpp

install:	dcsgateway dcsgatewayd
			install -g bin -o root -m 0775 dcsgateway $(BINDIR)
			install -g bin -o root -m 0775 dcsgatewayd $(BINDIR)

clean:
			rm -f core dcsgateway dcsgatewayd *.o *~ *.bak

