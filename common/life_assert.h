#ifndef LIFE_ASSERT_H
#define LIFE_ASSERT_H

#ifdef TARGET_DESKTOP
    #include <assert.h>

    #define ASSERT( c ) \
    { \
        assert( (c) ); \
    } 
#else
    #define ASSERT( c ) \
        { \
            if ( !(c) ) \
            { \
                asm("CPSID IF"); \
                while(1) \
                { \
                } \
            } \
        } 
#endif

#endif /* LIFE_ASSERT_H */
