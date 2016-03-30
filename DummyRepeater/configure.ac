AC_PREREQ([2.69])
AC_INIT([dummyrepeater], [1.0])
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-extra-portability foreign no-define subdir-objects])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AM_OPTIONS_WXCONFIG
reqwx=3.0.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi

CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"

PKG_CHECK_MODULES([LIBUSB], [libusb-1.0 >= 1.0.0])

PKG_CHECK_MODULES([ALSA], [alsa >= 1.0.0])

AC_ARG_WITH([gpio],
  [AS_HELP_STRING([--with-gpio],
    [enable support for WiringPi GPIO signalling])],
  [],
  [with_gpio=no])

GPIO=
AS_IF([test "x$with_gpio" != xno],
  [AC_CHECK_LIB([wiringPi], [wiringPiSetup],
    [AC_DEFINE([GPIO])
    AM_CONDITIONAL([WITH_GPIO], true)
    ],
    [AC_MSG_FAILURE(
      [--with-gpio was given but the wiringPi library is not available])],
  )],
  [AM_CONDITIONAL([WITH_GPIO], false)]
)

AC_ARG_WITH([stardv],
  [AS_HELP_STRING([--with-stardv],
    [enable support for Star*DV dongle])],
  [],
  [with_stardv=no])

STARDV=
AS_IF([test "x$with_stardv" != xno],
  [AC_CHECK_LIB([ftd2xx], [FT_Open],
    [AM_CONDITIONAL([WITH_STARDV], true)
    ],
    [AC_MSG_FAILURE(
      [--with-stardv was given but the ftd2xx library is not available])],
  )],
  [AM_CONDITIONAL([WITH_STARDV], false)]
)

AC_ARG_WITH([ambeserver],
  [AS_HELP_STRING([--with-ambeserver],
    [enable support for AMBEserver on the PiDV/ThumbDV])],
  [AM_CONDITIONAL([WITH_AMBESERVER], true)],
  [AM_CONDITIONAL([WITH_AMBESERVER], false)])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
