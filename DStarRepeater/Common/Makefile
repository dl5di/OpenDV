all:		Common.a

OBJS :=	AddressTextCtrl.o AMBEFEC.o AnnouncementUnit.o ArduinoController.o BeaconUnit.o CallsignList.o CallsignTextCtrl.o CCITTChecksum.o CCITTChecksumReverse.o \
		DStarGMSKDemodulator.o DStarGMSKModulator.o DStarRepeaterConfig.o DummyController.o DVAPController.o DVMegaController.o DVRPTRV1Controller.o \
		DVRPTRV2Controller.o DVRPTRV3Controller.o DVTOOLFileReader.o DVTOOLFileWriter.o ExternalController.o FIRFilter.o GatewayProtocolHandler.o GMSKController.o \
		GMSKModem.o GMSKModemLibUsb.o Golay.o HardwareController.o HeaderData.o K8055Controller.o LogEvent.o Logger.o MessageTextCtrl.o Modem.o OutputQueue.o \
		PortTextCtrl.o RaspberryController.o RepeaterProtocolHandler.o RestrictedTextCtrl.o SerialDataController.o SerialLineController.o SlowDataDecoder.o \
		SlowDataEncoder.o SoundCardController.o SoundCardReaderWriter.o SplitController.o TCPReaderWriter.o Timer.o UDPReaderWriter.o Utils.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

AMBEFEC.o:	AMBEFEC.cpp AMBEFEC.h Golay.h
		$(CC) $(CFLAGS) -c AMBEFEC.cpp

AnnouncementUnit.o:	AnnouncementUnit.cpp AnnouncementUnit.h AnnouncementCallback.h HeaderData.h DStarDefines.h DVTOOLFileReader.h DVTOOLFileWriter.h
		$(CC) $(CFLAGS) -c AnnouncementUnit.cpp

ArduinoController.o:	ArduinoController.cpp ArduinoController.h SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c ArduinoController.cpp

BeaconUnit.o:	BeaconUnit.cpp BeaconUnit.h BeaconCallback.h SlowDataEncoder.h DStarDefines.h
		$(CC) $(CFLAGS) -c BeaconUnit.cpp

CallsignList.o:	CallsignList.cpp CallsignList.h DStarDefines.h
		$(CC) $(CFLAGS) -c CallsignList.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksum.o:	CCITTChecksum.cpp CCITTChecksum.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksum.cpp

CCITTChecksumReverse.o:	CCITTChecksumReverse.cpp CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksumReverse.cpp

DStarGMSKDemodulator.o:	DStarGMSKDemodulator.cpp DStarGMSKDemodulator.h FIRFilter.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c DStarGMSKDemodulator.cpp

DStarGMSKModulator.o:	DStarGMSKModulator.cpp DStarGMSKModulator.h DStarDefines.h FIRFilter.h
		$(CC) $(CFLAGS) -c DStarGMSKModulator.cpp

DStarRepeaterConfig.o:	DStarRepeaterConfig.cpp DStarRepeaterConfig.h DStarDefines.h
		$(CC) $(CFLAGS) -c DStarRepeaterConfig.cpp

DummyController.o:	DummyController.cpp DummyController.h HardwareController.h
		$(CC) $(CFLAGS) -c DummyController.cpp

DVAPController.o:	DVAPController.cpp DVAPController.h Modem.h SerialDataController.h HeaderData.h Utils.h RingBuffer.h DStarDefines.h CCITTChecksumReverse.h \
						Timer.h
		$(CC) $(CFLAGS) -c DVAPController.cpp

DVMegaController.o:	DVMegaController.cpp DVMegaController.h Modem.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h CCITTChecksum.h \
						CCITTChecksumReverse.h Timer.h
		$(CC) $(CFLAGS) -c DVMegaController.cpp

DVRPTRV1Controller.o:	DVRPTRV1Controller.cpp DVRPTRV1Controller.h Modem.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h CCITTChecksum.h \
						CCITTChecksumReverse.h Timer.h
		$(CC) $(CFLAGS) -c DVRPTRV1Controller.cpp

DVRPTRV2Controller.o:	DVRPTRV2Controller.cpp DVRPTRV2Controller.h Modem.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h Timer.h \
						DStarDefines.h TCPReaderWriter.h
		$(CC) $(CFLAGS) -c DVRPTRV2Controller.cpp

DVRPTRV3Controller.o:	DVRPTRV3Controller.cpp DVRPTRV3Controller.h Modem.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h Timer.h \
						DStarDefines.h TCPReaderWriter.h
		$(CC) $(CFLAGS) -c DVRPTRV3Controller.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h HeaderData.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

