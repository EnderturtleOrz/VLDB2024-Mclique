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
    vector<set<int> > adjList;

    // The adjacency list of the graph; InAdjList[i] stores the in-neighbor set of node i
    vector<set<int> > InAdjList;

    // The adjacency list of the graph; OutAdjList[i] stores the out-neighbor set of node i
    vector<set<int> > OutAdjList;

    // The edge list
    vector<EDGE> edges;

    // labels of nodes. Node id {0, ..., nodeNum-1}
    vector<int> labels;

    Graph();

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
};


#endif    /* GRAPH_H */

