#!/bin/bash
# not use graph-reduce POMA
motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/DBLP/graph_new.txt -motif /mnt/data/database/mclique/DBLP/motif_"$i".txt -output /mnt/data/database/mclique/DBLP/results_DBLP_"$i".txt&
done
