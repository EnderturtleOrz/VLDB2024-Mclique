
#ifndef MOTIF_SRC_MAXIMAL_MOTIF_CLIQUE_H
#define MOTIF_SRC_MAXIMAL_MOTIF_CLIQUE_H

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include "Graph.h"
#include "SetTrieNode.h"
#include "global-variable.h"
#include "subgraph_match.h"
#include <fstream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

void GetLabel2NodesMap(const Graph &graph, map<int, set<int>> &label2nodes);

void GetLabelAdj(const Graph &g, map<int, set<int>> &labelAdj);

void GetLabel2NodesMap(const int current_node, const set<int> &U, const vector<int> &labels,
                       map<int, set<int>> &label2nodes, set<int> &label_set);

bool EarlyStopCheck(const set<int> &current_node_set, const Graph &motif, const Graph &graph, const unordered_set<int> &candidates,
                    const set<int> &NOT);

bool EarlyStopCheck_New(const set<int> &current_node_set, const Graph &motif, const Graph &graph, const unordered_set<int> &candidates,
                        const set<int> &NOT, vector<vector<int>> &matched_subgraphs);

void FindMaximalMotifClique(map<int, set<int>> &label2adj,
                            const vector<int> &subgraph, const Graph &motif, const Graph &graph,
                            set<int> &current_node_set,
                            set<int> &candidates, vector<set<int>> &maximal_motif_cliques);

void FindMaximalMotifClique_new(map<int, map<int, set<int>>> &label2adj, map<int, set<int>> &label2cover, const vector<int> &matched_subgraph,
                                const Graph &motif, const Graph &graph, set<int> &current_node_set,
                                unordered_set<int> &candidates, vector<set<int>> &maximal_motif_cliques,
                                int initial_id, vector<vector<int>> &motif_neighbor, vector<vector<int>> &matched_subgraphs);

void GetCandidates(const map<int, map<int, set<int>>> &label2adj, const set<int> &current_node_set, const Graph &motif,
                   const Graph &graph,  unordered_set<int> &candidates);

void GetCandidates(const set<int> &current_node_set, const int new_node, const map<int, map<int, set<int>>> &label2adj,
                   const unordered_set<int> &candidates, const Graph &mitif, const Graph &graph, const set<int> &NOT,
                   set<int> &NOT_new,
                   unordered_set<int> &candidates_new);

void DFSMaximal(map<int, set<int>> &label2adj, set<int> &current_node_set, const Graph &motif,
                const Graph &graph, set<int> &candidates, set<int> &NOT,
                vector<set<int>> &maximal_motif_cliques);

void DFSMaximal_new( map<int, map<int, set<int>>> &label2adj, map<int, set<int>> &label2cover, set<int> &current_node_set,
                    const Graph &motif, const Graph &graph, unordered_set<int> &candidates, set<int> &NOT,
                    vector<set<int>> &maximal_motif_clique,
                   int initial_id, vector<vector<int>> &motif_neighbor, vector<vector<int>> &matched_subgraphs);

// void DFSMaximal_Approx(map<int, set<int> > &label2adj, set<int> &current_node_set, const Graph &motif,
//                 const Graph &graph, set<int> &candidates, set<int> &NOT, int step,
//                 vector<set<int> > &maximal_motif_cliques);

void UsePivotPruning (const set<int>& NOT, const unordered_set<int> &candidates, unordered_set<int> &candidates_copy, int choice);
bool SubgraphInSetTrieNode(vector<int> subgraph);

void InsertNode(map<int, set<int>> &cands, const int v, const int label);

template <typename T>
void Print(const T &A);

void Print(const vector<vector<int>> &subgraphs);

void PrintMap(const map<int, set<int>> &mp);

bool Check(set<int> &A, set<int> &B);

bool CheckSubset(const vector<int> &A, const set<int> &B);

bool IsomorphismCheck(const set<int> &U, const int u, const Graph &motif, const Graph &graph);

#endif // MOTIF_SRC_MAXIMAL_MOTIF_CLIQUE_H
