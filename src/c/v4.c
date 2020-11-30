#include "include/main.h"
#include <vector>

/*
 * 1 -> binary search
 * 2 -> linear search
 */
#define SUM_MODE 1

uint32_t* spmv(uint64_t*, uint32_t*, uint32_t*, std::vector<uint32_t>, uint32_t*, const uint32_t, const uint32_t);
int binary_search (uint32_t *, uint32_t, int32_t, int32_t);
uint32_t sum_common(uint32_t, uint32_t, uint32_t*, uint32_t*);


extern void print_csr(uint32_t *, uint32_t *, uint32_t, uint32_t);

/*
 * Input: the adjacency matrix in a csc scheme for both the complete symmetric and the down triagonal
 *
 * The down triagonal will be used to walk only the half matrix
 *
 */
void v4(uint64_t *vertices, uint32_t *csc_row_complete, uint32_t *csc_col_complete, uint32_t *csc_row_down, uint32_t *csc_col_down,
            const uint32_t nnz_complete, const uint32_t n) {
    printf("\n----------Version 4 is called----------\n");

    std::vector<uint32_t> values;

    /* printf("Complete:\n"); */
    /* print_csr(csc_row_complete, csc_col_complete, nnz_complete, n); */
    /* printf("Down\n"); */
    /* print_csr(csc_row_down, csc_col_down, nnz_complete/2, n); */

    for(uint32_t i = 0; i < n; i++) {
        for (uint32_t j = csc_col_down[i]; j < csc_col_down[i+1]; j++) {
            // now we have i and csc_row_down[j], the coordinates
            uint32_t c = csc_row_down[j];
            //printf("The i is %d and the c is %d\n", i, c);
            //printf("The i is %d and the c is %d\n", i, c);
            values.push_back(sum_common(i, c, (uint32_t*)csc_row_complete, (uint32_t*)csc_col_complete));
        }
    }

    /* printf("Printing vector values\n"); */
    /* for (int i = 0; i< values.size(); i++){ */
    /*     printf("%u,", values[i]); */
    /* } */
    /* printf("\n"); */

    //uint32_t* y = (uint32_t*)calloc(n, sizeof(uint32_t));
    uint32_t x[n] = {0};
    for (int i = 0; i < n; i++) x[i] = 1;
    spmv(vertices, csc_row_down, csc_col_down, values, x, (nnz_complete/2), n);

    /* printf("The vertices\n"); */
    /* for(int i=0; i<n; i++){ */
    /*     printf("%d, ", vertices[i]); */
    /* } */
    /* printf("\n"); */

    uint32_t count = 0;
    for(uint32_t i = 0; i < n; i++) count += vertices[i];
    printf("Total number of triangles: %u\n", count/3);
    

}

/*
 * Returns the sum of the common elements of a symmetric csc matrix for two nodes
 */
uint32_t sum_common(uint32_t i,uint32_t j, uint32_t *csc_row, uint32_t *csc_col) {
    
    //printf("Sum common complete\n");
    //print_csr(csc_row, csc_col, 10, 5);

    uint32_t value = 0;

    uint32_t start1 = csc_col[i];
    uint32_t end1 = csc_col[i+1];
    uint32_t diff1 = end1 - start1;

    uint32_t start2 = csc_col[j];
    uint32_t end2 = csc_col[j+1];
    uint32_t diff2 = end2 - start2;

    /* printf("The %d node\n", i); */
    /* for(int i = start1; i < end1; i++) printf("%d ", csc_row[i]); */
    /* printf("\n"); */
    /* printf("The %d node\n", j); */
    /* for(int i = start2; i < end2; i++) printf("%d ", csc_row[i]); */
    /* printf("\n"); */

#if SUM_MODE == 1
    // iterate the elements of the smaller one and use binary search for the bigger one
    if (diff1 <= diff2) {
        for (uint32_t k = start1; k < end1; k++)
            value += binary_search((uint32_t*)csc_row, csc_row[k], (int32_t)start2, (int32_t)end2);
    }
        
    if (diff2 < diff1) {
        for (uint32_t k = start2; k < end2; k++)
            value += binary_search((uint32_t*)csc_row, csc_row[k], (int32_t)start1, (int32_t)end1);
    }
#endif

#if SUM_MODE == 2
    // linear search sorted arrays
    uint32_t i = start1;
    uint32_t j = start2;
    while (uint32_t i < end_one && uint32_t j < end_two) {
        if (csc_row[i] == csc_row[j]) {
            values++;
            i ++;
            j ++;
        }
        else if(csc_row[i] < csc_row[j]) i++;
        else j ++;
    }
#endif
    //printf("The value is %d\n", value);

    return value;
}

// high should be signed!!
int binary_search (uint32_t *array, uint32_t key, int32_t low, int32_t high) {

    /* for (int i=low; i<high; i++){ */
    /*     printf("The element array is: %d\n", array[i]); */
    /* } */
    if (high >= low) { 
        //printf("The high is %d and %u and the low is %d\n", high,high, low);
        int32_t mid = low + (high - low) / 2; 
        //printf("The mid is %d\n", mid);
  
        if (array[mid] == key) 
            return 1; 
  
        if (array[mid] > key) 
            return binary_search(array, key, low, mid-1); 
  
        return binary_search(array, key, mid + 1, high); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return 0; 
} 

/*
 * Sparse symmetric matrix vector multiplication 
 *
 * We divide the values by two to find the correct number of triangles.
 *
 */
uint32_t* spmv(uint64_t *y, uint32_t *csc_row, uint32_t *csc_col, std::vector<uint32_t> values, uint32_t* x, const uint32_t nnz, const uint32_t n) {

    // x vector will be always 1, so change x -> 1
    for(uint32_t i = 0; i<n; i++) {
        for(uint32_t j = csc_col[i]; j < csc_col[i+1]; j++) {
            //printf("The i is %d and the j is %d\n", i, j);
            y[i] += values[j] * x[csc_row[j]];
            y[csc_row[j]] += values[j] * x[i]; 
        }
    }

    /* printf("the test was %d", values[0]); */
    /* float temp = (float)values[0] * 0.5; // why this */ 
    /* printf("Test is %f", temp); */
    for (int i = 0; i < n; i ++) y[i] /=2;
}
