all:			xreflector xreflectord

# XXX
CFLAGS :=		-I../ircDDB -I../Common $(CFLAGS)

xreflector:		XReflectorApp.o XReflectorConfig.o XReflectorDExtraHandler.o XReflectorDPlusHandler.o XReflectorFrame.o XReflectorLogRedirect.o \
				XReflectorMiscellaneousSet.o XReflectorPreferences.o XReflectorReflectorHandler.o XReflectorReflectorSet.o XReflectorThread.o \
				XReflectorDPlusSet.o XReflectorUserLog.o XReflectorThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o xreflector XReflectorApp.o XReflectorConfig.o XReflectorDExtraHandler.o XReflectorDPlusHandler.o XReflectorFrame.o \
				XReflectorLogRedirect.o XReflectorMiscellaneousSet.o XReflectorPreferences.o XReflectorReflectorHandler.o XReflectorReflectorSet.o \
				XReflectorThread.o XReflectorDPlusSet.o XReflectorUserLog.o XReflectorThreadHelper.o ../Common/Common.a $(LIBS)

xreflectord:	XReflectorAppD.o XReflectorConfig.o XReflectorDExtraHandler.o XReflectorDPlusHandler.o XReflectorReflectorHandler.o XReflectorThread.o \
				XReflectorUserLog.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o xreflectord XReflectorAppD.o XReflectorConfig.o XReflectorDExtraHandler.o XReflectorDPlusHandler.o \
				XReflectorReflectorHandler.o XReflectorThread.o XReflectorUserLog.o ../Common/Common.a $(LIBS)

XReflectorApp.o:	XReflectorApp.cpp XReflectorApp.h XReflectorThread.h XReflectorFrame.h XReflectorConfig.h XReflectorLogRedirect.h \
				XReflectorDefs.h XReflectorReflectorHandler.h XReflectorThreadHelper.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c XReflectorApp.cpp

XReflectorAppD.o:	XReflectorAppD.cpp XReflectorAppD.h XReflectorThread.h XReflectorConfig.h XReflectorDefs.h XReflectorReflectorHandler.h \
				../Common/DStarDefines.h ../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c XReflectorAppD.cpp

XReflectorConfig.o:	XReflectorConfig.cpp XReflectorConfig.h XReflectorDefs.h
				$(CC) $(CFLAGS) -c XReflectorConfig.cpp

XReflectorDExtraHandler.o:	XReflectorDExtraHandler.cpp XReflectorDExtraHandler.h XReflectorReflectorHandler.h XReflectorUserLog.h \
					 XReflectorReflectorCallback.h ../Common/DStarDefines.h ../Common/Utils.h ../Common/DExtraProtocolHandler.h \
					 ../Common/ConnectData.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/PollData.h ../Common/Timer.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c XReflectorDExtraHandler.cpp

XReflectorDPlusHandler.o:	XReflectorDPlusHandler.cpp XReflectorDPlusHandler.h XReflectorReflectorHandler.h XReflectorUserLog.h \
					XReflectorReflectorCallback.h ../Common/DStarDefines.h ../Common/Utils.h ../Common/DPlusProtocolHandler.h ../Common/ConnectData.h \
					../Common/HeaderData.h ../Common/AMBEData.h ../Common/PollData.h ../Common/Timer.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c XReflectorDPlusHandler.cpp

XReflectorDPlusSet.o:	XReflectorDPlusSet.cpp XReflectorDPlusSet.h ../Common/Defs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c XReflectorDPlusSet.cpp

XReflectorFrame.o:	XReflectorFrame.cpp XReflectorFrame.h XReflectorPreferences.h XReflectorApp.h XReflectorDefs.h ../Common/Defs.h \
					../Common/Version.h ../Common/LogEvent.h
				$(CC) $(CFLAGS) -c XReflectorFrame.cpp

XReflectorLogRedirect.o:	XReflectorLogRedirect.cpp XReflectorLogRedirect.h XReflectorApp.h
				$(CC) $(CFLAGS) -c XReflectorLogRedirect.cpp

XReflectorMiscellaneousSet.o:	XReflectorMiscellaneousSet.cpp XReflectorMiscellaneousSet.h ../Common/Defs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c XReflectorMiscellaneousSet.cpp

XReflectorPreferences.o:	XReflectorPreferences.cpp XReflectorPreferences.h  XReflectorMiscellaneousSet.h XReflectorReflectorSet.h \
					XReflectorDefs.h XReflectorDPlusSet.h
				$(CC) $(CFLAGS) -c XReflectorPreferences.cpp

XReflectorReflectorHandler.o:	XReflectorReflectorHandler.cpp XReflectorReflectorHandler.h XReflectorDExtraHandler.h XReflectorDPlusHandler.o \
					XReflectorReflectorCallback.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/DStarDefines.h ../Common/Defs.h \
					../Common/Utils.h ../Common/TextCollector.h ../Common/Timer.h
				$(CC) $(CFLAGS) -c XReflectorReflectorHandler.cpp

XReflectorReflectorSet.o:	XReflectorReflectorSet.cpp XReflectorReflectorSet.h ../Common/DStarDefines.h ../Common/AddressTextCtrl.h \
					../Common/CallsignTextCtrl.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c XReflectorReflectorSet.cpp

XReflectorThreadHelper.o:		XReflectorThreadHelper.cpp XReflectorThreadHelper.h
				$(CC) $(CFLAGS) -c XReflectorThreadHelper.cpp

XReflectorThread.o:		XReflectorThread.cpp XReflectorThread.h XReflectorReflectorHandler.h XReflectorDExtraHandler.h XReflectorDPlusHandler.h \
					XReflectorDefs.h XReflectorUserLog.h ../Common/Defs.h ../Common/HeaderData.h ../Common/AMBEData.h \
					../Common/DExtraProtocolHandler.h ../Common/DPlusProtocolHandler.h ../Common/ConnectData.h ../Common/DStarDefines.h \
					../Common/Utils.h ../Common/Timer.h ../Common/PollData.h
				$(CC) $(CFLAGS) -c XReflectorThread.cpp

XReflectorUserLog.o:	XReflectorUserLog.cpp XReflectorUserLog.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c XReflectorUserLog.cpp

install:		xreflector xreflectord
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 xreflector $(BINDIR)
				install -g bin -o root -m 0775 xreflectord $(BINDIR)

clean:
				rm -f core xreflector xreflectord *.o *~ *.bak
