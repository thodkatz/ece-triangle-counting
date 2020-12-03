#include "include/main.h"
#include <time.h>

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


uint32_t binary_search_yav(uint32_t* array, uint32_t key, int32_t low, int32_t high);


void v3_pre_cilk(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {
    printf("\n----------Version 3 Pre Cilk is called----------\n");

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
 * Yet another version of binary search. In this implementation we need to know the mid var
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
