#include "unity.h"
#include "trig_tests.h"
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
    return UNITY_END();
}
