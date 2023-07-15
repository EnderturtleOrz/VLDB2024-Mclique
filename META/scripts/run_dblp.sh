
#!/bin/bash
# starts=(1 6 11 16)

# for i in "${starts[@]}"
# do
#     ./motif_src -mmc -vf3 -graph /mnt/database/mclique/DBLP/graph_new.txt -motif /mnt/database/mclique/DBLP/motif_6.txt -output /mnt/database/mclique/DBLP/results_base_6.txt -PARALLELRUN -WORKS 4 -START "$i" &
# done
starts=(1 6 11 16)

for i in "${starts[@]}"
do
    ./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/Freebase/graph_new.txt -motif /mnt/data/database/mclique/Freebase/motif_4.txt -output /m-output /home/code/work/Freebase/results_sca_80.txt -SCA 80 -PARALLELRUN -WORKS 4 -START "$i" &
done
#bash scripts/run_dblp.sh