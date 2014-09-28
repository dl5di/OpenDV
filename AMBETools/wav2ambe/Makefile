all:			wav2ambe

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			wav2ambe.o

wav2ambe:	$(OBJS)
				$(CC) $(LDFLAGS) -o wav2ambe $(OBJS) ../Common/Common.a $(LIBS)

wav2ambe.o:	wav2ambe.cpp wav2ambe.h ../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/Version.h ../Common/EncodeCallback.h \
				../Common/DongleThread.h ../Common/DV3000Controller.h ../Common/DVDongleController.h ../Common/AMBE3000Thread.h ../Common/DVDongleThread.h \
				../Common/AMBEFileWriter.h ../Common/Version.h
				$(CC) $(CFLAGS) -c wav2ambe.cpp

install:		wav2ambe
				install -g bin -o root -m 0775 wav2ambe $(BINDIR)

clean:
				rm -f core wav2ambe *.o *~ *.bak
