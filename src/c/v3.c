#include<stdio.h>
#include<stdlib.h>

// declare functions
int* count_triangles_v2(int**,const size_t length);
int* count_triangles_v3(int***);

int main() {

    int adjacency[6][6] = {{0, 1, 0, 0, 1, 0},
                       {1, 0, 1, 0, 1, 0},
                       {0, 1, 0, 1, 0, 0},
                       {0, 0, 1, 0, 1, 1},
                       {1, 1, 0, 1, 0, 0},
                       {0, 0, 0, 1, 0, 0}};

    const int nodes = 6;
    printf("The length of the array is %d", nodes);
    printf("\nTest %d", *(adjacency[0] + 5));
    int ***csc;

    int* vertices = count_triangles_v2(adjacency, nodes);

    printf("\n");
    int i;
    for (i = 0; i<nodes; i++) {
            printf("The %d node is participating in %d triangles\n", i, vertices[i]);
        }

    return 0;
}

int* count_triangles_v2 (int** adjacency,const size_t length) {
    int *vertices = (int*)malloc(length * sizeof(int));

    int i, j, k;
    for (i=0;i<length;i++){
        for (j=i+1;j<length;j++) {
            for (k=j+1;k<length;k++) {
                if (adjacency[i][j] == adjacency[j][k] == adjacency[k][i] == 1){
                    vertices[i] +=1;
                    vertices[j] +=1;
                    vertices[k] +=1;
                }
            }
        }
    }
    return vertices;
}

int* count_triangles_v3 (int*** adjacency) {
    // from csc to adjacency? Of course not because the point is to use csc for large data
}
