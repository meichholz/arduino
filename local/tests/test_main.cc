/**
 * This is the main program driving all tests of the suite by CppUTest and/or
 * Google Test.
 */

#define USE_GTEST_CONVERTOR
#include "test_helper.h"

#ifdef HAVE_GPERFTOOLS
#include "gperftools/heap-checker.h"
// see http://gperftools.googlecode.com/svn/trunk/doc/heap_checker.html
#endif

#ifdef HAVE_CPPUTEST

IMPORT_TEST_GROUP(CU_SanityTest);

// {{{ main functions for the various tool combinations
int run_tests_with_cpputest(int argc, char **argv)
{
    GTestConvertor convertor;
    convertor.addAllGTestToTestRegistry();
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
#endif

#ifndef HAVE_CPPUTEST
int run_tests_with_googlemock_driver(int argc, char **argv)
{
    int result;
    ::testing::InitGoogleTest(&argc,argv);
    // the testing envorinment will silence out tcmalloc at last
    ::testing::Environment* const foo_env = ::testing::AddGlobalTestEnvironment(new CheckerEnvironment);
    // contain test runner in own code block for tcmalloc heap checker wellness
    {
        result = RUN_ALL_TESTS();
    }
    return result;
}
#endif

int main(int argc, char **argv) /* or just link -lgtest_main */
{
#ifdef HAVE_CPPUTEST
    return run_tests_with_cpputest(argc, argv);
#else
    return run_tests_with_googlemock_driver(argc, argv);
#endif
}

// }}}

