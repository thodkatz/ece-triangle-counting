#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<stdint.h>

uint32_t* count_triangles_v2(int *, const size_t);
//int* count_triangles_v3();

int main() {

    const uint32_t nodes = 1000; 
    // int adjacency[][nodes] = {{0, 1, 0, 0, 1, 0},
    //                    {1, 0, 1, 0, 1, 0},
    //                    {0, 1, 0, 1, 0, 0},
    //                    {0, 0, 1, 0, 1, 1},
    //                    {1, 1, 0, 1, 0, 0},
    //                    {0, 0, 0, 1, 0, 0}};

    // create a random array
    int adjacency[nodes][nodes] = {0};
    srand(time(NULL));
    printf("The random generated array is: \n[");
    for (int i = 0; i < nodes; i++)
    {
        //printf("[");
        for (int j = 0; j < nodes; j++)
        {
            adjacency[i][j] = rand()%2;
            //printf("%d,", adjacency[i][j]);
        }
        //printf("],\n");
    }
    printf("]\n");
    
    
    size_t length = sizeof(adjacency)/sizeof(adjacency[0][0]);
    printf("The length of the array is %d and the nodes are %d\n", length, nodes);

    uint32_t *vertices = count_triangles_v2((int *)adjacency, nodes);

    for (int k = 0; k<nodes; k++) {
            printf("The %d node is participating in %d triangles\n", k, vertices[k]);
        }

    free(vertices);

    return 0;
}

uint32_t* count_triangles_v2 (int *adjacency, const size_t nodes) {
    uint32_t *vertices = (uint32_t*)calloc(nodes, sizeof(uint32_t));
    int count = 0;

    struct timespec tic;
    struct timespec toc;
    clock_gettime(CLOCK_MONOTONIC, &tic);
    printf("Toc: %ld\n", tic.tv_nsec);
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

/* int* count_triangles_v3 () { */
/*     // from csc to adjacency? Of course not because the point is to use csc for large data */
/* } */
