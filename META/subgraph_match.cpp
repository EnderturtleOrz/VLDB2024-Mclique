#include "subgraph_match.h"
#include "assert.h"
#include "maximal-motif-clique.h"

using namespace std;

int invalid = 0;

/* determines initial candidates of query vertices:
 * criteria --> label(u) = label(u'), degree(u) <= degree(u')
 */
void Initialize(vector<set<int> > &cand_set_for_each_node, const Graph &motif, const Graph &graph) {
    // store out_degree of query node and data node
    int deg_u, deg_v;
    // go through each motif node
    for (int u = 0; u < motif.nodeNum; ++u) {
        //get out_degree of motif node
        deg_u = motif.adjList[u].size();

        // go through each graph node
        for (int v = 0; v < graph.nodeNum; ++v) {
            // get out_degree of graph node
            deg_v = graph.adjList[v].size();

            // determine if the current graph node (v) is a candidate 
            // of the current motif node (u)
            if (motif.labels[u] == graph.labels[v] && deg_u <= deg_v) {
                cand_set_for_each_node[u].insert(v);
            }
        }
    }
}

/*
 * finds all candidate edges for given motif edge (u->v)
 * returns a candidate set of edges.
 */
void FindMatchedEdges(const int u, const int v, vector<set<int> > &c_set, const Graph &graph, set<EDGE> &ret) {

    // get candidate set of given lead vertex (u)
    set<int> &leadCand = c_set[u];

    // get candidate set of follow vertex (v)
    set<int> &followCand = c_set[v];

    // go through candidate set of lead vertex
    for (const auto lead_nid : leadCand) {
        // go through candidate set of follow vertex. If the number of neighbors
        // of the node (*leadIter) is larger than the size of followCand, we 
        // check the elements in followCand. Otherwise, we check its neighbors.
        if (graph.adjList[lead_nid].size() > followCand.size()) {
            for (const auto follow_nid : followCand) {
                //double check that vertices are next to one another
                if (graph.adjList[lead_nid].find(follow_nid) != graph.adjList[lead_nid].end()) {
                    ret.insert(EDGE(lead_nid, follow_nid));
                }
            }
        } else {
            for (const auto e : graph.adjList[lead_nid]) {
                //double check that vertices are next to one another
                if (followCand.find(e) != followCand.end()) {
                    ret.insert(EDGE(lead_nid, e));
                }
            }
        }
    }
}

// determines the visit order for finding partial solutions
// returns the order as a integer vector with each value = index that should be 
// visited, i.e. at index 0 the value is 15 so the first index visited is 15

void Setup(vector<set<EDGE> > &c_edges, const Graph &motif, vector<int> &ret) {
    //edges visited
    bool *edgeVisit = new bool[motif.edgeNum];
    memset(edgeVisit, 0, sizeof(bool) * motif.edgeNum);

    // query nodes visited
    bool *nodeVisit = new bool[motif.nodeNum];
    memset(nodeVisit, 0, sizeof(bool) * motif.nodeNum);

    // stores minimum index
    int _min = 0, index = 0;

    // first find query edge with smallest number of candidates
    for (index = 1; index < motif.edgeNum; ++index) {
        if (c_edges[index].size() <= c_edges[_min].size())
            _min = index;
    }

    // mark edge as visited
    edgeVisit[_min] = 1;

    // mark lead vertex node as visited
    nodeVisit[motif.edges[_min].from] = true;

    // mark follow vertex node as visited
    nodeVisit[motif.edges[_min].to] = true;

    // add first index to return vector
    ret.push_back(_min);

    // continue selecting vertices as long as all edges have not been visited
    while (ret.size() != motif.edgeNum) {
        //node A tracks edge with 2 visited nodes with smallest number of candidates
        //node B tracks edge with 1 visited node with smallest number of candidates
        //node C tracks edge with 0 visited nodes with smallest number of candidates --> used for multiple (disconnected) solutions
        int nodeA = -1, nodeB = -1, nodeC = -1;
        for (index = 0; index < motif.edgeNum; ++index) {
            if (!edgeVisit[index]) {
                //vertices of current edge
                int lead = motif.edges[index].from;
                int follow = motif.edges[index].to;

                if (nodeVisit[lead] && nodeVisit[follow]) {
                    if (nodeA == -1 || c_edges[index].size() <= c_edges[nodeA].size()) {
                        nodeA = index;
                    }
                } else if (nodeVisit[lead] || nodeVisit[follow]) {
                    if (nodeB == -1 || c_edges[index].size() <= c_edges[nodeB].size()) {
                        nodeB = index;
                    }
                } else {
                    if (nodeC == -1 || c_edges[index].size() <= c_edges[nodeC].size()) {
                        nodeC = index;
                    }
                }
            }
        }

        int current = -1;
        if (nodeA != -1) {
            current = nodeA;
        } else if (nodeB != -1) {
            current = nodeB;
        } else if (nodeC != -1) {
            current = nodeC;
        }

        //mark edge as visited
        edgeVisit[current] = true;

        //mark lead vertex as visited
        nodeVisit[motif.edges[current].from] = true;

        //mark follow vertex node as visited
        nodeVisit[motif.edges[current].to] = true;

        //add index to return vector
        ret.push_back(current);
    }
    delete[] edgeVisit;
    delete[] nodeVisit;
}

