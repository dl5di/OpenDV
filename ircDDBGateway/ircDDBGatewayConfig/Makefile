all:			ircddbgatewayconfig

CFLAGS :=		-I../Common $(CFLAGS)

ircddbgatewayconfig:	IRCDDBGatewayConfigApp.o IRCDDBGatewayConfigFrame.o IRCDDBGatewayConfigGatewaySet.o IRCDDBGatewayConfigIrcDDBSet.o \
				IRCDDBGatewayConfigMiscellaneousSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o ircddbgatewayconfig IRCDDBGatewayConfigApp.o IRCDDBGatewayConfigFrame.o IRCDDBGatewayConfigGatewaySet.o \
				IRCDDBGatewayConfigIrcDDBSet.o IRCDDBGatewayConfigMiscellaneousSet.o ../Common/Common.a $(LIBS)

IRCDDBGatewayConfigApp.o:	IRCDDBGatewayConfigApp.cpp IRCDDBGatewayConfigApp.h IRCDDBGatewayConfigFrame.h IRCDDBGatewayConfigDefs.h ../Common/Defs.h \
				../Common/Version.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigApp.cpp

IRCDDBGatewayConfigFrame.o:	IRCDDBGatewayConfigFrame.cpp IRCDDBGatewayConfigFrame.h ../Common/Defs.h ../Common/Version.h ../Common/IRCDDBGatewayConfig.h \
				IRCDDBGatewayConfigMiscellaneousSet.h IRCDDBGatewayConfigGatewaySet.h IRCDDBGatewayConfigIrcDDBSet.h ../Common/RepeaterDataSet.h ../Common/DPRSSet.h \
				../Common/DPlusSet.h ../Common/RemoteSet.h ../Common/StarNetSet.h ../Common/DExtraSet.h ../Common/DCSSet.h ../Common/RepeaterInfoSet.h
				$(CC) $(CFLAGS) -c IRCDDBGatewayConfigFrame.cpp

IRCDDBGatewayConfigGatewaySet.o:	IRCDDBGatewayConfigGatewaySet.cpp IRCDDBGatewayConfigGatewaySet.h ../Common/DStarDefines.h ../Common/AddressTextCtrl.h \
				../Common/PortTextCtrl.h ../Common/CallsignTextCtrl.h ../Common/DescriptionTextCtrl.h ../Common/Defs.h
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
