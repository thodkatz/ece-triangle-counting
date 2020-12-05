#include "include/main.h"
#include <vector>
#include <pthread.h>
#include <time.h>

//#define NUM_THREADS 8

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
            const uint32_t nnz_complete, const uint32_t n, int numThreads) {

    //printf("\n----------Version 4 Pthread----------\n");
    printf("----------Version 4 Pthread Binary----------\n");


    struct timespec tic;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    //printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);

    uint32_t *values = (uint32_t*)malloc(nnz_complete/2 * sizeof(uint32_t));


    size_t stacksize;
    pthread_attr_t atr;
    pthread_attr_init(&atr); // this is mandatory. Otherwise you get grabage memory values
    pthread_attr_getstacksize(&atr, &stacksize);
    //printf("Current stack size -> %d\n", stacksize);

    int rc;
    pthread_t threads[numThreads];
    thread_data data[numThreads];
    printf("%d\n", numThreads);

        for (uint16_t i = 0; i < numThreads; i++) {
        // init
        data[i].tid = i;
        data[i].csc_row_low = csc_row_low; 
        data[i].csc_col_low = csc_col_low; 
        data[i].csc_row_complete = csc_row_complete; 
        data[i].csc_col_complete = csc_col_complete; 
        data[i].values = values; 
        data[i].start = i*(n/numThreads);
        data[i].end = data[i].start + n/numThreads;
        if (i == numThreads - 1) data[i].end = n;

        if (rc = pthread_create(&threads[i], NULL, count_triangles, &data[i]))
            printf("Error creating thread %d\n", rc);
    }

    for (uint16_t i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);


    spmv(vertices, csc_row_low, csc_col_low, values, (nnz_complete/2), n);
    free(values);

    uint32_t count = 0;
    for(uint32_t i = 0; i < n; i++) count += vertices[i];

    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &toc);
    //printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);

    double diff = diff_time(tic, toc);
    //printf("Time elapsed (seconds): %0.6f\n", diff);
    printf("%0.6f\n", diff);
    
    //printf("Total number of triangles: %u\n", count/3);

}

void* count_triangles(void *arg) {
    thread_data *data = (thread_data *)arg;

    //printf("Hello, I am %d\n", data->tid);
    //printf("For id: %d. Start is %u and end is %u\n", data->tid, data->start, data->end);

    struct timespec ping;
    clock_gettime(CLOCK_MONOTONIC, &ping);

    for(uint32_t i = data->start; i < data->end; i++) {
        for (uint32_t j = data->csc_col_low[i]; j < data->csc_col_low[i+1]; j++) {

            uint32_t c = data->csc_row_low[j];
            data->values[j] = sum_common(i, c, (uint32_t*)data->csc_row_complete, (uint32_t*)data->csc_col_complete);
        }
    }
    // load balancing testing
    struct timespec pong;
    clock_gettime(CLOCK_MONOTONIC, &pong);
    double elapsed = diff_time(ping, pong);
    //printf("Finished id: %d. Elapsed time: %0.6f\n", data->tid, elapsed);



    pthread_exit(NULL);
}
