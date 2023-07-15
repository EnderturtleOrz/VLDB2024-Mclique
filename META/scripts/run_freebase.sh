
#!/bin/bash
starts=(1 5 9 13 17)

for i in "${starts[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/yingli/mclique/Freebase/graph_new.txt -motif /mnt/yingli/mclique/Freebase/motif_5.txt -output /mnt/yingli/mclique/Freebase/results_base_5.txt -PARALLELRUN -WORKS 5 -START "$i" &
done
#bash scripts/run_freebase.sh
# ./motif_src -genMotif -graph /mnt/yingli/mclique/Freebase/graph_new.txt -output /mnt/yingli/mclique/Freebase/motif.txt -motif_size 5  -motif_num 100
