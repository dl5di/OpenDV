# Makefile-include
#
# Special settings for  Emdebian Compile for ARM processor, John Hays, June 2012
# modified by Hans-J. Barthen, DL5DI, 2012-09-07
# Rename this file to settings.mk before you compile the source for the Debian armel platform
#
	export DEB_HOST_GNU_TYPE := arm-linux-gnueabi
	export DATADIR := "/home/opendv/data"
	export BINDIR  := "$(DESTDIR)/usr/bin"
	export LOGDIR  := "/var/log/opendv"
	export CONFDIR := "/home/opendv/dstarrepeater"
	export CC      := $(DEB_HOST_GNU_TYPE)-g++
	export LDFLAGS := -Xlinker -rpath-link /lib/$(DEB_HOST_GNU_TYPE) -Xlinker -rpath-link /usr/lib/$(DEB_HOST_GNU_TYPE)
	export CFLAGS  := -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DLOG_DIR='$(LOGDIR)' -DCONF_DIR='$(CONFDIR)' -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/include/$(DEB_HOST_GNU_TYPE) -I/usr/$(DEB_HOST_GNU_TYPE)/include -I/usr/include -I/usr/lib/$(DEB_HOST_GNU_TYPE)/wx/include/gtk2-unicode-release-2.8 -I/usr/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread
	export LIBS    := -L/usr/lib/$(DEB_HOST_GNU_TYPE) -L/usr/$(DEB_HOST_GNU_TYPE)/lib -lportaudio -lusb-1.0 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu-2.8 -pthread
