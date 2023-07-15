starts=(1 6 11 16)

for i in "${starts[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/DBLP/graph_new.txt -motif /mnt/data/yingli/mclique/DBLP/motif_4.txt -output /mnt/data/yingli/mclique/DBLP/results_base_4_visited.txt -PARALLELRUN -WORKS 4 -START "$i" &
done
#bash scripts/run_dblp.sh