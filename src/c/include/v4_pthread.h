#ifndef V4_PTHREAD_H
#define V4_PTHREAD_H


void v4_pthread(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low, 
                const uint32_t nnz_complete, const uint32_t n, int numThreads);

#endif 
