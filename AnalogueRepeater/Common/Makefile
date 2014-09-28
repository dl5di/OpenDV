all:		Common.a

OBJS :=	ActiveHangSet.o AddressTextCtrl.o APRSTX.o ArduinoController.o AudioDelay.o AX25Checksum.o AX25TX.o CallsignTextCtrl.o CWKeyer.o CWTextCtrl.o \
		DTMFController.o DTMFDecoder.o DTMFTextCtrl.o DummyController.o ExternalController.o FirstOrderIIR.o FIRFilter.o FixedAudioSource.o Goertzel.o \
		HardwareController.o K8055Controller.o LogEvent.o Logger.o NCO.o NetworkController.o PTTDelay.o RaspberryController.o RestrictedTextCtrl.o \
		SerialDataController.o SerialLineController.o SoundCardReaderWriter.o TimeoutTones.o Timer.o Utils.o VOGAD.o WAVFileReader.o WAVFileStore.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

ActiveHangSet.o:	ActiveHangSet.cpp ActiveHangSet.h
		$(CC) $(CFLAGS) -c ActiveHangSet.cpp

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

APRSTX.o:	APRSTX.cpp APRSTX.h AnalogueDefines.h AX25Checksum.h AX25TX.h Utils.h
		$(CC) $(CFLAGS) -c APRSTX.cpp

ArduinoController.o:	ArduinoController.cpp ArduinoController.h SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c ArduinoController.cpp

AudioDelay.o:	AudioDelay.cpp AudioDelay.h
		$(CC) $(CFLAGS) -c AudioDelay.cpp

AX25Checksum.o:	AX25Checksum.cpp AX25Checksum.h
		$(CC) $(CFLAGS) -c AX25Checksum.cpp

AX25TX.o:	AX25TX.cpp AX25TX.h AnalogueDefines.h
		$(CC) $(CFLAGS) -c AX25TX.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CWKeyer.o:	CWKeyer.cpp CWKeyer.h FixedAudioSource.h
		$(CC) $(CFLAGS) -c CWKeyer.cpp

CWTextCtrl.o:	CWTextCtrl.cpp CWTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CWTextCtrl.cpp

DTMFController.o:	DTMFController.cpp DTMFController.h DTMFDecoder.h
		$(CC) $(CFLAGS) -c DTMFController.cpp

DTMFDecoder.o:	DTMFDecoder.cpp DTMFDecoder.h Goertzel.h
		$(CC) $(CFLAGS) -c DTMFDecoder.cpp

DTMFTextCtrl.o:	DTMFTextCtrl.cpp DTMFTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c DTMFTextCtrl.cpp

DummyController.o:	DummyController.cpp DummyController.h HardwareController.h
		$(CC) $(CFLAGS) -c DummyController.cpp

ExternalController.o:	ExternalController.cpp ExternalController.h AnalogueDefines.h HardwareController.h
		$(CC) $(CFLAGS) -c ExternalController.cpp

FirstOrderIIR.o:	FirstOrderIIR.cpp FirstOrderIIR.h
		$(CC) $(CFLAGS) -c FirstOrderIIR.cpp

FIRFilter.o:	FIRFilter.cpp FIRFilter.h
		$(CC) $(CFLAGS) -c FIRFilter.cpp

FixedAudioSource.o:	FixedAudioSource.cpp FixedAudioSource.h
		$(CC) $(CFLAGS) -c FixedAudioSource.cpp

Goertzel.o:	Goertzel.cpp Goertzel.h Utils.h
		$(CC) $(CFLAGS) -c Goertzel.cpp

HardwareController.o:	HardwareController.cpp HardwareController.h
		$(CC) $(CFLAGS) -c HardwareController.cpp

K8055Controller.o:	K8055Controller.cpp K8055Controller.h HardwareController.h
		$(CC) $(CFLAGS) -c K8055Controller.cpp

LogEvent.o:	LogEvent.cpp LogEvent.h
		$(CC) $(CFLAGS) -c LogEvent.cpp

Logger.o:	Logger.cpp Logger.h
		$(CC) $(CFLAGS) -c Logger.cpp

NCO.o:		NCO.cpp NCO.h
		$(CC) $(CFLAGS) -c NCO.cpp

NetworkController.o:	NetworkController.cpp NetworkController.h SerialLineController.h
		$(CC) $(CFLAGS) -c NetworkController.cpp

PTTDelay.o:	PTTDelay.cpp PTTDelay.h
		$(CC) $(CFLAGS) -c PTTDelay.cpp

RaspberryController.o:	RaspberryController.cpp RaspberryController.h HardwareController.h
		$(CC) $(CFLAGS) -c RaspberryController.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SerialDataController.o:	SerialDataController.cpp SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialDataController.cpp

SerialLineController.o:	SerialLineController.cpp SerialLineController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialLineController.cpp

SoundCardReaderWriter.o:	SoundCardReaderWriter.cpp SoundCardReaderWriter.h AudioCallback.h
		$(CC) $(CFLAGS) -c SoundCardReaderWriter.cpp

TimeoutTones.o:	TimeoutTones.cpp TimeoutTones.h NCO.h
		$(CC) $(CFLAGS) -c TimeoutTones.cpp

Timer.o:	Timer.cpp Timer.h
		$(CC) $(CFLAGS) -c Timer.cpp

Utils.o:	Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

VOGAD.o:	VOGAD.cpp VOGAD.h
		$(CC) $(CFLAGS) -c VOGAD.cpp

WAVFileReader.o:	WAVFileReader.cpp WAVFileReader.h
		$(CC) $(CFLAGS) -c WAVFileReader.cpp

WAVFileStore.o:	WAVFileStore.cpp WAVFileStore.h FixedAudioSource.h WAVFileReader.h AnalogueDefines.h
		$(CC) $(CFLAGS) -c WAVFileStore.cpp

clean:
		rm -f core Common.a *.o *~ *.bak
