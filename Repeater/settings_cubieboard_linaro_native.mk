# Makefile-include
#
# Special settings to compile for ARM processor on Cubieboard with Linaro,
# provided by Svend OZ1EKD, 2013-05-26
# Rename this file to settings.mk before you compile the source for this platform
#
-I/usr/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread
export WX_INC :=$(shell wx-config --cxxflags)
export WX_LIB :=$(shell wx-config --libs)
export DEB_HOST_GNU_TYPE := arm-linux-gnueabihf
export DATADIR := "/home/opendv/data"
export BINDIR := "$(DESTDIR)/usr/bin"
export CC := $(DEB_HOST_GNU_TYPE)-g++
export LDFLAGS := -Xlinker -rpath-link /lib/$(DEB_HOST_GNU_TYPE)
-Xlinker -rpath-link /usr/lib/$(DEB_HOST_GNU_TYPE)
export CFLAGS := -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing
-Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)'
-I/usr/include/$(DEB_HOST_GNU_TYPE) -I/usr/$(DEB_HOST_GNU_TYPE)/include
-I/usr/include $(WX_INC)
export LIBS := -lportaudio -lusb-1.0 -L/usr/local/lib
-L/usr/lib/$(DEB_HOST_GNU_TYPE) -L/usr/$(DEB_HOST_GNU_TYPE)/lib $(WX_LIB)
