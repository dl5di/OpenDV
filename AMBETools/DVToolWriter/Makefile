all:			dvtoolwriter

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DVToolWriterApp.o DVToolWriterFrame.o DVToolWriterPreferences.o DVToolWriterThread.o

dvtoolwriter:	$(OBJS)
				$(CC) $(LDFLAGS) -o dvtoolwriter $(OBJS) ../Common/Common.a $(LIBS)

DVToolWriterApp.o:	DVToolWriterApp.cpp DVToolWriterApp.h DVToolWriterThread.h DVToolWriterFrame.h DVToolWriterDefs.h ../Common/FileReader.h \
				../Common/DStarDefines.h ../Common/DongleSet.h ../Common/HeaderData.h ../Common/Logger.h ../Common/Version.h \
				../Common/DongleThread.h ../Common/DV3000Controller.h ../Common/DVDongleController.h ../Common/AMBE3000Thread.h ../Common/DVDongleThread.h
				$(CC) $(CFLAGS) -c DVToolWriterApp.cpp

DVToolWriterFrame.o:	DVToolWriterFrame.cpp DVToolWriterFrame.h DVToolWriterPreferences.h DVToolWriterDefs.h DVToolWriterApp.h ../Common/AMBEFileWriter.h \
				../Common/DVTOOLFileWriter.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Version.h ../Common/FileWriter.h \
				../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DVToolWriterFrame.cpp

DVToolWriterPreferences.o:	DVToolWriterPreferences.cpp DVToolWriterPreferences.h DVToolWriterDefs.h ../Common/SoundcardSet.h \
				../Common/DongleSet.h ../Common/CallsignSet.h
				$(CC) $(CFLAGS) -c DVToolWriterPreferences.cpp

DVToolWriterThread.o:	DVToolWriterThread.cpp DVToolWriterThread.h DVToolWriterDefs.h DVToolWriterApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/SlowDataEncoder.h ../Common/FileWriter.h ../Common/AudioCallback.h ../Common/MessageData.h \
				../Common/RingBuffer.h ../Common/HeaderData.h ../Common/Bleeper.h ../Common/DongleThread.h ../Common/EncodeCallback.h ../Common/HeaderData.h \
				../Common/WAVFileReader.h
				$(CC) $(CFLAGS) -c DVToolWriterThread.cpp

install:		dvtoolwriter
				install -g bin -o root -m 0775 dvtoolwriter $(BINDIR)

clean:
				rm -f core dvtoolwriter *.o *~ *.bak
