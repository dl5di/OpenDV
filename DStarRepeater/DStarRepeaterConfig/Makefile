all:			dstarrepeaterconfig

CFLAGS :=		-I../Common $(CFLAGS)

dstarrepeaterconfig:	DStarRepeaterConfigActiveHangSet.o DStarRepeaterConfigAnnouncementSet.o DStarRepeaterConfigApp.o DStarRepeaterConfigBeaconSet.o \
					DStarRepeaterConfigCallsignSet.o DStarRepeaterConfigControl1Set.o DStarRepeaterConfigControl2Set.o DStarRepeaterConfigControllerSet.o \
					DStarRepeaterConfigDVAPSet.o DStarRepeaterConfigDVMegaSet.o DStarRepeaterConfigDVRPTR1Set.o DStarRepeaterConfigDVRPTR2Set.o \
					DStarRepeaterConfigDVRPTR3Set.o DStarRepeaterConfigFrame.o DStarRepeaterConfigGMSKSet.o DStarRepeaterConfigModemSet.o DStarRepeaterConfigNetworkSet.o \
					DStarRepeaterConfigSoundCardSet.o DStarRepeaterConfigSplitSet.o DStarRepeaterConfigTimesSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeaterconfig DStarRepeaterConfigActiveHangSet.o DStarRepeaterConfigAnnouncementSet.o DStarRepeaterConfigApp.o \
					DStarRepeaterConfigBeaconSet.o DStarRepeaterConfigCallsignSet.o DStarRepeaterConfigControl1Set.o DStarRepeaterConfigControl2Set.o \
					DStarRepeaterConfigControllerSet.o DStarRepeaterConfigDVAPSet.o DStarRepeaterConfigDVMegaSet.o DStarRepeaterConfigDVRPTR1Set.o \
					DStarRepeaterConfigDVRPTR2Set.o DStarRepeaterConfigDVRPTR3Set.o DStarRepeaterConfigFrame.o DStarRepeaterConfigGMSKSet.o DStarRepeaterConfigModemSet.o \
					DStarRepeaterConfigNetworkSet.o DStarRepeaterConfigSoundCardSet.o DStarRepeaterConfigSplitSet.o DStarRepeaterConfigTimesSet.o ../Common/Common.a $(LIBS)

DStarRepeaterConfigActiveHangSet.o:	DStarRepeaterConfigActiveHangSet.cpp DStarRepeaterConfigActiveHangSet.h
		$(CC) $(CFLAGS) -c DStarRepeaterConfigActiveHangSet.cpp

DStarRepeaterConfigAnnouncementSet.o:	DStarRepeaterConfigAnnouncementSet.cpp DStarRepeaterConfigAnnouncementSet.h ../Common/CallsignTextCtrl.h \
					../Common/DStarDefines.h
		$(CC) $(CFLAGS) -c DStarRepeaterConfigAnnouncementSet.cpp

DStarRepeaterConfigApp.o:	DStarRepeaterConfigApp.cpp DStarRepeaterConfigApp.h DStarRepeaterConfigFrame.h DStarRepeaterConfigDefs.h ../Common/DStarDefines.h \
					../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigApp.cpp

DStarRepeaterConfigBeaconSet.o:	DStarRepeaterConfigBeaconSet.cpp DStarRepeaterConfigBeaconSet.h ../Common/MessageTextCtrl.h ../Common/DStarDefines.h
		$(CC) $(CFLAGS) -c DStarRepeaterConfigBeaconSet.cpp

DStarRepeaterConfigCallsignSet.o:	DStarRepeaterConfigCallsignSet.cpp DStarRepeaterConfigCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigCallsignSet.cpp

DStarRepeaterConfigControl1Set.o:	DStarRepeaterConfigControl1Set.cpp DStarRepeaterConfigControl1Set.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigControl1Set.cpp

DStarRepeaterConfigControl2Set.o:	DStarRepeaterConfigControl2Set.cpp DStarRepeaterConfigControl2Set.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigControl2Set.cpp

