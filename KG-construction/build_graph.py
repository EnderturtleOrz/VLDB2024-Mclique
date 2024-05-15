import random

n = 115821243 
ID_NAME = 16715
ID_TYPE = 266257437

edges = []
freebase_new = '/mnt/data/yingli/mclique/freebase_new/new_detected/'

with open(freebase_new+'graph_edge.txt', 'r') as file:
    for line in file:
        edges.append(tuple(map(int, line.strip().split())))
        
print("Read Edges finished")

new_edges = []
for i in range(n):
    new_edges.append((i, i + n))
    new_edges.append((i, i + 2 * n))

print("Create and Shuffle New Edges finished")

M = 2 * (len(edges) + len(new_edges))
N = 3 * n

with open(freebase_new+'entity_type_index_min.txt', 'r') as file:
    numbers = list(map(int, file.read().split()))
    
print("Read Types finished")

for i in range(n):
    numbers.append(ID_NAME)
    
for i in range(n):
    numbers.append(ID_TYPE)
    
print("Insert Types finished")

assert(N == len(numbers))


with open(freebase_new+'graph.txt', 'w') as file:
    file.write(f'{N} {M}\n')
    for e in edges:
        file.write(f'{e[0]} {e[1]}\n')
        file.write(f'{e[1]} {e[0]}\n')
        
    for e in new_edges:
        file.write(f'{e[0]} {e[1]}\n')
        file.write(f'{e[1]} {e[0]}\n')
        
    for i in numbers:
        file.write(f'{i} ')
    file.write('\n')