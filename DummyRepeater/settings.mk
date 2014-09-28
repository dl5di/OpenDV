# Makefile-include
#
# Build the ircDDB Gateway software on Linux with default settings (x86)
#
	export DATADIR := "/usr/local/etc"
	export BINDIR  := "/usr/local/bin"
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := 
	export CFLAGS  := -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
	export LIBS := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
