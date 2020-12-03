#include "include/main.h"
#include <time.h>


//extern int binary_search (uint32_t*, uint32_t, int32_t, int32_t); 

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

void v3(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {
    printf("\n----------Version 3 is called----------\n");
    uint32_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    uint32_t j = 0;
    uint32_t diffc = 0;
    uint32_t diffr = 0;

    for(uint32_t start=0; start<(nnz-2);start++) {
        //printf("Current nz: %u\n", start); // be sure that it is still running in large graphs
        diffc = csc_col[j+1] - csc_col[j]; // get the numbers of elements in the ith column    
        diffr = csc_col[j+1] - start - 1; // get the remaining elements per column

        while(diffc == 0) { // Is there any element to work with?
            j++;
            diffc = csc_col[j+1] - csc_col[j];
        }

        if (diffc == 1 || diffr == 0) { // 1) At least two elements are needed in a column, 2) Did we iterate all the elements in a given column? 
            j++; 
            continue;
        }

        if ((csc_col[csc_row[start] + 1] - csc_col[csc_row[start]]) == 0) continue; // if there are no elements in the csc_row[start] then skip
        //if (j == csc_row[start]) continue; // ignore elements in the diagonal

        uint32_t m, k = 0;
        for(m = start + 1; m<csc_col[j+1]; m++) {
            for(k = csc_col[csc_row[start]]; k < csc_col[csc_row[start] + 1]; k++) { // find the start with id: csc_row[start] and iterate over its elements
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
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    printf("Total triangles: %lu\n", count);

}