DStarRepeaterConfigControllerSet.o:	DStarRepeaterConfigControllerSet.cpp DStarRepeaterConfigControllerSet.h ../Common/SerialDataController.h \
					../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigControllerSet.cpp

DStarRepeaterConfigDVAPSet.o:	DStarRepeaterConfigDVAPSet.cpp DStarRepeaterConfigDVAPSet.h ../Common/SerialDataController.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVAPSet.cpp

DStarRepeaterConfigDVMegaSet.o:	DStarRepeaterConfigDVMegaSet.cpp DStarRepeaterConfigDVMegaSet.h ../Common/SerialDataController.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVMegaSet.cpp

DStarRepeaterConfigDVRPTR1Set.o:	DStarRepeaterConfigDVRPTR1Set.cpp DStarRepeaterConfigDVRPTR1Set.h ../Common/SerialDataController.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVRPTR1Set.cpp

DStarRepeaterConfigDVRPTR2Set.o:	DStarRepeaterConfigDVRPTR2Set.cpp DStarRepeaterConfigDVRPTR2Set.h ../Common/SerialDataController.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVRPTR2Set.cpp

DStarRepeaterConfigDVRPTR3Set.o:	DStarRepeaterConfigDVRPTR3Set.cpp DStarRepeaterConfigDVRPTR3Set.h ../Common/SerialDataController.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVRPTR3Set.cpp

DStarRepeaterConfigFrame.o:	DStarRepeaterConfigFrame.cpp DStarRepeaterConfigFrame.h DStarRepeaterConfigDefs.h DStarRepeaterConfigControllerSet.h \
					DStarRepeaterConfigModemSet.h DStarRepeaterConfigTimesSet.h DStarRepeaterConfigCallsignSet.h DStarRepeaterConfigNetworkSet.h \
					DStarRepeaterConfigBeaconSet.h DStarRepeaterConfigAnnouncementSet.h DStarRepeaterConfigControl1Set.h DStarRepeaterConfigControl2Set.h \
					../Common/DStarRepeaterConfig.h ../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigFrame.cpp

DStarRepeaterConfigGMSKSet.o:	DStarRepeaterConfigGMSKSet.cpp DStarRepeaterConfigGMSKSet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigGMSKSet.cpp

DStarRepeaterConfigModemSet.o:	DStarRepeaterConfigModemSet.cpp DStarRepeaterConfigModemSet.h DStarRepeaterConfigDefs.h DStarRepeaterConfigSoundCardSet.h \
					DStarRepeaterConfigDVRPTR1Set.h DStarRepeaterConfigDVRPTR2Set.h DStarRepeaterConfigDVRPTR3Set.h DStarRepeaterConfigModemSet.h \
					DStarRepeaterConfigGMSKSet.h DStarRepeaterConfigDVAPSet.h DStarRepeaterConfigDVMegaSet.h DStarRepeaterConfigSplitSet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigModemSet.cpp

DStarRepeaterConfigNetworkSet.o:	DStarRepeaterConfigNetworkSet.cpp DStarRepeaterConfigNetworkSet.h ../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
		$(CC) $(CFLAGS) -c DStarRepeaterConfigNetworkSet.cpp

DStarRepeaterConfigSplitSet.o:	DStarRepeaterConfigSplitSet.cpp DStarRepeaterConfigSplitSet.h ../Common/AddressTextCtrl.h  ../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigSplitSet.cpp

DStarRepeaterConfigSoundCardSet.o:	DStarRepeaterConfigSoundCardSet.cpp DStarRepeaterConfigSoundCardSet.h ../Common/SoundCardReaderWriter.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigSoundCardSet.cpp

DStarRepeaterConfigTimesSet.o:	DStarRepeaterConfigTimesSet.cpp DStarRepeaterConfigTimesSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dstarrepeaterconfig $(BINDIR)

clean:
				rm -f core dstarrepeaterconfig *.o *~ *.bak
