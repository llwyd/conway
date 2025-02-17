#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include "lut.h"
#include <math.h>

#define Q_NUM (15)
#define Q_SCALE (7U)
#define SPEED (0x100)

static void Move(point_t * const p, uint8_t angle)
{
    int16_t x = Q_UPSCALE(p->x, Q_SCALE);
    int16_t y = Q_UPSCALE(p->y, Q_SCALE);
    
    x += QMath_Mul(0x100, qcos[angle], Q_NUM);
    y += QMath_Mul(0x100, qsin[angle], Q_NUM);

    p->x = Q_DNSCALE(x, Q_SCALE);
    p->y = Q_DNSCALE(y, Q_SCALE);
}

static void test_MOVE_N()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_270;

    Move(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(5, p.x);
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

    Move(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(6, p.x);
    TEST_ASSERT_EQUAL_UINT8(5, p.y);
}

static void test_MOVE_S()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_90;

    Move(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(4, p.x);
    TEST_ASSERT_EQUAL_UINT8(6, p.y);
}

static void test_MOVE_W()
{
    point_t p =
    {
        .x = 5,
        .y = 5,
    };
    uint8_t angle = DEG_180;

    Move(&p, angle);

    TEST_ASSERT_EQUAL_UINT8(4, p.x);
    TEST_ASSERT_EQUAL_UINT8(5, p.y);
}

extern void MOVE_TestsRun(void)
{
    RUN_TEST(test_MOVE_N);
    RUN_TEST(test_MOVE_E);
    RUN_TEST(test_MOVE_S);
    RUN_TEST(test_MOVE_W);
}
