stats = {}

node_num = 0

freebase_new = '/mnt/data/yingli/mclique/freebase_new/new_detected/'

with open(freebase_new+'stats.txt', 'r') as file:
    for line in file:
        key, value = line.strip().split(', ')
        stats[key] = stats.get(key, 0) + int(value)
        node_num += int(value)
        
unique_strings = len(stats)

with open('stats_full.txt', 'w') as file:
    file.write(f'{unique_strings} {node_num}\n')
    for key, value in stats.items():
        file.write(f'{key} {value}\n')