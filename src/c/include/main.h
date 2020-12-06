#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

void v1(uint32_t*, int*, const uint32_t);
void v2(uint32_t*, int*, const uint32_t);
void v3(uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
void v3_pre_cilk(uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
void v4(uint32_t*, uint32_t*, uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
void v4_yav(uint32_t*, uint32_t*, uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);


/*
 * \brief Elapsed time between two reference points using monotonic clock
 *
 * \return Elapsed time in seconds
 */
double diff_time(struct timespec, struct timespec);

/*
 * \brief Matrix Market format to COO
 *
 * source: https://math.nist.gov/MatrixMarket/mmio/c/example_read.c
 *
 */
void mm2coo(int argc, char* argv[], uint32_t**, uint32_t**, uint32_t&, uint32_t&);


/**
 *  \brief COO to CSC conversion
 *
 *  Converts a square matrix from COO to CSC format.
 *
 *  Note: The routine assumes the input COO and the output CSC matrix
 *  to be square.
 *
 */
void coo2csc(
  uint32_t       * const row,       /*!< CSC row start indices */
  uint32_t       * const col,       /*!< CSC column indices */
  uint32_t const * const row_coo,   /*!< COO row indices */
  uint32_t const * const col_coo,   /*!< COO column indices */
  uint32_t const         nnz,       /*!< Number of nonzero elements */
  uint32_t const         n,         /*!< Number of rows/columns */
  uint32_t const         isOneBased /*!< Whether COO is 0- or 1-based */
); 


/*
 *  \brief Merge two csc schemes of a lower and upper triangle and create one csc scheme of a complete symmetric matrix.
 *
 *  Note: This scheme will be used to exploit the sorted lists in the v4.
 *
 *  \param nnz The non zero elements of the complete symmetic matrix in csc scheme
 */
void merge_csc(uint32_t*, uint32_t*, uint32_t*, uint32_t*, 
               uint32_t*, uint32_t*, uint32_t&, uint32_t&); 

#endif
