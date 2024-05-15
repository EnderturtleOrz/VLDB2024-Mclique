
motifs=(5)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/freebase_new/new_detected/graph.txt -motif /mnt/data/yingli/mclique/freebase_new/new_detected/motif_"$i".txt -output /mnt/data/yingli/mclique/freebase_new/new_detected/results_"$i".txt
done