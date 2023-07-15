#!/bin/bash

for i in {20,40,60,80}
do
	./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/Freebase/graph_new.txt -motif /mnt/data/database/mclique/Freebase/motif_4.txt -output /home/code/work/Freebase/advance_results_sca_$i.txt -SCA $i &
done
