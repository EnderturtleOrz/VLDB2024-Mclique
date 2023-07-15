#ifndef VF3IMPL_H
#define VF3IMPL_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "match.hpp"
#include "argloader.hpp"
#include "argraph.hpp"
#include "argedit.hpp"
#include "nodesorter.hpp"
#include "probability_strategy.hpp"
#include "vf3_sub_state.hpp"
//#include "vf2_sub_state.hpp"
#include "nodesorter.hpp"
#include "nodeclassifier.hpp"
#include"../maximal-motif-clique.h"
#include"../global-variable.h"

//#define TIME_LIMIT 1


template<> long long VF3SubState<int, int, Empty, Empty>::instance_count = 0;
static long long state_counter = 0;


typedef struct visitor_data_s {
    unsigned long first_solution_time;
    long solutions;
} visitor_data_t;

//bool visitor(int n, node_id ni1[], node_id ni2[], void* state, void *usr_data)
//{
//	/*AbstractVFState<int, int, Empty, Empty>* s = static_cast<AbstractVFState<int, int, Empty, Empty>*>(state);
//	while (s)
//	{
//		if (!s->IsUsed())
//		{
//			s->SetUsed();
//			state_counter++;
//		}
//		s = s->GetParent();
//	}*/
//
//  visitor_data_t* data = (visitor_data_t*)usr_data;
//  data->solutions++;
//  if(data->solutions == 1)
//  {
//    data->first_solution_time = clock();
//  }
//
//  return false;
//}


void GetMMC_VF3(const Graph &motif, const Graph &graph, const vector<int> &subgraph) {
    current_node_set.clear();
    candidates.clear();
    label2adj.clear();
    extern int early_stop_cnt;
    extern int subgraph_cnt;
    
    // cerr << "current sugraph is: " << subgraph_cnt << "  early stop used:  " << early_stop_cnt << endl;
    subgraph_cnt += 1;
    if (SubgraphInSetTrieNode(subgraph)) return;
    FindMaximalMotifClique(label2adj, subgraph, motif, graph, current_node_set, candidates,
                           maximal_motif_cliques);
    if(!DUPLICATION)
        root->AddSet(set<int>(subgraph.begin(), subgraph.end()), global_hash_id);
}

// This function is re-implemented by Jiafeng Hu.
bool
visitor(int n, node_id ni1[], node_id ni2[], void *state, void *usr_data, vector<vector<int> > &matched_subgraphs) {
    /* For debug: */
    //cout<<"In visitor"<<endl;
    //if(matched_subgraphs.size()%100==0)
    //    cout<<matched_subgraphs.size()<<endl;

    vector<int> result;
    for (int i = 0; i < n; ++i) {
        result.push_back(int(ni2[i]));
    }
    matched_subgraphs.push_back(result);

//    if(matched_subgraphs.size()%100==0)
//        cout<<"subgraph"<<endl;

    extern Graph motif;
    extern Graph graph;
    GetMMC_VF3(motif, graph, result);

    visitor_data_t *data = (visitor_data_t *) usr_data;
    data->solutions++;
    if (data->solutions == 1) {
        data->first_solution_time = clock();
    }

    return false;
}

