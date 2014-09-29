all:			dextragateway dextragatewayd

CFLAGS :=		-I../Common $(CFLAGS)

dextragateway:	DExtraGatewayApp.o DExtraGatewayAMBEData.o DExtraGatewayAudioUnit.o DExtraGatewayConfig.o DExtraGatewayFrame.o DExtraGatewayEchoUnit.o \
				DExtraGatewayHeaderData.o DExtraGatewayRepeaterProtocolHandler.o DExtraGatewayDExtraProtocolHandler.o DExtraGatewayRepeaterSet.o \
				DExtraGatewayReflectorSet.o DExtraGatewayPreferences.o DExtraGatewayHostFile.o DExtraGatewayStatusData.o DExtraGatewayThread.o \
				../Common/Common.a
				$(CC) $(LDFLAGS) -o dextragateway DExtraGatewayApp.o DExtraGatewayAMBEData.o DExtraGatewayAudioUnit.o DExtraGatewayConfig.o \
				DExtraGatewayEchoUnit.o DExtraGatewayFrame.o DExtraGatewayHeaderData.o DExtraGatewayRepeaterProtocolHandler.o \
				DExtraGatewayDExtraProtocolHandler.o DExtraGatewayRepeaterSet.o DExtraGatewayReflectorSet.o DExtraGatewayPreferences.o \
				DExtraGatewayHostFile.o DExtraGatewayStatusData.o DExtraGatewayThread.o ../Common/Common.a $(LIBS)

dextragatewayd:	DExtraGatewayD.o DExtraGatewayAMBEData.o DExtraGatewayAudioUnit.o DExtraGatewayConfig.o DExtraGatewayEchoUnit.o \
				DExtraGatewayRepeaterProtocolHandler.o DExtraGatewayDExtraProtocolHandler.o DExtraGatewayStatusData.o DExtraGatewayThread.o \
				DExtraGatewayHostFile.o DExtraGatewayHeaderData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dextragatewayd DExtraGatewayD.o DExtraGatewayAMBEData.o DExtraGatewayAudioUnit.o DExtraGatewayConfig.o \
				DExtraGatewayEchoUnit.o DExtraGatewayRepeaterProtocolHandler.o DExtraGatewayDExtraProtocolHandler.o DExtraGatewayStatusData.o \
				DExtraGatewayThread.o DExtraGatewayHostFile.o DExtraGatewayHeaderData.o ../Common/Common.a $(LIBS)

DExtraGatewayAMBEData.o:	DExtraGatewayAMBEData.cpp DExtraGatewayAMBEData.h ../Common/DStarDefines.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DExtraGatewayAMBEData.cpp

DExtraGatewayApp.o:	DExtraGatewayApp.cpp DExtraGatewayApp.h DExtraGatewayThread.h DExtraGatewayFrame.h DExtraGatewayDefs.h DExtraGatewayStatusData.h \
					DExtraGatewayConfig.h DExtraGatewayRepeaterProtocolHandler.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DExtraGatewayApp.cpp

DExtraGatewayAudioUnit.o:	DExtraGatewayAudioUnit.cpp DExtraGatewayAudioUnit.h DExtraGatewayCallback.h DExtraGatewayHeaderData.h \
							DExtraGatewayAMBEData.h DExtraGatewayDefs.h ../Common/Timer.h ../Common/DStarDefines.h ../Common/SlowDataEncoder.h
				$(CC) $(CFLAGS) -c DExtraGatewayAudioUnit.cpp

DExtraGatewayD.o:	DExtraGatewayD.cpp DExtraGatewayD.h DExtraGatewayThread.h DExtraGatewayDefs.h DExtraGatewayConfig.h \
					DExtraGatewayRepeaterProtocolHandler.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DExtraGatewayD.cpp

DExtraGatewayConfig.o:	DExtraGatewayConfig.cpp DExtraGatewayConfig.h DExtraGatewayDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DExtraGatewayConfig.cpp

