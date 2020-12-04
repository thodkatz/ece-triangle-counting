#include "include/main.h"
#include "include/v4_openmp.h"
#include <omp.h>
#include <vector>

/*
 * 1 -> binary search
 * 2 -> linear search
 */
#define SUM_MODE 2

// #define NUM_THREADS 8 The setting will be done via env variable in sbatch script


extern void print_csr(uint32_t *, uint32_t *, uint32_t, uint32_t);


extern int binary_search(uint32_t*, uint32_t, int32_t, int32_t);

/*
 * Input: the adjacency matrix in a csc scheme for both the complete symmetric and the down triagonal
 *
 * The down triagonal will be used to walk only the half matrix
 *
 */
void v4_openmp(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_down, uint32_t *csc_col_down,
            const uint32_t nnz_complete, const uint32_t n) {
    printf("\n----------Version 4 OpenMP----------\n");


    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);


    uint32_t *values = (uint32_t*)malloc(nnz_complete/2 * sizeof(uint32_t));

    //omp_set_num_threads(NUM_THREADS);

    int tid = 0;
    #pragma omp parallel
    {
        tid = omp_get_thread_num();
        if (tid == 0) {
            printf("The number of threads are : %d\n", omp_get_num_threads());
        }
        #pragma omp for schedule(dynamic) 
        for (uint32_t i = 0; i < n; i++) {
            for (uint32_t j = csc_col_down[i]; j < csc_col_down[i+1]; j++) {

                uint32_t c = csc_row_down[j];
                uint32_t product = sum_common_openmp(i, c, (uint32_t*)csc_row_complete, (uint32_t*)csc_col_complete);
                values[j] = product;
            }
        }
    }


    // this function takes a little time. Not worthy for parallelism
    spmv_openmp(vertices, csc_row_down, csc_col_down, values, (nnz_complete/2), n);

    free(values);


    uint32_t count = 0;
    for(uint32_t i = 0; i < n; i++) count += vertices[i];

    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);
    
    printf("Total number of triangles: %u\n", count/3);

}

/*
 * Returns the sum of the common elements of a symmetric csc matrix for two nodes
 */
uint32_t sum_common_openmp(uint32_t i,uint32_t j, uint32_t *csc_row, uint32_t *csc_col) {
    
    uint32_t value = 0;

    uint32_t start1 = csc_col[i];
    uint32_t end1 = csc_col[i+1];
    uint32_t diff1 = end1 - start1;

    uint32_t start2 = csc_col[j];
    uint32_t end2 = csc_col[j+1];
    uint32_t diff2 = end2 - start2;


#if SUM_MODE == 1
    // iterate the elements of the smaller one and use binary search for the bigger one
    if (diff1 <= diff2) {
        for (uint32_t k = start1; k < end1; k++)
            value += binary_search_openmp((uint32_t*)csc_row, csc_row[k], (int32_t)start2, (int32_t)end2 - 1);
    }
        
    if (diff2 < diff1) {
        for (uint32_t k = start2; k < end2; k++)
            value += binary_search_openmp((uint32_t*)csc_row, csc_row[k], (int32_t)start1, (int32_t)end1 - 1);
    }
#endif

#if SUM_MODE == 2
    // linear search sorted arrays
    while (start1 < end1 && start2 < end2) {
        if (csc_row[start1] == csc_row[start2]) {
            value++;
            start1++;
            start2++;
        }
        else if(csc_row[start1] < csc_row[start2]) start1++;
        else start2++;
    }
#endif

    return value;
}

// high should be signed!!
int binary_search_openmp(uint32_t *array, uint32_t key, int32_t low, int32_t high) {

    if (high >= low) { 
        int32_t mid = low + (high - low) / 2; 
  
        if (array[mid] == key) 
            return 1; 
  
        if (array[mid] > key) 
            return binary_search_openmp(array, key, low, mid-1); 
  
        return binary_search_openmp(array, key, mid + 1, high); 
    } 
  

    return 0; 
} 

/*
 * Sparse symmetric matrix vector multiplication 
 *
 * We divide the values by two to find the correct number of triangles.
 *
 */
void spmv_openmp(uint32_t *y, uint32_t *csc_row, uint32_t *csc_col, uint32_t *values, const uint32_t nnz, const uint32_t n) {

    // x vector will be always 1, so change x -> 1
    for(uint32_t i = 0; i<n; i++) {
        for(uint32_t j = csc_col[i]; j < csc_col[i+1]; j++) {
            y[i] += values[j] * 1;
            y[csc_row[j]] += values[j] * 1; 
        }
    }

    for (uint32_t i = 0; i < n; i++) y[i] /= 2;
        
}