// goes through all possible solutions and returns the matches

void Join(const vector<int> &order, const Graph &motif, const Graph &graph,
          const vector<set<EDGE> > &c_edges, int *motif_assign, int *graph_assign,
          int current, int *levelSet, vector<vector<int> > &matched_subgraphs,
          const bool Isomorphism_Check) {
    //if dfs has reached last level
    if (current == motif.edgeNum) return;
    if (Isomorphism_Check && !matched_subgraphs.empty()) return;
    if (SUBGRAPH_LIMIT != -1 && matched_subgraphs.size() >= SUBGRAPH_LIMIT) return;

    int lead, follow;
    int leadCand, followCand;
    //gets the lead node of edge order[current]
    lead = motif.edges[order[current]].from;

    //gets the follow node of edge order[current]
    follow = motif.edges[order[current]].to;

    //for the current level (query node), given by order[current], go through all candidate edges 
    if (motif_assign[lead] != -1 && motif_assign[follow] != -1) {
        EDGE temp = EDGE(motif_assign[lead], motif_assign[follow]);
        if (c_edges[order[current]].find(temp) != c_edges[order[current]].end()) {
            if (current + 1 >= motif.edgeNum) {
                matched_subgraphs.push_back(vector<int>(motif_assign, motif_assign + motif.nodeNum));
                if (Isomorphism_Check) return;
                GetMMC(motif, graph, matched_subgraphs[matched_subgraphs.size() - 1]);
                if (SUBGRAPH_LIMIT != -1 && matched_subgraphs.size() >= SUBGRAPH_LIMIT) return;
            } else { // call join function on next level
                Join(order, motif, graph, c_edges, motif_assign, graph_assign, current + 1, levelSet, matched_subgraphs,
                     Isomorphism_Check);
            }
            assert(levelSet[lead] != current);
            if (levelSet[lead] == current) {
                graph_assign[motif_assign[lead]] = -1;
                motif_assign[lead] = -1;
                levelSet[lead] = -1;
            }
            assert(levelSet[follow] != current);
            if (levelSet[follow] == current) {
                graph_assign[motif_assign[follow]] = -1;
                motif_assign[follow] = -1;
                levelSet[follow] = -1;
            }
        }
    } else {
        for (const auto &e : c_edges[order[current]]) {
            if (Isomorphism_Check && !matched_subgraphs.empty()) return;
            invalid = 0;
            leadCand = e.from;
            followCand = e.to;

            // first check data node lead assignment
            if (graph_assign[leadCand] != -1 && graph_assign[leadCand] != lead) {
                invalid = 1;
            } else if (graph_assign[followCand] != -1 &&
                       graph_assign[followCand] != follow) { //data node follow assignment
                invalid = 1;
            } else if (motif_assign[lead] != -1 && motif_assign[lead] != leadCand) { //query node lead
                invalid = 1;
            } else if (motif_assign[follow] != -1 && motif_assign[follow] != followCand) { //query node follow
                invalid = 1;
            }

            //if still a valid solution path
            if (invalid == 0) {
                //if query nodes are unassigned
                //set level where assigned
                if (motif_assign[lead] == -1) {
                    graph_assign[leadCand] = lead; //set new data assignments
                    motif_assign[lead] = leadCand; //set new query assignments
                    levelSet[lead] = current;
                }
                if (motif_assign[follow] == -1) {
                    graph_assign[followCand] = follow;
                    motif_assign[follow] = followCand;
                    levelSet[follow] = current;
                }
                //if the current level is the last level then the assignments represent a solution
                if (current + 1 >= motif.edgeNum) {
                    matched_subgraphs.push_back(vector<int>(motif_assign, motif_assign + motif.nodeNum));
                    if (Isomorphism_Check) return;
                    GetMMC(motif, graph, matched_subgraphs[matched_subgraphs.size() - 1]);
                    if (matched_subgraphs.size() >= SUBGRAPH_LIMIT) return;
                } else {//call join function on next level
                    Join(order, motif, graph, c_edges, motif_assign, graph_assign,
                         current + 1, levelSet, matched_subgraphs, Isomorphism_Check);
                }
                if (levelSet[lead] == current) {
                    graph_assign[motif_assign[lead]] = -1;
                    motif_assign[lead] = -1;
                    levelSet[lead] = -1;
                }
                if (levelSet[follow] == current) {
                    graph_assign[motif_assign[follow]] = -1;
                    motif_assign[follow] = -1;
                    levelSet[follow] = -1;
                }
            }//if match was invalid then do nothing
        }
    }
}

/* 
 * black box for finding all matched subgraphs
 */
