all:		Common.a

OBJS :=	AddressTextCtrl.o CallsignTextCtrl.o CCITTChecksumReverse.o DVTOOLFileReader.o DVTOOLFileWriter.o HeaderData.o Logger.o PortTextCtrl.o \
		RestrictedTextCtrl.o SlowDataEncoder.o Timer.o UDPReaderWriter.o Utils.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksumReverse.o:	CCITTChecksumReverse.cpp CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksumReverse.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h HeaderData.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

DVTOOLFileWriter.o:	DVTOOLFileWriter.cpp DVTOOLFileWriter.h HeaderData.h CCITTChecksumReverse.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVTOOLFileWriter.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c HeaderData.cpp

Logger.o:	Logger.cpp Logger.h
		$(CC) $(CFLAGS) -c Logger.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SlowDataEncoder.o:	SlowDataEncoder.cpp SlowDataEncoder.h DStarDefines.h HeaderData.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c SlowDataEncoder.cpp

Timer.o:	Timer.cpp Timer.h
		$(CC) $(CFLAGS) -c Timer.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h Utils.h
		$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

Utils.o:	Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

clean:
		rm -f core Common.a *.o *~ *.bak

