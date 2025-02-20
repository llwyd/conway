#include "unity.h"
#include "trig_tests.h"
#include "movement_tests.h"
#include "qmath_tests.h"
#include <stdio.h>
#include <stdlib.h>


void setUp(void)
{
}

void tearDown(void)
{
}

int main( void )
{
    UNITY_BEGIN();
    TRIG_TestsRun();
    MOVE_TestsRun();
    QMATH_TestsRun();
    return UNITY_END();
}
