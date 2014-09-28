# Makefile-include
#
# Special settings for  Emdebian Compile for ARM processor, John Hays, June 2012
# Rename this file to settings.mk before you compile the source for the RaspberryPi platform
#
	export DATADIR := "/opt/opendv/data"
	export BINDIR  := "$(DESTDIR)/opt/opendv/bin"
	export CC      := arm-linux-gnueabi-g++
	export LDFLAGS := -L/usr/arm-linux-gnueabi/lib 
	export CFLAGS  := -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/arm-linux-gnueabi/include -I/usr/arm-linux-gnueabi/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/arm-linux-gnueabi/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread
	export LIBS := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
