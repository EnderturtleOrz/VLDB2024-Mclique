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
// #include "vf2_sub_state.hpp"
#include "nodesorter.hpp"
#include "nodeclassifier.hpp"
#include "../maximal-motif-clique.h"
#include "../global-variable.h"
#include <unordered_map>
// #define TIME_LIMIT 1
#include <omp.h>

template <>
long long VF3SubState<int, int, Empty, Empty>::instance_count = 0;
static long long state_counter = 0;

typedef struct visitor_data_s
{
    unsigned long first_solution_time;
    long solutions;
} visitor_data_t;

class SubgraphDegree
{
public:
    unsigned id;
    unsigned degree;
    inline bool operator<(const SubgraphDegree &other) const
    {
        return degree < other.degree || (degree == other.degree && id < other.id);
    }

    SubgraphDegree();
    SubgraphDegree(unsigned v, unsigned d);
    ~SubgraphDegree();
};

SubgraphDegree::SubgraphDegree() : id(-1), degree(-1) {}

SubgraphDegree::SubgraphDegree(unsigned v, unsigned d) : id(v), degree(d) {}

SubgraphDegree::~SubgraphDegree() {}

class SubgraphVertex
{
public:
    unsigned id;
    map<int, pair<int, int>> label2order;
    set<int> labels;
    inline bool operator<(SubgraphVertex &other)
    {
        // intother.label2order[e];
        // return degree < other.degree || (degree == other.degree && id < other.id);
        for (auto &e : labels)
        {
            return label2order[e].first < other.label2order[e].first || (label2order[e].second < other.label2order[e].second);
        }
        return id < other.id;
    }

    SubgraphVertex();
    SubgraphVertex(unsigned v, map<int, pair<int, int>> d, set<int> l);
    ~SubgraphVertex();
};

SubgraphVertex::SubgraphVertex() {}

SubgraphVertex::SubgraphVertex(unsigned v, map<int, pair<int, int>> d, set<int> l) : id(v), label2order(d), labels(l) {}

SubgraphVertex::~SubgraphVertex() {}

// For each subgrqaph, we find the maximal-motif-clique that contain this subgraph.
void GetMMC_VF3(const Graph &motif, const Graph &graph, const vector<int> &subgraph)
{
    current_node_set.clear();
    candidates.clear();
    label2adj.clear();
    if (SubgraphInSetTrieNode(subgraph))
        return;
    // FindMaximalMotifClique(label2adj, subgraph, motif, graph, current_node_set, candidates,
    //                        maximal_motif_cliques);
    if (!DUPLICATION)
        root->AddSet(set<int>(subgraph.begin(), subgraph.end()), global_hash_id);
}
void GetMMC_VF3_new(const Graph &motif, const Graph &graph, const vector<int> &subgraph,
                    int initial_id, vector<vector<int>> &motif_neighbor, vector<vector<int>> &matched_subgraphs, vector<set<int>> &maximal_motif_clique)
{
   
    // current_node_set.clear();
    // candidates.clear();
    // label2adj.clear();
    // label2cover.clear();
    set<int> current_node_set_th;
    unordered_set<int> candidates_th;
    map<int, map<int, set<int>>> label2adj_th;
    map<int,  set<int>> label2cover_th;

    if (InitCandidate)
    {

        if (motif_neighbor[initial_id].empty() && !current_node_set_th.empty())
        {
            maximal_motif_clique.push_back(current_node_set_th);
            root->AddSet(set<int>(subgraph.begin(), subgraph.end()), global_hash_id);
            return;
        }
    }
    FindMaximalMotifClique_new(label2adj_th, label2cover_th, subgraph, motif, graph, current_node_set_th, candidates_th, maximal_motif_clique,
                               initial_id, motif_neighbor, matched_subgraphs);
    root->AddSet(set<int>(subgraph.begin(), subgraph.end()), global_hash_id);
}

bool visitor(int n, node_id ni1[], node_id ni2[], void *state, void *usr_data, vector<vector<int>> &matched_subgraphs)
{

    vector<int> result;
    for (int i = 0; i < n; ++i)
    {
        result.push_back(int(ni2[i]));
    }
    if (SubgraphInSetTrieNode(result))
        return false;

    matched_subgraphs.push_back(result);
    root->AddSet(set<int>(result.begin(), result.end()), global_hash_id);

    return false;
}
bool visitorOrbit(int n, node_id ni1[], node_id ni2[], void *state, void *usr_data, vector<vector<int>> &matched_subgraphs)
{

    vector<int> result;
    for (int i = 0; i < n; ++i)
    {
        result.push_back(int(ni2[i]));
    }
    matched_subgraphs.push_back(result);
    return false;
}

