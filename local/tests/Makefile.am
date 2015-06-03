AM_CPPFLAGS = @CPPUTEST_CXXFLAGS@

CLEANFILES = @CLEANFILES@ reports/*.xml *.o *.log *.trs

AM_CXXFLAGS=    @CPPUTEST_CXXFLAGS@ \
                        @GTEST_CXXFLAGS@ @GMOCK_CXXFLAGS@ \
                        @GLOBAL_CXXFLAGS@ \
                        @GPERFTOOLS_CXXFLAGS@ \
                        -I@GMOCK_DIR@/gtest \
                        -I$(top_builddir)/src

AM_LDFLAGS =   @CPPUTEST_LDFLAGS@ @CPPUTEST_LIBS@ \
                        @GPERFTOOLS_LIBS@ \
                        @GTEST_LIBS@ @GMOCK_LIBS@ \
                        @GLOBAL_LDFLAGS@

check_PROGRAMS = test_main 
TESTS = $(check_PROGRAMS)

test_main_SOURCES = test_main.cc test_sanity.cc test_cppusanity.cc













