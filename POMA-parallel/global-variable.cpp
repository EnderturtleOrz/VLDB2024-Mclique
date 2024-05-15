
#include "global-variable.h"

using namespace std;
int Pruning_vertices_sp = 0;
int All_vertices_sp = 0;
int SUBGRAPH_LIMIT = -1; // default value -1
int MMC_LIMIT = -1;      // default value -1
// int CandidateLimit=50;
bool PARALLELRUN = false;
int START = 1;
int WORKS = 1;
bool RandomSelection = false;
bool DUPLICATION = false;
bool WithoutIsoCheckPruning = false;
int ORDER = 3; // default value 3 -> (k,m)-core
int PIVOT = 3; // default value 3 -> advanced selection
bool InitCandidate = true; 
bool NotReduce = false;
int Candidates = 0;
bool directed = false;
bool APPROXIMATION = false;
bool NOEARLYSTOPCHECK = false;
bool GETEXACTMMC = false;
vector<vector<int>> node2Subgraphs;
vector<vector<int>> tmp_matched_subgraphs;
vector<vector<int>> motifNer;
double R = 5;
double DELTA = 0.01;
int OMEGA;
bool UseAP = true;
double pick_time = 0;
int compute_motif_neighbor = 0;
double use_pivot = 0;
double use_duplicate = 0;
double update_candidates = 0;
int duplicate_cnt = 0;
int early_stop_cnt = 0;
int average_number_mmc = 0;
int average_size_mmc = 0;
double average_density_mmc = 0;
int duplicated_answer_num = 0;
int iso_check_num = 0;
// for analysis
long mclique_number = 0;
long mclique_size = 0;
long reduced_vertex = 0;
long reduced_edge = 0;
double vf3_time = 0;
double reduceHIN_time = 0;
double init_time = 0;

int visited_vertices = 0;
int global_hash_id = 0;
int motif_node_size = 0;
int solNum = 0;
int maximum_clique_size = 0;
map<int, int> vertexOrder;
vector<int> vertexColor;
SetTrieNode *root = NULL;
Graph *temp_graph = NULL;

int test_num = 0;
int max_test_num = 0;

// Global structures used in IsoCheck
vector<vector<int>> temp_matched_subgraphs;
map<int, set<int>> cand_label2nodes;
set<int> label_set;
map<int, set<int>> cands;
map<int, int> currentOrder;
map<int, map<int,int>> id2SubgraphOrder;
set<int> pre_IsoCheck_set;
map<int, set<int>> id2NOT;
int current_u;
// map<int, int> vertexOrder;
bool *IsVisited;
map<int, string> id2name;      // map node id to real info.
vector<vector<int>> subgraphs; // matched subgraphs
map<int, set<int>> motif_label2nodes;
map<int, set<int>> motif_labelAdj;
map<int, int> motif_label2Orbit;
bool hasDifference;
vector<set<int>> maximal_motif_cliques; // the result set
map<int, int> candidates_labels;
set<int> maximum_motif_clique;
set<int> current_node_set;
unordered_set<int> candidates;
map<int, map<int, set<int>>> label2adj;
map<int,  set<int>> label2cover;
map<int, int> vertexLabel;
Graph motif;
Graph graph;
Graph copy_graph;
set<int> candidates_subgraph;
vector<double> approx_improvement;

vector<double> real_approx_ratio;

vector<double> real_failure_prob;

bool CHECKSPECIES = false;
set<int> validNodes;