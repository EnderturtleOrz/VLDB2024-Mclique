#!/bin/bash
motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/DBLP/graph_new.txt -motif /mnt/data/yingli/mclique/DBLP/motif_"$i".txt -output /mnt/data/yingli/mclique/DBLP/results_DBLP_"$i".txt&
done
