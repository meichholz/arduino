dnl vim: ft=m4
dnl >>> run "autoreconf --install" to process this

AC_PREREQ([2.69])
AC_INIT([sketchsupport], [0.1], [marian@bugslayer.de])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/cli.cc])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([foreign 1.9.6])
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl include and call local package macro set at once
m4_define([LOCAL_PKG_CONFIG], [PKG_$1_CONFIG])
m4_define([LOCAL_PKG],[
         # configure local package $1
         m4_include(autostuff/[$1].m4)
         LOCAL_PKG_CONFIG(m4_toupper($1))
])
dnl allow several package macro sets at once to be included and activated
AC_DEFUN([LOCAL_PKG_CONFIGS], [
          m4_foreach_w([localpkgname], [$1], [LOCAL_PKG(localpkgname)] )
          ])

# configure local packages 
LOCAL_PKG_CONFIGS([googletest cpputest googleperftools])
PKG_GOOGLETEST_CONFIG([1.7.0])

dnl >>> CHECK: Is this the way to select all the flags?
if test "$GCC" = yes
then
  dnl too critical: EXTRA_GCFLAGS="-Wextra"
  _cxxflags='-std=c++11 -D_GLIBC_USE_NANOSLEEP'
  _ldflags='-lpthread'
else  
  _cxxflags=''
  _ldflags='-lpthread'
fi

AC_SUBST([GLOBAL_CXXFLAGS],["$_cxxflags"])
AC_SUBST([GLOBAL_LDFLAGS], ["$_ldflags"])

AC_SUBST([CLEANFILES], ["*.gcda *.gcno"])

AH_TEMPLATE([ENABLE_CHECK_FAILURES],[make it intentionally red])
AC_ARG_ENABLE([check-failures],
            [AS_HELP_STRING([--enable-check-failures],[make it red!])],
            [AC_DEFINE([ENABLE_CHECK_FAILURES],[1])
            enable_check_failures=YES],
            [enable_check_failures=no])

AC_CONFIG_FILES([Makefile
                 src/Makefile
				 tests/Makefile
                 ])
AC_OUTPUT

AC_DEFUN([PKG_ECHO_SUMMARY],[
          AC_MSG_RESULT([$1 : ${with_$1}])
          ])

AS_BOX([Project Configure Option Summary])
m4_foreach_w([localpkgname],
             [cpputest gperftools memcached gtest gmock],
             [PKG_ECHO_SUMMARY(localpkgname)])
AC_MSG_RESULT([enable check failures: ${enable_check_failures}])
AC_MSG_RESULT([use GNU ld: ${use_gnu_ld}])
AS_BOX([now: make && make check])

