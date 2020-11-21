def countTriangles (adjacency):
    length = len(adjacency[0])
    vertices = [0] * length 
    count = 0

    for i in range(length-2):
        for j in range(i+1, length-1): # --> for j in range(length) will result to take into acvertices 3 times one triangle
            for k in range(j+1,length): # --> for k in range(length) will result to take into acvertices 2 times one triangle
                if (adjacency[i][j] == 1 and adjacency[j][k] == 1 and adjacency[i][k] == 1):
                    vertices[i] += 1
                    vertices[k] += 1
                    vertices[j] += 1
                    count += 1
    return vertices, count

if __name__ == "__main__":
    # Driver code
    # Warning: this array isn't symmetrical as it was supposed to be. Unidrected graph -> symmetrical adjacency

    # graph =[[0,0,0,1,1,1,],
    #         [0,0,1,0,0,1,],
    #         [0,1,0,1,1,0,],
    #         [1,0,1,0,1,0,],
    #         [1,0,1,1,0,1,],
    #         [1,1,0,0,1,0,]]
    graph =[[0,0,0,1,1,1,],
            [0,0,1,0,0,1,],
            [0,0,0,1,1,0,],
            [0,0,0,0,1,0,],
            [0,0,0,0,0,1,],
            [0,0,0,0,0,0,]]
    print("How many times a vertix belong to a triangle?") 
    vertices, count = countTriangles(graph)
    for i in range(len(graph)):
        print("The vertice: " + str(i) + " belong to " + str(vertices[i]) + " triangles")
    print("Total numbers of triangles: " + str(count))
