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

static void test_CYCLE_4()
{
    uint8_t test_data[16] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 16u));
}

static void test_CYCLE_5()
{
    uint8_t test_data[16] = {5,4,7,6,3,5,4,7,6,3,5,6,7,4,3,5};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 16u));
}

static void test_CYCLE_6()
{
    uint8_t test_data[16] = {6,5,4,3,2,1,6,5,4,3,2,1,6,5,4,3};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 16u));
}

static void test_CYCLE_7()
{
    uint8_t test_data[16] = {7,6,5,4,3,2,1,7,6,5,4,3,2,1,7,6};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 0u, 16u));
}

static void test_CYCLE_0_StartMid()
{
    uint8_t test_data[8] = {1,2,3,4,5,6,7,8};
    TEST_ASSERT_FALSE(Cycle_Check8(test_data, 4u, 8u));
}

static void test_CYCLE_1_StartMid()
{
    uint8_t test_data[8] = {3,3,3,3,3,3,3,3};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 4u, 8u));
}

static void test_CYCLE_2_StartMid()
{
    uint8_t test_data[8] = {1,2,1,2,1,2,1,2};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 4u, 8u));
}

static void test_CYCLE_3_StartMid()
{
    uint8_t test_data[8] = {2,3,1,2,1,2,3,1};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 4u, 8u));
}

static void test_CYCLE_4_StartMid()
{
    uint8_t test_data[16] = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 8u, 16u));
}

static void test_CYCLE_5_StartMid()
{
    uint8_t test_data[16] = {9,5,6,7,8,5,4,5,6,7,8,9,5,6,7,8};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 8u, 16u));
}

static void test_CYCLE_6_StartMid()
{
    uint8_t test_data[16] = {2,1,0,5,4,3,2,5,4,3,2,1,0,5,4,3};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 8u, 16u));
}

static void test_CYCLE_7_StartMid()
{
    uint8_t test_data[16] = {5,4,3,2,1,7,6,7,6,5,4,3,2,1,7,6};
    TEST_ASSERT_TRUE(Cycle_Check8(test_data, 8u, 16u));
}

extern void CYCLE_TestsRun(void)
{
    RUN_TEST(test_CYCLE_0);
    RUN_TEST(test_CYCLE_1);
    RUN_TEST(test_CYCLE_2);
    RUN_TEST(test_CYCLE_3);
    RUN_TEST(test_CYCLE_4);
    RUN_TEST(test_CYCLE_5);
    RUN_TEST(test_CYCLE_6);
    RUN_TEST(test_CYCLE_7);
    
    RUN_TEST(test_CYCLE_0_StartMid);
    RUN_TEST(test_CYCLE_1_StartMid);
    RUN_TEST(test_CYCLE_2_StartMid);
    RUN_TEST(test_CYCLE_3_StartMid);
    RUN_TEST(test_CYCLE_4_StartMid);
    RUN_TEST(test_CYCLE_5_StartMid);
    RUN_TEST(test_CYCLE_6_StartMid);
    RUN_TEST(test_CYCLE_7_StartMid);
}
