all:			texttransmit

CFLAGS :=		-I../Common $(CFLAGS)

texttransmit:	TextTransmit.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o texttransmit TextTransmit.o ../Common/Common.a $(LIBS)

TextTransmit.o:		TextTransmit.cpp TextTransmit.h ../Common/UDPReaderWriter.h ../Common/HeaderData.h ../Common/AMBEData.h ../Common/SlowDataEncoder.h \
									../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c TextTransmit.cpp

install:		texttransmit
				install -d -g bin -o root -m 0775 $(BINDIR)
				install -g bin -o root -m 0775 texttransmit $(BINDIR)

clean:
				rm -f core texttransmit *.o *~ *.bak
