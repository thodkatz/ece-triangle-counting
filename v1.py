def countTriangles (adjacency):
    length = len(adjacency[0])
    vertices = [0] * length 

    for i in range(length):
        for j in range(i+1, length): # --> for j in range(length) will result to take into acvertices 3 times one triangle
            if (adjacency[i][j]==1):
                for k in range(j+1,length): # --> for k in range(length) will result to take into acvertices 2 times one triangle
                    if (adjacency[j][k] == 1 and adjacency[k][i] == 1):
                        vertices[i] +=1
                        vertices[k] +=1
                        vertices[j] +=1
    return vertices


# Driver code
graph = [[0, 1, 1, 0],  
         [1, 0, 1, 1],  
         [1, 1, 0, 1],  
         [0, 1, 1, 0]] 
  
print("How many times a vertix belong to a triangle?") 
vertices = countTriangles(graph)
for i in range(len(graph)):
    print("The vertice: " + str(i) + " belong to " + str(vertices[i]) + " triangles")
