import os
from multiprocessing import Process, Manager, Queue


freebase_new = '/mnt/data/yingli/mclique/freebase_new/new_detected/'

def get_edge_in_chunk(args, entity_dict_share):
    filepath, start, end, cid = args

    with open(filepath, 'r', errors='ignore') as f, open('log_edge.txt', 'a') as log_file, open(freebase_new+'graph_edge.txt', 'a') as graph:
        log_file.write(f'Cid = {cid}: Start reading\n')
        log_file.flush()
        f.seek(start)
        while f.tell() < end:
            try:
                line = f.readline()
                parts = line.split()

                if len(parts) == 4:
                    entity_name_1 = parts[0].split('/')[-1].rstrip('>')
                    entity_name_2 = parts[2].split('/')[-1].rstrip('>')
                    
                    if(entity_name_1[0] == 'm' and entity_name_1[1] == '.' and entity_name_2[0] == 'm' and entity_name_2[1] =='.'):
                        
                        entity_id_1 = entity_dict_share.get(entity_name_1, None)
                        entity_id_2 = entity_dict_share.get(entity_name_2, None)

                        if entity_id_1 is not None and entity_id_2 is not None:
                            graph.write(f'{entity_id_1} {entity_id_2}\n')
                            graph.flush()
            except:
                pass
            
        log_file.write(f'Cid = {cid}: Finish reading\n')
        log_file.flush()
            
def get_edge(filepath, K, k, num_processes, entity_dict_share):
    with open('log.txt', 'a') as log_file:
        log_file.write(f'id = {k}/{K} start\n')
        log_file.flush()       
    
    total_size = os.path.getsize(filepath)
    chunk_size = total_size // K
    start = k * chunk_size
    end = min((k + 1) * chunk_size, total_size)
    
    sub_chunk_size = (end - start) // num_processes
    chunks = [(filepath, start + i * sub_chunk_size, min(start + (i + 1) * sub_chunk_size, end), i) for i in range(num_processes)]
    
    processes = []
    for chunk in chunks:
        p = Process(target=get_edge_in_chunk, args=(chunk, entity_dict_share))
        p.start()
        processes.append(p)

    for p in processes:
        p.join()

    with open('log.txt', 'a') as log_file:
        log_file.write(f'id = {k}/{K} finish\n')
        log_file.flush()          
        
import argparse

# def process_lines(lines, queue):
#     result = {}
#     for line in lines:
#         key, value = line.strip().split()
#         result[key] = int(value)
#     queue.put(result)

# def pre(num):
#     with open(freebase_new+'entity_index.txt', 'r') as f:
#         lines = f.readlines()
#     chunks = [lines[i::num] for i in range(num)]
#     queue = Queue()
#     processes = [Process(target=process_lines, args=(chunk, queue)) for chunk in chunks]
#     for p in processes:
#         p.start()
#     for p in processes:
#         p.join()
#     result = {}
#     while not queue.empty():
#         result.update(queue.get())
        
#     with open('log.txt', 'a') as log_file:
#         log_file.write(f'Get entity_index\n')
#         log_file.flush()  
            
#     return result

def pre(_):
    lines = []
    result = {}
    
    with open(freebase_new+'entity_index.txt', 'r') as f:
        lines = f.readlines()

    for line in lines:
        key, value = line.strip().split()
        result[key] = int(value)

    with open('log.txt', 'a') as log_file:
        log_file.write(f'Get entity_index\n')
        log_file.flush()         
    
    return result     

def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('k', type=int, help='the segment to process')
    parser.add_argument('K', type=int, help='the total number of segments')
    parser.add_argument('num_processes', type=int, help='the number of processes to use')
    args = parser.parse_args()
    
    manager = Manager()
    entity_dict = pre(args.num_processes)
    entity_dict_share = manager.dict(entity_dict)
    del entity_dict
    
    for k in range(0, args.k):
        get_edge('/mnt/data/yingli/mclique/freebase-rdf-latest', args.K, k, args.num_processes, entity_dict_share)

if __name__ == '__main__':
    main()