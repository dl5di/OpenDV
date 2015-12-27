all:			aprstransmit

CFLAGS :=		-I../Common $(CFLAGS)

aprstransmit:	APRSTransmit.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o aprstransmit APRSTransmit.o ../Common/Common.a $(LIBS)

APRSTransmit.o:		APRSTransmit.cpp APRSTransmit.h ../Common/UDPReaderWriter.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/SlowDataEncoder.h \
									../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c APRSTransmit.cpp

install:		aprstransmit
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 aprstransmit $(BINDIR)

clean:
				rm -f core aprstransmit *.o *~ *.bak
