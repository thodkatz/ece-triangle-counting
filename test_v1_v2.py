import networkx as nx
import matplotlib.pyplot as plt
import random
import v1
import v2
import trace_cubed_matrix as trace

G = nx.erdos_renyi_graph(50, 0.35, seed=random.seed(), directed=False)
adjacency = nx.to_numpy_array(G)
print("The adjacency for this test case will be: ")
#print(adjacency)

nx.draw(G)
#plt.show()
plt.savefig("docs/undirected_graph.png")


vertices, count = v1.countTriangles(adjacency)
print("\n<----------v1---------->")
print("How many times a vertix belong to a triangle?")
for i in range(len(G)):
    print("The vertice: " + str(i) + " belong to " + str(vertices[i]) + " triangles")
print("Total numbers of triangles: " + str(count))

vertices, count = v2.countTriangles(adjacency)
print("\n<----------v2---------->")
print("How many times a vertix belong to a triangle?")
for i in range(len(G)):
    print("The vertice: " + str(i) + " belong to " + str(vertices[i]) + " triangles")
print("Total numbers of triangles: " + str(count))

print("\n<----------GeeksForGeeks trace method---------->")
trace.V = 50 # number of nodes
print("Total number of Triangle in Graph :", trace.triangleInGraph(adjacency))

print("\n<----------NetworkX built-in function---------->")
numTriangles = sum(nx.triangles(G).values()) / 3
print("Total number of triangles: "+ str(numTriangles))
print("How many times a vertix belong to a triangle?")
print(nx.triangles(G))
