all:		Common.a

OBJS :=	AudioDelay.o Bleeper.o BleepSet.o BusyData.o BusyEvent.o CallsignTextCtrl.o CCITTChecksum.o CCITTChecksumReverse.o DCSProtocolHandler.o \
		DExtraProtocolHandler.o DStarFECDecoder.o DStarFECEncoder.o DStarGMSKDemodulator.o DStarGMSKModulator.o DStarInterleaver.o DStarScrambler.o \
		DummyController.o DVAPController.o DVDongleController.o DVDongleSet.o DVDongleThread.o DVRPTRControllerV1.o DVRPTRControllerV2.o DVTOOLFileReader.o \
		DVTOOLFileWriter.o ErrorEvent.o FIRFilter.o GMSKModemLibUsb.o HeaderData.o HeaderEvent.o InversionSet.o K8055Controller.o LimitedLengthBuffer.o \
		LimitedPatternMatcher.o Logger.o MessageData.o MessageEvent.o MessageSet.o MessageTextCtrl.o PatternMatcher.o PTTDelay.o RadioHeaderDecoder.o \
		RadioHeaderEncoder.o RestrictedTextCtrl.o SerialController.o SerialDataController.o SlowDataDecoder.o SlowDataEncoder.o SoundCardReaderWriter.o \
		SoundcardSet.o Timer.o UDPReaderWriter.o URIUSBController.o Utils.o WAVFileReader.o WAVFileWriter.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

AudioDelay.o:	AudioDelay.cpp AudioDelay.h
		$(CC) $(CFLAGS) -c AudioDelay.cpp

Bleeper.o:	Bleeper.cpp Bleeper.h
		$(CC) $(CFLAGS) -c Bleeper.cpp

BleepSet.o:	BleepSet.cpp BleepSet.h
		$(CC) $(CFLAGS) -c BleepSet.cpp

BusyData.o:	BusyData.cpp BusyData.h
		$(CC) $(CFLAGS) -c BusyData.cpp

BusyEvent.o:	BusyEvent.cpp BusyEvent.h BusyData.h
		$(CC) $(CFLAGS) -c BusyEvent.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksum.o:	CCITTChecksum.cpp CCITTChecksum.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksum.cpp

CCITTChecksumReverse.o:	CCITTChecksumReverse.cpp CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksumReverse.cpp

DCSProtocolHandler.o:	DCSProtocolHandler.cpp DCSProtocolHandler.h DStarDefines.h Utils.h UDPReaderWriter.h HeaderData.h Timer.h Version.h
		$(CC) $(CFLAGS) -c DCSProtocolHandler.cpp

DExtraProtocolHandler.o:	DExtraProtocolHandler.cpp DExtraProtocolHandler.h DStarDefines.h Utils.h UDPReaderWriter.h HeaderData.h Timer.h
		$(CC) $(CFLAGS) -c DExtraProtocolHandler.cpp

DStarFECDecoder.o:	DStarFECDecoder.cpp DStarFECDecoder.h FECDecoder.h
		$(CC) $(CFLAGS) -c DStarFECDecoder.cpp

DStarFECEncoder.o:	DStarFECEncoder.cpp DStarFECEncoder.h FECEncoder.h
		$(CC) $(CFLAGS) -c DStarFECEncoder.cpp

DStarGMSKDemodulator.o:	DStarGMSKDemodulator.cpp DStarGMSKDemodulator.h Demodulator.h FIRFilter.h DStarDefines.h
		$(CC) $(CFLAGS) -c DStarGMSKDemodulator.cpp

DStarGMSKModulator.o:	DStarGMSKModulator.cpp DStarGMSKModulator.h DStarDefines.h Modulator.h FIRFilter.h
		$(CC) $(CFLAGS) -c DStarGMSKModulator.cpp

DStarInterleaver.o:	DStarInterleaver.cpp DStarInterleaver.h DStarDefines.h Interleaver.h
		$(CC) $(CFLAGS) -c DStarInterleaver.cpp

DStarScrambler.o:	DStarScrambler.cpp DStarScrambler.h Scrambler.h
		$(CC) $(CFLAGS) -c DStarScrambler.cpp

DummyController.o:	DummyController.cpp DummyController.h ExternalController.h
		$(CC) $(CFLAGS) -c DummyController.cpp

DVAPController.o:	DVAPController.cpp DVAPController.h CCITTChecksumReverse.h DStarDefines.h SerialDataController.h RingBuffer.h HeaderData.h Utils.h
		$(CC) $(CFLAGS) -c DVAPController.cpp

