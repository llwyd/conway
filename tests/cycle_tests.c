#include "unity.h"
#include "cycle.h"

static void test_CYCLE_0()
{
    uint8_t test_data[8] = {1,2,3,4,5,6,7,8};
    TEST_ASSERT_FALSE(Cycle_Check8(test_data, 0u, 8u));
}

static void test_CYCLE_1()
{
    uint8_t test_data[8] = {3,3,3,3,3,3,3,3};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 8u));
}

static void test_CYCLE_2()
{
    uint8_t test_data[8] = {1,2,1,2,1,2,1,2};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 8u));
}

static void test_CYCLE_3()
{
    uint8_t test_data[8] = {1,2,3,1,2,3,1,2};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 8u));
}

extern void CYCLE_TestsRun(void)
{
    RUN_TEST(test_CYCLE_0);
    RUN_TEST(test_CYCLE_1);
    RUN_TEST(test_CYCLE_2);
    RUN_TEST(test_CYCLE_3);
}
