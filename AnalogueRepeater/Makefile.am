opendvlogdir = ${localstatedir}/log/opendv
opendvlog_DATA = 

pkgsysconfdir = ${sysconfdir}/opendv
pkgsysconf_DATA = 

DEFS += \
	-DLOG_DIR=\"${opendvlogdir}\" \
	-DCONF_DIR=\"${pkgsysconfdir}\"

noinst_LIBRARIES = libCommon.a

libCommon_a_SOURCES = \
 	Common/ArduinoController.cpp \
 	Common/AudioDelay.cpp \
 	Common/CWKeyer.cpp \
 	Common/DTMFController.cpp \
 	Common/DTMFDecoder.cpp \
 	Common/DummyController.cpp \
 	Common/ExternalController.cpp \
 	Common/FirstOrderIIR.cpp \
 	Common/FIRFilter.cpp \
 	Common/FixedAudioSource.cpp \
 	Common/Goertzel.cpp \
 	Common/HardwareController.cpp \
 	Common/K8055Controller.cpp \
 	Common/LogEvent.cpp \
 	Common/Logger.cpp \
 	Common/NCO.cpp \
 	Common/NetworkController.cpp \
 	Common/PTTDelay.cpp \
 	Common/SerialDataController.cpp \
 	Common/SerialLineController.cpp \
 	Common/SerialPortSelector.cpp \
 	Common/SoundCardReaderWriter.cpp \
 	Common/TimeoutTones.cpp \
 	Common/Timer.cpp \
 	Common/Utils.cpp \
 	Common/VOGAD.cpp \
 	Common/WAVFileReader.cpp \
 	Common/WAVFileStore.cpp
libCommon_a_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

if WITH_GPIO
libCommon_a_SOURCES += Common/GPIOController.cpp
endif

sbin_PROGRAMS = analoguerepeaterd
analoguerepeaterd_SOURCES = \
	AnalogueRepeater/AnalogueRepeaterD.cpp \
 	AnalogueRepeater/AnalogueRepeaterConfig.cpp \
 	AnalogueRepeater/AnalogueRepeaterThread.cpp \
 	AnalogueRepeater/AnalogueRepeaterStatusData.cpp
analoguerepeaterd_LDADD = libCommon.a $(BASE_WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
analoguerepeaterd_CPPFLAGS = -ICommon $(BASE_WX_CPPFLAGS)

if WITH_GPIO
analoguerepeaterd_LDADD += -lwiringPi
endif

if WITH_GUI
bin_PROGRAMS = analoguerepeater
analoguerepeater_SOURCES = \
	Common/ActiveHangSet.cpp \
	Common/AddressTextCtrl.cpp \
	Common/CallsignTextCtrl.cpp \
	Common/CWTextCtrl.cpp \
	Common/DTMFTextCtrl.cpp \
	Common/RestrictedTextCtrl.cpp \
	AnalogueRepeater/AnalogueRepeaterAckSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterApp.cpp \
 	AnalogueRepeater/AnalogueRepeaterCallsignSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterConfig.cpp \
 	AnalogueRepeater/AnalogueRepeaterControllerSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterDTMF1Set.cpp \
 	AnalogueRepeater/AnalogueRepeaterDTMF2Set.cpp \
 	AnalogueRepeater/AnalogueRepeaterExternalSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterFeelSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterFrame.cpp \
 	AnalogueRepeater/AnalogueRepeaterLogger.cpp \
 	AnalogueRepeater/AnalogueRepeaterPreferences.cpp \
 	AnalogueRepeater/AnalogueRepeaterRadioSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterStatusData.cpp \
 	AnalogueRepeater/AnalogueRepeaterThread.cpp \
 	AnalogueRepeater/AnalogueRepeaterTimesSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterTonesSet.cpp \
 	AnalogueRepeater/AnalogueRepeaterThreadHelper.cpp
analoguerepeater_LDADD = libCommon.a $(GUI_WX_LIBS) $(LIBUSB_LIBS) $(ALSA_LIBS)
analoguerepeater_CPPFLAGS = -ICommon $(GUI_WX_CPPFLAGS)

if WITH_GPIO
analoguerepeater_LDADD += -lwiringPi
endif
endif

