
#!/bin/bash
starts=(1 6 11 16 21)

for i in "${starts[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/yingli/mclique/DBPedia/graph_new.txt -motif /mnt/yingli/mclique/DBPedia/motif_7_50.txt -output /mnt/yingli/mclique/DBPedia/results_base_7.txt -PARALLELRUN -WORKS 5 -START "$i" &
done
#bash scripts/run_depedia.sh
