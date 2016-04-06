noinst_LIBRARIES = libCommon.a

libCommon_a_SOURCES = \
	Common/AddressTextCtrl.cpp \
	Common/ArduinoController.cpp \
	Common/Bleeper.cpp \
	Common/CallsignTextCtrl.cpp \
	Common/CCITTChecksumReverse.cpp \
	Common/DongleThread.cpp \
	Common/DummyController.cpp \
	Common/DV3000Controller.cpp \
	Common/DV3000NetworkController.cpp \
	Common/DV3000SerialController.cpp \
	Common/DV3000Thread.cpp \
	Common/DVMEGAAMBEController.cpp \
	Common/DVDongleController.cpp \
	Common/DVDongleThread.cpp \
	Common/ExternalController.cpp \
	Common/FIRFilter.cpp \
	Common/HardwareController.cpp \
	Common/HeaderData.cpp \
	Common/K8055Controller.cpp \
	Common/Logger.cpp \
	Common/MessageData.cpp \
	Common/MessageTextCtrl.cpp \
	Common/PortTextCtrl.cpp \
	Common/RepeaterProtocolHandler.cpp \
	Common/RestrictedTextCtrl.cpp \
	Common/SerialDataController.cpp \
	Common/SerialLineController.cpp \
	Common/SerialPortSelector.cpp \
	Common/SlowDataDecoder.cpp \
	Common/SlowDataEncoder.cpp \
	Common/SoundCardReaderWriter.cpp \
	Common/STARDVController.cpp \
	Common/STARDVNetworkController.cpp \
	Common/STARDVThread.cpp \
	Common/Timer.cpp \
	Common/DVMEGAAMBEThread.cpp \
	Common/UDPReaderWriter.cpp \
	Common/URIUSBController.cpp \
	Common/Utils.cpp
libCommon_a_CPPFLAGS = -ICommon $(WX_CPPFLAGS)

bin_PROGRAMS = dummyrepeater
dummyrepeater_SOURCES = \
	DummyRepeater/DummyRepeaterApp.cpp \
	DummyRepeater/DummyRepeaterBleepSet.cpp \
	DummyRepeater/DummyRepeaterCallsignSet.cpp \
	DummyRepeater/DummyRepeaterControllerSet.cpp \
	DummyRepeater/DummyRepeaterDongleSet.cpp \
	DummyRepeater/DummyRepeaterErrorEvent.cpp \
	DummyRepeater/DummyRepeaterFrame.cpp \
	DummyRepeater/DummyRepeaterHeaderEvent.cpp \
	DummyRepeater/DummyRepeaterMessageEvent.cpp \
	DummyRepeater/DummyRepeaterMessageSet.cpp \
	DummyRepeater/DummyRepeaterNetworkSet.cpp \
	DummyRepeater/DummyRepeaterPreferences.cpp \
	DummyRepeater/DummyRepeaterSlowDataEvent.cpp \
	DummyRepeater/DummyRepeaterSoundcardSet.cpp \
	DummyRepeater/DummyRepeaterStatusEvent.cpp \
	DummyRepeater/DummyRepeaterThread.cpp \
	DummyRepeater/DummyRepeaterTimeoutSet.cpp \
	DummyRepeater/DummyRepeaterTXEvent.cpp 
dummyrepeater_LDADD = libCommon.a $(WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
dummyrepeater_CPPFLAGS = -ICommon $(WX_CPPFLAGS)

sbin_PROGRAMS = 

if WITH_GPIO
dummyrepeater_SOURCES += Common/GPIOController.cpp
dummyrepeater_LDADD += -lwiringPi
endif

if WITH_STARDV
bin_PROGRAMS += dvemu
dvemu_SOURCES = \
	STARDV/dvemu.cpp \
	STARDV/sdvtools.cpp
dvemu_CPPFLAGS = -ISTARDV
dvemu_LDADD = -lftd2xx
endif

if WITH_AMBESERVER
sbin_PROGRAMS += AMBEserver

AMBEserver_SOURCES = DV3000/AMBEserver.c

init_ddir = /etc/init.d
dist_init_d_SCRIPTS = DV3000/init.d/AMBEserver

if WITH_GPIO
sbin_PROGRAMS += dv3000d

dv3000d_SOURCES = DV3000/dv3000d.c
dv3000d_LDADD = -lwiringPi
dv3000d_CPPFLAGS = -DRASPBERRY_PI

AMBEserver_LDADD = -lwiringPi
AMBEserver_CPPFLAGS = -DRASPBERRY_PI
endif

endif
