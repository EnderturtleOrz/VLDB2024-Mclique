#POMA
motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/wordnet/graph_new.txt -motif /mnt/data/yingli/mclique/wordnet/motif_"$i".txt -output /mnt/data/yingli/mclique/wordnet/results_"$i".txt&
done