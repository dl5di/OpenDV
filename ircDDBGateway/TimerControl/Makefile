all:			timercontrol timercontrold

CFLAGS :=		-I../Common $(CFLAGS)

timercontrol:	TimerControlApp.o TimerControlConfig.o TimerControlFrame.o TimerControlItemFile.o TimerControlPreferences.o \
				TimerControlRemoteControlHandler.o TimerControlRemoteSet.o TimerControlRepeaterPanel.o TimerControlThread.o TimerControlThreadHelper.o \
				../Common/Common.a
				$(CC) $(LDFLAGS) -o timercontrol TimerControlApp.o TimerControlConfig.o TimerControlFrame.o TimerControlItemFile.o \
				TimerControlPreferences.o TimerControlRemoteControlHandler.o TimerControlRemoteSet.o TimerControlRepeaterPanel.o \
				TimerControlThread.o TimerControlThreadHelper.o ../Common/Common.a $(LIBS)

timercontrold:	TimerControlAppD.o TimerControlConfig.o TimerControlItemFile.o TimerControlRemoteControlHandler.o TimerControlThread.o \
				../Common/Common.a
				$(CC) $(LDFLAGS) -o timercontrold TimerControlAppD.o TimerControlConfig.o TimerControlItemFile.o TimerControlRemoteControlHandler.o \
				TimerControlThread.o ../Common/Common.a $(LIBS)

TimerControlApp.o:		TimerControlApp.cpp TimerControlApp.h TimerControlFrame.h TimerControlConfig.h TimerControlDefs.h TimerControlThread.h \
				TimerControlThreadHelper.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c TimerControlApp.cpp

TimerControlAppD.o:		TimerControlAppD.cpp TimerControlAppD.h TimerControlFrame.h TimerControlConfig.h TimerControlDefs.h ../Common/Logger.h \
				../Common/Version.h
				$(CC) $(CFLAGS) -c TimerControlAppD.cpp

TimerControlConfig.o:	TimerControlConfig.cpp TimerControlConfig.h
				$(CC) $(CFLAGS) -c TimerControlConfig.cpp

TimerControlFrame.o:	TimerControlFrame.cpp TimerControlFrame.h TimerControlRemoteControlHandler.h TimerControlRepeaterPanel.h \
				TimerControlItemFile.h TimerControlPreferences.h TimerControlApp.h TimerControlDefs.h TimerControlItem.h ../Common/Defs.h \
				../Common/Version.h ../Common/SHA256.h
				$(CC) $(CFLAGS) -c TimerControlFrame.cpp

TimerControlItemFile.o:	TimerControlItemFile.cpp TimerControlItemFile.h  TimerControlItem.h
				$(CC) $(CFLAGS) -c TimerControlItemFile.cpp

TimerControlPreferences.o:	TimerControlPreferences.cpp TimerControlPreferences.h  TimerControlRemoteSet.h TimerControlDefs.h
				$(CC) $(CFLAGS) -c TimerControlPreferences.cpp

TimerControlRemoteControlHandler.o:		TimerControlRemoteControlHandler.cpp TimerControlRemoteControlHandler.h \
				../Common/UDPReaderWriter.h ../Common/DStarDefines.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c TimerControlRemoteControlHandler.cpp

TimerControlRemoteSet.o:	TimerControlRemoteSet.cpp TimerControlRemoteSet.h ../Common/PortTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c TimerControlRemoteSet.cpp

TimerControlRepeaterPanel.o:	TimerControlRepeaterPanel.cpp TimerControlRepeaterPanel.h TimerControlApp.h TimerControlItem.h \
								../Common/DStarDefines.h ../Common/HostFile.h
				$(CC) $(CFLAGS) -c TimerControlRepeaterPanel.cpp

TimerControlThreadHelper.o:	TimerControlThreadHelper.cpp TimerControlThreadHelper.h TimerControlThread.h
				$(CC) $(CFLAGS) -c TimerControlThreadHelper.cpp

TimerControlThread.o:	TimerControlThread.cpp TimerControlThread.h TimerControlRemoteControlHandler.h TimerControlItem.h TimerControlItemFile.h \
				../Common/Defs.h ../Common/SHA256.h
				$(CC) $(CFLAGS) -c TimerControlThread.cpp

install:		timercontrol timercontrold
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 timercontrol  $(BINDIR)
				install -g bin -o root -m 0775 timercontrold $(BINDIR)

clean:
				rm -f core timercontrol timercontrold *.o *~ *.bak
