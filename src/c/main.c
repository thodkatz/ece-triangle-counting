#include "include/main.h"
#include "include/mmio.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
//#include "include/cilk_api.h""

void print_vertix(uint64_t*, uint32_t);
uint64_t *vertices;

int main(int argc, char *argv[]) {


    /**********************************************************/
    /*                  Version 1 and 2                       */
    /**********************************************************/

    const uint32_t nodes = 6; // maximum 1430
    int adjacency[nodes][nodes] = {0};
    size_t length = sizeof(adjacency)/sizeof(adjacency[0][0]);
    printf("The length of the array is %lu and the nodes are %u\n", length, nodes);

    /* int adjacency[nodes][nodes] = {{0, 0, 0, 1, 1, 1}, */
    /*                            {0, 0, 1, 0, 0, 1}, */
    /*                            {0, 1, 0, 1, 1, 0}, */
    /*                            {1, 0, 1, 0, 1, 0}, */
    /*                            {1, 0, 1, 1, 0, 1}, */
    /*                            {1, 1, 0, 0, 1, 0}}; */

    /* // get only the down half of a symmetrical array */
    /* for(int i = 0; i < nodes; i++) { */
    /*     for (int j = 0; j < nodes; j++) { */
    /*         if(i<j) adjacency[i][j] = 0; */
    /*     } */
    /* } */


    // create a random array WARNING: it should be symmetrical. Not just random */
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
    
    for (int i = 0; i < nodes; i++)
    {
        if (i == 0)
            printf("[[");
        else
            printf(" [");
        for (int j = 0; j < nodes; j++)
            printf("%d,", adjacency[i][j]);
        if (i == nodes -1)
            printf("]]\n");
        else
            printf("],\n");
    }


    vertices = v1((int *)adjacency, nodes);
    //print_vertix(vertices, nodes);
    free(vertices);

    vertices = v2((int *)adjacency, nodes);
    //print_vertix(vertices, nodes);
    free(vertices);

    /**********************************************************/
    /*                    Version 3                           */
    /**********************************************************/

    printf("\n----------Version 3 Prerequisites----------\n");

    // Matrix Market format to COO
    uint32_t *coo_row; 
    uint32_t *coo_col; 
    uint32_t nnz = 0; // nnz: number of non zero elements 
    uint32_t n = 0; // rows/columns

    mm2coo(argc, argv, &coo_row, &coo_col, nnz, n);

    printf("Number of nnz: %u\n", nnz, n);
    printf("Rows/columns: %u\n", n);

    int i, j;
    for (i=0; i<nnz; i++) {
        //printf("[%lu, %lu]\n", coo_row[i], coo_col[i]);
    }

    // COO format to CSC
   
    // testing stuff
    //nnz = 9;
    //n = 6;
    uint32_t *csc_row = (uint32_t*)malloc(nnz * sizeof(uint32_t)); 
    uint32_t *csc_col = (uint32_t*)malloc((n+1) * sizeof(uint32_t)); 
    uint32_t isOneBased = 0; // COO is zero based
    
    //uint32_t coo_col_test[9] = {0, 0, 0, 1, 1, 2, 2, 3, 4};
    //uint32_t coo_row_test[9] = {3, 4, 5, 2, 5, 3, 4, 4, 5};
    coo2csc(csc_row, csc_col, coo_row, coo_col, nnz, n, 0);
    /* int rr, cc; */
    /* printf("csr: "); */
    /* for (rr=0; rr<nnz; rr++) { */
    /*     printf("%ld, ", csc_row[rr]); */
    /* } */
    /* printf("csc: "); */
    /* for (cc=0; cc<(n+1); cc++) { */
    /*     printf("%ld, ", csc_col[cc]); */
    /* } */
    /* printf("\n"); */

    
    // call version 3
    vertices = v3((uint32_t*)csc_row, (uint32_t*)csc_col, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);

    vertices = v3_cilk((uint32_t*)csc_row, (uint32_t*)csc_col, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);

    // free space
    free(csc_row);
    free(csc_col);
    free(coo_row);
    free(coo_col);

    return 0;
}

void print_vertix(uint64_t *array, uint32_t nodes) {
    for (int i = 0; i<nodes; i++) {
        printf("The %d node is participating in %lu triangles\n", i, array[i]);
    }
}
