#!/bin/bash

for i in {20,40,60,80}
do
	./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/DBPedia/graph_new.txt -motif /mnt/data/yingli/mclique/DBPedia/motif_4_50.txt -output /home/tianci/work/DBPedia/results_sca_$i.txt -SCA $i &
done
