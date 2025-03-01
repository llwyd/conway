#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include <math.h>

#define SPEED (0x100)

static void test_MOVE_N()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_270;

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

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

    TRIG_Translate(&p, angle, SPEED);

    TEST_ASSERT_EQUAL_UINT8(4, p.x);
    TEST_ASSERT_EQUAL_UINT8(4, p.y);
}

static void test_MOVE_N16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_270;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(0x0, p.x);
    TEST_ASSERT_EQUAL_INT16(0xff00, p.y);
}

static void test_MOVE_NE16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_315;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(181, p.x);
    TEST_ASSERT_EQUAL_INT16(-182, p.y);
}

static void test_MOVE_E16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_0;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(0x00FF, p.x);
    TEST_ASSERT_EQUAL_INT16(0x0000, p.y);
}

static void test_MOVE_SE16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_45;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(181, p.x);
    TEST_ASSERT_EQUAL_INT16(181, p.y);
}

static void test_MOVE_S16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_90;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(0x0, p.x);
    TEST_ASSERT_EQUAL_INT16(0x00FF, p.y);
}

static void test_MOVE_SW16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_135;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(-182, p.x);
    TEST_ASSERT_EQUAL_INT16(181, p.y);
}

static void test_MOVE_W16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_180;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(0xFF00, p.x);
    TEST_ASSERT_EQUAL_INT16(0x0000, p.y);
}

static void test_MOVE_NW16()
{
    pointf16_t p =
    {
        .x = 0x0,
        .y = 0x0,
    };
    uint8_t angle = DEG_225;

    TRIG_Translate16(&p, angle, 0x100);

    TEST_ASSERT_EQUAL_INT16(-182, p.x);
    TEST_ASSERT_EQUAL_INT16(-182, p.y);
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
    
    RUN_TEST(test_MOVE_N16);
    RUN_TEST(test_MOVE_NE16);
    RUN_TEST(test_MOVE_E16);
    RUN_TEST(test_MOVE_SE16);
    RUN_TEST(test_MOVE_S16);
    RUN_TEST(test_MOVE_SW16);
    RUN_TEST(test_MOVE_W16);
    RUN_TEST(test_MOVE_NW16);
}
