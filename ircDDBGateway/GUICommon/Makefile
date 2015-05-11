all:			GUICommon.a

CFLAGS :=		-I../Common $(CFLAGS)

GUICommon.a:		AddressTextCtrl.o CallsignTextCtrl.o DCSSet.o DescriptionTextCtrl.o DExtraSet.o DPlusSet.o DPRSSet.o PortTextCtrl.o RemoteSet.o RepeaterDataSet.o \
				RepeaterInfoSet.o RestrictedTextCtrl.o StarNetSet.o
				$(AR) rcs GUICommon.a AddressTextCtrl.o CallsignTextCtrl.o DCSSet.o DescriptionTextCtrl.o DExtraSet.o DPlusSet.o DPRSSet.o PortTextCtrl.o RemoteSet.o \
				RepeaterDataSet.o RepeaterInfoSet.o RestrictedTextCtrl.o StarNetSet.o

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

DescriptionTextCtrl.o:	DescriptionTextCtrl.cpp DescriptionTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c DescriptionTextCtrl.cpp

DCSSet.o:		DCSSet.cpp DCSSet.h HostFile.h Defs.h
				$(CC) $(CFLAGS) -c DCSSet.cpp

DExtraSet.o:	DExtraSet.cpp DExtraSet.h
				$(CC) $(CFLAGS) -c DExtraSet.cpp

DPlusSet.o:		DPlusSet.cpp DPlusSet.h Defs.h CallsignTextCtrl.h DStarDefines.h
				$(CC) $(CFLAGS) -c DPlusSet.cpp

DPRSSet.o:		DPRSSet.cpp DPRSSet.h PortTextCtrl.h
				$(CC) $(CFLAGS) -c DPRSSet.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RemoteSet.o:	RemoteSet.cpp RemoteSet.h PortTextCtrl.h DStarDefines.h
				$(CC) $(CFLAGS) -c RemoteSet.cpp

RepeaterDataSet.o:	RepeaterDataSet.cpp RepeaterDataSet.h Defs.h DStarDefines.h AddressTextCtrl.h PortTextCtrl.h HostFile.h
				$(CC) $(CFLAGS) -c RepeaterDataSet.cpp

RepeaterInfoSet.o:	RepeaterInfoSet.cpp RepeaterInfoSet.h Defs.h DStarDefines.h DescriptionTextCtrl.h
				$(CC) $(CFLAGS) -c RepeaterInfoSet.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
				$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

StarNetSet.o:	StarNetSet.cpp StarNetSet.h CallsignTextCtrl.h DStarDefines.h Defs.h
				$(CC) $(CFLAGS) -c StarNetSet.cpp

clean:
				rm -f core GUICommon.a *.o *~ *.bak
