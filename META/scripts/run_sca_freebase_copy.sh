#!/bin/bash

for j in {1,5,9,13,17}
do
	./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/Freebase/graph_new.txt -motif /mnt/data/database/mclique/Freebase/motif_4.txt -output /home/code/work/Freebase/results_sca_80.txt -PARALLELRUN -WORKS 5 -START $j -SCA 80 &
done
