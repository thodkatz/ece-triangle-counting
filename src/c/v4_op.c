#include "include/main.h"
#include <vector>

/*
 * 1 -> binary search
 * 2 -> linear search
 */
#define SUM_MODE 1

extern void print_csr(uint32_t *, uint32_t *, uint32_t, uint32_t);
extern int binary_search (uint32_t*, uint32_t, int32_t, int32_t);
extern uint32_t sum_common(uint32_t, uint32_t, uint32_t*, uint32_t*);

void spmv_op(uint64_t*, std::vector<uint32_t>&, uint32_t*, std::vector<uint32_t>&, const uint32_t, const uint32_t);

/*
 * Input: the adjacency matrix in a csc scheme for both the complete symmetric and the lower triangular
 *
 * The lower triangular will be used to walk only the half matrix
 *
 */
void v4_op(uint64_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low,
            const uint32_t nnz_complete, const uint32_t n) {
    printf("\n----------Version 4 OP is called----------\n");


    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    // create a new csc scheme that will exclude zero values
    uint32_t nnz_filtered = 0;
    std::vector<uint32_t> csc_row_filtered;
    std::vector<uint32_t> values;
    uint32_t *csc_col_filtered = (uint32_t*)malloc((n+1) * sizeof(uint32_t));
    csc_col_filtered[0] = 0;

    for(uint32_t i = 0; i < n; i++) {
        for (uint32_t j = csc_col_low[i]; j < csc_col_low[i+1]; j++) {

            uint32_t c = csc_row_low[j];
            uint32_t product = sum_common(i, c, (uint32_t*)csc_row_complete, (uint32_t*)csc_col_complete);
            if (product != 0) {
                nnz_filtered++;
                values.push_back(product);
                csc_row_filtered.push_back(c);
            }
        }
        csc_col_filtered[i+1] = nnz_filtered;
    }

    spmv_op(vertices, csc_row_filtered, csc_col_filtered, values, nnz_filtered, n);


    uint32_t count = 0;
    for(uint32_t i = 0; i < n; i++) count += vertices[i];

    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);
    
    printf("Total number of triangles: %u\n", count/3);

}

/*
 * Sparse symmetric matrix vector multiplication 
 *
 * We divide the values by two to find the correct number of triangles.
 *
 */
void spmv_op(uint64_t *y, std::vector<uint32_t> &csc_row, uint32_t *csc_col, std::vector<uint32_t> &values, const uint32_t nnz, const uint32_t n) {

    // x vector will be always 1, so change x -> 1
    for(uint32_t i = 0; i<n; i++) {
        for(uint32_t j = csc_col[i]; j < csc_col[i+1]; j++) {
            y[i] += values[j] * 1;
            y[csc_row[j]] += values[j] * 1; 
        }
    }


    for (uint32_t i = 0; i < n; i++) y[i] /= 2;
        
}
