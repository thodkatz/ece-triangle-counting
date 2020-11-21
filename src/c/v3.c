#include "include/main.h"

extern void print_vertix(uint64_t *array, uint32_t nodes);
/*
 * nnz: Number of non zero elements (half of the total because matrix symmetric)
 * n: Rows/columns 
 */
uint64_t* v3(uint32_t *csc_row, uint32_t *csc_col, const uint32_t nnz, const uint32_t n) {
    printf("\n----------Version 3 is called----------\n");
    uint64_t* vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    uint64_t count = 0;

    uint32_t j = 0;
    uint32_t diffc = 0;
    uint32_t diffr = 0;
    for(uint32_t start=0; start<(nnz-2);start++) {
        printf("Current nz: %lu\n", start); // be sure that it is still running in large graphs
        diffc = csc_col[j+1] - csc_col[j]; // get the numbers of elements in the ith column    
        diffr = csc_col[j+1] - start - 1;

        while(diffc == 0) {
            j++;
            diffc = csc_col[j+1] - csc_col[j];
        }

        if (diffc == 1 || diffr == 0) {
            j++; 
            continue;
        }

        if (j == csc_row[start]) continue; // ignore elements in the diagonal

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

    printf("Total triangles: %lu\n", count);

    return vertices;
}
