DEFS += -DDATA_DIR=\"/home/opendv/data\"

noinst_LIBRARIES = libcommon.a
libcommon_a_SOURCES = AMBEFEC.cpp AnnouncementUnit.cpp ArduinoController.cpp \
                   BeaconUnit.cpp CallsignList.cpp CCITTChecksum.cpp \
                   CCITTChecksumReverse.cpp DStarGMSKDemodulator.cpp \
                   DStarGMSKModulator.cpp DStarRepeaterConfig.cpp \
                   DummyController.cpp DVAPController.cpp DVMegaController.cpp \
                   DVRPTRV1Controller.cpp DVRPTRV2Controller.cpp \
                   DVRPTRV3Controller.cpp DVTOOLFileReader.cpp \
                   DVTOOLFileWriter.cpp ExternalController.cpp FIRFilter.cpp \
                   GatewayProtocolHandler.cpp GMSKController.cpp GMSKModem.cpp \
                   GMSKModemLibUsb.cpp Golay.cpp \
                   HardwareController.cpp HeaderData.cpp K8055Controller.cpp \
                   LogEvent.cpp Logger.cpp MMDVMController.cpp Modem.cpp \
                   OutputQueue.cpp RepeaterProtocolHandler.cpp \
                   SerialDataController.cpp SerialLineController.cpp \
                   SerialPortSelector.cpp SlowDataDecoder.cpp \
                   SlowDataEncoder.cpp SoundCardController.cpp \
                   SoundCardReaderWriter.cpp SplitController.cpp \
                   TCPReaderWriter.cpp Timer.cpp UDPReaderWriter.cpp \
                   URIUSBController.cpp Utils.cpp

if WITH_GPIO
	libcommon_a_SOURCES += GPIOController.cpp
	libcommon_a_CPPFLAGS += -DGPIO
endif
