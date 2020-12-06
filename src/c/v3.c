#include "include/main.h"
#include <time.h>


//extern int binary_search (uint32_t*, uint32_t, int32_t, int32_t); 

void v3(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {

    printf("\n----------Version 3----------\n");

    uint32_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    //printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    uint32_t j = 0;
    uint32_t diffc = 0;
    uint32_t diffr = 0;

    for(uint32_t start=0; start<(nnz-2);start++) {
        diffc = csc_col[j+1] - csc_col[j]; // get the numbers of elements in the jth column    
        diffr = csc_col[j+1] - start - 1; // get the remaining elements per column

        while(diffc == 0) { // Is there any element to work with?
            j++;
            diffc = csc_col[j+1] - csc_col[j];
        }

        if (diffc == 1 || diffr == 0) {  
            j++; 
            continue;
        }

        // if there are no elements in the csc_row[start] then skip
        if ((csc_col[csc_row[start] + 1] - csc_col[csc_row[start]]) == 0) continue; 
        //if (j == csc_row[start]) continue; // ignore elements in the diagonal

        uint32_t m, k = 0;
        for(m = start + 1; m<csc_col[j+1]; m++) {
            for(k = csc_col[csc_row[start]]; k < csc_col[csc_row[start] + 1]; k++) { 
                if (csc_row[k] == csc_row[m]) {
                    vertices[j]++;
                    vertices[csc_row[start]]++;
                    vertices[csc_row[k]]++;
                    count++;
                    break;
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &toc);
    //printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    printf("Total triangles: %lu\n", count);

}
