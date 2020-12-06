#include "include/v3_cilk.h"
#include "include/main.h"
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_opadd.h>
#include <cilk/reducer_string.h>
#include <cilk/reducer_list.h>
#include <list>

#define NWORKERS "4" 


extern uint32_t binary_search_yav(uint32_t* array, uint32_t key, int32_t low, int32_t high);

void v3_cilk(uint32_t *vertices, uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {

    printf("\n----------Version 3 Cilk----------\n");

    uint32_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    //printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    // cilkrts set param doesnt work with clang
    __cilkrts_end_cilk();
    if (0!= __cilkrts_set_param("nworkers", NWORKERS))
    {
        printf("Failed to set worker count\n");
    }

    int numWorkers = __cilkrts_get_nworkers();
    printf("Numbers of workers: %d\n", numWorkers);
    

    //cilk::reducer_opadd<unsigned int> cilk_count;
    cilk::reducer_list_append<unsigned int> cilk_list;
    std::list<unsigned int> indeces; 

    //uint32_t cilk_count_mem[numWorkers] = {};
    //uint32_t vertices_cilk[n][numWorkers]; 

    cilk_for (uint32_t i = 0; i < n; i++) {
        cilk_for (uint32_t m = csc_col[i]; m < csc_col[i+1] - 1; m++) {
            for (uint32_t k = m + 1; k < csc_col[i+1]; k++) {
                uint32_t ret = 0;
                ret = binary_search_yav(csc_row, csc_row[k], csc_col[csc_row[m]],  csc_col[csc_row[m]+1] - 1);
                if (ret != -1) {
                    //cilk_count++;
                    //cilk_count_mem[currWorker]++; 

                    cilk_list->push_back(i);
                    cilk_list->push_back(csc_row[m]);
                    cilk_list->push_back(csc_row[ret]);
                }
            }
        }
    }

    indeces = cilk_list.get_value();
    for (std::list<unsigned int>::iterator i = indeces.begin(); i != indeces.end(); i++) {
        vertices[*i]++;
    }
    // the total numbers of triangle is the length of the list divided by three


    /* uint32_t count_mem = 0; */
    /* for (int i=0; i<numWorkers;i++) { */
    /*     count_mem += cilk_count_mem[i]; */
    /* } */

    clock_gettime(CLOCK_MONOTONIC, &toc);
    //printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);

    //printf("Total triangles using opadd: %u\n", cilk_count.get_value()); 
    printf("Total triangles: %u\n", indeces.size()/3); 
    //printf("Total triangles using mem: %u\n", count_mem); 

}
