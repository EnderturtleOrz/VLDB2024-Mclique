#!/bin/bash

for i in {20,40,60,80}
do
	./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/DBPedia/graph_new.txt -motif /mnt/data/database/mclique/DBPedia/motif_4_50.txt -output /home/code/work/DBPedia/advance_results_sca_$i.txt -SCA $i &
done
