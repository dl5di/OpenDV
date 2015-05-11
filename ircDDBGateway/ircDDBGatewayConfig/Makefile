all:			ircddbgatewayconfig

CFLAGS :=		-I../Common ../GUICommon $(CFLAGS)

ircddbgatewayconfig:	IRCDDBGatewayConfigApp.o IRCDDBGatewayConfigFrame.o IRCDDBGatewayConfigGatewaySet.o IRCDDBGatewayConfigIrcDDBSet.o \
				IRCDDBGatewayConfigMiscellaneousSet.o ../Common/Common.a ../GUICommon/GUICommon.a
				$(CC) $(LDFLAGS) -o ircddbgatewayconfig IRCDDBGatewayConfigApp.o IRCDDBGatewayConfigFrame.o IRCDDBGatewayConfigGatewaySet.o \
				IRCDDBGatewayConfigIrcDDBSet.o IRCDDBGatewayConfigMiscellaneousSet.o ../Common/Common.a ../GUICommon/GUICommon.a $(LIBS)

IRCDDBGatewayConfigApp.o:	IRCDDBGatewayConfigApp.cpp IRCDDBGatewayConfigApp.h IRCDDBGatewayConfigFrame.h IRCDDBGatewayConfigDefs.h ../Common/Defs.h \
				../Common/Version.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigApp.cpp

IRCDDBGatewayConfigFrame.o:	IRCDDBGatewayConfigFrame.cpp IRCDDBGatewayConfigFrame.h ../Common/Defs.h ../Common/Version.h ../Common/IRCDDBGatewayConfig.h \
				IRCDDBGatewayConfigMiscellaneousSet.h IRCDDBGatewayConfigGatewaySet.h IRCDDBGatewayConfigIrcDDBSet.h ../GUICommon/RepeaterDataSet.h \
				../GUICommon/DPRSSet.h ../GUICommon/DPlusSet.h ../GUICommon/RemoteSet.h ../GUICommon/StarNetSet.h ../GUICommon/DExtraSet.h ../GUICommon/DCSSet.h \
				../GUICommon/RepeaterInfoSet.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigFrame.cpp

IRCDDBGatewayConfigGatewaySet.o:	IRCDDBGatewayConfigGatewaySet.cpp IRCDDBGatewayConfigGatewaySet.h ../Common/DStarDefines.h ../GUICommon/AddressTextCtrl.h \
				../GUICommon/PortTextCtrl.h ../GUICommon/CallsignTextCtrl.h ../GUICommon/DescriptionTextCtrl.h ../Common/Defs.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigGatewaySet.cpp

IRCDDBGatewayConfigIrcDDBSet.o:	IRCDDBGatewayConfigIrcDDBSet.cpp IRCDDBGatewayConfigIrcDDBSet.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigIrcDDBSet.cpp

IRCDDBGatewayConfigMiscellaneousSet.o:	IRCDDBGatewayConfigMiscellaneousSet.cpp IRCDDBGatewayConfigMiscellaneousSet.h ../Common/Defs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigMiscellaneousSet.cpp

install:		ircddbgatewayconfig
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 ircddbgatewayconfig $(BINDIR)

clean:
				rm -f core ircddbgatewayconfig *.o *~ *.bak
