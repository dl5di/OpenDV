all:			aprstransmit aprstransmitd

CFLAGS :=		-I../Common $(CFLAGS)

aprstransmit:	APRSTransmitApp.o APRSTransmit.o
		 $(CC) $(LDFLAGS) -o aprstransmit APRSTransmitApp.o APRSTransmit.o ../Common/Common.a $(LIBS)

aprstransmitd:	APRSTransmitAppD.o APRSTransmit.o
		 $(CC) $(LDFLAGS) -o aprstransmitd APRSTransmitAppD.o APRSTransmit.o ../Common/Common.a $(LIBS)

APRSTransmitApp.o: APRSTransmitApp.cpp ../Common/Common.a
			$(CC) $(CFLAGS) -c APRSTransmitApp.cpp

APRSTransmitAppD.o: APRSTransmitAppD.cpp APRSTransmitAppD.h ../Common/APRSWriterThread.h
			$(CC) $(CFLAGS) -c APRSTransmitAppD.cpp

APRSTransmit.o:		APRSTransmit.cpp APRSTransmit.h ../Common/UDPReaderWriter.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/SlowDataEncoder.h \
									../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c APRSTransmit.cpp

install:		aprstransmit aprstransmitd
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 aprstransmit $(BINDIR)
				install -g bin -o root -m 0775 aprstransmitd $(BINDIR)

clean:
				rm -f core aprstransmit aprstransmitd *.o *~ *.bak
