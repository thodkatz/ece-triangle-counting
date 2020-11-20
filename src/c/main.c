#include "include/main.h"
#include "include/mmio.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

void print_vertix(uint64_t*, uint32_t);
uint64_t *vertices;

int main(int argc, char *argv[]) {


    /**********************************************************/
    /*                  Version 1 and 2                       */
    /**********************************************************/

    const uint32_t nodes = 50; // maximum 1430
    int adjacency[nodes][nodes] = {0};
    size_t length = sizeof(adjacency)/sizeof(adjacency[0][0]);
    printf("The length of the array is %d and the nodes are %d\n", length, nodes);

    // create a random array WARNING: it should be symmetrical. Not just random
    srand(time(NULL));
    printf("The random generated array is:\n");
    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < i+1; j++)
        {
            adjacency[i][j] = rand()%2;
            adjacency[j][i] = adjacency[i][j];
            if (i == j)
                adjacency[i][i] = 0; // no self loops
        }
    }
    
    /* for (int i = 0; i < nodes; i++) */
    /* { */
    /*     if (i == 0) */
    /*         printf("[["); */
    /*     else */
    /*         printf(" ["); */
    /*     for (int j = 0; j < nodes; j++) */
    /*         printf("%d,", adjacency[i][j]); */
    /*     if (i == nodes -1) */
    /*         printf("]]\n"); */
    /*     else */
    /*         printf("],\n"); */
    /* } */


    vertices = v1((int *)adjacency, nodes);
    //print(vertices, nodes);
    free(vertices);

    vertices = v2((int *)adjacency, nodes);
    //print(vertices, nodes);
    free(vertices);

    /**********************************************************/
    /*                    Version 3                           */
    /**********************************************************/

    // Matrix Market format to COO
    uint32_t *coo_row; 
    uint32_t *coo_col; 
    uint32_t nnz = 0; // nnz: number of non zero elements 
    uint32_t n = 0; // rows/columns

    mm2coo(argc, argv, &coo_row, &coo_col, nnz, n);

    printf("Number of nnz: %lu\n", nnz, n);
    printf("Rows/columns: %lu\n", n);

    int i, j;
    for (i=0; i<nnz; i++) {
        //printf("[%lu, %lu]\n", coo_row[i], coo_col[i]);
    }

    // COO format to CSC
    uint32_t *csc_row = (uint32_t*)malloc(nnz * sizeof(uint32_t)); 
    uint32_t *csc_col = (uint32_t*)malloc((n+1) * sizeof(uint32_t)); 
    uint32_t isOneBased = 0; // COO is zero based
    
    coo2csc(csc_row, csc_col, coo_row, coo_col, nnz, n, isOneBased);
    
    // call version 3
    uint64_t count = v3(csc_row, csc_col, nnz, n);

    // free space
    free(csc_row);
    free(csc_col);
    free(coo_row);
    free(coo_col);

    return 0;
}

void print_vertix(uint64_t *array, uint32_t nodes) {
    for (int i = 0; i<nodes; i++) {
        printf("The %d node is participating in %d triangles\n", i, array[i]);
    }
}
