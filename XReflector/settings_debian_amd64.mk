# Makefile-include
#
# Build the ircDDB Gateway software on Linux with settings for Debian (amd64)
# 2012-09-07 Hans-J. Barthen, DL5DI 
# Rename this file to settings.mk before you compile the source for the Debian amd64 platform
#
	export DATADIR := "/home/opendv/data"
	export LIBS    := $(shell wx-config --libs adv,core)
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := -g
	export BINDIR  := "$(DESTDIR)/usr/bin"
#
# Comment out the first CFLAGS entry and uncomment the second to enable the DExtra Link. For DCS Link you must comment out the first
# CFLAGS entry and uncomment the thrird entrt. You must do a "make clean" before rebuilding the software.
#
	export CFLAGS := -g -O2 -m64 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
#	export CFLAGS := -g -O2 -m64 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDEXTRA_LINK $(shell wx-config --cxxflags)
#	export CFLAGS := -g -O2 -m64 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDCS_LINK $(shell wx-config --cxxflags)
