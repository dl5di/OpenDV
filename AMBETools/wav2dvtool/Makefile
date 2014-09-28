all:			wav2dvtool

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			wav2dvtool.o

wav2dvtool:	$(OBJS)
				$(CC) $(LDFLAGS) -o wav2dvtool $(OBJS) ../Common/Common.a $(LIBS)

wav2dvtool.o:	wav2dvtool.cpp wav2dvtool.h ../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Version.h ../Common/EncodeCallback.h \
				../Common/DongleThread.h ../Common/DV3000Controller.h ../Common/DVDongleController.h ../Common/AMBE3000Thread.h ../Common/DVDongleThread.h \
				../Common/DVTOOLFileWriter.h ../Common/SlowDataEncoder.h ../Common/Version.h
				$(CC) $(CFLAGS) -c wav2dvtool.cpp

install:		wav2dvtool
				install -g bin -o root -m 0775 wav2dvtool $(BINDIR)

clean:
				rm -f core wav2dvtool *.o *~ *.bak
