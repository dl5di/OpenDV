SUBDIRS = LinuxUSB 

DEFS += \
	-DDATA_DIR=\"${datadir}\" \
	-DLOG_DIR=\"${localstatedir}/log/opendv\" \
        -DCONF_DIR=\"${sysconfdir}\"

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

COMMON_SRCS = \
	Common/AMBEFEC.cpp \
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
	Common/Utils.cpp \
	DStarRepeater/DStarRepeaterThread.cpp \
	DStarRepeater/DStarRepeaterTRXThread.cpp \
	DStarRepeater/DStarRepeaterTXRXThread.cpp \
	DStarRepeater/DStarRepeaterTXThread.cpp \
	DStarRepeater/DStarRepeaterRXThread.cpp \
	DStarRepeater/DStarRepeaterStatusData.cpp

sbin_PROGRAMS = dstarrepeaterd
dstarrepeaterd_SOURCES = DStarRepeater/DStarRepeaterD.cpp \
                         $(COMMON_SRCS) \
                         $(CONTROLLER_SRCS)
dstarrepeaterd_LDADD = $(BASE_WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
dstarrepeaterd_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

if WITH_GPIO
COMMON_SRCS += Common/GPIOController.cpp
dstarrepeaterd_LDADD += -lwiringPi
endif

if WITH_GUI
bin_PROGRAMS = dstarrepeater
dstarrepeater_SOURCES = DStarRepeater/DStarRepeaterApp.cpp \
                        DStarRepeater/DStarRepeaterFrame.cpp \
                        DStarRepeater/DStarRepeaterLogger.cpp \
                        DStarRepeater/DStarRepeaterThreadHelper.cpp \
                        $(COMMON_SRCS) \
                        $(CONTROLLER_SRCS)
dstarrepeater_LDADD = $(GUI_WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
dstarrepeater_CPPFLAGS = -ICommon $(GUI_WX_CPPFLAGS)

if WITH_GPIO
dstarrepeater_LDADD += -lwiringPi
endif

bin_PROGRAMS += dstarrepeaterconfig
dstarrepeaterconfig_SOURCES = \
	DStarRepeaterConfig/DStarRepeaterConfigActiveHangSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigAnnouncementSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigApp.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigBeaconSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigCallsignSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigControl1Set.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigControl2Set.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigControllerSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigDVAPSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigDVMegaSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigDVRPTR1Set.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigDVRPTR2Set.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigDVRPTR3Set.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigFrame.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigGMSKSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigMMDVMSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigModemSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigNetworkSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigSoundCardSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigSplitSet.cpp \
	DStarRepeaterConfig/DStarRepeaterConfigTimesSet.cpp \
	GUICommon/AddressTextCtrl.cpp \
	GUICommon/CallsignTextCtrl.cpp \
	GUICommon/MessageTextCtrl.cpp \
	GUICommon/PortTextCtrl.cpp \
	GUICommon/RestrictedTextCtrl.cpp \
 	Common/SerialPortSelector.cpp \
	Common/DStarRepeaterConfig.cpp \
	Common/SoundCardReaderWriter.cpp

dstarrepeaterconfig_LDADD = $(GUI_WX_LIBS) $(ALSA_LIBS)
dstarrepeaterconfig_CPPFLAGS = -ICommon -IGUICommon $(GUI_WX_CPPFLAGS)
endif
