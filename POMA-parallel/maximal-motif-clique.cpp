#include "maximal-motif-clique.h"
#include <sys/time.h>
#include <unordered_set>

void GetLabel2NodesMap(const Graph &g, map<int, set<int>> &label2nodes)
{
    for (auto e : label2nodes)
        e.second.clear();
    label2nodes.clear();

    for (int i = 0; i < g.nodeNum; ++i)
    {
        if (label2nodes.find(g.labels[i]) != label2nodes.end())
        {
            label2nodes[g.labels[i]].insert(i);
        }
        else
        {
            set<int> temp;
            temp.insert(i);
            label2nodes.insert(make_pair(g.labels[i], temp));
        }
    }
}

bool SubgraphInSetTrieNode(vector<int> subgraph)
{
    extern SetTrieNode *root;
    sort(subgraph.begin(), subgraph.end());
    int i = 0;
    SetTrieNode *temp = root;
    while (!temp->IsEndPoint())
    {
        temp = temp->GetChild(subgraph[i++]);
        if (temp == NULL)
            return false;
    }
    return true;
}

bool FindEndPointInSetTrieNode(const set<int> &current_node_set_th)
{
    extern int motif_node_size;
    extern map<int, int> vertexOrder;
    extern map<int, int> currentOrder;
    extern SetTrieNode *root;
    set<SetTrieNode *> temp_set;
    set<SetTrieNode *> stn_cands;
    SetTrieNode *temp;
    stn_cands.insert(root);
    int n = current_node_set_th.size();
    for (const auto &e : current_node_set_th)
    {
        n--;

        temp_set.clear();
        for (const auto &stn : stn_cands)
        {
            temp = stn->GetChild(e);

            if (temp != NULL)
            {
                if (temp->IsEndPoint())
                {
                    return true;
                }
                temp_set.insert(temp);
            }
            if (stn->GetHeight() + n >= motif_node_size)
                temp_set.insert(stn);
        }
        stn_cands = temp_set;
    }
    return false;
}

bool UpdateSTNCands(SetTrieNode *stn, const set<int> &rerun_set, int cand_size, set<SetTrieNode *> &stn_cands_new)
{
    set<SetTrieNode *> temp_set1;
    set<SetTrieNode *> temp_set2;
    temp_set1.insert(stn);
    SetTrieNode *temp;
    set<SetTrieNode *> *pointer[2] = {&temp_set1, &temp_set2};
    int index = 0;
    int n = rerun_set.size();
    for (const auto &e : rerun_set)
    {
        n--;
        pointer[1 - index]->clear();
        for (const auto &x : (*pointer[index]))
        {
            temp = x->GetChild(e);
            if (temp != NULL)
            {
                if (temp->IsEndPoint())
                    return true;
                pointer[1 - index]->insert(temp);
            }
            pointer[1 - index]->insert(x);
        }
        index = 1 - index;
    }
    stn_cands_new.insert(pointer[index]->begin(), pointer[index]->end());
    return false;
}

bool FindEndPointInSetTrieNode(int new_node, const set<int> &current_node_set,
                               set<SetTrieNode *> &stn_cands, int cand_size, set<SetTrieNode *> &stn_cands_new)
{
    extern int motif_node_size;
    set<int> rerun_set(current_node_set.find(new_node), current_node_set.end());
    for (const auto &stn : stn_cands)
    {
        if (stn->GetId() < new_node)
        {
            if (UpdateSTNCands(stn, rerun_set, cand_size, stn_cands_new))
                return true;
        }
        else
            stn_cands_new.insert(stn);
    }
    return false;
}
void GetLabelAdj(const Graph &g, map<int, set<int>> &labelAdj)
{
    for (auto e : labelAdj)
        e.second.clear();
    labelAdj.clear();
    int i, j;
    for (const auto &e : g.edges)
    {
        i = e.from;
        j = e.to;
        if (labelAdj.find(g.labels[i]) != labelAdj.end())
        {
            labelAdj[g.labels[i]].insert(g.labels[j]);
        }
        else
        {
            set<int> temp;
            temp.insert(g.labels[j]);
            labelAdj.insert(make_pair(g.labels[i], temp));
        }
    }
}

void GetLabel2NodesMap(const int current_node, const set<int> &U, const vector<int> &labels,
                       map<int, set<int>> &label2nodes, set<int> &label_set)
{
    for (const auto &u : U)
    {
        if (u == current_node)
            continue;
        if (label2nodes.find(labels[u]) != label2nodes.end())
        {
            label2nodes[labels[u]].insert(u);
        }
        else
        {
            set<int> temp;
            temp.insert(u);
            label2nodes.insert(make_pair(labels[u], temp));
            label_set.insert(labels[u]);
        }
    }
}

void copyGraph(Graph &target, const Graph &origin, const set<int> &nodes)
{
    map<int, int> nodeMap;
    int count = 0;
    target.labels.resize(nodes.size());
    for (const auto &e : nodes)
    {
        nodeMap[e] = count;
        target.labels[count] = origin.labels[e];
        count++;
    }
    target.nodeNum = count;
    for (const auto &e : nodes)
    {
        set<int> adj;
        for (const auto &v : nodes)
        {
            if (origin.adjList[e].find(v) != origin.adjList[e].end())
            {
                adj.insert(nodeMap[v]);

                target.edges.push_back(EDGE(nodeMap[e], nodeMap[v]));
            }
        }
        target.adjList.push_back(adj);
    }
    target.edgeNum = target.edges.size();
}