DExtraGatewayDExtraProtocolHandler.o:	DExtraGatewayDExtraProtocolHandler.cpp DExtraGatewayDExtraProtocolHandler.h DExtraGatewayHeaderData.h \
						DExtraGatewayAMBEData.h ../Common/UDPReaderWriter.h ../Common/DStarDefines.h ../Common/Timer.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DExtraGatewayDExtraProtocolHandler.cpp

DExtraGatewayEchoUnit.o:	DExtraGatewayEchoUnit.cpp DExtraGatewayEchoUnit.h DExtraGatewayCallback.h DExtraGatewayHeaderData.h DExtraGatewayAMBEData.h \
							../Common/Timer.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DExtraGatewayEchoUnit.cpp

DExtraGatewayFrame.o:	DExtraGatewayFrame.cpp DExtraGatewayFrame.h DExtraGatewayPreferences.h DExtraGatewayDefs.h DExtraGatewayApp.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DExtraGatewayFrame.cpp

DExtraGatewayHeaderData.o:	DExtraGatewayHeaderData.cpp DExtraGatewayHeaderData.h ../Common/CCITTChecksumReverse.h ../Common/DStarDefines.h \
							../Common/Utils.h
				$(CC) $(CFLAGS) -c DExtraGatewayHeaderData.cpp

DExtraGatewayHostFile.o:	DExtraGatewayHostFile.cpp DExtraGatewayHostFile.h
				$(CC) $(CFLAGS) -c DExtraGatewayHostFile.cpp

DExtraGatewayPreferences.o:	DExtraGatewayPreferences.cpp DExtraGatewayPreferences.h DExtraGatewayDefs.h DExtraGatewayRepeaterSet.h \
							DExtraGatewayReflectorSet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DExtraGatewayPreferences.cpp

DExtraGatewayReflectorSet.o:	DExtraGatewayReflectorSet.cpp DExtraGatewayReflectorSet.h DExtraGatewayHostFile.h DExtraGatewayDefs.h \
						../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DExtraGatewayReflectorSet.cpp

DExtraGatewayRepeaterProtocolHandler.o:	DExtraGatewayRepeaterProtocolHandler.cpp DExtraGatewayRepeaterProtocolHandler.h DExtraGatewayHeaderData.h \
						DExtraGatewayAMBEData.h ../Common/UDPReaderWriter.h ../Common/DStarDefines.h ../Common/CCITTChecksumReverse.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DExtraGatewayRepeaterProtocolHandler.cpp

DExtraGatewayRepeaterSet.o:	DExtraGatewayRepeaterSet.cpp DExtraGatewayRepeaterSet.h DExtraGatewayDefs.h ../Common/DStarDefines.h \
						../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DExtraGatewayRepeaterSet.cpp

DExtraGatewayStatusData.o:	DExtraGatewayStatusData.cpp DExtraGatewayStatusData.h DExtraGatewayDefs.h
				$(CC) $(CFLAGS) -c DExtraGatewayStatusData.cpp

DExtraGatewayThread.o:	DExtraGatewayThread.cpp DExtraGatewayThread.h DExtraGatewayDefs.h DExtraGatewayApp.h DExtraGatewayStatusData.h \
						DExtraGatewayHeaderData.h DExtraGatewayAMBEData.h DExtraGatewayHostFile.h DExtraGatewayRepeaterProtocolHandler.h \
						DExtraGatewayDExtraProtocolHandler.h DExtraGatewayAudioUnit.h DExtraGatewayEchoUnit.h DExtraGatewayCallback.h \
						../Common/DStarDefines.h ../Common/Utils.h ../Common/Timer.h
				$(CC) $(CFLAGS) -c DExtraGatewayThread.cpp

install:	dextragateway dextragatewayd
			install -g bin -o root -m 0775 dextragateway $(BINDIR)
			install -g bin -o root -m 0775 dextragatewayd $(BINDIR)

clean:
			rm -f core dextragateway dextragatewayd *.o *~ *.bak

