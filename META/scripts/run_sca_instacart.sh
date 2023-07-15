#!/bin/bash

for i in {20,40,60,80}
do
	./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/instacart/graph.txt -motif /mnt/data/database/mclique/instacart/motif_4_300.txt -output /home/code/work/baseline/results_sca_$i.txt -SCA $i &
done
