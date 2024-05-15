import os
from collections import Counter
from multiprocessing import Process, Queue

def count_types_in_chunk(args, queue):
    filepath, start, end, cid = args
    counter = Counter()
    # total_bytes = end - start
    # progress_interval = total_bytes // 10
    # next_progress = start + progress_interval
    # num_interval = 1
    
    with open(filepath, 'r', errors='ignore') as f, open('log.txt', 'a') as log_file, open('/mnt/data/yingli/mclique/freebase_new/new_detected/index_type.txt', 'a') as index:
        log_file.write(f'Cid = {cid}: Start reading\n')
        log_file.flush()
        f.seek(start)
        while f.tell() < end:
            current_position = f.tell()
            line = f.readline()
            parts = line.split()

            if len(parts) == 4 and parts[1] == '<http://rdf.freebase.com/ns/type.type.instance>':
                # type_name = parts[0].split('/')[-1]
                # if '.' in type_name:
                #     type_name = type_name.split('.')[0]
                type_name = parts[0].split('/')[-1]
                counter[type_name] += 1
                
                entity_id = parts[2].split('/')[-1].rstrip('>')
                
                index.write(f'{entity_id} {type_name}\n')
                index.flush()

            # if current_position >= next_progress:
            #     log_file.write(f'Cid = {cid}: Progress: {num_interval * 10}%\n')
            #     log_file.flush()
            #     next_progress += progress_interval
            #     num_interval += 1
        log_file.write(f'Cid = {cid}: Finish reading\n')
        log_file.flush()
    queue.put(counter)

def count_types(filepath, K, k, num_processes):
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
    queue = Queue()
    for chunk in chunks:
        p = Process(target=count_types_in_chunk, args=(chunk, queue))
        p.start()
        processes.append(p)

    for p in processes:
        p.join()

    counters = []
    while not queue.empty():
        counters.append(queue.get()) 

    counter = sum(counters, Counter())

    with open('log.txt', 'a') as log_file:
        log_file.write(f'id = {k}/{K} finish\n')
        log_file.flush()            

    return counter

def write_counts_to_file(counter, output_filepath, output_filepath_s):
    total_count = sum(counter.values())
    with open('log.txt', 'a') as log_file:
        log_file.write(f'write a part\n')
        log_file.flush() 
    with open(output_filepath, 'a') as f:
        f.write(f'{total_count}\n')
    with open(output_filepath_s, 'a') as f:
        for type_instance, count in counter.items():
            f.write(f'{type_instance}, {count}\n')

import argparse

def main():
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('k', type=int, help='the segment to process')
    parser.add_argument('K', type=int, help='the total number of segments')
    parser.add_argument('num_processes', type=int, help='the number of processes to use')
    args = parser.parse_args()

    for k in range(0, args.k):
        counter = count_types('/mnt/data/yingli/mclique/freebase-rdf-latest', args.K, k, args.num_processes)
        write_counts_to_file(counter, '/mnt/data/yingli/mclique/freebase_new/new_detected/output.txt', '/mnt/data/yingli/mclique/freebase_new/new_detected/stats.txt')

if __name__ == '__main__':
    main()
