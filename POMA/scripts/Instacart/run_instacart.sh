motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/instacart/graph_new.txt -motif /mnt/data/database/mclique/instacart/motif_"$i"_300.txt -output /mnt/data/database/mclique/instacart/results_"$i".txt&
done