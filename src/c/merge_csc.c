#include "include/main.h"
/*
 *  Merge two csc schemes of a lower and upper triangle and create one csc scheme of a complete symmetric matrix.
 *  This scheme will be used to exploit the sorted lists in the v4.
 *
 *  nnz: The non zero elements of the complete symmetic matrix in csc scheme
 */
void merge_csc(uint32_t* csc_row_down, uint32_t* csc_col_down, uint32_t* csc_row_up, uint32_t* csc_col_up, 
               uint32_t* csc_row_complete, uint32_t* csc_col_complete, uint32_t& nnz, uint32_t& n) {

    csc_col_complete[0] = 0;
    uint32_t diff_down = 0;
    uint32_t diff_up = 0;

    for(uint32_t i = 0; i < n; i++) {
        uint32_t start_up = csc_col_up[i];
        uint32_t end_up = csc_col_up[i+1];
        diff_up += end_up - start_up;
        for (uint32_t j = start_up; j < end_up; j++){
            csc_row_complete[j + diff_down] = csc_row_up[j];
        }

        uint32_t start_down = csc_col_down[i];
        uint32_t end_down = csc_col_down[i+1];
        diff_down += end_down - start_down;
        for (uint32_t j = start_down; j < end_down; j++) {
            csc_row_complete[j + diff_up] = csc_row_down[j];
        }

        nnz = end_down + end_up;
        csc_col_complete[i+1] = nnz;
    }

    csc_col_complete[n] = nnz;
}

