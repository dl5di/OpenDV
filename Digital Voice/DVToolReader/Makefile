all:			DVToolReader

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DVToolReaderApp.o DVToolReaderFrame.o DVToolReaderPreferences.o DVToolReaderThread.o DVToolReaderUserSoundcardSet.o

DVToolReader:	$(OBJS)
				$(CC) $(LDFLAGS) -o DVToolReader $(OBJS) ../Common/Common.a $(LIBS)

DVToolReaderApp.o:	DVToolReaderApp.cpp DVToolReaderApp.h DVToolReaderThread.h DVToolReaderFrame.h DVToolReaderDefs.h ../Common/DVTOOLFileReader.h \
				../Common/SerialDataController.h ../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h \
				../Common/Logger.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVToolReaderApp.cpp

DVToolReaderFrame.o:	DVToolReaderFrame.cpp DVToolReaderFrame.h DVToolReaderPreferences.h DVToolReaderDefs.h DVToolReaderApp.h DVToolReader.xpm \
				../Common/DVTOOLFileReader.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/MessageEvent.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DVToolReaderFrame.cpp

DVToolReaderPreferences.o:	DVToolReaderPreferences.cpp DVToolReaderPreferences.h DVToolReaderDefs.h DVToolReaderUserSoundcardSet.h \
				../Common/DVDongleSet.h ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DVToolReaderPreferences.cpp

DVToolReaderThread.o:	DVToolReaderThread.cpp DVToolReaderThread.h DVToolReaderDefs.h DVToolReaderApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/DVDongleController.h ../Common/DVTOOLFileWriter.h ../Common/SlowDataDecoder.h \
				../Common/DVTOOLFileReader.h ../Common/AudioCallback.h ../Common/MessageData.h ../Common/RingBuffer.h ../Common/HeaderData.h \
				../Common/Bleeper.h ../Common/DVDongleThread.h
				$(CC) $(CFLAGS) -c DVToolReaderThread.cpp

DVToolReaderUserSoundcardSet.o:	DVToolReaderUserSoundcardSet.cpp DVToolReaderUserSoundcardSet.h ../Common/SoundCardReaderWriter.h
				$(CC) $(CFLAGS) -c DVToolReaderUserSoundcardSet.cpp

install:		DVToolReader
				install -g bin -o root -m 0775 DVToolReader $(BINDIR)

clean:
				rm -f core DVToolReader *.o *~ *.bak
