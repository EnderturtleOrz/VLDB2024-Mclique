
motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/Freebase/graph_new.txt -motif /mnt/data/yingli/mclique/Freebase/motif_"$i".txt -output /mnt/data/yingli/mclique/Freebase/results_"$i".txt&
done