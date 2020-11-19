#include"main.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>

void print(uint64_t*, uint32_t);

int main() {

    const uint32_t nodes = 1000; // maximum 1430
    /* int adjacency[][10] ={{0, 0, 0, 1, 1, 0, 0, 0, 1, 1,}, */
    /*                       {0, 0, 0, 1, 1, 0, 0, 0, 0, 0}, */
    /*                       {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, */
    /*                       {1, 1, 0, 0, 0, 0, 0, 0, 0, 0}, */
    /*                       {1, 1, 0, 0, 0, 0, 1, 1, 0, 0}, */
    /*                     1 {0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, */
    /*                       {0, 0, 0, 0, 1, 1, 0, 1, 1, 1}, */
    /*                       {0, 0, 1, 0, 1, 0, 1, 0, 0, 0}, */
    /*                       {1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, */
    /*                       {1, 0, 0, 0, 0, 1, 1, 0, 0, 0}}; */

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

    return 0;
}

void print(uint64_t *array, uint32_t nodes) {
    for (int i = 0; i<nodes; i++) {
        printf("The %d node is participating in %d triangles\n", i, array[i]);
    }
}


/* int* count_triangles_v3 () { */
/*     // from csc to adjacency? Of course not because the point is to use csc for large data */
/* } */
