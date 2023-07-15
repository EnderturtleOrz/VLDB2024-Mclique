/* 
 * File:   subgraph_match.h
 * Author: hjf
 *
 * Created on July 15, 2017, 11:00 PM
 */

#ifndef SUBGRAPH_MATCH_H
#define SUBGRAPH_MATCH_H

#include "Graph.h"
//#include"./include/argraph.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <memory.h>
//#include "./include/vf3_impl.hpp"

using namespace std;

extern int invalid;

/* Determines initial candidates of query vertices:
 * for each node u \in Motif, find its mapping set {u'} \in G
 * criteria: label(u) = label(u'), degree(u) <= degree(u')
 */
void Initialize(vector<set<int> > &candidates, const Graph &motif, const Graph &graph);

//finds all candidate edges for query edges with given lead vertex
//returns candidate set as a vector with even indices as lead vertices and odd as follow vertices
void FindMatchedEdges(int u, int v, vector<set<int> > &candidates,
                      const Graph &graph, set<EDGE> &edge_map_list);

//determines the visit order for finding partial solutions
//returns the order as a integer vector with each value = index that should be 
//visited, i.e. at index 0 the value is 15 so the first index visited is 15
void Setup(vector<set<EDGE> > &c_edges, const Graph &motif, vector<int> &order);


//goes through all possible solutions and returns the correct matches
void Join(const vector<int> &order, const Graph &motif, const Graph &graph, const vector<set<EDGE> > &c_edges,
          int *motif_assign, int *graph_assign, int current, int *levelSet,
          vector<vector<int> > &result, const bool Isomorphism_Check);

void GetMatchedSubgraph(const Graph &motif, const Graph &graph, vector<vector<int> > &matched_subgraphs,
                        const bool Isomorphism_Check = false);

void GetMatchedSubgraph(const Graph &motif, const Graph &graph, vector<vector<int> > &matched_subgraphs,
                        const int must_contain_node);



void GetMMC(const Graph &motif, const Graph &graph, const vector<int> &subgraph);


#endif    /* SUBGRAPH_MATCH_H */

