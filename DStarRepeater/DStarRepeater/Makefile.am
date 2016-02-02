AM_CXXFLAGS = -I../Common -DLOG_DIR=\'/var/log/opendv\' \
              -DCONF_DIR=\'/home/opendv/dstarrepeater\'
LDADD = ../Common/libcommon.a $(LIBUSB_LIBS) $(ALSA_LIBS)

bin_PROGRAMS = dstarrepeaterd
sbin_PROGRAMS = dstarrepeater
dstarrepeater_SOURCES = DStarRepeaterApp.cpp DStarRepeaterFrame.cpp \
                        DStarRepeaterLogger.cpp DStarRepeaterStatusData.cpp \
                        DStarRepeaterThread.cpp DStarRepeaterTRXThread.cpp \
                        DStarRepeaterTXRXThread.cpp DStarRepeaterTXThread.cpp \
                        DStarRepeaterRXThread.cpp DStarRepeaterThreadHelper.cpp

dstarrepeaterd_SOURCES = DStarRepeaterD.cpp DStarRepeaterThread.cpp \
                         DStarRepeaterTRXThread.cpp \
                         DStarRepeaterTXRXThread.cpp \
                         DStarRepeaterTXThread.cpp DStarRepeaterRXThread.cpp \
                         DStarRepeaterStatusData.cpp
