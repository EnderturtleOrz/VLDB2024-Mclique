
#ifndef GRAPH_H
#define GRAPH_H

#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<vector>
#include "assert.h"
#include<cstdlib>


using namespace std;

//#define UNLABELED 1

class EDGE {
public:
    int from, to;

    EDGE() {
    }

    EDGE(int a, int b) : from(a), to(b) {
    }

    bool operator<(const EDGE &rhs) const {
        return from < rhs.from || (from == rhs.from && to < rhs.to);
    }

    bool operator==(const EDGE &rhs) const {
        return from == rhs.from && to == rhs.to;
    }
};

struct Less : public std::binary_function<EDGE, EDGE, bool> {

    bool operator()(const EDGE &lhs, const EDGE &rhs) const {
        return !(lhs == rhs) && (lhs < rhs);
    }
};

class Graph {
public:
    // Number of nodes (edges)
    int nodeNum, edgeNum;

    // The adjacency list of the graph; adjList[i] stores the neighbor set of node i
    vector<set<int> > adjList = vector<set<int>>(0);

    // The adjacency list of the graph; InAdjList[i] stores the in-neighbor set of node i
    vector<set<int> > InAdjList;

    // The adjacency list of the graph; OutAdjList[i] stores the out-neighbor set of node i
    vector<set<int> > OutAdjList;

    // The edge list
    vector<EDGE> edges;

    // labels of nodes. Node id {0, ..., nodeNum-1}
    vector<int> labels;

    Graph();

    void cloneGraph(Graph &graph){
        nodeNum = graph.nodeNum;
        edgeNum = graph.edgeNum;
        adjList.clear();
        adjList.resize(graph.adjList.size());
#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < graph.adjList.size(); i ++){
            adjList[i] = graph.adjList[i];
        }
        
        InAdjList.resize(graph.InAdjList.size());
#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < graph.InAdjList.size(); i ++){
            InAdjList[i] = graph.InAdjList[i];
        }

        OutAdjList.resize(graph.OutAdjList.size());
#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < graph.OutAdjList.size(); i ++){
            OutAdjList[i] = graph.OutAdjList[i];
        }

        edges.resize(graph.edges.size());
#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < graph.edges.size(); i ++){
            EDGE e(graph.edges[i].from, graph.edges[i].to);
            edges[i] = e;
        }

        labels.resize(graph.labels.size());
#pragma omp parallel for schedule(dynamic)
        for(int i = 0; i < graph.labels.size(); i ++){
            labels[i] = graph.labels[i];
        }
    }

    // Graph(const Graph &other) : nodeNum(other.nodeNum), edgeNum(other.edgeNum), adjList(other.adjList), labels(other.labels) {

    // }

    /*
     * Read the graph from the given path
     * Graph file format [all values are integers][undirected]:
     * Line1: n(nodeNum) m(edgeNum)
     * Line2: from_1 to_1
     * Line3: from_2 to_2
     * ...
     * Line_(m+1): from_m to_m
     * Line_(m+2): label of node_1
     * ...
     * Line_(1+m+n): label of node_n
     */
    void Read(string path);

    /*
     * print out the whole graph. [For debug]
     */
    void Print() const;

    void Print(ostream &ofile) const ;

    void clear();
    
    void Reformat(string path) const;

    // inline bool operator = ( Graph &other) 
    // {   bool adj_flag = true;
    //     bool label_flag = true;
    //     for (auto &e: adjList) {
            
    //     }
    //     return (nodeNum  == other.nodeNum && edgeNum == other.edgeNum) && 
    // }

};


#endif    /* GRAPH_H */

