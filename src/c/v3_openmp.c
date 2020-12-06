#include "include/main.h"
#include "include/v3_openmp.h"
#include <time.h>
#include <omp.h>

//#define NUM_THREADS 8 The setting will be done via env variable in sbatch script


/*
 * BINARY = 0 -> linear search
 * BINARY = 1 -> binary search
 */
#define BINARY 1

extern uint32_t binary_search_yav(uint32_t* array, uint32_t key, int32_t low, int32_t high);

void v3_openmp(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n, int numThreads) {
    printf("\n----------Version 3 OpenMP----------\n");


    uint32_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    //printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    //printf("The number of threads were : %d\n", omp_get_num_threads());
    omp_set_num_threads(numThreads);


    uint32_t *vertices_openmp = (uint32_t*)calloc(n, sizeof(uint32_t));  // some cases seg fault :(
    #pragma omp parallel 
    {
        int tid = omp_get_thread_num();
        if (tid == 0) printf("The numbers of threads are %d\n", omp_get_num_threads());
        #pragma omp single 
        {
            //printf("The number of available procs are: %d\n", omp_get_num_procs());
        }

        //#pragma omp  for reduction(+:count, vertices[:n]) seg fault, because vertices was global var maybe
        #pragma omp  for schedule(dynamic) reduction(+:count, vertices_openmp[:n])
        for (uint32_t i = 0; i < n; i++) {
            for (uint32_t m = csc_col[i]; m < csc_col[i+1]; m++) {
                for (uint32_t k = m + 1; k < csc_col[i+1]; k++) {
                    #if BINARY == 1
                    uint32_t ret = 0;
                    ret = binary_search_yav(csc_row, csc_row[k], csc_col[csc_row[m]],  csc_col[csc_row[m]+1] - 1);
                    if (ret != -1) {                    
                    #elif BINARY == 0
                    for (uint32_t p = csc_col[csc_row[m]]; p < csc_col[csc_row[m]+1]; p++) {
                        if (csc_row[p] == csc_row[k]) {
                    #endif
                            //count++;
                            vertices_openmp[i]++;
                            vertices_openmp[csc_row[m]]++;
                            #if BINARY == 1
                            vertices_openmp[csc_row[ret]]++;
                            #elif BINARY == 0
                            vertices_openmp[csc_row[p]]++;
                        }
                            #endif
                    }
                }
            }
        }
    }


    clock_gettime(CLOCK_MONOTONIC, &toc);
    //printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    //printf("Time elapsed (seconds): %0.6f\n", diff);
    printf("%0.6f\n", diff);

    //printf("Total triangles openmp: %lu\n", count);
}
