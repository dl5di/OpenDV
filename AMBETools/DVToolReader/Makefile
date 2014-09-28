all:			dvtoolreader

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DVToolReaderApp.o DVToolReaderFrame.o DVToolReaderPreferences.o DVToolReaderThread.o

dvtoolreader:	$(OBJS)
				$(CC) $(LDFLAGS) -o dvtoolreader $(OBJS) ../Common/Common.a $(LIBS)

DVToolReaderApp.o:	DVToolReaderApp.cpp DVToolReaderApp.h DVToolReaderThread.h DVToolReaderFrame.h DVToolReaderDefs.h ../Common/FileReader.h \
				../Common/DStarDefines.h ../Common/MessageData.h ../Common/DongleSet.h ../Common/HeaderData.h ../Common/Logger.h ../Common/Version.h \
				../Common/DongleThread.h ../Common/DV3000Controller.h ../Common/DVDongleController.h ../Common/AMBE3000Thread.h ../Common/DVDongleThread.h
				$(CC) $(CFLAGS) -c DVToolReaderApp.cpp

DVToolReaderFrame.o:	DVToolReaderFrame.cpp DVToolReaderFrame.h DVToolReaderPreferences.h DVToolReaderDefs.h DVToolReaderApp.h ../Common/AMBEFileReader.h \
				../Common/DVTOOLFileReader.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/MessageEvent.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/Version.h ../Common/FileReader.h
				$(CC) $(CFLAGS) -c DVToolReaderFrame.cpp

DVToolReaderPreferences.o:	DVToolReaderPreferences.cpp DVToolReaderPreferences.h DVToolReaderDefs.h ../Common/SoundcardSet.h \
				../Common/DongleSet.h ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DVToolReaderPreferences.cpp

DVToolReaderThread.o:	DVToolReaderThread.cpp DVToolReaderThread.h DVToolReaderDefs.h DVToolReaderApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/SlowDataDecoder.h ../Common/FileReader.h ../Common/AudioCallback.h ../Common/MessageData.h \
				../Common/RingBuffer.h ../Common/HeaderData.h ../Common/Bleeper.h ../Common/DongleThread.h ../Common/DecodeCallback.h
				$(CC) $(CFLAGS) -c DVToolReaderThread.cpp

install:		dvtoolreader
				install -g bin -o root -m 0775 dvtoolreader $(BINDIR)

clean:
				rm -f core dvtoolreader *.o *~ *.bak