DVTOOLFileWriter.o:	DVTOOLFileWriter.cpp DVTOOLFileWriter.h HeaderData.h CCITTChecksumReverse.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVTOOLFileWriter.cpp

ExternalController.o:	ExternalController.cpp ExternalController.h DStarDefines.h HardwareController.h
		$(CC) $(CFLAGS) -c ExternalController.cpp

FIRFilter.o:	FIRFilter.cpp FIRFilter.h
		$(CC) $(CFLAGS) -c FIRFilter.cpp

GatewayProtocolHandler.o:	GatewayProtocolHandler.cpp GatewayProtocolHandler.h DStarDefines.h UDPReaderWriter.h CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c GatewayProtocolHandler.cpp

GMSKController.o:	GMSKController.cpp GMSKController.h Modem.h RingBuffer.h Utils.h DStarDefines.h Timer.h GMSKModem.h GMSKModemLibUsb.h
		$(CC) $(CFLAGS) -c GMSKController.cpp

GMSKModem.o:	GMSKModem.cpp GMSKModem.h
		$(CC) $(CFLAGS) -c GMSKModem.cpp

GMSKModemLibUsb.o:	GMSKModemLibUsb.cpp GMSKModemLibUsb.h GMSKModem.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c GMSKModemLibUsb.cpp

Golay.o:	Golay.cpp Golay.h
		$(CC) $(CFLAGS) -c Golay.cpp

HardwareController.o:	HardwareController.cpp HardwareController.h
		$(CC) $(CFLAGS) -c HardwareController.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c HeaderData.cpp

K8055Controller.o:	K8055Controller.cpp K8055Controller.h HardwareController.h
		$(CC) $(CFLAGS) -c K8055Controller.cpp

LogEvent.o:	LogEvent.cpp LogEvent.h
		$(CC) $(CFLAGS) -c LogEvent.cpp

Logger.o:	Logger.cpp Logger.h
		$(CC) $(CFLAGS) -c Logger.cpp

MessageTextCtrl.o:	MessageTextCtrl.cpp MessageTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c MessageTextCtrl.cpp

Modem.o:	Modem.cpp Modem.h RingBuffer.h HeaderData.h DStarDefines.h
		$(CC) $(CFLAGS) -c Modem.cpp

OutputQueue.o:	OutputQueue.cpp OutputQueue.h RingBuffer.h HeaderData.h
		$(CC) $(CFLAGS) -c OutputQueue.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RaspberryController.o:	RaspberryController.cpp RaspberryController.h HardwareController.h
		$(CC) $(CFLAGS) -c RaspberryController.cpp

RepeaterProtocolHandler.o:	RepeaterProtocolHandler.cpp RepeaterProtocolHandler.h DStarDefines.h HeaderData.h UDPReaderWriter.h CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c RepeaterProtocolHandler.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SerialDataController.o:	SerialDataController.cpp SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialDataController.cpp

SerialLineController.o:	SerialLineController.cpp SerialLineController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialLineController.cpp

SlowDataDecoder.o:	SlowDataDecoder.cpp SlowDataDecoder.h DStarDefines.h HeaderData.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c SlowDataDecoder.cpp

SlowDataEncoder.o:	SlowDataEncoder.cpp SlowDataEncoder.h DStarDefines.h HeaderData.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c SlowDataEncoder.cpp

SoundCardController.o:	SoundCardController.cpp SoundCardController.h Modem.h SoundCardReaderWriter.h RingBuffer.h HeaderData.h Utils.h CCITTChecksumReverse.h \
					DStarGMSKDemodulator.h DStarGMSKModulator.h AudioCallback.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c SoundCardController.cpp

SoundCardReaderWriter.o:	SoundCardReaderWriter.cpp SoundCardReaderWriter.h AudioCallback.h
		$(CC) $(CFLAGS) -c SoundCardReaderWriter.cpp

SplitController.o:	SplitController.cpp SplitController.h RingBuffer.h Utils.h DStarDefines.h Timer.h GatewayProtocolHandler.h
		$(CC) $(CFLAGS) -c SplitController.cpp

TCPReaderWriter.o:	TCPReaderWriter.cpp TCPReaderWriter.h UDPReaderWriter.h
		$(CC) $(CFLAGS) -c TCPReaderWriter.cpp

Timer.o:	Timer.cpp Timer.h
		$(CC) $(CFLAGS) -c Timer.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h Utils.h
		$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

Utils.o:	Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

clean:
		rm -f core Common.a *.o *~ *.bak

