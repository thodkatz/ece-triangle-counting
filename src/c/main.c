#include "include/main.h"
#include "include/mmio.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/*
 * MODE =
 * 0 --> sequential
 * 1 --> cilk
 * 2 --> openmp
 * 3 --> pthreads
 */
#define MODE 1

# if MODE == 1
#include "include/v3_cilk.h"
#include "include/v4_cilk.h"
#elif MODE == 2
#include "include/v3_openmp.h"
#endif


void print_vertix(uint64_t*, uint32_t);
void print_csr(uint32_t *, uint32_t *, uint32_t, uint32_t);
void print_coo(uint32_t *, uint32_t *, uint32_t);
uint64_t *vertices;

int main(int argc, char *argv[]) {


    /**********************************************************/
    /*                  Version 1 and 2                       */
    /**********************************************************/

    printf("\n----------Version 1 and 2 Prerequisites----------\n");
    const uint32_t nodes = 5; // maximum 1430
    int adjacency[nodes][nodes] = {0};
    size_t length = sizeof(adjacency)/sizeof(adjacency[0][0]);
    printf("The length of the array is %lu and the nodes are %u\n", length, nodes);

    /* int adjacency[nodes][nodes] = {{0, 0, 0, 1, 1, 1}, */
    /*                            {0, 0, 1, 0, 0, 1}, */
    /*                            {0, 1, 0, 1, 1, 0}, */
    /*                            {1, 0, 1, 0, 1, 0}, */
    /*                            {1, 0, 1, 1, 0, 1}, */
    /*                            {1, 1, 0, 0, 1, 0}}; */

    /* // get only the low half of a symmetrical array */
    /* for(int i = 0; i < nodes; i++) { */
    /*     for (int j = 0; j < nodes; j++) { */
    /*         if(i<j) adjacency[i][j] = 0; */
    /*     } */
    /* } */


    // create a random array WARNING: it should be symmetrical. Not just random */
    srand(time(NULL));
    printf("The random generated array is:\n");
    for (uint32_t i = 0; i < nodes; i++)
    {
        for (uint32_t j = 0; j < i+1; j++)
        {
            adjacency[i][j] = rand()%2;
            adjacency[j][i] = adjacency[i][j];
            if (i == j)
                adjacency[i][i] = 0; // no self loops
        }
    }
    
    for (uint32_t i = 0; i < nodes; i++)
    {
        if (i == 0)
            printf("[[");
        else
            printf(" [");
        for (uint32_t j = 0; j < nodes; j++)
            printf("%d,", adjacency[i][j]);
        if (i == nodes -1)
            printf("]]\n");
        else
            printf("],\n");
    }


    vertices = (uint64_t*)calloc(nodes, sizeof(uint64_t));
    v1((uint64_t*)vertices, (int *)adjacency, nodes);
    //print_vertix(vertices, nodes);
    free(vertices);
    vertices = NULL;

    vertices = (uint64_t*)calloc(nodes, sizeof(uint64_t));
    v2((uint64_t*)vertices, (int *)adjacency, nodes);
    //print_vertix(vertices, nodes);
    free(vertices);
    vertices = NULL;

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

    printf("Number of nnz (excluding diagonal elements): %u\n", nnz);
    printf("Rows/columns: %u\n", n);

    //print_coo(coo_row, coo_col, nnz);

    // COO format to CSC
   
    uint32_t *csc_row_low = (uint32_t*)malloc(nnz * sizeof(uint32_t)); // from the dataset we get the lower triangular by default
    uint32_t *csc_col_low = (uint32_t*)malloc((n+1) * sizeof(uint32_t)); 
    uint32_t isOneBased = 0; // COO is zero based
    
    coo2csc(csc_row_low, csc_col_low, coo_row, coo_col, nnz, n, isOneBased);
    //print_csr(csc_row_low, csc_col_low, nnz, n);
    
    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    v3((uint64_t*)vertices, (uint32_t*)csc_row_low, (uint32_t*)csc_col_low, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;

    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    //v3_pre_cilk((uint64_t*)vertices, (uint32_t*)csc_row_low, (uint32_t*)csc_col_low, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;
    
#if MODE == 1
    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    v3_cilk((uint64_t*)vertices, (uint32_t*)csc_row_low, (uint32_t*)csc_col_low, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;

#elif MODE == 2
    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    v3_openmp((uint64_t*)vertices, (uint32_t*)csc_row_low, (uint32_t*)csc_col_low, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;

    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    v3_openmp_playground((uint64_t*)vertices, (uint32_t*)csc_row_low, (uint32_t*)csc_col_low, nnz, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;
#endif

    printf("\n----------Version 4 Prerequisites----------\n");

    uint32_t* csc_row_up = (uint32_t*)malloc(nnz * sizeof(uint32_t));
    uint32_t* csc_col_up = (uint32_t*)malloc((n+1) * sizeof(uint32_t));

    printf("Swapping the rows and columns. Creating the upper triangle in csc scheme\n");
    coo2csc(csc_row_up, csc_col_up, coo_col, coo_row, nnz, n, 0);
    //print_csr(csc_row_up, csc_col_up, nnz, n);

    uint32_t* csc_row_complete = (uint32_t*)malloc(2*nnz * sizeof(uint32_t));
    uint32_t* csc_col_complete = (uint32_t*)malloc((n+1) * sizeof(uint32_t));
    uint32_t nnz_complete = 0;
    merge_csc(csc_row_low, csc_col_low, csc_row_up, csc_col_up, csc_row_complete, csc_col_complete, nnz_complete, n);

    printf("Numbers of nnz symmetric matrix: %u\nRows/columns: %u\n", nnz_complete, n);
    if (nnz_complete != 2*nnz) printf("Invalid number of non zeros. The function merge_csc() has a bug\n");

    //print_csr(csc_row_complete, csc_col_complete, nnz_complete, n);

    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    v4((uint64_t*)vertices, (uint32_t*)csc_row_complete, (uint32_t*)csc_col_complete, csc_row_low, csc_col_low, nnz_complete, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;

#if MODE == 1
    vertices = (uint64_t*)calloc(n, sizeof(uint64_t));
    v4_cilk((uint64_t*)vertices, (uint32_t*)csc_row_complete, (uint32_t*)csc_col_complete, csc_row_low, csc_col_low, nnz_complete, n);
    //print_vertix(vertices, n);
    free(vertices);
    vertices = NULL;
#endif
    


    // free space 
    free(csc_row_low);
    free(csc_col_low);
    free(csc_row_up);
    free(csc_col_up);
    free(csc_row_complete);
    free(csc_col_complete);
    free(coo_row);
    free(coo_col);

    return 0;
}



void print_vertix(uint64_t *array, uint32_t nodes) {
    for (uint32_t i = 0; i<nodes; i++) {
        printf("The %u node is participating in %lu triangles\n", i, array[i]);
    }
}

void print_csr(uint32_t *csc_row, uint32_t *csc_col, uint32_t nnz, uint32_t n) {
    printf("The csc scheme is: \nRows: ");
    for (uint32_t i = 0; i < nnz; i++) {
        printf("%u, ", csc_row[i]);
    }
    printf("\nColumns: ");
    for (uint32_t i = 0; i < (n+1); i++) {
        printf("%u, ", csc_col[i]);
    }
    printf("\n");

}

void print_coo(uint32_t *coo_row, uint32_t *coo_col, uint32_t nnz) {
    for (uint32_t i=0; i<nnz; i++) {
        printf("[%u, %u]\n", coo_row[i], coo_col[i]);
    }
}
