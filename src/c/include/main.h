#ifndef TRIANGLE_COUNTING_H
#define TRIANGLE_COUNTING_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

uint64_t* v1(int*, const uint32_t);
uint64_t* v2(int*, const uint32_t);
uint64_t* v3(uint32_t*, uint32_t*, const uint32_t, const uint32_t);
uint64_t* v3_cilk(uint32_t*, uint32_t*, const uint32_t, const uint32_t);
uint64_t* v3_pre_cilk(uint32_t*, uint32_t*, const uint32_t, const uint32_t);

double diff_time(struct timespec, struct timespec);

void mm2coo(int argc, char* argv[], uint32_t**, uint32_t**, uint32_t&, uint32_t&);

void coo2csc(
  uint32_t       * const row,       /*!< CSC row start indices */
  uint32_t       * const col,       /*!< CSC column indices */
  uint32_t const * const row_coo,   /*!< COO row indices */
  uint32_t const * const col_coo,   /*!< COO column indices */
  uint32_t const         nnz,       /*!< Number of nonzero elements */
  uint32_t const         n,         /*!< Number of rows/columns */
  uint32_t const         isOneBased /*!< Whether COO is 0- or 1-based */
); 

#endif
