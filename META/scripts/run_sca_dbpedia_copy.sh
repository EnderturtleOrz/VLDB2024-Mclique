#!/bin/bash

for i in {6,7,8,20}
do
	./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/DBPedia/graph_new.txt -motif /mnt/data/yingli/mclique/DBPedia/motif_4_50.txt -output /home/tianci/work/DBPedia/results_sca_801.txt -PARALLELRUN  -WORKS 20 -START $i -SCA 80 &
done
