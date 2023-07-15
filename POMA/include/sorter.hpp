#ifndef SORTER_HPP
#define SORTER_HPP

#include "argraph.hpp"

typedef unsigned char pred_dir_t;
#define IN 0
#define OUT	1


template<typename Node, typename Edge,
typename Probability >
class Sorter
{
  private:
    int node_count;          //Number of nodes
    float* probabilities;   //Vector of node probabilities
    bool* inserted;
    vector<node_id> nodes_order;	//Node ids for next pairs
    node_id* m_deg_count;
    node_id* predecessor;
    pred_dir_t* pred_edge_dir;       //From which edge set of the predecessor the node is get
    ARGraph<Node,Edge>* g1;
    ARGraph<Node,Edge>* g2;

    //Private Methods
    node_id GetMaxDegNode();
    void AddCoverageTreeNode(node_id node, node_id level);
    
  public:
    Sorter(ARGraph<Node,Edge>* g1, ARGraph<Node,Edge>* g2);
    ~Sorter();
    vector<node_id> operator();
};

template<typename Node, typename Edge,
typename Probability >
Sorter<Node,Edge,Probability >::Sorter(ARGraph<Node,Edge>* g1, ARGraph<Node,Edge>* g2)
{
  this->g1 = g1;
  this->g2 = g2;
  node_count = g1->NodeCount();
  probabilities = new float[node_count];
  inserted = new bool[node_count];
  predecessor = new node_id[node_count];
  
  for(i = 0; i < n1; i++)
  {
    predecessor[i] = NULL_NODE;
    m_deg_count[i] = 0;
    inserted[i] = false;
  }
  
}

template<typename Node, typename Edge,
typename Probability >
Sorter<Node,Edge,Probability >::~Sorter(ARGraph<Node,Edge>* g1, ARGraph<Node,Edge>* g2)
{
  delete[] probabilities;
  delete[] predecessor;
  delete[] inserted;
  delete[] m_deg_count;
}

template<typename Node, typename Edge,
typename Probability >
node_id Sorter<Node,Edge,Probability >::GetMaxDegNode()
{
  node_id i;
  int curr_deg, max_deg;
  node_id node = NULL_NODE;
  
  curr_deg = 0;
  max_deg = 0;
  
  for (i = 0; i < node_count; i++) {
    if (!inserted[i]) {
      curr_deg = g1->EdgeCount(i);
      if (curr_deg > max_deg) {
        max_deg = curr_deg;
        node = i;
      }
    }
  }
  
  return node;
}

template<typename Node, typename Edge,
typename Probability >
void Sorter<Node,Edge,Probability >::AddCoverageTreeNode(node_id node, node_id level)
{
  node_id i, neigh;
  node_id in1_count, out1_count;
  
  nodes_order.push_back(node);
  m_deg_count[node] = 0;		//Cleaning Deg Count for inserted nodes
  inserted[node] = true;
  
  //Updating Terminal set size count And degree
  in1_count = g1->InEdgeCount(node);
  out1_count = g1->OutEdgeCount(node);
  
  //Updating Inner Nodes not yet inserted
  for (i = 0; i < in1_count; i++)
  {
    //Getting Neighborhood
    neigh = g1->GetInEdge(node,i);
    if(!inserted[neigh])
    {
      m_deg_count[neigh]++;
      if(parent[neigh] == NULL_NODE)
      {
        pred_edge_dir[neigh] = IN;
        parent[neigh] = node;
      }
    }
  }
  
  //Updating Outer Nodes not yet insered
  for (i = 0; i < out1_count; i++)
  {
    //Getting Neighborhood
    neigh = g1->GetOutEdge(node,i);
    if(!inserted[neigh])
    {
      m_deg_count[neigh]++;
      if(parent[neigh] == NULL_NODE)
      {
        pred_edge_dir[neigh] = OUT;
        parent[neigh] = node;
      }
    }
  }
}

template<typename Node, typename Edge,
typename Probability >
void Sorter<Node,Edge,Probability >::operator()
{
  //The algorithm start with the node with the maximum degree
  node_id i,k;
  node_id n;	//Tree State Level
  bool *in, *out; //Internal Terminal Set used for updating the size of
  node_id node;	//Current Node
  node_id max_deg, max_node, min_prob_node;	//Used for searching the node with max M degree
  float min_prob;
  
  //Init vectors and variables
  node = 0;
  n = 0;
  
  in = new bool[node_count];
  out = new bool[node_count];
  
  //Probability Computation
  Probability probability(g2);
  for(i = 0; i < node_count; i++)
    {
    probabilities[i] = probability(g1,i);
    }
  
  /*
   * The algorithm get the less probabile node, if two or more
   * nodes shares the same probability, we selecte those have the max degree.
   * Then add the node in the solution.
   */
  
  //Searching for the node with the minumum probability
  min_prob = prob[0];
  min_prob_node = 0;
  int deg = g1->EdgeCount(min_prob_node);
  int deg2 = 0;
  for(i = 1; i < node_count; i++)
  {
    if(min_prob >= prob[i])
      {
        deg2 = g1->EdgeCount(i);
      //If two nodes shares the same probability
      //those having the max degree is selected
        if((min_prob == prob[i]
           && deg < deg2)
           || min_prob > prob[i])
          {
            min_prob == prob[i];
            min_prob_node = i;
          }
      }
  }
  
  //Adding the current node
  node = min_prob_node;
  AddCoverageTreeNode(node, n);
  n++; 	//Increasing tree level
  
  /*
   * The algorithm always select the node with the maximum number of connection into the core
   * If two or more nodes have the same number of connections
   * the algoritm is going to select the less probabile
   */
  for (; n < n1; n++) {
    //Searching for max M degree
    //Note that just nodes inserted in terminal sets and not yet inserted in M have M degree > 0
    max_deg = 0;
    max_node = NULL_NODE;
    
    for (i = 0; i < n1; i++) {
      if(!inserted[i]){
        if(m_deg_count[i] > max_deg)
          {
          max_deg = m_deg_count[i];
          max_node = i;
          }
        //If the have the same degM select the less probabile
        else if (max_node != NULL_NODE
                 && m_deg_count[i] == max_deg)
          {
          //Comparing node order
          if(probabilities[i] < probabilities[max_node])
            {
            max_deg = m_deg_count[i];
            max_node = i;
            }
          }
      }
    }
    
    //If Getting nodes outside
    if(max_deg == 0 || max_node == NULL_NODE)
      max_node = GetMaxDegNode(g1,inserted);
      
    AddCoverageTreeNode(max_node, n);
  }
  
  delete [] in;
  delete [] out;
  return nodes_order;
}

#endif
