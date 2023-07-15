#!/bin/bash

for i in {20,40,60,80}
do
	./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/wordnet/graph.txt -motif /mnt/data/yingli/mclique/wordnet/motif_4_300.txt -output /home/tianci/work/wordnet/advance_results_sca_$i.txt -SCA $i &
done
