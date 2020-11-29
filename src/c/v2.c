#include "include/main.h"
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void v2(uint64_t *vertices, int *adjacency, const uint32_t nodes) {
    printf("\n----------Version 2 is called----------\n");
    uint64_t count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %lu seconds and %lu nanoseconds\n", tic.tv_sec, tic.tv_nsec);
    int i, j, k;
    for (i=0;i<nodes-2;i++){
        for (j=i+1;j<nodes-1;j++) {
            for (k=j+1;k<nodes;k++) {
                if (*(adjacency + i*nodes + j) == 1 && *(adjacency + j*nodes + k) == 1 && *(adjacency + k*nodes + i) == 1) {
                    vertices[i] +=1;
                    vertices[j] +=1;
                    vertices[k] +=1;
                    count++;
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &toc);
    printf("Toc: %lu seconds and %lu nanoseconds\n", toc.tv_sec, toc.tv_nsec);
    double diff = diff_time(tic, toc);
    printf("Time elapsed (seconds): %0.6f\n", diff);
    printf("Total triangles: %lu\n", count);

}
