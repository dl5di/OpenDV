# Makefile-include
#
# Build the D-Star Repeater software on Linux with default settings (x86)
#
	export DATADIR := "/usr/local/etc"
	export BINDIR  := "/usr/local/bin"
	export LOGDIR  := "/var/log"
	export CONFDIR := "/etc"
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := 
	export CFLAGS  := -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
	export LIBS    := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
