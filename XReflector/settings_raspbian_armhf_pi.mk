# Makefile-include
#
# Settings for Raspbian Wheezy compile in qemu-debootstrap environment
# Hans-J. Barthen, DL5DI 2013-01-21
# Rename this file to settings.mk before you compile the source for the Raspbian platform
#
	export DATADIR := "/home/opendv/data"
	export LIBS    := $(shell wx-config --libs adv,core)
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := -g
	export BINDIR  := "$(DESTDIR)/usr/bin"
#
# Comment out the first CFLAGS entry and uncomment the second to enable the DExtra Link. For DCS Link you must comment out the first
# CFLAGS entry and uncomment the third entry. You must do a "make clean" before rebuilding the software.
#
	export CFLAGS  := -g -O2 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)'  -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread $(shell wx-config --cxxflags)
# 	export CFLAGS := -g -O2 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread -DDEXTRA_LINK $(shell wx-config --cxxflags)
# 	export CFLAGS := -g -O2 -march=armv6 -mfpu=vfp -mfloat-abi=hard -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread -DDCS_LINK $(shell wx-config --cxxflags)
