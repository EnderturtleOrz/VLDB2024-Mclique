
#!/bin/bash
#only use pivot, not use order

./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/Freebase/graph_new.txt -motif /mnt/data/database/mclique/Freebase/motif_4.txt -output /mnt/data/database/mclique/Freebase/results_POMAP.txt -PIVOT 0 

#remove pivot