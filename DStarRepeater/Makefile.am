SUBDIRS = LinuxUSB 

DEFS += -DDATA_DIR=\"/home/opendv/data\" -DLOG_DIR=\"/var/log/opendv\" \
        -DCONF_DIR=\"/home/opendv/dstarrepeater\"

AM_CPPFLAGS = -ICommon

CONTROLLER_SRCS =  Common/ArduinoController.cpp \
                   Common/DummyController.cpp \
                   Common/DVAPController.cpp \
                   Common/DVMegaController.cpp \
                   Common/DVRPTRV1Controller.cpp \
                   Common/DVRPTRV2Controller.cpp \
                   Common/DVRPTRV3Controller.cpp \
                   Common/GMSKController.cpp \
                   Common/K8055Controller.cpp \
                   Common/MMDVMController.cpp \
                   Common/SerialDataController.cpp \
                   Common/SerialLineController.cpp \
                   Common/URIUSBController.cpp

COMMON_SRCS =      Common/AMBEFEC.cpp \
                   Common/AnnouncementUnit.cpp \
                   Common/BeaconUnit.cpp \
                   Common/CallsignList.cpp \
                   Common/CCITTChecksum.cpp \
                   Common/CCITTChecksumReverse.cpp \
                   Common/DStarGMSKDemodulator.cpp \
                   Common/DStarGMSKModulator.cpp \
                   Common/DStarRepeaterConfig.cpp \
                   Common/DVTOOLFileReader.cpp \
                   Common/DVTOOLFileWriter.cpp \
                   Common/ExternalController.cpp \
                   Common/FIRFilter.cpp \
                   Common/GatewayProtocolHandler.cpp \
                   Common/GMSKModem.cpp \
                   Common/GMSKModemLibUsb.cpp \
                   Common/Golay.cpp \
                   Common/HardwareController.cpp \
                   Common/HeaderData.cpp \
                   Common/LogEvent.cpp \
                   Common/Logger.cpp \
                   Common/Modem.cpp \
                   Common/OutputQueue.cpp \
                   Common/RepeaterProtocolHandler.cpp \
                   Common/SerialPortSelector.cpp \
                   Common/SlowDataDecoder.cpp \
                   Common/SlowDataEncoder.cpp \
                   Common/SoundCardController.cpp \
                   Common/SoundCardReaderWriter.cpp \
                   Common/SplitController.cpp \
                   Common/TCPReaderWriter.cpp \
                   Common/Timer.cpp \
                   Common/UDPReaderWriter.cpp \
                   Common/Utils.cpp

if WITH_GPIO
	COMMON_SRCS += Common/GPIOController.cpp
	DEFS += -DGPIO
endif

bin_PROGRAMS = dstarrepeater
sbin_PROGRAMS = dstarrepeaterd

COMMON_SRCS += DStarRepeater/DStarRepeaterThread.cpp \
               DStarRepeater/DStarRepeaterTRXThread.cpp \
               DStarRepeater/DStarRepeaterTXRXThread.cpp \
               DStarRepeater/DStarRepeaterTXThread.cpp \
               DStarRepeater/DStarRepeaterRXThread.cpp \
               DStarRepeater/DStarRepeaterStatusData.cpp

dstarrepeater_SOURCES = DStarRepeater/DStarRepeaterApp.cpp \
                        DStarRepeater/DStarRepeaterFrame.cpp \
                        DStarRepeater/DStarRepeaterLogger.cpp \
                        DStarRepeater/DStarRepeaterThreadHelper.cpp \
                        $(COMMON_SRCS) \
                        $(CONTROLLER_SRCS)
dstarrepeater_LDADD = $(GUI_WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
dstarrepeater_CPPFLAGS = -ICommon $(GUI_WX_CPPFLAGS)

dstarrepeaterd_SOURCES = DStarRepeater/DStarRepeaterD.cpp \
                         $(COMMON_SRCS) \
                         $(CONTROLLER_SRCS)
dstarrepeaterd_LDADD = $(BASE_WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
dstarrepeaterd_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)
