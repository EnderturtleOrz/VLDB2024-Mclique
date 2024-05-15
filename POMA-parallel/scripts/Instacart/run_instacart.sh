motifs=(4)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/yingli/mclique/instacart/graph.txt -motif /mnt/data/yingli/mclique/instacart/motif_"$i"_300.txt -output /mnt/data/yingli/mclique/instacart/results_"$i"_pa.txt
done