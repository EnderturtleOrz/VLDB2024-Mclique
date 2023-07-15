#!/bin/bash

for i in {6,7,8,20}
do
	./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/DBPedia/graph_new.txt -motif /mnt/data/database/mclique/DBPedia/motif_4_50.txt -output /home/code/work/DBPedia/results_sca_801.txt -PARALLELRUN  -WORKS 20 -START $i -SCA 80 &
done
