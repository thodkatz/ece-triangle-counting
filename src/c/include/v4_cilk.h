#ifndef V4_CILK_H
#define V4_CILK_H

#include <vector>

/*
 * Version 4 using cilk
 *
 */
void v4_cilk(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low,
            const uint32_t nnz_complete, const uint32_t n); 

void spmv_cilk(uint32_t*, uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
int binary_search_cilk(uint32_t*, uint32_t, int32_t, int32_t);
uint32_t sum_common_cilk(uint32_t, uint32_t, uint32_t*, uint32_t*);

#endif
