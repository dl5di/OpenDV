AC_PREREQ([2.69])
AC_INIT([dstarrepeater], [1.0])
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-extra-portability foreign no-define])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AM_OPTIONS_WXCONFIG
reqwx=2.4.0
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
 
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

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
    [AC_DEFINE([GPIO], [1], [Define if you have GPIO])
    AM_CONDITIONAL([WITH_GPIO], true)
    ],
    [AC_MSG_FAILURE(
      [--with-gpio was given but the wiringPi library is not available])],
  )],
  [AM_CONDITIONAL([WITH_GPIO], false)]
)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gethostbyname inet_ntoa memset select socket strchr])

#AC_CONFIG_FILES([Common/Makefile
#                 DStarRepeater/Makefile
#                 DStarRepeaterConfig/Makefile
#                 Data/Makefile
#                 GUICommon/Makefile
#                 Makefile])
AC_CONFIG_FILES([Common/Makefile
		 GUICommon/Makefile
                 LinuxUSB/Makefile
                 DStarRepeater/Makefile
                 Makefile])
AC_OUTPUT
