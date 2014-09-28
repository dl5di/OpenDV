all:		Common.a

OBJS :=	AddressTextCtrl.o AMBE3000Thread.o AMBEFileReader.o AMBEFileWriter.o Bleeper.o BleepSet.o CallsignSet.o CallsignTextCtrl.o CCITTChecksumReverse.o \
		DongleThread.o DV3000Controller.o DVDongleController.o DongleSet.o DVDongleThread.o DVTOOLFileReader.o DVTOOLFileWriter.o ErrorEvent.o FileReader.o \
		FileWriter.o FIRFilter.o HeaderData.o HeaderEvent.o Logger.o MessageData.o MessageEvent.o MessageSet.o MessageTextCtrl.o PortTextCtrl.o RestrictedTextCtrl.o \
		SerialDataController.o SlowDataDecoder.o SlowDataEncoder.o SoundCardReaderWriter.o SoundcardSet.o UDPReaderWriter.o Utils.o WAVFileReader.o WAVFileWriter.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

AMBE3000Thread.o:	AMBE3000Thread.cpp AMBE3000Thread.h DongleThread.h DV3000Controller.h DStarDefines.h
		$(CC) $(CFLAGS) -c AMBE3000Thread.cpp

AMBEFileReader.o:	AMBEFileReader.cpp AMBEFileReader.h FileReader.h DStarDefines.h
		$(CC) $(CFLAGS) -c AMBEFileReader.cpp

AMBEFileWriter.o:	AMBEFileWriter.cpp AMBEFileWriter.h FileWriter.h DStarDefines.h
		$(CC) $(CFLAGS) -c AMBEFileWriter.cpp

Bleeper.o:	Bleeper.cpp Bleeper.h
		$(CC) $(CFLAGS) -c Bleeper.cpp

BleepSet.o:	BleepSet.cpp BleepSet.h
		$(CC) $(CFLAGS) -c BleepSet.cpp

CallsignSet.o:	CallsignSet.cpp CallsignSet.h CallsignTextCtrl.h DStarDefines.h
		$(CC) $(CFLAGS) -c CallsignSet.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksumReverse.o:	CCITTChecksumReverse.cpp CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksumReverse.cpp

DongleThread.o:	DongleThread.cpp DongleThread.h EncodeCallback.h DecodeCallback.h FIRFilter.h Bleeper.h RingBuffer.h
		$(CC) $(CFLAGS) -c DongleThread.cpp

DV3000Controller.o:	DV3000Controller.cpp DV3000Controller.h DStarDefines.h UDPReaderWriter.h Utils.h
		$(CC) $(CFLAGS) -c DV3000Controller.cpp

DongleSet.o:	DongleSet.cpp DongleSet.h SerialDataController.h AddressTextCtrl.h PortTextCtrl.h
		$(CC) $(CFLAGS) -c DongleSet.cpp

DVDongleController.o:	DVDongleController.cpp DVDongleController.h DStarDefines.h SerialDataController.h
		$(CC) $(CFLAGS) -c DVDongleController.cpp

DVDongleThread.o:	DVDongleThread.cpp DVDongleThread.h DongleThread.h DVDongleController.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVDongleThread.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h FileReader.h
		$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

DVTOOLFileWriter.o:	DVTOOLFileWriter.cpp DVTOOLFileWriter.h FileWriter.h CCITTChecksumReverse.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVTOOLFileWriter.cpp

ErrorEvent.o:	ErrorEvent.cpp ErrorEvent.h
		$(CC) $(CFLAGS) -c ErrorEvent.cpp

FileReader.o:	FileReader.cpp FileReader.h
		$(CC) $(CFLAGS) -c FileReader.cpp

FileWriter.o:	FileWriter.cpp FileWriter.h HeaderData.h
		$(CC) $(CFLAGS) -c FileWriter.cpp

FIRFilter.o:	FIRFilter.cpp FIRFilter.h
		$(CC) $(CFLAGS) -c FIRFilter.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c HeaderData.cpp

HeaderEvent.o:	HeaderEvent.cpp HeaderEvent.h HeaderData.h
		$(CC) $(CFLAGS) -c HeaderEvent.cpp

Logger.o:	Logger.cpp Logger.h
		$(CC) $(CFLAGS) -c Logger.cpp

MessageData.o:	MessageData.cpp MessageData.h
		$(CC) $(CFLAGS) -c MessageData.cpp

MessageEvent.o:	MessageEvent.cpp MessageEvent.h MessageData.h
		$(CC) $(CFLAGS) -c MessageEvent.cpp

MessageSet.o:	MessageSet.cpp MessageSet.h MessageTextCtrl.h
		$(CC) $(CFLAGS) -c MessageSet.cpp

MessageTextCtrl.o:	MessageTextCtrl.cpp MessageTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c MessageTextCtrl.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SerialDataController.o:	SerialDataController.cpp SerialDataController.h
		$(CC) $(CFLAGS) -c SerialDataController.cpp

SlowDataDecoder.o:	SlowDataDecoder.cpp SlowDataDecoder.h DStarDefines.h MessageData.h HeaderData.h
		$(CC) $(CFLAGS) -c SlowDataDecoder.cpp

SlowDataEncoder.o:	SlowDataEncoder.cpp SlowDataEncoder.h DStarDefines.h CCITTChecksumReverse.h MessageData.h HeaderData.h
		$(CC) $(CFLAGS) -c SlowDataEncoder.cpp

SoundCardReaderWriter.o:	SoundCardReaderWriter.cpp SoundCardReaderWriter.h AudioCallback.h
		$(CC) $(CFLAGS) -c SoundCardReaderWriter.cpp

SoundcardSet.o:	SoundcardSet.cpp SoundcardSet.h SoundCardReaderWriter.h
		$(CC) $(CFLAGS) -c SoundcardSet.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h Utils.h
		$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

Utils.o:		Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

WAVFileReader.o:	WAVFileReader.cpp WAVFileReader.h
		$(CC) $(CFLAGS) -c WAVFileReader.cpp

WAVFileWriter.o:	WAVFileWriter.cpp WAVFileWriter.h
		$(CC) $(CFLAGS) -c WAVFileWriter.cpp

clean:
		rm -f core Common.a *.o *~ *.bak
