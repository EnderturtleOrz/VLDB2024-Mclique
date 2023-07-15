#ifndef PROBABILITY_NODE_SORTER_HPP
#define PROBABILITY_NODE_SORTER_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include "argraph.hpp"


/******************************************************
 ************** SORTING STRATEGIES ******************
 ******************************************************/
typedef struct
{
    node_id id;
    double ordering_value;
}NodeInfo;

/* QUICK SORT STRATEGY */
template<typename Node, typename Edge>
class QuickSorter
{
  private:
    static bool compareNodes(const NodeInfo &a, const NodeInfo &b){
        return a.ordering_value < b.ordering_value;
    }  

  public:
    vector<node_id> operator()(vector<NodeInfo>& base)
    {
      std::sort(base.begin(), base.end(), compareNodes);
      vector<node_id> nodes_out;
    
      for(int i=0; i<base.size(); i++){
        nodes_out.push_back(base[i].id);
      }

      return nodes_out; 
    }
};

/******************************************************
 ************** PROBABILITY SORTER ******************
 ******************************************************/

template<typename Node, typename Edge,
typename Probability,
typename Sorter = QuickSorter<Node,Edge> >
class ProbabilitySorter
{
  private:
    float* probabilities;
    Probability* prob;
  public:
    //Sort g1 using the info evalutated on g2
    vector<node_id> operator()(ARGraph<Node,Edge>* pattern);
    float* getProbabilities();
    ProbabilitySorter(ARGraph<Node,Edge>* target);
    ~ProbabilitySorter();
};

template<typename Node, typename Edge,
typename Probability, typename Sorter>
ProbabilitySorter<Node,Edge,Probability,Sorter>::ProbabilitySorter(ARGraph<Node,Edge>* target)
{
  prob = new Probability(target);
}

template<typename Node, typename Edge,
typename Probability, typename Sorter>
ProbabilitySorter<Node,Edge,Probability,Sorter>::~ProbabilitySorter()
{
  delete prob;
}

template<typename Node, typename Edge,
typename Probability, typename Sorter>
vector<node_id> ProbabilitySorter<Node,Edge,Probability,Sorter>::operator()(ARGraph<Node,Edge>* g1){
    int i;
    int node_count = g1->NodeCount();
    Sorter sorter;
    probabilities = new float[node_count];
  
    vector<NodeInfo> nodes(node_count);
    
    for(i = 0; i < node_count; i++){
        nodes[i].id = i;
        nodes[i].ordering_value = prob->getProbability(g1,i);
        probabilities[i] = nodes[i].ordering_value;
    }
    
    vector<node_id> nodes_out = sorter(nodes);
    return nodes_out;
}

template<typename Node, typename Edge,
typename Probability, typename Sorter>
float* ProbabilitySorter<Node,Edge,Probability,Sorter>::getProbabilities(){
  return probabilities;
}

#endif
