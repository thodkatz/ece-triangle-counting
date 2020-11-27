#include "include/main.h"
#include "include/v3_openmp.h"
#include <time.h>
#include <omp.h>

/*
 * Counting triangles: The concept is, given a csc format from a down half symmetric matrix, to counte all the possible triangles.
 * One criteria to follow to be sure that you count properly without permuations is the rule: i<j<k following edges. In our test case
 * matrices are symmetrical so you can think this problem given a csr format. So, in this function we are trying first to find two
 * potential points of a triangle in a row e.g. a[i][j] and a[i][k]. The elements in a csr format are scanned by row in a snake way. So 
 * it is convenient for a given row to check the elemets right of the current in the ith row. Now that we have spotted a[i][j] and a[i][k],
 * find the jth row and iteratevthe elements by columns. Is there a[j][k]? Then you find a triangle. Still the rule is applied i<j<k.
 *
 * Notes:
 * Key elemets are: 1) The compressed array in the ith element indicates how many nnz have passed until (including) the ith-1 row. 
 *                  2) Substracting the values of index (i+1) and i, will result to the number of nnz in the ith row.
 * In the same way the above can be applied to a csc scheme mindset
 *
 * nnz: Number of non zero elements (half of the total because matrix symmetric)
 * n: Rows/columns 
 */

#define NUM_THREADS 8

uint64_t* v3_openmp(uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {
    printf("\n----------Version 3 OpenMP is called----------\n");



    uint64_t* vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    uint64_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    printf("The number of threads were : %d\n", omp_get_num_threads());
    omp_set_num_threads(NUM_THREADS);

    int tid = 0;
    
    #pragma omp parallel
    {
        tid = omp_get_thread_num();
        if (tid == 0) printf("The numbers of threads are %d\n", omp_get_num_threads());

        #pragma omp  for reduction(+:count) reduction(+:vertices[:n])
        for (uint32_t i = 0; i < n; i++) {
            for (uint32_t m = csc_col[i]; m < csc_col[i+1]; m++) {
                if (csc_row[m] == i) continue; // ignore elements in diagonal
                for (uint32_t k = m + 1; k < csc_col[i+1]; k++) {
                    for (uint32_t p = csc_col[csc_row[m]]; p < csc_col[csc_row[m]+1]; p++) {
                        if (csc_row[p] == csc_row[k]) {
                            vertices[i]++;
                            vertices[csc_row[m]]++;
                            vertices[csc_row[p]]++;
                            count++;
                        }
                    }
                }
            }
        }
    }



    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    printf("Total triangles openmp: %lu\n", count);

    return vertices;
}
