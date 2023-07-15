//
// Created by Hujiafeng on 16/10/2017.
//

#include"Graph.h"
#include"global-variable.h"


Graph::Graph() {
}

void Graph::Read(string path) {
    cout << "Start to load undirected graph from " << path << " for the basic subgraph matching algorithm." << endl;
    FILE *fin;
    fin = fopen(path.c_str(), "rb");
    if (fin == NULL) {
        printf("ERROR: network file not found!\n");
        exit(1);
    }

    fscanf(fin, "%d %d", &nodeNum, &edgeNum);
    cout << "#Nodes:" << nodeNum << " #Edegs:" << edgeNum << endl;

    int from, to;
    adjList.resize(nodeNum);

    if(directed) {
        InAdjList.resize(nodeNum);
        OutAdjList.resize(nodeNum);
    }

    int self_edge = 0;
    for (int i = 0; i < edgeNum; ++i) {
        fscanf(fin, "%d %d", &from, &to);

        if(CHECKSPECIES){
            if(validNodes.find(from) == validNodes.end() || validNodes.find(to) == validNodes.end()){
                continue;
            }
        }


        // Ignore self-edge
        if (to == from) {
            self_edge++;
            continue;
        }

        if (adjList[from].find(to) != adjList[from].end()) {
            // This edge has existed.
            continue;
        }
        if (from >= nodeNum || to >= nodeNum) continue;

        adjList[from].insert(to);
        edges.push_back(EDGE(from, to));

        if(!directed) {
            // insert its reverse edge
            edges.push_back(EDGE(to, from));
            adjList[to].insert(from);
        }else{
            InAdjList[to].insert(from);
            OutAdjList[from].insert(to);
        }
    }
    int label;
    for (int i = 0; i < nodeNum; ++i) {
        fscanf(fin, "%d", &label);
#ifdef UNLABELED
        // Ignore all node labels.
            label=0;
#endif
        labels.push_back(label);
    }

    assert(labels.size() == nodeNum);
    edgeNum = edges.size();
    fclose(fin);
}

/*
 * print out the whole graph. [For debug]
 */
void Graph::Print() const {
    cout << "nid\tlabel\tNeighbors" << endl;
    for (int i = 0; i < nodeNum; i++) {
        cout << i << "\t" << labels[i] << "\t";
        for (const auto &e : adjList[i]) {
            cout << "\t" << e;
        }
        cout << endl;
    }
}

void Graph::Print(ostream &ofile) const {
    ofile << nodeNum << "\t" << edges.size() / 2 << endl;
    for (int i = 0; i < edges.size(); ++i) {
        if(edges[i].from < edges[i].to)
            ofile << edges[i].from << "\t" << edges[i].to << endl;
    }
    for (int i = 0; i < nodeNum; i++) {
        if (i == 0) ofile << labels[i];
        else ofile << "\t" << labels[i];
    }
    ofile << endl;
}

void Graph::clear() {
    for (auto &e : adjList) {
        e.clear();
    }
    adjList.clear();

    if(directed) {
        for (auto &e : InAdjList) {
            e.clear();
        }

        for (auto &e : OutAdjList) {
            e.clear();
        }
        InAdjList.clear();
        OutAdjList.clear();
    }

    edges.clear();
    labels.clear();
}