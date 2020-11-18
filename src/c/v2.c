#include"main.h"
#include<time.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>

uint64_t* v2(int *adjacency, const uint32_t nodes) {
    printf("Version 2 is called\n");
    uint64_t *vertices = (uint64_t *)calloc(nodes, sizeof(uint64_t));
    int count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Tic: %ld\n", tic.tv_nsec);
    int i, j, k;
    for (i=0;i<nodes;i++){
        for (j=i+1;j<nodes;j++) {
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
    printf("Toc: %ld\n", toc.tv_nsec);
    printf("Time elapsed (seconds): %0.6f\n", (toc.tv_nsec - tic.tv_nsec)/(float)(1e9));
    printf("Total triangles: %d\n", count);

    return vertices;
}
