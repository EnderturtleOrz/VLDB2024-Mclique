import random

freebase_new = '/mnt/data/yingli/mclique/freebase_new/new_detected/'

def process_lines(lines, type_dict):
    result_dict = {}
    for line in lines:
        try:
            entity, type_name = line.strip().split(' ')
            type_index = type_dict.get(type_name, -1)
            if type_index == -1:
                continue
            if entity in result_dict:
                result_dict[entity].append(type_index)
            else:
                result_dict[entity] = [type_index]
        except:
            pass
    return result_dict

if __name__ == "__main__":
    
    type_dict = {}
    
    with open('stats_full.txt', 'r') as file:
        next(file) 
        index = 0
        for line in file:
            try:
                type_name, _ = line.strip().split(' ')
                type_dict[type_name] = index
                index += 1
            except:
                pass
            
    print("Get type dict")

    # Read the file and process it line by line
    with open(freebase_new+'index_type.txt', 'r') as file:
        lines = file.readlines()
    
    print("Read finished")
    # Process the lines
    final_dict = process_lines(lines, type_dict)
    print("Process finished")
    print("Get final_dict")
    
    with open(freebase_new+'type_index.txt', 'w') as file:
        for type_name, index in type_dict.items():
            file.write(f'{type_name} {index}\n')
            
    print("type_index finished")
    entity_index = 0
    
    n = len(final_dict)
    print("1st round files")
    with open(freebase_new+'entity_index.txt', 'w') as entity_index_file, \
            open(freebase_new+'entity_type_index_min.txt', 'w') as entity_type_index_min_file, \
            open(freebase_new+'entity_type_index.txt', 'w') as file:
        
        for entity, type_indices in final_dict.items():
            # Write to entity_index.txt
            entity_index_file.write(f'{entity} {entity_index}\n')
            entity_index += 1

            # Write to entity_type_index_min.txt
            type_index = random.choice(type_indices)
            entity_type_index_min_file.write(f'{type_index} ')
            
            # Write to entity_type_index.txt
            file.write(f'{entity} {type_index}\n')
            
    assert(entity_index == n)

    # # Add name
    # print("2ed round files")
    # with open(freebase_new+'entity_index.txt', 'a') as entity_index_file, \
    #         open(freebase_new+'entity_type_index_min.txt', 'a') as entity_type_index_min_file, \
    #         open(freebase_new+'entity_type_index.txt', 'a') as file:
    #     for _ in range(n):
    #         # Write to entity_index.txt
    #         entity_index_file.write(f'{entity_index} {entity_index}\n')
    #         entity_index += 1

    #         # Write to entity_type_index_min.txt
    #         type_index = type_dict["NAME"]
    #         entity_type_index_min_file.write(f'{type_index} ')
            
    #         # Write to entity_type_index.txt
    #         file.write(f'{entity_index} {type_index}\n')
    
    print(n)
    with open('loglog.txt','a') as loglog:
        loglog.write(f'{n}\n')
        
    with open('final_n','a') as ff:
        ff.write(f'{n}\n')
    