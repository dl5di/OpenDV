all:			remotecontrol remotecontrold

CFLAGS :=		-I../Common $(CFLAGS)

remotecontrol:	RemoteControlApp.o RemoteControlCallsignData.o RemoteControlConfig.o RemoteControlFrame.o RemoteControlLinkData.o \
				RemoteControlPreferences.o RemoteControlRemoteControlHandler.o RemoteControlRemoteSet.o RemoteControlRepeaterData.o \
				RemoteControlRepeaterPanel.o RemoteControlStarNetGroup.o RemoteControlStarNetPanel.o RemoteControlStarNetUser.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o remotecontrol RemoteControlApp.o RemoteControlCallsignData.o RemoteControlConfig.o RemoteControlFrame.o \
				RemoteControlLinkData.o RemoteControlPreferences.o RemoteControlRemoteControlHandler.o RemoteControlRemoteSet.o \
				RemoteControlRepeaterData.o RemoteControlRepeaterPanel.o RemoteControlStarNetGroup.o RemoteControlStarNetPanel.o \
				RemoteControlStarNetUser.o ../Common/Common.a $(LIBS)

remotecontrold:	RemoteControlAppD.o RemoteControlCallsignData.o RemoteControlConfig.o RemoteControlLinkData.o RemoteControlRemoteControlHandler.o \
				RemoteControlRepeaterData.o RemoteControlStarNetGroup.o RemoteControlStarNetUser.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o remotecontrold RemoteControlAppD.o RemoteControlCallsignData.o RemoteControlConfig.o RemoteControlLinkData.o \
				RemoteControlRemoteControlHandler.o	RemoteControlRepeaterData.o RemoteControlStarNetGroup.o RemoteControlStarNetUser.o ../Common/Common.a $(LIBS)

RemoteControlApp.o:		RemoteControlApp.cpp RemoteControlApp.h RemoteControlFrame.h RemoteControlConfig.h RemoteControlDefs.h ../Common/Version.h
				$(CC) $(CFLAGS) -c RemoteControlApp.cpp

RemoteControlAppD.o:		RemoteControlAppD.cpp RemoteControlConfig.h RemoteControlRemoteControlHandler.h RemoteControlDefs.h ../Common/SHA256.h ../Common/Defs.h \
				../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c RemoteControlAppD.cpp

RemoteControlCallsignData.o:		RemoteControlCallsignData.cpp RemoteControlCallsignData.h
				$(CC) $(CFLAGS) -c RemoteControlCallsignData.cpp

RemoteControlConfig.o:	RemoteControlConfig.cpp RemoteControlConfig.h
				$(CC) $(CFLAGS) -c RemoteControlConfig.cpp

RemoteControlFrame.o:	RemoteControlFrame.cpp RemoteControlFrame.h RemoteControlRemoteControlHandler.h RemoteControlRepeaterPanel.h \
				RemoteControlStarNetPanel.h RemoteControlPreferences.h RemoteControlApp.h RemoteControlDefs.h ../Common/Defs.h ../Common/Version.h \
				../Common/SHA256.h
				$(CC) $(CFLAGS) -c RemoteControlFrame.cpp

RemoteControlLinkData.o:	RemoteControlLinkData.cpp RemoteControlLinkData.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c RemoteControlLinkData.cpp

RemoteControlPreferences.o:	RemoteControlPreferences.cpp RemoteControlPreferences.h  RemoteControlRemoteSet.h RemoteControlDefs.h
				$(CC) $(CFLAGS) -c RemoteControlPreferences.cpp

RemoteControlRemoteControlHandler.o:		RemoteControlRemoteControlHandler.cpp RemoteControlRemoteControlHandler.h RemoteControlCallsignData.h \
				RemoteControlRepeaterData.h RemoteControlStarNetGroup.h ../Common/UDPReaderWriter.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c RemoteControlRemoteControlHandler.cpp

RemoteControlRemoteSet.o:	RemoteControlRemoteSet.cpp RemoteControlRemoteSet.h ../Common/PortTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c RemoteControlRemoteSet.cpp

RemoteControlRepeaterData.o:	RemoteControlRepeaterData.cpp RemoteControlRepeaterData.h RemoteControlLinkData.h
				$(CC) $(CFLAGS) -c RemoteControlRepeaterData.cpp

RemoteControlRepeaterPanel.o:	RemoteControlRepeaterPanel.cpp RemoteControlRepeaterPanel.h RemoteControlApp.h RemoteControlRepeaterData.h \
								../Common/DStarDefines.h ../Common/HostFile.h
				$(CC) $(CFLAGS) -c RemoteControlRepeaterPanel.cpp

RemoteControlStarNetGroup.o:	RemoteControlStarNetGroup.cpp RemoteControlStarNetGroup.h RemoteControlStarNetUser.h
				$(CC) $(CFLAGS) -c RemoteControlStarNetGroup.cpp

RemoteControlStarNetPanel.o:	RemoteControlStarNetPanel.cpp RemoteControlStarNetPanel.h RemoteControlApp.h RemoteControlStarNetGroup.h
				$(CC) $(CFLAGS) -c RemoteControlStarNetPanel.cpp

RemoteControlStarNetUser.o:	RemoteControlStarNetUser.cpp RemoteControlStarNetUser.h
				$(CC) $(CFLAGS) -c RemoteControlStarNetUser.cpp

install:		remotecontrol remotecontrold
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 remotecontrol $(BINDIR)
				install -g bin -o root -m 0775 remotecontrold $(BINDIR)

clean:
				rm -f core remotecontrol remotecontrold *.o *~ *.bak

