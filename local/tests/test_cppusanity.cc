#include "test_helper.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(CU_SanityTest){};

TEST(CU_SanityTest, NoCheckNoLeak)
{
   int *leaker = new int[20];
   delete [] leaker;
};

TEST(CU_SanityTest, HarnessDetectsAndCountsLeak)
{
   EXPECT_N_LEAKS(1);
   int *leaker = new int[20];
   leaker++; // silence compiler
};

TEST(CU_SanityTest, ChecksOK)
{
#ifdef ENABLE_CHECK_FAILURES
   CHECK(false);
#else
   CHECK(true);
#endif
};


