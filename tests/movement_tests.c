#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include <math.h>

static void test_MOVE_N()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_270;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(5, p.x);
    TEST_ASSERT_EQUAL_UINT8(4, p.y);
}

static void test_MOVE_NE()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_315;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(6, p.x);
    TEST_ASSERT_EQUAL_UINT8(4, p.y);
}

static void test_MOVE_E()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = 0;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(6, p.x);
    TEST_ASSERT_EQUAL_UINT8(5, p.y);
}

static void test_MOVE_SE()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_45;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(6, p.x);
    TEST_ASSERT_EQUAL_UINT8(6, p.y);
}

static void test_MOVE_S()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_90;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(5, p.x);
    TEST_ASSERT_EQUAL_UINT8(6, p.y);
}

static void test_MOVE_SW()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_135;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(6, p.y);
    TEST_ASSERT_EQUAL_UINT8(4, p.x);
}

static void test_MOVE_W()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_180;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(4, p.x);
    TEST_ASSERT_EQUAL_UINT8(5, p.y);
}

static void test_MOVE_NW()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_225;

    TRIG_Translate(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(4, p.x);
    TEST_ASSERT_EQUAL_UINT8(4, p.y);
}

extern void MOVE_TestsRun(void)
{
    RUN_TEST(test_MOVE_N);
    RUN_TEST(test_MOVE_NE);
    RUN_TEST(test_MOVE_E);
    RUN_TEST(test_MOVE_SE);
    RUN_TEST(test_MOVE_S);
    RUN_TEST(test_MOVE_SW);
    RUN_TEST(test_MOVE_W);
    RUN_TEST(test_MOVE_NW);
}
