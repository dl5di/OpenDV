all:			parrotcontroller parrotcontrollerd

CFLAGS :=		-I../Common $(CFLAGS)

parrotcontroller:	ParrotControllerApp.o ParrotControllerBeaconSet.o ParrotControllerConfig.o ParrotControllerFrame.o ParrotControllerModeSet.o \
					ParrotControllerNetworkSet.o ParrotControllerParrotSet.o ParrotControllerPreferences.o ParrotControllerProtocolHandler.o \
					ParrotControllerStatusData.o ParrotControllerThread.o ParrotControllerThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o parrotcontroller ParrotControllerApp.o ParrotControllerBeaconSet.o ParrotControllerConfig.o ParrotControllerFrame.o \
				ParrotControllerModeSet.o ParrotControllerNetworkSet.o ParrotControllerParrotSet.o ParrotControllerPreferences.o \
				ParrotControllerProtocolHandler.o ParrotControllerStatusData.o ParrotControllerThread.o ParrotControllerThreadHelper.o ../Common/Common.a $(LIBS)

parrotcontrollerd:	ParrotControllerD.o ParrotControllerConfig.o ParrotControllerProtocolHandler.o ParrotControllerStatusData.o ParrotControllerThread.o \
					../Common/Common.a
				$(CC) $(LDFLAGS) -o parrotcontrollerd ParrotControllerD.o ParrotControllerConfig.o ParrotControllerProtocolHandler.o \
				ParrotControllerStatusData.o ParrotControllerThread.o ../Common/Common.a $(LIBS)

ParrotControllerApp.o:	ParrotControllerApp.cpp ParrotControllerApp.h ParrotControllerThread.h ParrotControllerFrame.h ParrotControllerDefs.h \
						ParrotControllerStatusData.h ParrotControllerConfig.h ParrotControllerProtocolHandler.h ParrotControllerThreadHelper.o ../Common/Logger.h \
						../Common/Version.h
				$(CC) $(CFLAGS) -c ParrotControllerApp.cpp

ParrotControllerD.o:	ParrotControllerD.cpp ParrotControllerD.h ParrotControllerThread.h ParrotControllerDefs.h ParrotControllerConfig.h \
						ParrotControllerProtocolHandler.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c ParrotControllerD.cpp

ParrotControllerBeaconSet.o:	ParrotControllerBeaconSet.cpp ParrotControllerBeaconSet.h ParrotControllerDefs.h
				$(CC) $(CFLAGS) -c ParrotControllerBeaconSet.cpp

ParrotControllerConfig.o:	ParrotControllerConfig.cpp ParrotControllerConfig.h ParrotControllerDefs.h
				$(CC) $(CFLAGS) -c ParrotControllerConfig.cpp

ParrotControllerFrame.o:	ParrotControllerFrame.cpp ParrotControllerFrame.h ParrotControllerPreferences.h ParrotControllerDefs.h ParrotControllerApp.h \
							../Common/Version.h
				$(CC) $(CFLAGS) -c ParrotControllerFrame.cpp

ParrotControllerModeSet.o:	ParrotControllerModeSet.cpp ParrotControllerModeSet.h ParrotControllerDefs.h
				$(CC) $(CFLAGS) -c ParrotControllerModeSet.cpp

ParrotControllerNetworkSet.o:	ParrotControllerNetworkSet.cpp ParrotControllerNetworkSet.h ParrotControllerDefs.h ../Common/AddressTextCtrl.h \
								../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c ParrotControllerNetworkSet.cpp

ParrotControllerPreferences.o:	ParrotControllerPreferences.cpp ParrotControllerPreferences.h ParrotControllerDefs.h ParrotControllerNetworkSet.h \
								ParrotControllerBeaconSet.h ParrotControllerParrotSet.h ParrotControllerModeSet.h
				$(CC) $(CFLAGS) -c ParrotControllerPreferences.cpp

ParrotControllerParrotSet.o:	ParrotControllerParrotSet.cpp ParrotControllerParrotSet.h ParrotControllerDefs.h
				$(CC) $(CFLAGS) -c ParrotControllerParrotSet.cpp

ParrotControllerProtocolHandler.o:	ParrotControllerProtocolHandler.cpp ParrotControllerProtocolHandler.h ../Common/UDPReaderWriter.h ../Common/DStarDefines.h \
									../Common/HeaderData.h ../Common/CCITTChecksumReverse.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c ParrotControllerProtocolHandler.cpp

ParrotControllerStatusData.o:	ParrotControllerStatusData.cpp ParrotControllerStatusData.h ParrotControllerDefs.h
				$(CC) $(CFLAGS) -c ParrotControllerStatusData.cpp

ParrotControllerThreadHelper.o:	ParrotControllerThreadHelper.cpp ParrotControllerThreadHelper.h ParrotControllerThread.h ParrotControllerStatusData.h
				$(CC) $(CFLAGS) -c ParrotControllerThreadHelper.cpp

ParrotControllerThread.o:	ParrotControllerThread.cpp ParrotControllerThread.h ParrotControllerDefs.h ParrotControllerApp.h ParrotControllerStatusData.h \
							ParrotControllerProtocolHandler.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Utils.h ../Common/Timer.h \
							../Common/DVTOOLFileReader.h ../Common/DVTOOLFileWriter.h
				$(CC) $(CFLAGS) -c ParrotControllerThread.cpp

install:	parrotcontroller parrotcontrollerd
				install -g bin -o root -m 0775 parrotcontroller $(BINDIR)
				install -g bin -o root -m 0775 parrotcontrollerd $(BINDIR)

clean:
				rm -f core parrotcontroller parrotcontrollerd *.o *~ *.bak

