// {{{ include C++ mode stuff

#ifdef __cplusplus
#include "config.h"

// {{{ what to do with CCPUTEST on Board
#ifdef HAVE_CPPUTEST

#include <CppUTest/MemoryLeakDetectorNewMacros.h>
#include <CppUTest/MemoryLeakDetectorMallocMacros.h>

// NOTICE: The combination of GMOCK, GTEST and the Test Runner still is
// brittle and probably plain WRONG, leading to "UndefinedFile" Failures.
#ifdef HAVE_GMOCK
#ifdef USE_GTEST_CONVERTOR
#include <CppUTest/CommandLineTestRunner.h>
#define GTEST_DONT_DEFINE_FAIL 1
#define GTEST_DONT_DEFINE_TEST 1
#include <CppUTestExt/GTestConvertor.h>
#else
#include <CppUTestExt/GMock.h>
#include <CppUTestExt/GTest.h>
#endif
#endif // HAVE_GMOCK

// cpputest includes the gtest wrapper, so we are done here

// include otherwise missing C++ boilerplate
//#include <cstdlib>
//#include <sstream>

// }}}
// {{{ What to do with only Google Testing
#else // without cpputest, we have to include gmock/gtest headers directly
#ifdef HAVE_GMOCK
#include <gmock/gmock.h>
#endif // HAVE_GMOCK
#endif
// }}}

#endif

// }}}
// {{{ define specific C testing interface prototypes
#ifdef __cplusplus
extern "C" {
#endif

#include "globals.h"

#ifdef __cplusplus
}
#endif
// }}}