inline void collect_two_hop_adj(vector<int> &adj, vector<vector<int>> &adj_vec)
{

    int num_vertices = adj_vec.size();

    for (unsigned i = 0; i < num_vertices; i++)
    {
        unordered_set<int> anchorSet;
        unordered_set<int> nbSet;
        int startLabel = graph.labels[i];
        for (const int &label : motif_labelAdj[startLabel])
        {
            for (const int &ver : graph.adjList[i])
            {
                if (label == graph.labels[ver])
                {
                    anchorSet.insert(ver);
                }
            }
            unordered_set<int> tmp_nbSet;
            for (const int &anchorId : anchorSet)
            {
                for (const int &nbVertexId : graph.adjList[anchorId])
                {
                    if (graph.labels[nbVertexId] == startLabel && nbVertexId != i)
                    {
                        tmp_nbSet.insert(nbVertexId);
                    }
                }
            }
            unordered_set<int> deleteSet;
            if (nbSet.empty())
                nbSet = tmp_nbSet;
            else
            {
                if (tmp_nbSet.empty())
                {
                    nbSet.clear();
                    break;
                }
                for (const auto &e : nbSet)
                {
                    if (tmp_nbSet.find(e) == tmp_nbSet.end())
                    {
                        deleteSet.insert(e);
                    }
                }
                if (!deleteSet.empty())
                {
                    for (const int &delVer : deleteSet)
                        nbSet.erase(delVer);
                }
            }
        }

        for (const int &nerVertexId : nbSet)
        {
            adj_vec[i].emplace_back(nerVertexId);
        }
        adj[i] = adj_vec[i].size();
    }
}
class Subgraph
{
public:
    unsigned id;
    vector<int> subgraphs;
    map<int, vector<int>> label2Order;

    Subgraph();
    Subgraph(unsigned v, vector<int> graph, map<int, vector<int>> label2Order_);
    ~Subgraph();
};

Subgraph::Subgraph() : id(-1) {}

Subgraph::Subgraph(unsigned v, vector<int> graph, map<int, vector<int>> label2Order_) : id(v), subgraphs(graph), label2Order(label2Order_)
{
}

Subgraph::~Subgraph() {}

