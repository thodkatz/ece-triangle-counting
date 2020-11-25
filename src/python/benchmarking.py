# This is a more clean approach for v1 and v2 for run time analysis. We removed the return statement and the count variable 

import numpy as np
import timeit
import matplotlib.pyplot as plt
import networkx as nx
import random


def v1(adjacency):
    length = len(adjacency[0])
    vertices = [0] * length 

    for i in range(length):
        for j in range(length): 
            for k in range(length): 
                if (adjacency[i][j] == 1 and adjacency[j][k] == 1 and adjacency[k][i] == 1):
                    vertices[i] += 1
                    vertices[k] += 1
                    vertices[j] += 1

def v2(adjacency):
    length = len(adjacency[0])
    vertices = [0] * length 

    for i in range(length):
        for j in range(i+1, length): 
            for k in range(j+1,length): 
                if (adjacency[i][j] == 1 and adjacency[j][k] == 1 and adjacency[k][i] == 1):
                    vertices[i] += 1
                    vertices[k] += 1
                    vertices[j] += 1


x = [100, 138, 193, 268, 372, 517, 719, 1000]
y = [0.015875, 0.013514, 0.025516, 0.084094, 0.165088, 0.475962, 1.247957, 3.402919]
z = [0.001374, 0.002365, 0.003725, 0.009741, 0.027067, 0.074512, 0.202955, 0.557182]
d = []
for i in range(len(x)):
    d.append(y[i]//z[i])

plt.rc('text', usetex=True)
plt.subplot(2,1,1)
plt.title("Benchmarking v1 and v2")
#plt.xlabel('Number of nodes', fontsize=12)
plt.ylabel(r'$t_\mathrm{v1}/t_\mathrm{v2}$', fontsize=12)
plt.rc('text', usetex=True)
plt.plot(x, d, 'o')
plt.grid(True)

plt.subplot(2,1,2)
plt.plot(x, y, 'o', label = "v1")
plt.plot(x, z, 'o', label = "v2")
plt.xlabel('Number of nodes', fontsize=12)
plt.ylabel('Time (s)', fontsize=12)
plt.grid(True)

plt.legend()
# plt.savefig('../../docs/benchmarking_C.png', dpi=250)
plt.show()

# source: https://gertingold.github.io/tools4scicomp/profiling.html
# run time analysis using timeit


#matrix_nodes = []
#matrix_t_v1 = []
#matrix_t_v2 = []
#ratio = []
#for n in np.logspace(2,3, 8):
#    G = nx.erdos_renyi_graph(int(n), 0.3, seed=random.seed(), directed=False)
#    adjacency = nx.to_numpy_array(G)
#    t_v1 = timeit.timeit('v1(adjacency)', number=3, globals=globals()) 
#    t_v2 = timeit.timeit('v2(adjacency)', number=3, globals=globals())
#    matrix_nodes.append(int(n))
#    matrix_t_v1.append(t_v1)
#    matrix_t_v2.append(t_v2)
#    ratio.append(t_v1/t_v2)
#    print(n)

# Tweak spacing to prevent clipping of ylabel
# plt.subplots_adjust(left=0.15)#plt.rc('text', usetex=True)

#plt.subplot(2,1,1)
#plt.title("Benchmarking v1 and v2")
##plt.xlabel('Number of nodes', fontsize=12)
#plt.ylabel(r'$t_\mathrm{v1}/t_\mathrm{v2}$', fontsize=12)
#plt.rc('text', usetex=True)
#plt.plot(matrix_nodes, ratio, 'o')
#plt.grid(True)

#plt.subplot(2,1,2)
#plt.plot(matrix_nodes, matrix_t_v1, 'o', label = "v1")
#plt.plot(matrix_nodes, matrix_t_v2, 'o', label = "v2")
#plt.xlabel('Number of nodes', fontsize=12)
#plt.ylabel('Time (s)', fontsize=12)
#plt.grid(True)

#plt.legend()
## plt.savefig('../../docs/benchmarking.png', dpi=250)
#plt.show()
