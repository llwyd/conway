#include "unity.h"
#include "cycle.h"

static void test_CYCLE_0()
{
    uint8_t test_data[8] = {1,1,1,1,1,1,1,1};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 8u));
}

extern void CYCLE_TestsRun(void)
{
    RUN_TEST(test_CYCLE_0);
}
