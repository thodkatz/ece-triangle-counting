#ifndef V4_PTHREAD_H
#define V4_PTHREAD_H


/*
 * Version 4 using pthread
 */
void v4_pthread(uint32_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_low, uint32_t *csc_col_low, 
                const uint32_t nnz_complete, const uint32_t n);


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

#endif