void IsoPreCheckNodeSet(const set<int> &nodes, const Graph &graph, set<int> &coverset)
{
    extern set<int> pre_IsoCheck_set;
    extern int current_u;
    bool flag;
    for (const auto &e : pre_IsoCheck_set)
    {
        if (e == current_u)
            continue;
        flag = true;
        for (const auto &w : nodes)
        {
            if (w == current_u || w == e)
                continue;
            if (!directed)
            {
                if (graph.adjList[e].find(w) != graph.adjList[e].end() && motif_labelAdj[graph.labels[e]].find(graph.labels[w]) != motif_labelAdj[graph.labels[e]].end() // only for undirected graph
                    && graph.adjList[current_u].find(w) == graph.adjList[current_u].end())
                {
                    flag = false;
                    break;
                }
            }
            else
            {
                if (graph.InAdjList[e].find(w) != graph.InAdjList[e].end() && graph.InAdjList[current_u].find(w) == graph.InAdjList[current_u].end())
                {
                    flag = false;
                    break;
                }
                if (graph.OutAdjList[e].find(w) != graph.OutAdjList[e].end() && graph.OutAdjList[current_u].find(w) == graph.OutAdjList[current_u].end())
                {
                    flag = false;
                    break;
                }
            }
        }
        if (flag)
            coverset.insert(e);
    }
}

bool IsoPreCheck(const set<int> &nodes, const Graph &graph)
{
    extern set<int> pre_IsoCheck_set;
    extern int current_u;
    int flag;
    for (const auto &e : pre_IsoCheck_set)
    {
        if (nodes.find(e) != nodes.end())
            continue;
        flag = true;
        for (const auto &w : nodes)
        {
            if (w == current_u)
                continue;
            if (!directed)
            {
                if (graph.adjList[e].find(w) != graph.adjList[e].end() && graph.adjList[current_u].find(w) == graph.adjList[current_u].end())
                {
                    flag = false;
                    break;
                }
            }
            else
            {
                if (graph.InAdjList[e].find(w) != graph.InAdjList[e].end() && graph.InAdjList[current_u].find(w) == graph.InAdjList[current_u].end())
                {
                    flag = false;
                    break;
                }
                if (graph.OutAdjList[e].find(w) != graph.OutAdjList[e].end() && graph.OutAdjList[current_u].find(w) == graph.OutAdjList[current_u].end())
                {
                    flag = false;
                    break;
                }
            }
        }
        if (flag)
            return true;
    }
    return false;
}

void DFSForIsomorphismCheck(map<int, set<int>> &cands,
                            map<int, set<int>> &cand_label2nodes,
                            // const map<int, set<int> > &motif_label2nodes,
                            const Graph &motif, const Graph &graph, set<int> &label_set, bool &Flag)
{

#ifdef DEBUG
    cout << "map cands" << endl;
    PrintMap(cands);

    cout << "map cand_label2nodes" << endl;
    PrintMap(cand_label2nodes);

    cout << "map motif_label2nodes" << endl;
    PrintMap(motif_label2nodes);
    // getchar();
#endif
    if (Flag == false)
        return;
    if (label_set.empty())
    {
        Graph *temp_graph = new Graph();
        
        set<int> nodes;
        for (auto e : cands)
        {
            for (auto u : e.second)
            {
                nodes.insert(u);
            }
        }
        // assert(nodes.size() == motif.nodeNum);

        //        if (!WithoutIsoCheckPruning) {
        //            //bool can_pass_the_test = false;
        //            if (IsoPreCheck(nodes, graph)) return;
        //        }
        vector<vector<int>> temp_matched_subgraphs;
        temp_matched_subgraphs.clear();
        

        copyGraph(*temp_graph, graph, nodes);

        // Here we still use the naive subgraph matching method to do checking.
        // TODO: consider to use vf3.
        iso_check_num++;
        GetMatchedSubgraph(motif, *temp_graph, temp_matched_subgraphs, true);

#ifdef DEBUG
        cout << "nodes" << endl;
        Print(nodes);
        cout << "matched: " << (!temp_matched_subgraphs.empty()) << endl;
#endif

        if (temp_matched_subgraphs.empty())
        {
            Flag = false;
#ifdef DEBUG
            if (can_pass_the_test)
            {
                cout << "Motif:" << endl;
                motif.Print();
                cout << "temp_graph:" << endl;
                temp_graph->Print();
                exit(0);
            }
#endif
        }
        else
        {
            test_num++;
        }
        return;
    }

    int label = *label_set.begin();

    int temp_node = *cand_label2nodes[label].begin();
    cand_label2nodes[label].erase(temp_node);
    InsertNode(cands, temp_node, label);
    bool erase_label = false;
    assert(motif_label2nodes.find(label) != motif_label2nodes.end());
    if (cands[label].size() == motif_label2nodes.at(label).size())
    {
        label_set.erase(label);
        erase_label = true;
    }

    DFSForIsomorphismCheck(cands, cand_label2nodes, motif, graph, label_set, Flag);

    if (erase_label == true)
    {
        label_set.insert(label);
    }
    cands[label].erase(temp_node);
    if (cand_label2nodes[label].size() >= (motif_label2nodes.at(label).size() - cands[label].size()))
        DFSForIsomorphismCheck(cands, cand_label2nodes, motif, graph, label_set, Flag);

    InsertNode(cand_label2nodes, temp_node, label);
    delete temp_graph;
}

void InsertNode(map<int, set<int>> &cands, const int v, const int label)
{
#ifdef DEBUG
    // cout << "In InsertNode" << endl;
    // cout << "Insert " << v << " with label " << label << endl;
#endif
    if (cands.find(label) != cands.end())
        cands[label].insert(v);
    else
    {
        set<int> temp;
        temp.insert(v);
        cands.insert(make_pair(label, temp));
    }
}

