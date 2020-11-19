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
void mm2coo(FILE *f, uint32_t *rows, uint32_t *columns, uint32_t nz) {
    MM_typecode matcode;
    // double *val; // dont need this. Our matrices are binary 1 or zero

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

    int i;
    for (i=0; i<nz; i++) {
        fscanf(f, "%lu %lu\n", &rows[i], &columns[i]);
        rows[i]--;  /* adjust from 1-based to 0-based */
        columns[i]--;
    }
    printf("Success, MM format is converted to COO\n");

    if (f !=stdin) {
        fclose(f);
        printf("File is closed\n");
    }
}

