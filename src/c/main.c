#include "include/main.h"
#include "include/mmio.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

void print_vertix(uint64_t*, uint32_t);

int main(int argc, char *argv[]) {

    const uint32_t nodes = 100; // maximum 1430
    int adjacency[nodes][nodes] = {0};
    size_t length = sizeof(adjacency)/sizeof(adjacency[0][0]);
    printf("The length of the array is %d and the nodes are %d\n", length, nodes);

    // create a random array WARNING: it should be symmetrical. Not just random
    srand(time(NULL));
    printf("The random generated array is: \n[");
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
    
    // print
    /* for (int i = 0; i < nodes; i++) */
    /* { */
    /*     if (i == 0) */
    /*         printf("["); */
    /*     else */
    /*         printf(" ["); */
    /*     for (int j = 0; j < nodes; j++) */
    /*         printf("%d,", adjacency[i][j]); */
    /*     if (i == nodes -1) */
    /*         printf("]]\n"); */
    /*     else */
    /*         printf("],\n"); */
    /* } */


    uint64_t *vertices = v1((int *)adjacency, nodes);
    //print(vertices, nodes);
    free(vertices);

    vertices = v2((int *)adjacency, nodes);
    //print(vertices, nodes);
    free(vertices);

    /**********************************************************/
    /*                    Version 3                           */
    /**********************************************************/

    FILE *f;
    int m, n; // MxN dimensions (square matrix M=N) 
    uint32_t nz; // nz: number of non zero elements 

    // expecting a filename to read (./main <filename>)
    if (argc < 2) {
        printf("Missed command line arguements\n");
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    else {
        if ((f = fopen(argv[1], "r")) == NULL) { 
            printf("Can't open file\n");
            exit(1);  
        }
    }

    // get number of non zero elements
    if ((mm_read_mtx_crd_size(f, &m, &n, &nz)) !=0)
        exit(1);

    // read MM format
    f = fopen(argv[1], "r"); // get back on track (due to the above call current line isn't the first)
    uint32_t *rows, *columns;
    rows = (uint32_t*) malloc(nz * sizeof(uint32_t));
    columns = (uint32_t*) malloc(nz * sizeof(uint32_t));

    mm2coo(f, rows, columns, nz);

    // coo2csc
    
    

    // call version 3

    return 0;
}

void print_vertix(uint64_t *array, uint32_t nodes) {
    for (int i = 0; i<nodes; i++) {
        printf("The %d node is participating in %d triangles\n", i, array[i]);
    }
}
