#ifndef LCG_H
#define LCG_H

#include <stdint.h>

/*
 * Simple 32-bit linear congruential generator.
 *
 * Recurrence:
 *     x_{n+1} = a x_n + c   (mod 2^32)
 *
 * This implementation is intentionally simple because the notebook
 * uses it as a teaching example and as a common source of random
 * numbers in all exercises.
 */
static inline uint32_t lcg32_next(uint32_t *state) {
    const uint32_t a = 1664525u;
    const uint32_t c = 1013904223u;
    *state = a * (*state) + c;
    return *state;
}

/*
 * Return a pseudo-random double in [0,1).
 */
static inline double lcg32_next_double(uint32_t *state) {
    return lcg32_next(state) / 4294967296.0;
}

#endif /* LCG_H */
