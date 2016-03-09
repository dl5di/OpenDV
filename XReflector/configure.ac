AC_PREREQ([2.69])
AC_INIT([xreflector], [1.0])
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-extra-portability foreign no-define subdir-objects])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AM_MAINTAINER_MODE

#  XXX These need to go away as non-standard paths
if test "x$prefix" = "xNONE"; then
	prefix=""
fi
if test "x$datadir" = 'x${datarootdir}'; then
	datadir="/home/opendv/data"
	pkgdatadir="/home/opendv/data"
	AC_SUBST(pkgdatadir)
fi
if test "x$sysconfdir" = 'x${prefix}/etc'; then
	sysconfdir="/home/opendv/xreflector"
fi

AC_ARG_WITH([gui],
  [AS_HELP_STRING([--without-gui],
    [disable building the GUI programs])])
AS_IF([test "x$with_gui" != "xno"],
  [AM_CONDITIONAL([WITH_GUI], true)],
  [AM_CONDITIONAL([WITH_GUI], false)]
)

AC_ARG_WITH([dextra],
  [AS_HELP_STRING([--with-dextra],
    [enable DExtra Reflector Support])],
  [AM_CONDITIONAL([WITH_DEXTRA], true)],
  [AM_CONDITIONAL([WITH_DEXTRA], false)]
)

AC_ARG_WITH([dcs],
  [AS_HELP_STRING([--with-dcs],
    [enable DCS Reflector Support])],
  [AM_CONDITIONAL([WITH_DCS], true)],
  [AM_CONDITIONAL([WITH_DCS], false)]
)

AS_IF([test "x$with_dextra" == "xyes" -a "x$with_dcs" == "xyes"],
	[AC_MSG_ERROR([
		--with-dcs and --with-dextra are mutually exclusive.
		
		This program can be compiled with either DCS support or
		DExtra support, not both.  Please remove one of the flags
		and retry configure.
	])]
)

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

AC_SUBST([GUI_WX_CPPFLAGS], $WX_CPPFLAGS)
AC_SUBST([GUI_WX_LIBS], $WX_LIBS)

WX_CONFIG_CHECK($reqwx, wxWin=1, wxWin=0, [base])
AC_SUBST([BASE_WX_CPPFLAGS], $WX_CPPFLAGS)
AC_SUBST([BASE_WX_LIBS], $WX_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
