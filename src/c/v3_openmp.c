#include "include/main.h"
#include "include/v3_openmp.h"
#include <time.h>
#include <omp.h>

#define NUM_THREADS 8 

extern int64_t binary_search_yav(uint32_t* array, uint32_t key, int32_t low, int32_t high);

void v3_openmp(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {

    printf("\n----------Version 3 OpenMP----------\n");

    uint32_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    //printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    //printf("The number of threads were : %d\n", omp_get_num_threads());
    omp_set_num_threads(NUM_THREADS);


    uint32_t *vertices_openmp = (uint32_t*)calloc(n, sizeof(uint32_t));  
    #pragma omp parallel 
    {
        int tid = omp_get_thread_num();
        if (tid == 0) printf("The numbers of threads are: %d\n", omp_get_num_threads());
        #pragma omp single 
        {
            //printf("The number of available procs are: %d\n", omp_get_num_procs());
        }

        //#pragma omp  for reduction(+:count, vertices[:n]) 
        #pragma omp  for reduction(+: vertices[:n]) 
        for (uint32_t i = 0; i < n; i++) {
            for (uint32_t m = csc_col[i]; m < csc_col[i+1]; m++) {
                for (uint32_t k = m + 1; k < csc_col[i+1]; k++) {
                    int64_t ret = 0;
                    ret = binary_search_yav(csc_row, csc_row[k], csc_col[csc_row[m]],  csc_col[csc_row[m]+1] - 1);
                    if (ret != -1) {                    
                        //count++;
                        
                        vertices[i]++;
                        vertices[csc_row[m]]++;
                        vertices[csc_row[ret]]++;
                    }
                }
            }
        }
    }

    for(uint32_t i = 0; i < n; i++) count += vertices[i];


    clock_gettime(CLOCK_MONOTONIC, &toc);
    //printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    printf("Total triangles: %lu\n", count/3);
}
