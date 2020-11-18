#include"main.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>

//uint32_t* count_triangles_v2(int *, const size_t);
//int* count_triangles_v3();
void print(uint64_t*, uint32_t);

int main() {

    const uint32_t nodes = 500; 
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

    uint64_t *vertices = v2((int *)adjacency, nodes);
    //print(vertices, nodes);
    free(vertices);

    vertices = v1((int *)adjacency, nodes);
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
