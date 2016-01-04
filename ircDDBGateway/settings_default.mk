# Makefile-include
#
# Build the ircDDB Gateway software on Linux with default settings (x86)
#
	export DATADIR := "/usr/local/etc"
	export LIBS := $(shell wx-config --libs adv,core)
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := -g
	export BINDIR  := "/usr/local/bin"
	export LOGDIR  := "/var/log"
	export CONFDIR := "/etc"
#
# Comment out the first CFLAGS entry and uncomment the second to enable the DExtra Link. For DCS Link you must comment out the first
# CFLAGS entry and uncomment the thrird entrt. You must do a "make clean" before rebuilding the software.
#
	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
#	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDEXTRA_LINK $(shell wx-config --cxxflags)
#	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDCS_LINK $(shell wx-config --cxxflags)
