#ifndef TRIG_H_
#define TRIG_H_

#include <stdint.h>
#include <stdbool.h>

#define DEG_0    (0U * (1U << 5U))
#define DEG_45   (1U * (1U << 5U))
#define DEG_90   (2U * (1U << 5U))
#define DEG_135  (3U * (1U << 5U))
#define DEG_180  (4U * (1U << 5U))
#define DEG_225  (5U * (1U << 5U))
#define DEG_270  (6U * (1U << 5U))
#define DEG_315  (7U * (1U << 5U))

typedef struct
{
    uint8_t x;
    uint8_t y;
}
point_t;

typedef struct
{
    int16_t x;
    int16_t y;
}
pointf16_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
}
point16_t;

typedef enum
{
    Quad_0,
    Quad_1,
    Quad_2,
    Quad_3,
}
quadrant_t;

extern void TRIG_Translate(point_t * const p, uint8_t angle, int16_t inc);
extern void TRIG_Translate16(pointf16_t * const p, uint8_t angle, int16_t inc);
extern uint8_t TRIG_ATan2(const pointf16_t * const a, const pointf16_t * const b);
extern point_t TRIG_FixedTo8Bit(pointf16_t * p, uint16_t x_max, uint16_t y_max);

extern quadrant_t TRIG_WhichQuadrant(const pointf16_t * const a, const pointf16_t * const b);

/* Shortest angle midpoint */
extern uint8_t TRIG_SAM(uint8_t a, uint8_t b);

#endif /* TRIG_H_ */

