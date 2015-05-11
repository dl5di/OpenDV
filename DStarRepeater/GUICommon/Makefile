all:		GUICommon.a

OBJS :=	AddressTextCtrl.o AMBEFEC.o AnnouncementUnit.o ArduinoController.o BeaconUnit.o CallsignList.o CallsignTextCtrl.o CCITTChecksum.o CCITTChecksumReverse.o \
		DStarGMSKDemodulator.o DStarGMSKModulator.o DStarRepeaterConfig.o DummyController.o DVAPController.o DVMegaController.o DVRPTRV1Controller.o \
		DVRPTRV2Controller.o DVRPTRV3Controller.o DVTOOLFileReader.o DVTOOLFileWriter.o ExternalController.o FIRFilter.o GatewayProtocolHandler.o GMSKController.o \
		GMSKModem.o GMSKModemLibUsb.o Golay.o GPIOController.o HardwareController.o HeaderData.o K8055Controller.o LogEvent.o Logger.o MessageTextCtrl.o Modem.o \
		OutputQueue.o PortTextCtrl.o RepeaterProtocolHandler.o RestrictedTextCtrl.o SerialDataController.o SerialLineController.o SerialPortSelector.o \
		SlowDataDecoder.o SlowDataEncoder.o SoundCardController.o SoundCardReaderWriter.o SplitController.o TCPReaderWriter.o Timer.o UDPReaderWriter.o Utils.o

GUICommon.a:	$(OBJS)
		ar rcs GUICommon.a $(OBJS)

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

MessageTextCtrl.o:	MessageTextCtrl.cpp MessageTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c MessageTextCtrl.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

clean:
		rm -f core GUICommon.a *.o *~ *.bak
