#include "include/main.h"
#include <time.h>


uint32_t binary_search_yav(uint32_t* array, uint32_t key, int32_t low, int32_t high);


/*
 * Restructure v3 function, removing loop dependencies, trying to exploit parallelism. Change linear to binary search
 *
 */
void v3_pre_cilk(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {
    printf("\n----------Version 3 Pre Cilk Sequential with Binary Search----------\n");

    uint32_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    uint32_t ret = 0;
    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t m = csc_col[i]; m < csc_col[i+1]; m++) {
            for (uint32_t k = m + 1; k < csc_col[i+1]; k++) {
                ret = binary_search_yav(csc_row, csc_row[k], csc_col[csc_row[m]],  csc_col[csc_row[m]+1] - 1);
                if (ret != -1) {
                    vertices[i]++;
                    vertices[csc_row[m]]++;
                    vertices[csc_row[ret]]++;
                    count++;
                }
            }
        }
    }


    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    printf("Total triangles: %lu\n", count);

}


/*
 * Yet another version of binary search. In this implementation we return the index
 */
uint32_t binary_search_yav(uint32_t *array, uint32_t key, int32_t low, int32_t high) {


    if (high >= low) { 
        int32_t mid = low + (high - low) / 2; 
  
        if (array[mid] == key) 
            return mid; 
  
        if (array[mid] > key) 
            return binary_search_yav(array, key, low, mid-1); 
  
        return binary_search_yav(array, key, mid + 1, high); 
    } 
  

    return -1; 
} 
