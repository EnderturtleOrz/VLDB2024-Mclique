
motifs=(4)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/freebase_new/graph.txt -motif /mnt/data/yingli/mclique/freebase_new/motif_"$i".txt -output /mnt/data/yingli/mclique/freebase_new/results_"$i".txt
done