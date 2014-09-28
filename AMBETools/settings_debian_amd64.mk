# Makefile-include
#
# Build the ircDDB Gateway software on Debian Linux with default settings (amd64)
# Hans-J. Barthen, DL5DI, 2013-01-20
# Rename this file to settings.mk before you compile the source for the Debian amd64 platform
#
	export DATADIR := "/usr/local/etc"
	export BINDIR  := $(DESTDIR)/usr/local/bin
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := -g
	export CFLAGS  := -g -O2 -m64 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
	export LIBS := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
