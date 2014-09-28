all:			ambe2wav

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			ambe2wav.o

ambe2wav:	$(OBJS)
				$(CC) $(LDFLAGS) -o ambe2wav $(OBJS) ../Common/Common.a $(LIBS)

ambe2wav.o:	ambe2wav.cpp ambe2wav.h ../Common/WAVFileWriter.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Version.h ../Common/DecodeCallback.h \
				../Common/DongleThread.h ../Common/DV3000Controller.h ../Common/DVDongleController.h ../Common/AMBE3000Thread.h ../Common/DVDongleThread.h \
				../Common/AMBEFileReader.h
				$(CC) $(CFLAGS) -c ambe2wav.cpp

install:		ambe2wav
				install -g bin -o root -m 0775 ambe2wav $(BINDIR)

clean:
				rm -f core ambe2wav *.o *~ *.bak
