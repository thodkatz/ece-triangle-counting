#include "include/main.h"
#include <vector>
#include <pthread.h>

/*
 * 1 -> binary search
 * 2 -> linear search
 */
#define SUM_MODE 1

#define NUM_THREADS 4

extern void print_csr(uint32_t *, uint32_t *, uint32_t, uint32_t);
extern void spmv(uint32_t*, uint32_t*, uint32_t*, uint32_t*, const uint32_t, const uint32_t);
extern int binary_search(uint32_t*, uint32_t, int32_t, int32_t);
extern uint32_t sum_common(uint32_t, uint32_t, uint32_t*, uint32_t*);


// thread argument struct 
typedef struct thread_data {
  int tid;
  uint32_t start;
  uint32_t end;
  uint32_t *csc_row_low;
  uint32_t *csc_col_low;
  uint32_t *csc_row_complete;
  uint32_t *csc_col_complete;
  uint32_t *values;
} thread_data;

// thread function
void* count_triangles(void *arg);


/*
 * Input: the adjacency matrix in a csc scheme for both the complete symmetric and the lower triangular
 *
 * The lower triangular will be used to walk only the half matrix
 *
 */
void v4_pthread(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low,
            const uint32_t nnz_complete, const uint32_t n) {
    printf("\n----------Version 4 Pthread is called----------\n");


    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);


    uint32_t *values = (uint32_t*)malloc(nnz_complete/2 * sizeof(uint32_t));

    // check return values too!
    // you can probably combined them 
    pthread_t threads[NUM_THREADS];
    thread_data data[NUM_THREADS];
    for (uint16_t i = 0; i < NUM_THREADS; i++) {
        data[i].tid = i;
        data[i].csc_row_low = csc_row_low; 
        data[i].csc_col_low = csc_col_low; 
        data[i].csc_row_complete = csc_col_complete; 
        data[i].csc_col_complete = csc_col_complete; 
        data[i].values = values; 
        data[i].start = i*(n/NUM_THREADS);
        data[i].end = data[i].start + n/NUM_THREADS;
        if (i == NUM_THREADS - 1) data[i].end = n;
        pthread_create(&threads[i], NULL, count_triangles, &data[i]);
    }

    for (uint16_t i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);
    //pthread_exit(NULL); //Should I? No because here will terminate the main thread



    for(uint32_t i = 0; i < n; i++) {
        for (uint32_t j = csc_col_low[i]; j < csc_col_low[i+1]; j++) {

            uint32_t c = csc_row_low[j];
            values[j] = sum_common(i, c, (uint32_t*)csc_row_complete, (uint32_t*)csc_col_complete);
        }
    }

    spmv(vertices, csc_row_low, csc_col_low, values, (nnz_complete/2), n);
    free(values);


    uint32_t count = 0;
    for(uint32_t i = 0; i < n; i++) count += vertices[i];

    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);
    
    printf("Total number of triangles: %u\n", count/3);

}

void* count_triangles(void *arg) {
    thread_data *data = (thread_data *)arg;

    printf("Hello, I am %d\n", data->tid);
    printf("Start is %u and end is %u\n", data->start, data->end);

    for(uint32_t i = data->start; i < data->end; i++) {
        for (uint32_t j = data->csc_col_low[i]; j < data->csc_col_low[i+1]; j++) {

            uint32_t c = data->csc_row_low[j];
            data->values[j] = sum_common(i, c, (uint32_t*)data->csc_row_complete, (uint32_t*)data->csc_col_complete);
        }
    }

    pthread_exit(NULL);
}