bool vf3Impl(ARGraph<int, Empty> &patt_graph, ARGraph<int, Empty> &targ_graph,
             vector<vector<int> > &matched_subgraphs) {

    visitor_data_t vis_data;
    state_counter = 0;
    int n = 0;
    double timeAll = 0;
    double timeFirst = 0;
    unsigned long firstSolTicks = 0;
    unsigned long endTicks = 0;
    unsigned long ticks = 0;
    //float limit = TIME_LIMIT;

    //std::ifstream graphInPat(motif_path.c_str());
    //std::ifstream graphInTarg(graph_path.c_str());
    //StreamARGLoader<int, Empty> pattloader(graphInPat);
    //StreamARGLoader<int, Empty> targloader(graphInTarg);
    //ARGraph<int, Empty> patt_graph(&pattloader);
    //ARGraph<int, Empty> targ_graph(&targloader);

    int nodes1, nodes2;
    nodes1 = patt_graph.NodeCount();
    nodes2 = targ_graph.NodeCount();
    node_id *n1, *n2;
    n1 = new node_id[nodes1];
    n2 = new node_id[nodes2];

    NodeClassifier<int, Empty> classifier(&targ_graph);
    NodeClassifier<int, Empty> classifier2(&patt_graph, classifier);
    std::vector<int> class_patt = classifier2.GetClasses();
    std::vector<int> class_targ = classifier.GetClasses();

    ticks = clock();
    vis_data.solutions = 0;
    vis_data.first_solution_time = 0;
    VF3NodeSorter<int, Empty, SubIsoNodeProbability<int, Empty> > sorter(&targ_graph);
    std::vector<node_id> sorted = sorter.SortNodes(&patt_graph);

    VF3SubState<int, int, Empty, Empty> s0(&patt_graph, &targ_graph, class_patt.data(),
                                           class_targ.data(), classifier.CountClasses(), sorted.data());

    //cout << "Start to do matching using VF3" << endl;
    match<VF3SubState<int, int, Empty, Empty> >(s0, &n, n1, n2, visitor, &vis_data, matched_subgraphs);
    timeAll = ((double) (clock() - ticks) / CLOCKS_PER_SEC);

    //std::cout << vis_data.solutions << " subgraphs have been found using VF3 in " << timeAll << " seconds" << endl;
    delete[] n1;
    delete[] n2;
    return true;
}

bool vf3Impl(const string &motif_path, const string &graph_path,
             vector<vector<int> > &matched_subgraphs) {

    visitor_data_t vis_data;
    state_counter = 0;
    int n = 0;
    double timeAll = 0;
    double timeFirst = 0;
    unsigned long firstSolTicks = 0;
    unsigned long endTicks = 0;
    unsigned long ticks = 0;
    //float limit = TIME_LIMIT;

    //std::ifstream graphInPat(motif_path.c_str());
    //std::ifstream graphInTarg(graph_path.c_str());
    //StreamARGLoader<int, Empty> pattloader(graphInPat);
    //StreamARGLoader<int, Empty> targloader(graphInTarg);
    //ARGraph<int, Empty> patt_graph(&pattloader);
    //ARGraph<int, Empty> targ_graph(&targloader);

    // Re-implemented the construction function of ARGraph by Jiafeng Hu.
    cout<<"Read the motif."<<endl;
    ARGraph<int, Empty> patt_graph(motif_path);
    cout<<"Read the target graph."<<endl;
    ARGraph<int, Empty> targ_graph(graph_path);

    int nodes1, nodes2;
    nodes1 = patt_graph.NodeCount();
    nodes2 = targ_graph.NodeCount();
    node_id *n1, *n2;
    n1 = new node_id[nodes1];
    n2 = new node_id[nodes2];

    NodeClassifier<int, Empty> classifier(&targ_graph);
    NodeClassifier<int, Empty> classifier2(&patt_graph, classifier);
    std::vector<int> class_patt = classifier2.GetClasses();
    std::vector<int> class_targ = classifier.GetClasses();

    ticks = clock();
    vis_data.solutions = 0;
    vis_data.first_solution_time = 0;
    VF3NodeSorter<int, Empty, SubIsoNodeProbability<int, Empty> > sorter(&targ_graph);
    std::vector<node_id> sorted = sorter.SortNodes(&patt_graph);

    VF3SubState<int, int, Empty, Empty> s0(&patt_graph, &targ_graph, class_patt.data(),
                                           class_targ.data(), classifier.CountClasses(), sorted.data());

    //cout << "Start to do matching using VF3" << endl;
    match<VF3SubState<int, int, Empty, Empty> >(s0, &n, n1, n2, visitor, &vis_data, matched_subgraphs);
    timeAll = ((double) (clock() - ticks) / CLOCKS_PER_SEC);

    //std::cout << vis_data.solutions << " subgraphs have been found using VF3 in " << timeAll << " seconds" << endl;
    delete[] n1;
    delete[] n2;
    return true;
}


#endif