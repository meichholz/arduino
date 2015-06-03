AC_DEFUN([PKG_CPPUTEST_CONFIG],[
        AH_TEMPLATE([HAVE_CPPUTEST], [Is compiled CppUTest available?])
        dnl --- use OPT-IN model
        AC_ARG_WITH([cpputest],
            [AS_HELP_STRING([--with-cpputest], [use LOCAL cpputest test framework])],
            [with_cpputest="$withval"],
            [with_cpputest=no]
            )
        dnl --- unless unwanted, see whether the library is available, and where ($with_xxx
        CPPUTEST_CXXFLAGS=
        CPPUTEST_LIBS=
        CPPUTEST_LDFLAGS=
        AC_MSG_CHECKING([Checking for CppUTest])
        AS_IF([test "x$with_cpputest" = xno], [
            AC_MSG_RESULT([skipping...])
        ], [
            dir="$ac_pwd/lib/aux"
            AS_IF([test -r "$dir/include/CppUTest/TestHarness.h"], [
                    AC_MSG_RESULT([local installation found in $dir])
                    dnl -- TODO: how to refer to top build dir in makefiles?
                    with_cpputest="$dir"
                    AC_DEFINE([HAVE_CPPUTEST])
                    CPPUTEST_CXXFLAGS="-I$with_cpputest/include"
                    CPPUTEST_LIBS="-lCppUTest -lCppUTestExt -lstdc++"
                    CPPUTEST_LDFLAGS="-L$with_cpputest/lib"
                    ], [
                    AC_MSG_RESULT([not found])
                    AC_MSG_FAILURE([cpputest not found in ${dir}])
                    with_cpputest=no
		    ])
            ])
    AC_SUBST([CPPUTEST_CXXFLAGS])
    AC_SUBST([CPPUTEST_LDFLAGS])
    AC_SUBST([CPPUTEST_LIBS])
    dnl indent me
])

