# Makefile-include
#
# Special settings for  Emdebian Compile for ARM processor, John Hays, June 2012
# modified by Hans-J. Barthen, DL5DI 2012-09-07
# Rename this file to settings.mk before you compile the source for the Debian armel platform
#
	export CC      := arm-linux-gnueabi-g++
	export LDFLAGS := -g -L/usr/arm-linux-gnueabi/lib
	export BINDIR  := "$(DESTDIR)/usr/bin"
	export DATADIR := "/home/opendv/data"
	export LIBS := $(shell wx-config --libs adv,core)
#
# Comment out the first CFLAGS entry and uncomment the second to enable the DExtra Link. For DCS Link you must comment out the first
# CFLAGS entry and uncomment the third entry. You must do a "make clean" before rebuilding the software.
#
	export CFLAGS  := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/arm-linux-gnueabi/include -I/usr/arm-linux-gnueabi/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/arm-linux-gnueabi/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread
# 	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/arm-linux-gnueabi/include -I/usr/arm-linux-gnueabi/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/arm-linux-gnueabi/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread -DDEXTRA_LINK
# 	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/arm-linux-gnueabi/include -I/usr/arm-linux-gnueabi/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/arm-linux-gnueabi/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread -DDCS_LINK
