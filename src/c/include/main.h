#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

void v1(uint32_t*, int*, const uint32_t);
void v2(uint32_t*, int*, const uint32_t);

/*
 * Naive approach of v3
 */
void v3(uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);

/*
 * Version 3 setting up parallelism
 *
 * Note: Loop dependencies are removed and binary search is used
 */
void v3_pre_cilk(uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);

/*
 * \brief Yet Another Version of Binary search
 *
 * \return The index that the key is found
 */
int64_t binary_search_yav(uint32_t *array, uint32_t key, int32_t low, int32_t high);


/*
 * Version 4
 *
 * Input: the adjacency matrix in a csc scheme for both the complete symmetric and the lower triangular
 *
 * Notes:
 * - A complete symmetric matrix is used instead of an upper or lower triangular to exploit efficient searching 
 * - The lower triangular will be used to walk only the half matrix
 */
void v4(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low, const uint32_t nnz_complete, const uint32_t n);

/*
 * \brief Sparse matrix vector multiplication given a csc scheme of the low triangular part of a symmetric matrix
 */
void spmv(uint32_t *y, uint32_t *csc_row, uint32_t *csc_col, uint32_t *values, const uint32_t nnz, const uint32_t n);

/*
 * \brief Binary search
 *
 * \return 1 if key found, 0 otherwise
 */
int binary_search(uint32_t *array, uint32_t key, int32_t low, int32_t high);

/*
 * \brief Sum the common elements of two lists
 */
uint32_t sum_common(uint32_t i,uint32_t j, uint32_t *csc_row, uint32_t *csc_col);

/*
 * v4 Yet Another Version
 *
 * Note: Excluding the zero values creating a new csc scheme before the SpMV
 */
void v4_yav(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low, const uint32_t nnz_complete, const uint32_t n);

/*
 * \breif SpMV utilizing vectors
 */
void spmv_yav(uint32_t *y, std::vector<uint32_t> &csc_row, uint32_t *csc_col, std::vector<uint32_t> &values, const uint32_t nnz, const uint32_t n);


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
void mm2coo(int argc, char* argv[], uint32_t **rows, uint32_t **columns, uint32_t &nnz, uint32_t &n);


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
 */
void merge_csc(uint32_t *csc_row_down, uint32_t *csc_col_down, uint32_t *csc_row_up, uint32_t *csc_col_up, 
               uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t& nnz, uint32_t& n); 

#endif
