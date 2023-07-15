motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/DBPedia/graph_new.txt -motif /mnt/data/yingli/mclique/DBPedia/motif_"$i"_50.txt -output /mnt/data/yingli/mclique/DBPedia/results_DBPedia_"$i".txt&
done