bool IsomorphismCheck(const set<int> &U, const int u, const Graph &motif, const Graph &graph)
{   
    // cout<<"In IsomorphismCheck"<<endl;
    if (U.find(u) != U.end())
        return false;
    extern map<int, set<int>> motif_label2nodes;
    extern map<int, set<int>> cand_label2nodes;
    extern set<int> pre_IsoCheck_set;
    extern int current_u;
    extern set<int> label_set;
    cand_label2nodes.clear();
    label_set.clear();

    GetLabel2NodesMap(u, U, graph.labels, cand_label2nodes, label_set);
    pre_IsoCheck_set.clear();
    for (auto e : cand_label2nodes[graph.labels[u]])
        pre_IsoCheck_set.insert(e);
    current_u = u;

    bool Flag = true;
    extern map<int, set<int>> cands;
    for (auto e : cands)
        e.second.clear();
    cands.clear();

    if (!WithoutIsoCheckPruning)
    {
        set<int> coverset;
        IsoPreCheckNodeSet(U, graph, coverset);
        if (coverset.size() + 1 > motif_label2nodes[graph.labels[u]].size())
            return true;

        for (auto e : coverset)
        {
            InsertNode(cands, e, graph.labels[e]);
            cand_label2nodes[graph.labels[e]].erase(e);
        }
    }

    InsertNode(cands, u, graph.labels[u]);
    if (motif_label2nodes[graph.labels[u]].size() == cands[graph.labels[u]].size())
    {
        label_set.erase(graph.labels[u]);
    }

    DFSForIsomorphismCheck(cands, cand_label2nodes, motif, graph, label_set, Flag);

    return Flag;
}
/**
 * @brief Check if v can append into the new candidates set
 * @param [U] current node set (R).
 * @param [u]
 * @param [v] the vertex needs to be checked
 * @param [motif] motif
 * @param [graph] the whole grpah
 * @returns If v can append into the candidates set? -> bool
 */