// 2-app greedy color algorithm
void colorGraph()
{

    int num_vertices = graph.adjList.size();
    vector<int> colorMap(num_vertices, -1);
    unordered_set<int> coloredNodes;
    for (int i = 0; i < num_vertices; i++)
    {
        int label = graph.labels[i];
        if (coloredNodes.count(i) == 0)
        {
            // obtain the color number from neighbors
            unordered_set<int> neighborColors;
            for (int neighbor : graph.adjList[i])
            {
                int ner_label = graph.labels[neighbor];
                if (motif_labelAdj[label].find(ner_label) == motif_labelAdj[label].end())
                    continue;
                if (colorMap[neighbor] != -1)
                {
                    neighborColors.insert(colorMap[neighbor]);
                }
            }

            // assign a color-number
            int color = 1;
            while (neighborColors.count(color) != 0)
            {
                color++;
            }

            // color this vertex
            colorMap[i] = color;
            coloredNodes.insert(i);
        }
        // color the neighbor
        for (int neighbor : graph.adjList[i])
        {
            int ner_label = graph.labels[neighbor];
            if (motif_labelAdj[label].find(ner_label) == motif_labelAdj[label].end())
                continue;
            if (colorMap[neighbor] == -1)
            {
                colorMap[neighbor] = colorMap[i];
                coloredNodes.insert(neighbor);
            }
        }
    }
    extern vector<int> vertexColor;
    vertexColor = colorMap;
}
/*
Sort by the given starategy
#parm: choice: discription
0: non-order
1: degree order
2: core order
3: (k,m)-core order
*/
void sort_subgraphs(unsigned strategy)
{
    switch (strategy)
    {
    case 0:
    {
        // non-order is needed
        break;
    }
    case 1:
    { // degree vertex order
        cout << "degree vertex order is applied..." << endl;
        std::map<int, int> ori2sort;

        int num_vertices = graph.nodeNum;
        vector<SubgraphDegree> subdegs(num_vertices);
        for (unsigned i = 0; i < num_vertices; i++)
        {
            SubgraphDegree vd(i, graph.adjList[i].size());
            subdegs[i] = vd;
        }
        sort(subdegs.begin(), subdegs.end());
        for (unsigned i = 0; i < num_vertices; i++)
        {
            ori2sort[subdegs[i].id] = i;
        }
        extern map<int, int> vertexOrder;
        vertexOrder.clear();
        vertexOrder = ori2sort;
        break;
    }
    case 2:
    case 3:
    {
        // For k-core and (k,m)-core:
        // The detailed difference is as follows:
        // 1. For k-core, we first calculate the core order and then invoke the "ReduceGraph" function.
        // 2. For (k, m)-core, we first invoke the "ReduceGraph" function, and then the k-core is transformed into (k,m)-core.
        double avgDegree = 0;
        cout << "core vertex order is applied..." << endl;
        std::map<int, int> sort2ori;
        std::map<int, int> ori2sort;
        int num_vertices = graph.adjList.size();
        unsigned *to_remove_vertices = new unsigned[num_vertices];
        bool *removed = new bool[num_vertices];
        int idx = num_vertices - 1;
        for (unsigned i = 0; i < num_vertices; i++)
        {
            to_remove_vertices[i] = 0;
            removed[i] = false;
        }
        vector<int> newVerticesIds(num_vertices);
        unsigned k_value = 1;
        vector<int> deg(num_vertices);
        vector<vector<int>> adj_vec(num_vertices);
        for (unsigned i = 0; i < num_vertices; i++)
        {
            deg[i] = graph.adjList[i].size();
            adj_vec[i].assign(graph.adjList[i].begin(), graph.adjList[i].end());
        }
        unsigned start_idx, end_idx;
        start_idx = end_idx = 0;
        while (idx >= 0)
        {
            for (unsigned i = 0; i < end_idx; i++)
            {
                to_remove_vertices[i] = 0;
            }
            start_idx = end_idx = 0;

            for (unsigned i = 0; i < num_vertices; i++)
            {
                if ((deg[i] < k_value) && !removed[i])
                {
                    to_remove_vertices[end_idx++] = i;
                    removed[i] = true;
                    newVerticesIds[idx--] = i;
                }
            }

            while (start_idx != end_idx)
            {
                unsigned vertex = to_remove_vertices[start_idx++];

                for (int i = 0; i < deg[vertex]; i++)
                {
                    unsigned other = adj_vec[vertex][i];

                    for (int j = 0; j < deg[other]; j++)
                    {
                        if (adj_vec[other][j] == vertex)
                        {
                            unsigned tmp = adj_vec[other][j];
                            adj_vec[other][j] = adj_vec[other][--deg[other]];
                            adj_vec[other][deg[other]] = tmp;
                            break;
                        }
                    }

                    if ((deg[other] < k_value) && !removed[other])
                    {
                        to_remove_vertices[end_idx++] = other;
                        removed[other] = true;
                        newVerticesIds[idx--] = other;
                    }
                }
                deg[vertex] = 0;
            }
            k_value++;
        }
        delete[] to_remove_vertices;
        delete[] removed;
        // reverse the order
        reverse(newVerticesIds.begin(), newVerticesIds.end());

        for (int i = 0; i < num_vertices; i++)
        {
            ori2sort[newVerticesIds[i]] = i;
        }
        extern map<int, int> vertexOrder;
        vertexOrder.clear();
        vertexOrder = ori2sort;
        break;
    }
        // case 4:
        // {
        //     cout << "core vertex order is applied..." << endl;
        //     std::map<int, int> sort2ori;
        //     std::map<int, int> ori2sort;
        //     int num_subgraphs = matched_subgraphs.size();
        //     int num_vertices = graph.adjList.size();
        //     unsigned *to_remove_vertices = new unsigned[num_vertices];
        //     bool *removed = new bool[num_vertices];

        //     int idx = num_vertices - 1;
        //     for (unsigned i = 0; i < num_vertices; i++)
        //     {
        //         to_remove_vertices[i] = 0;
        //         removed[i] = false;
        //     }
        //     vector<int> newVerticesIds(num_vertices);
        //     unsigned k_value = 1;

        //     vector<int> deg(num_vertices);
        //     vector<vector<int>> adj_vec(num_vertices);
        //     map<int, set<int>> id2flags;
        //     // for (unsigned i = 0; i < num_subgraphs; i++)
        //     // {
        //     //     for (const auto &x : matched_subgraphs[i])
        //     //     {
        //     //     //     for (const auto &y : motif_neighbor[i])
        //     //     //     {
        //     //     //         if (graph.labels[x] == graph.labels[y])
        //     //     //         {
        //     //     //             if (id2flags[x].find(y) == id2flags[x].end())
        //     //     //             {
        //     //     //                 id2flags[x].insert(y);
        //     //     //                 adj_vec[x].emplace_back(y);
        //     //     //                 deg[x] += 1;
        //     //     //             }
        //     //     //         }
        //     //     //     }
        //     //         if (id2flags[x].find(i) == id2flags[x].end())
        //     //         {
        //     //             id2flags[x].insert(i);
        //     //             adj_vec[x].emplace_back(i);
        //     //             deg[x] += 1;
        //     //         }
        //     //     }
        //     // }
        //     collect_two_hop_adj(deg, adj_vec);
        //     unsigned start_idx, end_idx;
        //     start_idx = end_idx = 0;
        //     while (idx >= 0)
        //     {
        //         for (unsigned i = 0; i < end_idx; i++)
        //         {
        //             to_remove_vertices[i] = 0;
        //         }
        //         start_idx = end_idx = 0;

        //         for (unsigned i = 0; i < num_vertices; i++)
        //         {
        //             if ((deg[i] < k_value) && !removed[i])
        //             {
        //                 to_remove_vertices[end_idx++] = i;
        //                 removed[i] = true;
        //                 newVerticesIds[idx--] = i;
        //             }
        //         }

        //         while (start_idx != end_idx)
        //         {
        //             unsigned vertex = to_remove_vertices[start_idx++];

        //             for (int i = 0; i < deg[vertex]; i++)
        //             {
        //                 unsigned other = adj_vec[vertex][i];

        //                 for (int j = 0; j < deg[other]; j++)
        //                 {
        //                     if (adj_vec[other][j] == vertex)
        //                     {
        //                         unsigned tmp = adj_vec[other][j];
        //                         adj_vec[other][j] = adj_vec[other][--deg[other]];
        //                         adj_vec[other][deg[other]] = tmp;
        //                         break;
        //                     }
        //                 }

        //                 if ((deg[other] < k_value) && !removed[other])
        //                 {
        //                     to_remove_vertices[end_idx++] = other;
        //                     removed[other] = true;
        //                     newVerticesIds[idx--] = other;
        //                 }
        //             }
        //             // if (id2flags[vertex].empty()) continue;

        //             // for (const auto &i : id2flags[vertex])
        //             // {
        //             //     for (const auto &other : matched_subgraphs[i])
        //             //     {if (other == vertex) {
        //             //         continue;
        //             //     }
        //             //         id2flags[other].erase(i);
        //             //         deg[other] -= 1;
        //             //         if ((deg[other] < k_value) && !removed[other])
        //             //         {
        //             //             to_remove_vertices[end_idx++] = other;
        //             //             removed[other] = true;
        //             //             newVerticesIds[idx--] = other;
        //             //         }
        //             //     }
        //             // }

        //             deg[vertex] = 0;
        //         }
        //         k_value++;
        //     }
        //     delete[] to_remove_vertices;
        //     delete[] removed;
        //     // reverse the order
        //     reverse(newVerticesIds.begin(), newVerticesIds.end());

        //     for (int i = 0; i < num_vertices; i++)
        //     {
        //         ori2sort[newVerticesIds[i]] = i;
        //     }
        //     extern map<int, int> vertexOrder;
        //     vertexOrder.clear();
        //     vertexOrder = ori2sort;
        //     break;
        // }
    }
}

