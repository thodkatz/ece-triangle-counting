#ifndef V4_CILK_H
#define V4_CILK_H

#include <vector>

void v4_cilk(uint64_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_down, uint32_t *csc_col_down,
            const uint32_t nnz_complete, const uint32_t n); 

void spmv_cilk(uint64_t*, uint32_t*, uint32_t*, std::vector<uint32_t>&, const uint32_t, const uint32_t);
int binary_search_cilk(uint32_t*, uint32_t, int32_t, int32_t);
uint32_t sum_common_cilk(uint32_t, uint32_t, uint32_t*, uint32_t*);

#endif