#include "trig.h"
#include "unity.h"
#include "qmath.h"
#include "lut.h"
#include <math.h>

static void TRIG_Translate(point_t * const p, uint8_t angle)
{
    assert(p!=NULL);
    int16_t x = 0U;
    int16_t y = 0U;
    
    if(angle < DEG_180)
    {
        x = QMath_Mul(0x100, qcos[angle], Q_NUM);
        y = QMath_Mul(0x100, qsin[angle], Q_NUM);
        p->x += Q_DNSCALE(x, Q_SCALE);
        p->y += Q_DNSCALE(y, Q_SCALE);
    }
    else
    {
        x = QMath_Mul(0x100, qcos[angle - DEG_180], Q_NUM);
        y = QMath_Mul(0x100, qsin[angle - DEG_180], Q_NUM);
        p->x -= Q_DNSCALE(x, Q_SCALE);
        p->y -= Q_DNSCALE(y, Q_SCALE);
    }
}

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

extern void MOVE_TestsRun(void)
{
    RUN_TEST(test_MOVE_N);
    RUN_TEST(test_MOVE_E);
    RUN_TEST(test_MOVE_SE);
    RUN_TEST(test_MOVE_S);
    RUN_TEST(test_MOVE_SW);
    RUN_TEST(test_MOVE_W);
}