bool IsomorphismCheck(const set<int> &U, const int u, const int v, const Graph &motif, const Graph &graph)
{
    // extern map<int, set<int>> motif_label2nodes;
    set<int> pre_IsoCheck_set;
    int current_u;
    map<int, set<int>> cand_label2nodes;
    set<int> label_set;

    for (auto e : cand_label2nodes)
        e.second.clear();
    cand_label2nodes.clear();
    label_set.clear();
    // cand_label2nodes(map): key: label, value: vector<int> with the key-label
    GetLabel2NodesMap(u, U, graph.labels, cand_label2nodes, label_set);

    bool Flag = true;
    map<int, set<int>> cands;
    cands.clear();

    pre_IsoCheck_set.clear();
    for (auto e : cand_label2nodes[graph.labels[v]])
        pre_IsoCheck_set.insert(e);
    current_u = v;

    if (!WithoutIsoCheckPruning)
    {
        set<int> coverset;
        set<int> Y = U;
        Y.insert(u);
        IsoPreCheckNodeSet(Y, graph, coverset);
        coverset.erase(u);
        int x = 1;
        if (graph.labels[u] == graph.labels[v])
            x++;
        if (coverset.size() + x > motif_label2nodes[graph.labels[v]].size())
            return true;
        for (const auto e : coverset)
        {
            InsertNode(cands, e, graph.labels[e]);
            cand_label2nodes[graph.labels[e]].erase(e);
        }
    }

    InsertNode(cands, u, graph.labels[u]);
    InsertNode(cands, v, graph.labels[v]);

    if (motif_label2nodes[graph.labels[u]].size() < cands[graph.labels[u]].size())
    {
        // assert(motif_label2nodes[graph.labels[u]].size() == 1 && graph.labels[u] == graph.labels[v]);
        return true;
    }
    else if (motif_label2nodes[graph.labels[u]].size() == cands[graph.labels[u]].size())
    {
        label_set.erase(graph.labels[u]);
    }

    if (graph.labels[v] != graph.labels[u] && motif_label2nodes[graph.labels[v]].size() == cands[graph.labels[v]].size())
    {
        label_set.erase(graph.labels[v]);
    }

    test_num = 0;

    DFSForIsomorphismCheck(cands, cand_label2nodes, motif, graph, label_set, Flag);

    // if (Flag == true) {
    max_test_num = max(max_test_num, test_num);
    //}

    // if (Flag == false) {

    // }
    return Flag;
}
void GetCandidates(const map<int, map<int, set<int>>> &label2adj_th, const set<int> &current_node_set_th,
                   const Graph &motif, const Graph &graph, unordered_set<int> &candidates_th)
{
    // current_node_set;
    // candidates;
    // label2adj;
    // label2cover;
    extern bool *IsVisited;
    memset(IsVisited, 0, sizeof(bool) * graph.nodeNum);
    for (auto e : current_node_set_th)
        IsVisited[e] = true;
    bool flag;
    if (!directed)
    {
        for (auto id : current_node_set_th)
        {
            for (auto neighbor : graph.adjList[id])
            {
                // if(candidates_th.size() > CandidateLimit) return;
                int label = graph.labels[neighbor];
                if (!IsVisited[neighbor])
                {
                    IsVisited[neighbor] = true;
                    // check label is in label2adj_th
                    if (label2adj_th.find(label) != label2adj_th.end())
                    {

                        flag = false;
                        int count = 0;
                        for (const auto elementAdj : label2adj_th.at(label))
                        {
                            bool sinle_flag = true;
                            for (const auto element : elementAdj.second)
                            {
                                if (graph.adjList[neighbor].find(element) == graph.adjList[neighbor].end())
                                {
                                    sinle_flag = false;
                                    break;
                                }
                            }
                            if (sinle_flag)
                            {
                                count += 1;
                            }
                            if (count > motif_label2nodes[label].size())
                            {
                                flag = true;
                                break;
                            }
                        }

                        if (WithoutIsoCheckPruning)
                        {
                            if (IsomorphismCheck(current_node_set_th, neighbor, motif, graph))
                                candidates_th.insert(neighbor);
                        }
                        else if (flag || IsomorphismCheck(current_node_set_th, neighbor, motif, graph))
                            candidates_th.insert(neighbor);
                    }
                }
            }
        }
    }
    else
    {
        for (auto id : current_node_set_th)
        {
            for (auto neighbor : graph.InAdjList[id])
            {
                // if(candidates_th.size() > CandidateLimit) return;
                int label = graph.labels[neighbor];
                if (!IsVisited[neighbor])
                {
                    IsVisited[neighbor] = true;
                    // check label is in label2adj_th
                    if (label2adj_th.find(label) != label2adj_th.end())
                    {
                        // check nid is connected to all nodes in label2adj[label]
                        if (IsomorphismCheck(current_node_set_th, neighbor, motif, graph))
                            candidates_th.insert(neighbor);
                    }
                }
            }

            for (auto neighbor : graph.OutAdjList[id])
            {
                // if(candidates_th.size() > CandidateLimit) return;
                int label = graph.labels[neighbor];
                if (!IsVisited[neighbor])
                {
                    IsVisited[neighbor] = true;
                    // check label is in label2adj_th
                    if (label2adj_th.find(label) != label2adj_th.end())
                    {
                        // check nid is connected to all nodes in label2adj_th[label]
                        if (IsomorphismCheck(current_node_set_th, neighbor, motif, graph))
                            candidates_th.insert(neighbor);
                    }
                }
            }
        }
    }
    return;
}
void GetCandidates(const set<int> &current_node_set_th, const int new_node, const map<int, map<int, set<int>>> &label2adj_th,
                   const unordered_set<int> &candidates_th, map<int,  set<int>> &label2cover_th, const Graph &mitif, const Graph &graph, const set<int> &NOT,
                   set<int> &NOT_new,
                   unordered_set<int> &candidates_new)
{
    // current_node_set
	// candidates
	// label2adj
	// label2cover

    // motif_label2nodes
    // pre_IsoCheck_set
    // current_u
    // cand_label2nodes
    // label_set
    
    bool flag;
    bool cover_flag;
    // generate new_candidates
    for (const auto &nid : candidates_th)
    {

        int label = graph.labels[nid];
        // advanced node expansion
        flag = false;
        cover_flag = true;
        if (!directed)
        {
            int count = 0;
            for (const auto &elementAdj : label2adj_th.at(label))
            {
                bool sinle_flag = true;
                for (const auto &element : elementAdj.second)
                {
                    if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                    {
                        sinle_flag = false;
                        break;
                    }
                }
                if (sinle_flag)
                {
                    count += 1;
                }
                if (count > motif_label2nodes[label].size())
                {
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
        {
            for (const auto element : label2cover_th.at(label))
            {
                if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                {
                    cover_flag = false;
                    break;
                }
            }
        }
        if (flag)
        {
            candidates_new.insert(nid);
        }
        else if (cover_flag)
        {
            candidates_new.insert(nid);
        }
        else if (graph.labels[new_node] == label)
        {
            if (motif_label2nodes[label].size() == 1)
            {
                candidates_new.insert(nid);
            }
            else if (IsomorphismCheck(current_node_set_th, new_node, nid, motif, graph))
            {
                candidates_new.insert(nid);
            }
        }
        else if (IsomorphismCheck(current_node_set_th, new_node, nid, motif, graph))
        {
            candidates_new.insert(nid);
        }
    }

    // generate NOT_new
    for (const auto &nid : NOT)
    {
        int label = graph.labels[nid];
        //  advanced node expansion
        flag = false;
        cover_flag = true;
        if (!directed)
        {
            int count = 0;
            for (const auto &elementAdj : label2adj_th.at(label))
            {
                bool sinle_flag = true;
                for (const auto &element : elementAdj.second)
                {
                    if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                    {
                        sinle_flag = false;
                        break;
                    }
                }
                if (sinle_flag)
                {
                    count += 1;
                }
                if (count > motif_label2nodes[label].size())
                {
                    flag = true;
                    break;
                }
            }
        }
        if (!flag)
        {
            for (const auto element : label2cover_th.at(label))
            {
                if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                {
                    cover_flag = false;
                    break;
                }
            }
        }

        if (flag)
        {

            NOT_new.insert(nid);
        }
        else if (cover_flag)
        {
            NOT_new.insert(nid);
        }
        else if (graph.labels[new_node] == label)
        {
            if (motif_label2nodes[label].size() == 1)
            {
                NOT_new.insert(nid);
            }
            else if (IsomorphismCheck(current_node_set_th, new_node, nid, motif, graph))
            {
                NOT_new.insert(nid);
            }
        }
        else if (IsomorphismCheck(current_node_set_th, new_node, nid, motif, graph))
        {
            NOT_new.insert(nid);
        }
    }
}

void FindMaximalMotifClique_new(map<int, map<int, set<int>>> &label2adj_th, map<int, set<int>> &label2cover_th, 
                                const vector<int> &matched_subgraph,
                                const Graph &motif, const Graph &graph, set<int> &current_node_set_th,
                                unordered_set<int> &candidates_th, vector<set<int>> &maximal_motif_clique,
                                int initial_id, vector<vector<int>> &motif_neighbor, vector<vector<int>> &matched_subgraphs)
{   
    // current_node_set;
    // candidates;
    // label2adj;
    // label2cover;

    map<int, int> currentOrder;
    extern map<int, int> vertexOrder;
    // currentOrder.clear();
    for (auto from : matched_subgraph)
    {
        if (ORDER)
        {
            if (currentOrder[graph.labels[from]] < vertexOrder[from])
            {
                currentOrder[graph.labels[from]] = vertexOrder[from];
            }
        }
        for (auto from : matched_subgraph)
            for (auto to : matched_subgraph)
            {
                if (from < to && graph.adjList[from].find(to) != graph.adjList[from].end() &&
                    motif_labelAdj[graph.labels[from]].find(graph.labels[to]) != motif_labelAdj[graph.labels[from]].end())
                {
                    if (label2adj_th.find(graph.labels[from]) == label2adj_th.end())
                    {
                        label2adj_th[graph.labels[from]].insert(make_pair(from, set<int>()));
                    }
                    label2adj_th[graph.labels[from]][from].insert(to);

                    if (label2adj_th.find(graph.labels[to]) == label2adj_th.end())
                    {
                        label2adj_th[graph.labels[to]].insert(make_pair(to, set<int>()));
                    }
                    label2adj_th[graph.labels[to]][to].insert(from);
                    // update label2cover_th:
                    if (label2cover_th.find(graph.labels[from]) == label2cover_th.end())
                    { // label[from] does not exist in label2adj_th
                        label2cover_th.insert(make_pair(graph.labels[from], set<int>()));
                    }
                    label2cover_th[graph.labels[from]].insert(to);

                    if (label2cover_th.find(graph.labels[to]) == label2cover_th.end())
                    { // label[to] does not exist in label2adj_th
                        label2cover_th.insert(make_pair(graph.labels[to], set<int>()));
                    }
                    label2cover_th[graph.labels[to]].insert(from);
                }
            }
    }
    std::copy(matched_subgraph.begin(), matched_subgraph.end(),
              std::inserter(current_node_set_th, current_node_set_th.end()));
    set<int> NOT;
    if (!InitCandidate)
    {
        GetCandidates(label2adj_th, current_node_set_th, motif, graph, candidates_th);
        for (const auto &e : candidates_th)
        {
            motif_neighbor[initial_id].emplace_back(e);
        }
        candidates_th.clear();
    }
    if (!ORDER)
    {
        for (auto &new_subgraph_node : motif_neighbor[initial_id])
        {
            candidates_th.insert(new_subgraph_node);
        }
    }
    else
    {
        for (auto &new_subgraph_node : motif_neighbor[initial_id])
        {
            if (currentOrder[graph.labels[new_subgraph_node]] < vertexOrder[new_subgraph_node])
            {
                candidates_th.insert(new_subgraph_node);
            }
            else
            {
                NOT.insert(new_subgraph_node);
            }
        }
    }

    DFSMaximal_new(label2adj_th, label2cover_th, current_node_set_th, motif, graph, candidates_th,
                   NOT, maximal_motif_clique, initial_id, motif_neighbor, matched_subgraphs);
    return;
}

int GetRandomElement(const unordered_set<int> &elements)
{
    if (elements.empty())
        return -1;
    int id = rand() % elements.size();
    auto iter = elements.begin();
    while (id--)
        iter++;
    return *iter;
}

int GetRandomPivot(const set<int> &elements)
{
    if (elements.empty())
        return -1;
    int id = rand() % elements.size();
    auto iter = elements.begin();
    while (id--)
        iter++;
    return *iter;
}

int GetMaximumDegree(const set<int> &elements)
{
    if (elements.empty())
        return -1;
    int max_select;
    int max_size = 0;
    for (const int &e : elements)
    {
        if (graph.adjList[e].size() > max_size)
        {
            max_size = graph.adjList[e].size();
            max_select = e;
        }
    }
    return max_select;
}

int GetMaximumColor(const set<int> &elements)
{
    if (elements.empty())
        return -1;
    int max_select;
    int max_size = 0;
    extern vector<int> vertexColor;
    for (const int &e : elements)
    {
        if (vertexColor[e] > max_size)
        {
            max_size = vertexColor[e];
            max_select = e;
        }
    }
    return max_select;
}

int GetMaximumColor(const unordered_set<int> &elements)
{
    if (elements.empty())
        return -1;
    int max_select;
    int max_size = 0;
    extern vector<int> vertexColor;
    for (const int &e : elements)
    {
        if (vertexColor[e] > max_size)
        {
            max_size = vertexColor[e];
            max_select = e;
        }
    }
    return max_select;
}

int GetMaximumDegree(const unordered_set<int> &elements)
{
    if (elements.empty())
        return -1;
    int max_select;
    int max_size = 0;
    for (const int &e : elements)
    {
        if (graph.adjList[e].size() > max_size)
        {
            max_size = graph.adjList[e].size();
            max_select = e;
        }
    }
    return max_select;
}
// An improved pivot select method
inline int GetPivot(const unordered_set<int> &candidates_th, const map<int, map<int, set<int>>> &label2adj_th, const map<int,  set<int>> &label2cover_th, set<int> NOT, const Graph &graph)
{
    // current_node_set
	// candidates
	// label2adj
	// label2cover

    bool flag;
    bool cover_flag;
    set<int> superArray;

    // // Get the initial pruning sets, which means the maximize sets.
    if (NOT.size())
    {
        for (const auto &nid : NOT)
        {
            int label = graph.labels[nid];
            // advanced node expansion
            flag = false;
            cover_flag = true;
            int count = 0;

            for (const auto &elementAdj : label2adj_th.at(label))
            {
                bool sinle_flag = true;
                for (const auto &element : elementAdj.second)
                {
                    if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                    {
                        sinle_flag = false;
                        break;
                    }
                }
                if (sinle_flag)
                {
                    count += 1;
                }
                if (count > motif_label2nodes[label].size())
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                for (const auto &element : label2cover_th.at(label))
                {
                    if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                    {
                        cover_flag = false;
                        break;
                    }
                }
            }
            if (flag)
            {
                return nid;
            }
            else if (cover_flag)
            {
                return nid;
            }
        }
    }
    for (const int &nid : candidates_th)
    {
        int label = graph.labels[nid];

        flag = false;
        int count = 0;
        for (const auto &elementAdj : label2adj_th.at(label))
        {
            bool sinle_flag = true;
            for (const auto &element : elementAdj.second)
            {
                if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                {
                    sinle_flag = false;
                    break;
                }
            }
            if (sinle_flag)
            {
                count += 1;
            }
            if (count > motif_label2nodes[label].size())
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            for (const auto &element : label2cover_th.at(label))
            {
                if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                {
                    cover_flag = false;
                    break;
                }
            }
        }
        if (flag)
        {
            superArray.insert(nid);
        }
        else if (cover_flag)
        {
            superArray.insert(nid);
        }
    }
    if (superArray.empty())
    {
        return -1;
    }
    return GetMaximumDegree(superArray);
}

int Pick(const set<int> &current_node_set_th, const Graph &motif, const Graph &graph, const unordered_set<int> &candidates_th, const unordered_set<int> &early_checks,
         const set<int> &NOT)
{
    // current_node_set
	// candidates
	// label2adj
	// label2cover

    // try to find a node x which can not be included into the result set with element_in_not simultaneously.

    if (NOT.empty() || RandomSelection)
    {
        return GetRandomElement(candidates_th);
    }

    if (EarlyStopCheck(current_node_set_th, motif, graph, early_checks, NOT))
    {
        return -1;
    }
    return GetRandomElement(candidates_th);
}

bool EarlyStopCheck(const set<int> &current_node_set_th, const Graph &motif, const Graph &graph, const unordered_set<int> &candidates_th,
                    const set<int> &NOT)
{
    if (NOEARLYSTOPCHECK)
        return false;
    extern map<int, set<int>> motif_label2nodes;
    int num = 0;
    bool flag;
    int label;

    for (const auto &element_in_not : NOT)
    {
        num = 0;
        label = graph.labels[element_in_not];
        for (const auto &e : current_node_set_th)
        {
            if (graph.labels[e] == label)
            {
                flag = true;
                for (const auto &w : current_node_set_th)
                {
                    if (e != w && graph.adjList[e].find(w) != graph.adjList[e].end() && motif_labelAdj[graph.labels[e]].find(graph.labels[w]) != motif_labelAdj[graph.labels[e]].end() // only for undirected graph
                        && graph.adjList[element_in_not].find(w) == graph.adjList[element_in_not].end())
                    {
                        flag = false;
                        break;
                    }
                }
                if (!flag)
                    continue;
                for (const auto &w : candidates_th)
                {
                    if (e != w && graph.adjList[e].find(w) != graph.adjList[e].end() && motif_labelAdj[graph.labels[e]].find(graph.labels[w]) != motif_labelAdj[graph.labels[e]].end() // only for undirected graph
                        && graph.adjList[element_in_not].find(w) == graph.adjList[element_in_not].end())
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag)
                    num++;
            }
        }

        if (1 + num > motif_label2nodes[label].size())
            return true;
    }
    return false;
}

void PrintClique(int id, const set<int> &A, const string &output_path)
{
    if (output_path.empty())
        return;
    FILE *fout;
    fout = fopen(output_path.c_str(), "a");
    // int index = 0;
    if (!fout)
    {
        fout = fopen(output_path.c_str(), "a");
    }
    // int index = 0;
    // fprintf(fout, "The %d th MMC: ", (id));
    // fprintf(fout, "\n");
    for (const auto &e : A)
    {

        fprintf(fout, "\t%d", e);
    }
    fprintf(fout, "\n");
    fclose(fout);
}
// Obtain the pruning sets based on the selected pivot.
inline void GetPruningSets(int pivot, const set<int> current_node_set_th, const unordered_set<int> &candidates_th, map<int, map<int, set<int>>> label2adj_th, map<int,  set<int>> label2cover_th, unordered_set<int> &erase_sets, const Graph &graph)
{   
    // current_node_set
	// candidates
	// label2adj
	// label2cover

    bool flag;
    bool cover_flag;
    for (const auto &nid : candidates_th)
    {

        int label = graph.labels[nid];
        // advanced node expansion
        flag = false;
        cover_flag = true;
        int count = 0;
        bool motif_link = motif_labelAdj[label].find(graph.labels[pivot]) != motif_labelAdj[label].end();
        bool graph_link = graph.adjList[pivot].find(nid) != graph.adjList[pivot].end();
        if (motif_link && !graph_link)
        {
            continue;
        }
        if (!UseAP)
        {
            if (IsomorphismCheck(current_node_set_th, pivot, nid, motif, graph))
                erase_sets.insert(nid);
        }
        else
        {
            for (const auto &elementAdj : label2adj_th.at(label))
            {
                bool sinle_flag = true;
                for (const auto &element : elementAdj.second)
                {
                    if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                    {
                        sinle_flag = false;
                        break;
                    }
                }
                if (sinle_flag)
                {
                    count += 1;
                }
                if (count > motif_label2nodes[label].size())
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
            {
                for (const auto &element : label2cover_th.at(label))
                {
                    if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                    {
                        cover_flag = false;
                        break;
                    }
                }
            }
            // condition 1
            if (flag)
            {
                erase_sets.insert(nid);
            }
            // condition 2
            else if (cover_flag)
            {
                erase_sets.insert(nid);
            }
            else if (graph.labels[pivot] == label)
            {
                //   condition 3
                if (motif_label2nodes[label].size() == 1)
                {
                    erase_sets.insert(nid);
                }
            }
        }
    }
}
void UsePivotPruning(const set<int> &NOT, set<int> &current_node_set_th, unordered_set<int> &candidates_th, map<int, map<int, set<int>>> &label2adj_th, map<int,  set<int>> &label2cover_th, unordered_set<int> &candidates_copy, int choice, const Graph &graph)
{
    // current_node_set
	// candidates
	// label2adj
	// label2cover

    int pivot;
    unordered_set<int> erase_sets;
    set<int> pivots(NOT);
    for (auto &e : candidates_th)
    {
        pivots.insert(e);
    }
    switch (choice)
    {
    case 1:

        pivot = GetRandomPivot(pivots);
        break;
    case 2:
        pivot = 1;
        break;
    case 3:
        pivot = GetPivot(candidates_th, label2adj_th, label2cover_th, NOT, graph);
        break;
    default:
        break;
    }
    if (pivot == -1)
        return;
    set<int> new_set;
    set<int> cover_set;
    current_node_set_th.insert(pivot);
    int label = graph.labels[pivot];
    bool flag;
    bool cover_flag;
    for (const auto &e : current_node_set_th)
    {
        if (graph.adjList[pivot].find(e) != graph.adjList[pivot].end() && motif_labelAdj[label].find(graph.labels[e]) != motif_labelAdj[label].end())
        {

            label2adj_th[graph.labels[e]][e].insert(pivot);
            new_set.insert(e);
            label2cover_th[graph.labels[e]].insert(pivot);
            if (label2cover_th[label].find(e) == label2cover_th[label].end())
            {
                cover_set.insert(e);
                label2cover_th[label].insert(e);
            }
            if (label2adj_th[label].find(pivot) == label2adj_th[label].end())
            {

                label2adj_th[label][pivot].insert(e);
            }
            else if (label2adj_th[label][pivot].find(e) == label2adj_th[label][pivot].end())
            {
                label2adj_th[label][pivot].insert(e);
            }
        }
    }

    GetPruningSets(pivot, current_node_set_th, candidates_th, label2adj_th, label2cover_th, erase_sets, graph);
    current_node_set_th.erase(pivot);
    erase_sets.erase(pivot);
    // Recover
    if (label2cover_th[label].find(pivot) != label2cover_th[label].end())
        label2cover_th[label].erase(pivot);
    for (auto &e : label2cover_th[label])
    {
        label2cover_th[graph.labels[e]].erase(pivot);
    }
    for (auto &e : cover_set)
    {
        label2cover_th[label].erase(e);
    }
    label2adj_th[graph.labels[pivot]].erase(pivot);
    for (auto &e : new_set)
    {
        label2adj_th[graph.labels[e]][e].erase(pivot);
    }

    for (const auto &e : erase_sets)
        candidates_copy.erase(e);
    Pruning_vertices_sp += (candidates_th.size() - candidates_copy.size());
    All_vertices_sp += 1;
}
void DFSMaximal_new(map<int, map<int, set<int>>> &label2adj_th, map<int, set<int>> &label2cover_th, set<int> &current_node_set_th,
                    const Graph &motif, const Graph &graph, unordered_set<int> &candidates_th, set<int> &NOT,
                    vector<set<int>> &maximal_motif_clique, int initial_id, vector<vector<int>> &motif_neighbor, vector<vector<int>> &matched_subgraphs)
{
    // current_node_set
	// candidates
	// label2adj
	// label2cover
    if (MMC_LIMIT != -1 && maximal_motif_clique.size() > MMC_LIMIT)
        return;

    if (candidates_th.empty() && NOT.empty())
    {
        maximum_clique_size = max(maximum_clique_size, (int)current_node_set_th.size());
        maximal_motif_clique.push_back(current_node_set_th);
        mclique_size += current_node_set_th.size();

        // For debug only!
        // Print(current_node_set_th);
        // string output_path = "/home/yingli/work/mclique_offical_paper_dominate/output3.txt";
        // PrintClique(initial_id, current_node_set_th, output_path + ".txt");

        // cout<<"result size: "<< maximal_motif_clique.size()<<endl;
        // if (MMC_LIMIT != -1 && maximal_motif_clique.size() >= MMC_LIMIT) return;
        //  Print(current_node_set_th);
        return;
    }
    if (candidates_th.empty())
    {
        return;
    }
    set<int> NOT_new;
    unordered_set<int> candidates_new;
    unordered_set<int> candidates_copy(candidates_th);

    extern double use_pivot;
    unsigned long ticks = clock();

    // There are three pivot select methods
    // Default:3
    // 0. non pivot
    // 1. random
    // 2. maximum degree
    // 3. hybrid
    if (PIVOT != 0)
    {
        UsePivotPruning(NOT, current_node_set_th, candidates_th, label2adj_th, label2cover_th, candidates_copy, PIVOT, graph);
    }

    use_pivot += ((double)(clock() - ticks) / CLOCKS_PER_SEC);
    while (!candidates_copy.empty())
    {

        int new_node;
        extern double pick_time;
        double start = clock();
        new_node = Pick(current_node_set_th, motif, graph, candidates_copy, candidates_th, NOT);
        pick_time += ((double)(clock() - start) / CLOCKS_PER_SEC);

        if (new_node == -1)
        {
            return;
        }
        visited_vertices += 1;
        candidates_th.erase(new_node);
        current_node_set_th.insert(new_node);

        extern double update_candidates;
        ticks = clock();
        if (ORDER || !FindEndPointInSetTrieNode(current_node_set_th))
        {
            set<int> new_set;
            set<int> cover_set;
            int label = graph.labels[new_node];
            for (const auto &e : current_node_set_th)
            {
                if (graph.adjList[new_node].find(e) != graph.adjList[new_node].end() && motif_labelAdj[label].find(graph.labels[e]) != motif_labelAdj[label].end())
                {

                    label2adj_th[graph.labels[e]][e].insert(new_node);
                    label2cover_th[graph.labels[e]].insert(new_node);
                    if (label2cover_th[label].find(e) == label2cover_th[label].end())
                    {
                        cover_set.insert(e);
                        label2cover_th[label].insert(e);
                    }
                    new_set.insert(e);
                    if (label2adj_th[label].find(new_node) == label2adj_th[label].end())
                    {

                        label2adj_th[label][new_node].insert(e);
                    }
                    else if (label2adj_th[label][new_node].find(e) == label2adj_th[label][new_node].end())
                    {
                        // new_set.insert(e);
                        label2adj_th[label][new_node].insert(e);
                    }
                }
            }

            candidates_new.clear();
            NOT_new.clear();
            // Compute the new candidates_th set and the new NOT set after adding the current node.

            GetCandidates(current_node_set_th, new_node, label2adj_th, candidates_th, label2cover_th, motif,
                          graph, NOT, NOT_new, candidates_new);
            update_candidates += ((double)(clock() - ticks) / CLOCKS_PER_SEC);
            if (!candidates_new.empty() || NOT_new.empty())
                DFSMaximal_new(label2adj_th, label2cover_th, current_node_set_th, motif, graph, candidates_new, NOT_new,
                               maximal_motif_clique, initial_id, motif_neighbor, matched_subgraphs);

            // Recover label2cover_th.
            if (label2cover_th[label].find(new_node) != label2cover_th[label].end())
                label2cover_th[label].erase(new_node);
            for (auto e : label2cover_th[label])
            {
                label2cover_th[graph.labels[e]].erase(new_node);
            }
            for (auto e : cover_set)
            {
                label2cover_th[label].erase(e);
            }
            // Recover label2adj_th.
            label2adj_th[graph.labels[new_node]].erase(new_node);
            for (auto e : new_set)
            {
                label2adj_th[graph.labels[e]][e].erase(new_node);
            }
        }
        current_node_set_th.erase(new_node);
        NOT.insert(new_node);
        candidates_copy.erase(new_node);
    }
}
void WriteSubgraph(vector<vector<int>> &subgraphs, string &output_path)
{
    ofstream fout(output_path);
    fout << subgraphs.size() << endl;
    for (int i = 0; i < subgraphs.size(); ++i)
    {
        fout << "Mapping " << i + 1 << " =>";
        for (int j = 0; j < subgraphs[i].size(); j++)
        {
            fout << " " << j << ":" << subgraphs[i][j];
        }
        fout << endl;
    }

    fout.close();
}

void CheckSubgraphMatchingAlgorithm(Graph &graph, Graph &motif, vector<vector<int>> &subgraphs)
{
    int count = 0;
    for (auto &e : subgraphs)
    {
        if (temp_graph == NULL)
        {
            temp_graph = new Graph();
        }
        else
        {
            temp_graph->clear();
        }
        set<int> nodes(e.begin(), e.end());
        copyGraph(*temp_graph, graph, nodes);
        temp_matched_subgraphs.clear();
        GetMatchedSubgraph(motif, *temp_graph, temp_matched_subgraphs, true);
        if (!temp_matched_subgraphs.empty())
            count++;
    }
    cout << "Exact subgraph match: " << count << endl;
}

// Checks whether two sets A and B are the same.

bool Check(set<int> &A, set<int> &B)
{
    if (A.size() != B.size())
        return false;
    for (const auto &e : A)
    {
        if (B.find(e) == B.end()) // cannot find *it in B
            return false;
    }
    return true;
}

// Check whether A is a subset of B

bool CheckSubset(const vector<int> &A, const set<int> &B)
{
    if (A.size() > B.size())
        return false;
    for (const auto &e : A)
    {
        if (B.find(e) == B.end()) // cannot find e in B
            return false;
    }
    return true;
}

template <typename T>
void Print(const T &A)
{
    for (auto const &a : A)
    {
        cout << " " << a;
    }
    cout << endl;
}

void Print(const vector<vector<int>> &subgraphs)
{
    for (int i = 0; i < subgraphs.size(); i++)
    {
        cout << "Solution " << i + 1 << ": " << endl;
        for (int j = 0; j < subgraphs[i].size(); j++)
        {
            cout << j << ". " << subgraphs[i][j] << endl;
        }
    }
}

void PrintMap(const map<int, set<int>> &mp)
{
    for (auto e : mp)
    {
        cout << "label " << e.first << ":";
        for (auto u : e.second)
        {
            cout << " " << u;
        }
        cout << endl;
    }
}