DVDongleController.o:	DVDongleController.cpp DVDongleController.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVDongleController.cpp

DVDongleSet.o:	DVDongleSet.cpp DVDongleSet.h SerialDataController.h
		$(CC) $(CFLAGS) -c DVDongleSet.cpp

DVDongleThread.o:	DVDongleThread.cpp DVDongleThread.h DVDongleController.h DStarDefines.h RingBuffer.h Bleeper.h
		$(CC) $(CFLAGS) -c DVDongleThread.cpp

DVRPTRControllerV1.o:	DVRPTRControllerV1.cpp DVRPTRControllerV1.h DVRPTRController.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h CCITTChecksum.h \
						CCITTChecksumReverse.h Timer.h
		$(CC) $(CFLAGS) -c DVRPTRControllerV1.cpp

DVRPTRControllerV2.o:	DVRPTRControllerV2.cpp DVRPTRControllerV2.h DVRPTRController.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h Timer.h
		$(CC) $(CFLAGS) -c DVRPTRControllerV2.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h
		$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

DVTOOLFileWriter.o:	DVTOOLFileWriter.cpp DVTOOLFileWriter.h HeaderData.h CCITTChecksumReverse.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVTOOLFileWriter.cpp

ErrorEvent.o:	ErrorEvent.cpp ErrorEvent.h
		$(CC) $(CFLAGS) -c ErrorEvent.cpp

FIRFilter.o:	FIRFilter.cpp FIRFilter.h Filter.h
		$(CC) $(CFLAGS) -c FIRFilter.cpp

GMSKModemLibUsb.o:	GMSKModemLibUsb.cpp GMSKModemLibUsb.h GMSKModem.h HeaderData.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c GMSKModemLibUsb.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c HeaderData.cpp

HeaderEvent.o:	HeaderEvent.cpp HeaderEvent.h HeaderData.h
		$(CC) $(CFLAGS) -c HeaderEvent.cpp

InversionSet.o:	InversionSet.cpp InversionSet.h
		$(CC) $(CFLAGS) -c InversionSet.cpp

K8055Controller.o:	K8055Controller.cpp K8055Controller.h ExternalController.h
		$(CC) $(CFLAGS) -c K8055Controller.cpp

LimitedLengthBuffer.o:	LimitedLengthBuffer.cpp LimitedLengthBuffer.h
		$(CC) $(CFLAGS) -c LimitedLengthBuffer.cpp

LimitedPatternMatcher.o:	LimitedPatternMatcher.cpp LimitedPatternMatcher.h PatternMatcher.h
		$(CC) $(CFLAGS) -c LimitedPatternMatcher.cpp

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

PatternMatcher.o:	PatternMatcher.cpp PatternMatcher.h
		$(CC) $(CFLAGS) -c PatternMatcher.cpp

PTTDelay.o:	PTTDelay.cpp PTTDelay.h
		$(CC) $(CFLAGS) -c PTTDelay.cpp

RadioHeaderDecoder.o:	RadioHeaderDecoder.cpp RadioHeaderDecoder.h DStarDefines.h RadioHeaderEncoder.h DStarInterleaver.h \
			DStarFECDecoder.h DStarScrambler.h CCITTChecksumReverse.h HeaderData.h
		$(CC) $(CFLAGS) -c RadioHeaderDecoder.cpp

RadioHeaderEncoder.o:	RadioHeaderEncoder.cpp RadioHeaderEncoder.h DStarDefines.h DStarInterleaver.h DStarFECEncoder.h \
			DStarScrambler.h CCITTChecksumReverse.h HeaderData.h
		$(CC) $(CFLAGS) -c RadioHeaderEncoder.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SerialController.o:	SerialController.cpp SerialController.h ExternalController.h
		$(CC) $(CFLAGS) -c SerialController.cpp

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

Timer.o:		Timer.cpp Timer.h
		$(CC) $(CFLAGS) -c Timer.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h
		$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

URIUSBController.o:	URIUSBController.cpp URIUSBController.h ExternalController.h
		$(CC) $(CFLAGS) -c URIUSBController.cpp

Utils.o:		Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

WAVFileReader.o:	WAVFileReader.cpp WAVFileReader.h
		$(CC) $(CFLAGS) -c WAVFileReader.cpp

WAVFileWriter.o:	WAVFileWriter.cpp WAVFileWriter.h
		$(CC) $(CFLAGS) -c WAVFileWriter.cpp

clean:
		rm -f core Common.a *.o *~ *.bak
