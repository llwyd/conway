#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include <math.h>

static void test_FP_2_DisplayN(void)
{
    pointf16_t p =
    {
        .x = 0x0000,
        .y = 0xFFFF,
    };

    point_t pd = TRIG_FixedTo8Bit(&p, 128, 64);

    TEST_ASSERT_EQUAL_UINT8(63, pd.x);
    TEST_ASSERT_EQUAL_UINT8(0, pd.y);
}

static void test_FP_2_DisplayE(void)
{
    pointf16_t p =
    {
        .x = 0x7FFF,
        .y = 0x0000,
    };

    point_t pd = TRIG_FixedTo8Bit(&p, 128, 64);

    TEST_ASSERT_EQUAL_UINT8(127, pd.x);
    TEST_ASSERT_EQUAL_UINT8(31, pd.y);
}

static void test_FP_2_DisplayS(void)
{
    pointf16_t p =
    {
        .x = 0,
        .y = 0x7FFF,
    };

    point_t pd = TRIG_FixedTo8Bit(&p, 128, 64);

    TEST_ASSERT_EQUAL_UINT8(63, pd.x);
    TEST_ASSERT_EQUAL_UINT8(63, pd.y);
}

static void test_FP_2_DisplayW(void)
{
    pointf16_t p =
    {
        .x = -1,
        .y = 0x0000,
    };

    point_t pd = TRIG_FixedTo8Bit(&p, 128, 64);

    TEST_ASSERT_EQUAL_UINT8(0, pd.x);
    TEST_ASSERT_EQUAL_UINT8(31, pd.y);
}

static void test_FP_2_DisplaySW_2(void)
{
    pointf16_t p =
    {
        .x = 0x3FFF,
        .y = 0x3FFF,
    };

    point_t pd = TRIG_FixedTo8Bit(&p, 128, 64);

    TEST_ASSERT_EQUAL_UINT8(95, pd.x);
    TEST_ASSERT_EQUAL_UINT8(47, pd.y);
}


extern void FP_TestsRun(void)
{
    RUN_TEST(test_FP_2_DisplayN);
    RUN_TEST(test_FP_2_DisplayE);
    RUN_TEST(test_FP_2_DisplayS);
    RUN_TEST(test_FP_2_DisplayW);
    RUN_TEST(test_FP_2_DisplaySW_2);
}