typedef unsigned long long ULL;

// const int  P = 131;

// inline ULL string_hash(string &str, int n) {

//     ULL p[str.size() + 1];
//     p[0] = 1;
//     ULL key = 0;
//     for (int i = 1; i <= str.size(); i ++)
//     {
//         key = key * P + str[i - 1];
//         p[i] = p[i - 1] * P;
//     }
//     return key;

// }
#include <string_view>
#include <cstdint>

const int P = 131;

inline ULL string_hash(string &str, int n)
{

    ULL p[str.size() + 1];
    p[0] = 1;
    ULL key = 0;
    for (int i = 1; i <= str.size(); i++)
    {
        key = key * P + str[i - 1];
        p[i] = p[i - 1] * P;
    }
    return key;
}

bool vf3ImpOrbitCheck(ARGraph<int, Empty> &patt_graph, ARGraph<int, Empty> &targ_graph, vector<vector<int>> &matched_subgraphs)
{

    visitor_data_t vis_data;
    state_counter = 0;
    int n = 0;

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

    vis_data.solutions = 0;
    vis_data.first_solution_time = 0;
    VF3NodeSorter<int, Empty, SubIsoNodeProbability<int, Empty>> sorter(&targ_graph);
    std::vector<node_id> sorted = sorter.SortNodes(&patt_graph);

    VF3SubState<int, int, Empty, Empty> s0(&patt_graph, &targ_graph, class_patt.data(),
                                           class_targ.data(), classifier.CountClasses(), sorted.data());

    match<VF3SubState<int, int, Empty, Empty>>(s0, &n, n1, n2, visitorOrbit, &vis_data, matched_subgraphs);

    delete[] n1;
    delete[] n2;
    return matched_subgraphs.size();
}

