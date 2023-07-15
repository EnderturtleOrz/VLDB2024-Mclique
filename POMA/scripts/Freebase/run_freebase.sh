
motifs=(3 4 5 6 7)

for i in "${motifs[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/Freebase/graph_new.txt -motif /mnt/data/database/mclique/Freebase/motif_"$i".txt -output /mnt/data/database/mclique/Freebase/results_"$i".txt&
done