import networkx as nx
import numpy as np
import matplotlib.pyplot as plt
import random
import time
import v1
import v2
import trace_cube_matrix as trace
import scipy as sp

nodes = 5
G = nx.erdos_renyi_graph(nodes, 0.5, seed=random.seed(), directed=False)
adjacency = nx.to_numpy_array(G, dtype='int8')
print("The adjacency for this test case will be: ")
print(repr(adjacency))

#nx.draw(G)
#plt.savefig("../../docs/undirected_graph.png")

print("\n<----------v1---------->")
start = time.process_time()
vertices, count = v1.countTriangles(adjacency)
print("Execution time: ", (time.process_time() - start))
print("How many times a vertix belong to a triangle?")
for i in range(len(G)):
    print("The vertice: " + str(i) + " belong to " + str(vertices[i]) + " triangles")
print("Total numbers of triangles (including permutations): ", count/6)

print("\n<----------v2---------->")
start = time.process_time()
vertices, count = v2.countTriangles(adjacency)
print("Execution time: ", (time.process_time() - start))
print("How many times a vertix belong to a triangle?")
for i in range(len(G)):
    print("The vertice: " + str(i) + " belong to " + str(vertices[i]) + " triangles")
print("Total number of triangles: ", count)

# print("\n<----------GeeksForGeeks trace method---------->")
# trace.V = 100 # number of nodes
# start = time.process_time()
# count = trace.triangleInGraph(adjacency)
# print("Execution time: ", (time.process_time() - start))
# print("Total number of triangles: ", count) 

print("\n<----------NetworkX built-in function---------->")
start = time.process_time()
numTriangles = sum(nx.triangles(G).values()) / 3
print("Execution time: ", (time.process_time() - start))
print("Total number of triangles: ", numTriangles)
print("How many times a vertix belong to a triangle?")
print(nx.triangles(G))


print("\n<----------Sparse matrix schemes---------->")
print("Adjacency:")
print(repr(adjacency))
print("COO:")
coo = nx.to_scipy_sparse_matrix(G, format='coo')
print(coo)
print("CSR:")
csr = nx.to_scipy_sparse_matrix(G, format='csr')
print(csr)
print("CSC:")
csc = nx.to_scipy_sparse_matrix(G, format='csc')
print(csc)
