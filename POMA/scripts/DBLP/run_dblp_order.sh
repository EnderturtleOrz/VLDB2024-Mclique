#!/bin/bash
#not use order, POMA-O
 ./motif_src -mmc -vf3 -graph /mnt/data/database/mclique/DBLP/graph_new.txt -motif /mnt/data/database/mclique/DBLP/motif_4.txt -output /mnt/data/database/mclique/DBLP/results_POMAO.txt -ORDER 0

# Degree-order : ORDER 1
#bash scripts/run_dblp_order.sh