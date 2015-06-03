dnl PKG_GOOGLETEST_PART([GTEST],[gtest],[Testing],[gmock/gtest])
AC_DEFUN([PKG_GOOGLETEST_PART], [
	AH_TEMPLATE([HAVE_$1], [Have Google Testing library?])
	AC_ARG_WITH($2,
	   [AS_HELP_STRING([--with-$2],
			   [Enable tests using the Google C++ Framework.])],
	   [AS_IF([test "x$withval" != "xyes"],
	   	[with_$2="$withval"],
	   	[with_$2="${ac_pwd}/lib/build/$4"])],
	   [with_$2=no])
	AC_MSG_CHECKING([availibility of $2])
	AS_IF([test "x${with_$2}" = xno],
	  [AC_MSG_RESULT([not selected, skipping...])],
	  [AS_IF([test -r "${with_$2}/src/$2-all.cc"],
		 [
		   $1_DIR="${with_$2}"
		   $1_CPPFLAGS="-I${$1_DIR}/include"
		   $1_LIBS="-L${$1_DIR}/lib/.libs -l$2"
		   $1_CXXFLAGS="$$1_CPPFLAGS"
		   AC_DEFINE([HAVE_$1])
		   AC_SUBST([$1_DIR])
		   AC_SUBST([$1_CPPFLAGS])
		   AC_SUBST([$1_CXXFLAGS])
		   AC_SUBST([$1_LIBS])
		   AC_MSG_RESULT([$1_DIR=${$1_DIR}])
		 ],
		 [
		   AC_MSG_FAILURE([cannot find $2 in ${with_$2}])
		 ])
	  ])
	])

dnl PKG_GOOGLETEST_CONFIG([foo])
AC_DEFUN([PKG_GOOGLETEST_CONFIG], [ m4_ifnblank([$1], [
	PKG_GOOGLETEST_PART([GTEST],[gtest],[Testing],[gmock/gtest])
	PKG_GOOGLETEST_PART([GMOCK],[gmock],[Mocking],[gmock])
	]) ])


