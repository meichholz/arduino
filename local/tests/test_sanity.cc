#include "test_helper.h"

// Sanity Test Fixture {{{
class SanityTest : public ::testing::Test {
    protected:
        char *leaker;
        virtual void SetUp();
        virtual void TearDown();
};

void SanityTest::SetUp()
{
  leaker = new char[20];
}

void SanityTest::TearDown()
{
  delete[] leaker;
}

// }}}
// {{{ sanity tests

TEST_F(SanityTest, CanAssert) {
  int *leaker = new int[20];
  ASSERT_TRUE(true);
  delete [] leaker;
}

TEST_F(SanityTest, DoesNotLeak) {
  int *leaker = new int[20];
  delete [] leaker;
}

TEST_F(SanityTest, leak_is_detected_by_harness) {
  int *leaker = new int[20];
  ASSERT_TRUE(true);
#ifndef ENABLE_CHECK_FAILURES
  delete [] leaker;
#endif
}

TEST_F(SanityTest, can_fail) {
#ifdef ENABLE_CHECK_FAILURES
    EXPECT_TRUE(false);
#else
    EXPECT_TRUE(true);
#endif
}

// }}}

