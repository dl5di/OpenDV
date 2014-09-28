# Makefile-include
#
# Build the D-Star Repeater software on Debian Linux with default settings (x86)
# Hans-J. Barthen, DL5DI, 2012-09-08
# Rename this file to settings.mk before you compile the source for the Debian i386 platform
#
	export DATADIR := "/home/opendv/data"
	export BINDIR  := $(DESTDIR)/usr/bin
	export LOGDIR  := "/var/log/opendv"
	export CONFDIR := "/home/opendv/dstarrepeater"
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := 
	export CFLAGS  := -O2 -m32 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
	export LIBS    := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
