all:			dvtool2wav

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			dvtool2wav.o

dvtool2wav:	$(OBJS)
				$(CC) $(LDFLAGS) -o dvtool2wav $(OBJS) ../Common/Common.a $(LIBS)

dvtool2wav.o:	dvtool2wav.cpp dvtool2wav.h ../Common/WAVFileWriter.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Version.h ../Common/DecodeCallback.h \
				../Common/DongleThread.h ../Common/DV3000Controller.h ../Common/DVDongleController.h ../Common/AMBE3000Thread.h ../Common/DVDongleThread.h \
				../Common/DVTOOLFileReader.h
				$(CC) $(CFLAGS) -c dvtool2wav.cpp

install:		dvtool2wav
				install -g bin -o root -m 0775 dvtool2wav $(BINDIR)

clean:
				rm -f core dvtool2wav *.o *~ *.bak
