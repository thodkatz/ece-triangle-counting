//#include "include/mmio.h"
#include "include/main.h"
#include "include/mmio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * source: https://math.nist.gov/MatrixMarket/mmio/c/example_read.c
 *
 * Convert Matrix Market format to COO
 */
void mm2coo(int argc, char *argv[], uint32_t **rows, uint32_t **columns, uint32_t &nz) {
    MM_typecode matcode;
    FILE *f;
    int m, n; // MxN dimensions (square matrix M=N) 
    // double *val; // dont need this. Our matrices are binary 1 or zero

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

    if (mm_read_banner(f, &matcode) != 0) {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    // what MM formats do you support?
    if (!(mm_is_matrix(matcode) && mm_is_coordinate(matcode) && mm_is_pattern(matcode) && 
            mm_is_symmetric(matcode))) {
        printf("Sorry, this application does not support ");
        printf("Matrix Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */
    if ((mm_read_mtx_crd_size(f, &m, &n, &nz)) !=0) exit(1);
    printf("Number of nnz: %lu\n", nz);

    *rows = (uint32_t*) calloc(nz, sizeof(uint32_t));
    *columns = (uint32_t*) calloc(nz, sizeof(uint32_t));

    int i;
    for (i=0; i<nz; i++) {
        fscanf(f, "%lu %lu\n", &((*rows)[i]), &((*columns)[i]));
        (*rows)[i]--;  /* adjust from 1-based to 0-based */
        (*columns)[i]--;
        //printf("Elements: [%lu, %lu]\n", rows[i], columns[i]);
    }
    printf("Success, MM format is converted to COO\n");

    if (f !=stdin) {
        fclose(f);
        printf("File is closed\n");
    }
}

