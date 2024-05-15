#POMA
motifs=(4)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/wordnet/graph.txt -motif /mnt/data/yingli/mclique/wordnet/motif_"$i".txt -output /mnt/data/yingli/mclique/wordnet/results_"$i".txt
done