void GetMatchedSubgraph(const Graph &motif, const Graph &graph,
                        vector<vector<int> > &matched_subgraphs, const bool Isomorphism_Check) {

    unsigned long ticks = 0;
    ticks = clock();

    //stores candidate information for each query node
    vector<set<int> > c_set(motif.nodeNum);
    //sets original candidates of each query node based on: label(u) = label(u') && deg(u) <= deg(u')
    Initialize(c_set, motif, graph);

    //create array to store candidate edges
    vector<set<EDGE> > c_edges(motif.edgeNum);
    //go through all query edges
    for (int i = 0; i < motif.edgeNum; ++i) {
        //get candidate edges
        FindMatchedEdges(motif.edges[i].from, motif.edges[i].to, c_set, graph, c_edges[i]);
    }

    //Determine the order in which edges should be visited
    vector<int> order;

    Setup(c_edges, motif, order);

    int *motif_values = new int[motif.nodeNum];
    int *l_set = new int[motif.nodeNum];
    int *graph_values = new int[graph.nodeNum];
    memset(motif_values, -1, sizeof(int) * motif.nodeNum);
    memset(l_set, -1, sizeof(int) * motif.nodeNum);
    memset(graph_values, -1, sizeof(int) * graph.nodeNum);

    Join(order, motif, graph, c_edges, motif_values, graph_values, 0, l_set,
         matched_subgraphs, Isomorphism_Check);

    double timeAll = ((double) (clock() - ticks) / CLOCKS_PER_SEC);
//    if (Isomorphism_Check == false)
//        cout << matched_subgraphs.size() << " subgraphs have been found using the basic method in " << timeAll
//             << "seconds" << endl;

    delete[] motif_values;
    delete[] l_set;
    delete[] graph_values;
    return;
}

/*
 * black box for finding all matched subgraphs with "must contain nodes"
 */
void GetMatchedSubgraph(const Graph &motif, const Graph &graph,
                        vector<vector<int> > &matched_subgraphs, const int must_contain_node) {

    unsigned long ticks = 0;
    ticks = clock();

    //stores candidate information for each query node
    vector<set<int> > c_set(motif.nodeNum);
    //sets original candidates of each query node based on: label(u) = label(u') && deg(u) <= deg(u')
    Initialize(c_set, motif, graph);

    //create array to store candidate edges
    vector<set<EDGE> > c_edges(motif.edgeNum);
    //go through all query edges
    for (int i = 0; i < motif.edgeNum; ++i) {
        //get candidate edges
        FindMatchedEdges(motif.edges[i].from, motif.edges[i].to, c_set, graph, c_edges[i]);
    }

    //Determine the order in which edges should be visited
    vector<int> order;

    Setup(c_edges, motif, order);

    int *motif_values = new int[motif.nodeNum];
    int *l_set = new int[motif.nodeNum];
    int *graph_values = new int[graph.nodeNum];
    memset(motif_values, -1, sizeof(int) * motif.nodeNum);
    memset(l_set, -1, sizeof(int) * motif.nodeNum);
    memset(graph_values, -1, sizeof(int) * graph.nodeNum);


    for (auto i : graph.adjList[must_contain_node]) {
        EDGE edge(must_contain_node, i);
        // edge is directed.
        for (int j = 0; j < motif.edges.size(); j++) {
            if (motif.labels[motif.edges[j].from] == graph.labels[edge.from]
                && motif.labels[motif.edges[j].to] == graph.labels[edge.to]) {

                motif_values[motif.edges[j].from] = must_contain_node;
                graph_values[must_contain_node] = motif.edges[j].from;
                motif_values[motif.edges[j].to] = i;
                graph_values[i] = motif.edges[j].to;

                vector<int> new_order(order.begin(), order.end());
                auto it = std::find(new_order.begin(), new_order.end(), j);
                if (it != new_order.end())
                    new_order.erase(it);
                l_set[motif.edges[j].to] = 0;
                l_set[motif.edges[j].from] = 0;
                Join(new_order, motif, graph, c_edges, motif_values, graph_values, 1, l_set,
                     matched_subgraphs, false);

                motif_values[motif.edges[j].from] = -1;
                graph_values[must_contain_node] = -1;
                motif_values[motif.edges[j].to] = -1;
                graph_values[i] = -1;
                l_set[motif.edges[j].to] = -1;
                l_set[motif.edges[j].from] = -1;
            }
        }
    }

    double timeAll = ((double) (clock() - ticks) / CLOCKS_PER_SEC);

//    cout << matched_subgraphs.size() << " subgraphs have been found using the basic method in " << timeAll << "seconds"
//         << endl;

    delete[] motif_values;
    delete[] l_set;
    delete[] graph_values;
    return;
}

void GetMMC(const Graph &motif, const Graph &graph, const vector<int> &subgraph) {
    current_node_set.clear();
    candidates.clear();
    if (!label2adj.empty())
        for (auto e : label2adj) e.second.clear();
    label2adj.clear();
    if (SubgraphInSetTrieNode(subgraph)) return;
    FindMaximalMotifClique(label2adj, subgraph, motif, graph, current_node_set, candidates,
                           maximal_motif_cliques);
    if (!DUPLICATION)
        root->AddSet(set<int>(subgraph.begin(), subgraph.end()), global_hash_id);
}