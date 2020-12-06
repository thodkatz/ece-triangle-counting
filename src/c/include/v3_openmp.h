#ifndef V3_OPENMP_H
#define V3_OPENMP_H 

#include <stdint.h>

/*
 * \brief Version 3 using OpenMP
 *
 * Note: Using reduction array for mutex
 */
void v3_openmp(uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);

/*
 * Yet Another Version of v3 OpenMP
 *
 * Note: Using local variables and critical for sync
 */
void v3_openmp_yav(uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);

#endif
