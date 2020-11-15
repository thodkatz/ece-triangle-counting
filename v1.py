# adjacency matrix is symmetrical because the graph that represents is undirected.
# So we know that we want to iterate only the one half of the array. Are you sure?
# If there is a triangle for the edge 1-2, then the node 1 and 2 has one triangle 
# as centered nodes of a triplet. Actually for that node there are two triangles,
# if you swap the other two element
def count_triangles (adjacency):
    length = len(adjacency)
    count = [[0] * length] 

    for i in range(length):
        for j in range(i, length):
            if (i == j): 
                continue
            if (adjacency[i][j]==1):
                for k in range (0,length):
                    if (k == j):
                        continue
                    if (adjacency[j][k] == 1):
                        if (adjacency[k][i] == 1):
                            count[i]+=1
                            count[j]+=1
                            count[k]+=1


