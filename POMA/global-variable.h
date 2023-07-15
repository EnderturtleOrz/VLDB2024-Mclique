
#ifndef MOTIF_SRC_GLOBAL_VARIABLE_H
#define MOTIF_SRC_GLOBAL_VARIABLE_H

#include"SetTrieNode.h"
#include<vector>
#include<set>
#include<map>
#include"Graph.h"
#include<unordered_set>
//#define DEBUG 1
extern int MMC_LIMIT;
extern int SUBGRAPH_LIMIT;
//extern int CandidateLimit;
extern bool UseAP;
extern int Pruning_vertices_sp;
extern int All_vertices_sp;
extern bool RandomSelection;
extern bool DUPLICATION;
extern bool WithoutIsoCheckPruning;

extern bool NOEARLYSTOPCHECK;
extern bool PARALLELRUN;
extern int START;
extern int WORKS;
extern bool directed;
extern bool APPROXIMATION;
extern bool GETEXACTMMC;

extern double R;
extern double DELTA;
extern int OMEGA;
extern int ORDER;
extern int Candidates;
extern vector<int> vertexColor;
extern int PIVOT;
extern bool InitCandidate; 
extern bool NotReduce;
extern int average_number_mmc;
extern int average_size_mmc;
extern double average_density_mmc;

extern int duplicated_answer_num;
extern int iso_check_num;

//for debug
extern int global_hash_id;
extern int motif_node_size;
extern int solNum;
extern int maximum_clique_size;

extern SetTrieNode *root;
extern Graph *temp_graph;

extern int test_num;
extern int max_test_num;

// Global structures used in IsoCheck
extern vector<vector<int> > temp_matched_subgraphs;
extern map<int, set<int>> cand_label2nodes;
extern set<int> label_set;
extern map<int, set<int> > cands;
extern set<int> pre_IsoCheck_set;
extern int current_u;

extern bool *IsVisited;
extern map<int, string> id2name; // map node id to real info.
extern vector<vector<int> > subgraphs; // matched subgraphs
extern map<int, set<int> > motif_label2nodes;

extern map<int, set<int> > motif_labelAdj;
extern map<int, int> motif_label2Orbit;
extern bool hasDifference;

extern vector<set<int> > maximal_motif_cliques; // the result set
extern set<int> current_node_set;
extern map<int, map<int, set<int>>> label2adj;
extern map<int, set<int>>label2cover;
extern unordered_set<int> candidates;
extern Graph motif;
extern Graph graph;
extern Graph copy_graph;
extern vector<double> approx_improvement;
extern long mclique_number;
extern long mclique_size;
extern long reduced_vertex;
extern long reduced_edge;
extern double vf3_time;
extern double reduceHIN_time;
extern double init_time;
extern vector<double> real_approx_ratio;
extern int visited_vertices;
extern vector<double> real_failure_prob;


extern bool CHECKSPECIES;
extern set<int> validNodes;


#endif //MOTIF_SRC_GLOBAL_VARIABLE_H