struct begin_cmp
{
    bool operator()(const pair<int, int> a, const pair<int, int> b)
    {
        return a.first > b.first;
    }
};
struct end_cmp
{
    bool operator()(const pair<int, int> a, const pair<int, int> b)
    {
        return a.second <= b.second;
    }
};
bool compareVectors(vector<int> &vec1, vector<int> &vec2)
{
    if (vec1.size() != vec2.size())
    {
        return false;
    }

    for (int i = 0; i < vec1.size(); i++)
    {
        if (vec1[i] != vec2[i])
        {
            return false;
        }
    }
    return true;
}

bool vf3Impl(ARGraph<int, Empty> &patt_graph, ARGraph<int, Empty> &targ_graph,
             vector<vector<int>> &matched_subgraphs)
{

    visitor_data_t vis_data;
    state_counter = 0;
    int n = 0;
    double timeAll = 0;
    double timeFirst = 0;
    unsigned long firstSolTicks = 0;
    unsigned long endTicks = 0;
    unsigned long ticks = 0;
    unsigned long startVF3 = 0;
    // float limit = TIME_LIMIT;

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

    vis_data.solutions = 0;
    vis_data.first_solution_time = 0;
    VF3NodeSorter<int, Empty, SubIsoNodeProbability<int, Empty>> sorter(&targ_graph);
    std::vector<node_id> sorted = sorter.SortNodes(&patt_graph);

    VF3SubState<int, int, Empty, Empty> s0(&patt_graph, &targ_graph, class_patt.data(),
                                           class_targ.data(), classifier.CountClasses(), sorted.data());

    cout << "Start to do matching using VF3" << endl;
    startVF3 = clock();
    match<VF3SubState<int, int, Empty, Empty>>(s0, &n, n1, n2, visitor, &vis_data, matched_subgraphs);
    // cerr << "VF3 Time is: " << ((double)(clock() - startVF3) / CLOCKS_PER_SEC) << endl;
    vf3_time += ((double)(clock() - startVF3) / CLOCKS_PER_SEC);
    ticks = clock();
    // DataStructs for computing the motif-neighbor
    vector<vector<int>> new_motif_neighbor(n + 1, vector<int>());
    unordered_map<ULL, vector<pair<int, int>>> motif_map;
    vector<vector<int>> motif_neighbor(matched_subgraphs.size(), vector<int>());
    unordered_map<unsigned, unordered_map<int, vector<ULL>>> id2Subgraphs;
    map<ULL, map<int, int>> key2ExceptVertex;
    if (InitCandidate)
    {
        for (unsigned i = 0; i < matched_subgraphs.size(); ++i)
        {
            // hash each subgraph
            // basic: string hash
            for (int k = 0; k < matched_subgraphs[i].size(); k++)
            {
                int u = matched_subgraphs[i][k];
                vector<int> tmp_subgraph;
                for (auto &e : matched_subgraphs[i])
                {
                    if (e != u)
                    {
                        tmp_subgraph.emplace_back(e);
                    }
                }
                sort(tmp_subgraph.begin(), tmp_subgraph.end());
                string to_hash = "";

                for (int i = 0; i < tmp_subgraph.size(); i++)
                {
                    to_hash += to_string(tmp_subgraph[i]);
                    if (i != tmp_subgraph.size() - 1)
                    {
                        to_hash += "#";
                    }
                }

                ULL key = string_hash(to_hash, motif.nodeNum);
                motif_map[key].emplace_back(make_pair(i, u));
                key2ExceptVertex[key][i] = u;
                id2Subgraphs[i][graph.labels[u]].emplace_back(key);
            }
        }

        // cerr << "string hash time: " << ((double)(clock() - ticks) / CLOCKS_PER_SEC) << endl;
        extern map<int, int> currentOrder;
        extern map<int, int> vertexOrder;
        // Compute the motif-neighbor
        for (unsigned i = 0; i < matched_subgraphs.size(); ++i)
        {
            unordered_set<int> visit;
            for (auto &label2keys : id2Subgraphs[i])
            {
                set<int> neighbors, tar_neighbors;
                unordered_map<int, int> count_map;

                for (unsigned j = 0; j < label2keys.second.size(); j++)
                {
                    ULL keyULL = label2keys.second[j];
                    int anchor_node = key2ExceptVertex[keyULL][i];
                    set<int> tmp_neighbors;

                    for (auto &element : motif_map[keyULL])
                    {
                        if (label2keys.first != graph.labels[element.second] || anchor_node == element.second)
                            continue;

                        if (j == 0)
                        {
                            neighbors.insert(element.second);
                        }
                        else
                        {
                            tmp_neighbors.insert(element.second);
                        }
                    }

                    if (j > 0)
                    {
                        tar_neighbors.clear();
                        if (neighbors.empty() || tmp_neighbors.empty())
                        {
                            neighbors.clear();
                            continue;
                        }

                        count_map.clear();
                        count_map.reserve(neighbors.size());
                        int min_val = std::numeric_limits<int>::max();
                        int max_val = std::numeric_limits<int>::min();

                        // Step 1: Insert elements of set1 into the map
                        for (const int &val : neighbors)
                        {
                            count_map[val]++;
                            min_val = std::min(min_val, val);
                            max_val = std::max(max_val, val);
                        }

                        // Step 2: Iterate through set2 and check for common elements
                        for (const int &val : tmp_neighbors)
                        {
                            // Step 3: Skip elements not in the min_val and max_val range
                            if (val < min_val || val > max_val)
                            {
                                continue;
                            }

                            if (count_map[val] > 0)
                            {
                                tar_neighbors.insert(val);
                                count_map[val]--;
                            }
                        }
                        neighbors = tar_neighbors;
                    }
                }

                for (auto &key : neighbors)
                {
                    motif_neighbor[i].emplace_back(key);
                }
            }
        }
    }
    init_time += ((double)(clock() - ticks) / CLOCKS_PER_SEC);

    // For each matched subgraph, search the maximal clique!
    if (!ORDER)
    {
        global_hash_id = 0;
        root = new SetTrieNode(-1, false, 0, global_hash_id++);
    }
    // colorGraph();

    omp_set_num_threads(8);
    int tnum = omp_get_max_threads();
    Graph **c_graphs = (Graph **)malloc(sizeof(Graph *) * tnum);
    for(int i = 0; i < tnum; i ++){
        c_graphs[i] = new Graph();
        c_graphs[i]->cloneGraph(graph);
    }

#pragma omp parallel for schedule(dynamic)
    for (unsigned int i = 0; i < matched_subgraphs.size(); ++i)
    {
        int tid = omp_get_thread_num();
        vector<set<int>> maximal_motif_clique;
        // GetMMC_VF3_new(motif, c_graphs[tid], matched_subgraphs[i], i, motif_neighbor, matched_subgraphs, maximal_motif_clique);
        GetMMC_VF3_new(motif, *c_graphs[tid], matched_subgraphs[i], i, motif_neighbor, matched_subgraphs, maximal_motif_clique);
#pragma omp critical
{
        maximal_motif_cliques.insert(maximal_motif_cliques.end(), maximal_motif_clique.begin(), maximal_motif_clique.end());
}
    }

    for(int i = 0; i < tnum; i ++){
        delete c_graphs[i];
    }

    free(c_graphs);

    delete[] n1;
    delete[] n2;
    return true;
}

#endif
