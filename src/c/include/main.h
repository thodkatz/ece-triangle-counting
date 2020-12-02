#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

void v1(uint64_t*, int*, const uint32_t);
void v2(uint64_t*, int*, const uint32_t);
void v3(uint64_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
void v3_pre_cilk(uint64_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
void v4(uint64_t*, uint32_t*, uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
void v4_op(uint64_t*, uint32_t*, uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);


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

void merge_csc(uint32_t*, uint32_t*, uint32_t*, uint32_t*, 
               uint32_t*, uint32_t*, uint32_t&, uint32_t&); 

#endif
