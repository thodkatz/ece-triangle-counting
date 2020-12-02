#include "include/v3_cilk.h"
#include "include/main.h"
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>
#include <cilk/reducer_string.h>
#include <cilk/reducer_list.h>
#include <list>

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

#define NWORKERS "4"

void v3_cilk(uint64_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {
    printf("\n----------Version 3 Cilk is called----------\n");



    uint64_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    // cilkrts set param doesnt work with clang
    /* __cilkrts_end_cilk(); */
    /* if (0!= __cilkrts_set_param("nworkers", NWORKERS)) */
    /* { */
    /*     printf("Failed to set worker count\n"); */
    /* } */

    int numWorkers = __cilkrts_get_nworkers();
    printf("Numbers of workers: %d\n", numWorkers);
    
    //int currWorker = __cilkrts_get_worker_number();
    //printf("Current worker: %d\n", currWorker);

    cilk::reducer_opadd<unsigned int> cilk_count;
    cilk::reducer_list_append<unsigned int> cilk_list;
    std::list<unsigned int> indeces; // wont work in parallel
    //std::list<unsigned int>::iterator it;

    //uint64_t cilk_count_mem[numWorkers] = {};
    //uint64_t vertices_cilk[n][numWorkers]; // seg fault

    cilk_for (uint32_t i = 0; i < n; i++) {
        cilk_for (uint32_t m = csc_col[i]; m < csc_col[i+1] - 1; m++) {
            cilk_for (uint32_t k = m + 1; k < csc_col[i+1]; k++) {
                for (uint32_t p = csc_col[csc_row[m]]; p < csc_col[csc_row[m]+1]; p++) {
                    if (csc_row[p] == csc_row[k]) {
                        count++; // race bug
                        //cilk_count_mem[currWorker]++; // should work

                        // how to keep track of the indeces? Are lists parallel safe? No check and act though
                        // eventually stl lists wont work. Use reducers
                        cilk_list->push_back(i);
                        cilk_list->push_back(csc_row[m]);
                        cilk_list->push_back(csc_row[p]);
                        cilk_count++;
                    }
                }
            }
        }
    }
    indeces = cilk_list.get_value() ;
    for (std::list<unsigned int>::iterator i = indeces.begin(); i != indeces.end(); i++) {
        vertices[*i]++;
    }
    // the total numbers of triangle is the length of the list divided by three


    /* uint64_t count_mem = 0; */
    /* for (int i=0; i<numWorkers;i++) { */
    /*     count_mem += cilk_count_mem[i]; */
    /* } */

    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    printf("Total triangles (race bug): %lu\n", count);
    printf("Total triangles using cilk: %u\n", cilk_count.get_value()); 
    printf("Total triangles using indeces: %u\n", indeces.size()/3); 
    //printf("Total triangles using mem: %u\n", count_mem); 

}
