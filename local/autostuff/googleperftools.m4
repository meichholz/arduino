dnl -- this version uses system google-perftools, i.e. version 2.0
dnl -- see http://stackoverflow.com/questions/10874308/how-to-use-google-perf-tools
AC_DEFUN([PKG_GOOGLEPERFTOOLS_PART], [
  dnl -- no automatical usage
	AH_TEMPLATE([HAVE_GPERFTOOLS], [Is GPerfTools included?])
	AC_ARG_WITH([$2],
	   [AS_HELP_STRING([--with-$2],
			   [Enable tests using distro Google $3.])],
	   [AS_IF([test "x$withval" = xyes],[with_$2=/usr])],
	   [with_$2=no])
	AC_MSG_CHECKING([availibility of $2])
	AS_IF([test "x${with_$2}" = xno ], [
	   AC_MSG_RESULT([skipping...])
	   $1_DIR=
	   $1_CPPFLAGS=
	   $1_CXXFLAGS="$$1_CPPFLAGS"
	   $1_LIBS=
    ], [test -r "${with_$2}/include/gperftools/malloc_extension_c.h"], [
     AC_DEFINE([HAVE_$1])
	   $1_DIR="${with_$2}"
	   $1_CPPFLAGS="-I ${$1_DIR}/include"
	   $1_CXXFLAGS="$$1_CPPFLAGS"
	   $1_LIBS="-L${$1_DIR}/lib -ltcmalloc"
	   AC_MSG_RESULT([$1_DIR=${$1_DIR}])
	 ], [ AC_MSG_FAILURE([cannot find $2 in ${with_$2}]) ])
  AC_SUBST([$1_DIR])
  AC_SUBST([$1_CPPFLAGS])
  AC_SUBST([$1_CXXFLAGS])
  AC_SUBST([$1_LIBS])
  AC_SUBST([$1_VERSION])
	])

AC_DEFUN([PKG_GOOGLEPERFTOOLS_CONFIG],[ 
	PKG_GOOGLEPERFTOOLS_PART([GPERFTOOLS],[gperftools],[GPerf Memorychecker])
	])

