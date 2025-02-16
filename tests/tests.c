#include "unity.h"
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
    printf("Hello, World\n");
    return UNITY_END();
}
