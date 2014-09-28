# Makefile-include
#
# Build the Digital Voice software on Linux with default settings (x86)
#

export BINDIR  := "/usr/local/bin"
export DATADIR := "/usr/local/etc"
export DESKTOPDIR := "/usr/local/share/applications"
export PNGDIR := "/usr/local/share/pixmaps"

#
# Change your architecture here
#
# export ARCH := pentium
export ARCH := x86-64


export CC := $(shell wx-config --cxx)

export LIBS := -lportaudio -lusb $(shell wx-config --libs adv,core)

export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -march=$(ARCH) -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)

export LDFLAGS := -g
