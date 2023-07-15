#include "maximal-motif-clique.h"
#include <sys/time.h>

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
        if (temp_graph == NULL)
        {
            temp_graph = new Graph();
        }
        else
        {
            temp_graph->clear();
        }
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

bool IsomorphismCheck(const set<int> &U, const int u, const int v, const Graph &motif, const Graph &graph)
{
    extern map<int, set<int>> motif_label2nodes;
    extern set<int> pre_IsoCheck_set;
    extern int current_u;
    extern map<int, set<int>> cand_label2nodes;
    extern set<int> label_set;

    for (auto e : cand_label2nodes)
        e.second.clear();
    cand_label2nodes.clear();
    label_set.clear();

    GetLabel2NodesMap(u, U, graph.labels, cand_label2nodes, label_set);

    bool Flag = true;
    extern map<int, set<int>> cands;
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

    if (Flag == false)
    {
        //        if (coverset.size() + 1 > motif_label2nodes[graph.labels[v]].size()) {
        //            cout << "Error!!!!" << endl;
        //            motif.Print();
        //            cout<< u<<"  "<<graph.labels[u]<<endl;
        //            cout<< v<<"  "<<graph.labels[v]<<endl;
        //            cout<<"Print the graph:\n";
        //            for (const auto x: U) {
        //                for (const auto w:U) {
        //                    if (x != w && x < w) {
        //                        if (graph.adjList[x].find(w) != graph.adjList[x].end()) {
        //                            printf("%d\t%d\n", x, w);
        //                        }
        //                    }
        //                }
        //            }
        //
        //            for (const auto w:U) {
        //                if (graph.adjList[v].find(w) != graph.adjList[v].end()) {
        //                    printf("%d\t%d\n", v, w);
        //                }
        //            }
        //
        //            for(auto e: U){
        //                cout<<e<<" "<<graph.labels[e]<<endl;
        //            }
        //
        //            for(auto e: coverset){
        //                cout<<e<<" "<<endl;
        //            }
        //            exit(0);
        //        }
    }
    return Flag;
}

void GetCandidates(const map<int, map<int, set<int>>> &label2adj, const set<int> &current_node_set,
                   const Graph &motif, const Graph &graph, set<int> &candidates)
{
    extern bool *IsVisited;
    memset(IsVisited, 0, sizeof(bool) * graph.nodeNum);
    for (auto e : current_node_set)
        IsVisited[e] = true;
    bool flag;
    if (!directed)
    {
        for (auto id : current_node_set)
        {
            for (auto neighbor : graph.adjList[id])
            {
                // if(candidates.size() > CandidateLimit) return;
                int label = graph.labels[neighbor];
                if (!IsVisited[neighbor])
                {
                    IsVisited[neighbor] = true;
                    // check label is in label2adj
                    if (label2adj.find(label) != label2adj.end())
                    {

                        flag = false;
                        int count = 0;
                        for (const auto elementAdj : label2adj.at(label))
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
                            if (IsomorphismCheck(current_node_set, neighbor, motif, graph))
                                candidates.insert(neighbor);
                        }
                        else if (flag || IsomorphismCheck(current_node_set, neighbor, motif, graph))
                            candidates.insert(neighbor);
                    }
                }
            }
        }
    }
    else
    {
        for (auto id : current_node_set)
        {
            for (auto neighbor : graph.InAdjList[id])
            {
                // if(candidates.size() > CandidateLimit) return;
                int label = graph.labels[neighbor];
                if (!IsVisited[neighbor])
                {
                    IsVisited[neighbor] = true;
                    // check label is in label2adj
                    if (label2adj.find(label) != label2adj.end())
                    {
                        // check nid is connected to all nodes in label2adj[label]
                        if (IsomorphismCheck(current_node_set, neighbor, motif, graph))
                            candidates.insert(neighbor);
                    }
                }
            }

            for (auto neighbor : graph.OutAdjList[id])
            {
                // if(candidates.size() > CandidateLimit) return;
                int label = graph.labels[neighbor];
                if (!IsVisited[neighbor])
                {
                    IsVisited[neighbor] = true;
                    // check label is in label2adj
                    if (label2adj.find(label) != label2adj.end())
                    {
                        // check nid is connected to all nodes in label2adj[label]
                        if (IsomorphismCheck(current_node_set, neighbor, motif, graph))
                            candidates.insert(neighbor);
                    }
                }
            }
        }
    }
    return;
}

void GetCandidates(const set<int> &current_node_set, const int new_node, const map<int, map<int, set<int>>> &label2adj,
                   set<int> &candidates, const Graph &motif, const Graph &graph, const set<int> &NOT, set<int> &NOT_new,
                   set<int> &new_candidate_set)
{
    bool flag;
    // generate new_candidates
    for (const auto &nid : candidates)
    {

        int label = graph.labels[nid];
        // advanced node expansion
        flag = false;
        if (!directed)
        {
            int count = 0;
            for (const auto elementAdj : label2adj.at(label))
            {
                bool sinle_flag = true;
                for (const auto element : elementAdj.second)
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

        if (WithoutIsoCheckPruning || directed)
        {
            if (IsomorphismCheck(current_node_set, new_node, nid, motif, graph))
                new_candidate_set.insert(nid);
        }
        else if (flag || IsomorphismCheck(current_node_set, new_node, nid, motif, graph))
        {
            new_candidate_set.insert(nid);
        }
    }
    // generate NOT_new
    for (const auto &nid : NOT)
    {
        int label = graph.labels[nid];
        //  advanced node expansion
        flag = false;
        if (!directed)
        {
            int count = 0;
            for (const auto elementAdj : label2adj.at(label))
            {
                bool sinle_flag = true;
                for (const auto element : elementAdj.second)
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

        if (WithoutIsoCheckPruning || directed)
        {
            if (IsomorphismCheck(current_node_set, new_node, nid, motif, graph))
                NOT_new.insert(nid);
        }
        else if (flag || IsomorphismCheck(current_node_set, new_node, nid, motif, graph))
        {
            NOT_new.insert(nid);
        }
    }
}

// void InitializeSetTrieNodeCandidates(set<int> &current_node_set, int cand_size, set<SetTrieNode *> &stn_cands) {
//     extern SetTrieNode *root;
//     stn_cands.insert(root);
//     SetTrieNode *temp;
//     set<SetTrieNode *> temp_set;
//     int index = 0;
//     set<SetTrieNode *> *pointer[2] = {&stn_cands, &temp_set};
//     for (const auto &e : current_node_set) {
//         pointer[1 - index]->clear();
//         for (const auto &stn : (*pointer[index])) {
//             temp = stn->GetChild(e);
//             if (temp != NULL) {
//                 pointer[1 - index]->insert(temp);
//             }
//             pointer[1 - index]->insert(stn);
//         }
//         index = 1 - index;
//     }
//     stn_cands = *(pointer[index]);
// }

void FindMaximalMotifClique(map<int, map<int, set<int>>> &label2adj, const vector<int> &matched_subgraph,
                            const Graph &motif, const Graph &graph, set<int> &current_node_set,
                            set<int> &candidates, vector<set<int>> &maximal_motif_cliques)
{
    // cout<<"In FindMaximalMotifClique()"<<endl;

    // Create Label2Adj structure
    for (auto from : matched_subgraph)
        for (auto to : matched_subgraph)
        {
            if (from < to && graph.adjList[from].find(to) != graph.adjList[from].end() &&
                motif_labelAdj[graph.labels[from]].find(graph.labels[to]) != motif_labelAdj[graph.labels[from]].end())
            {
                if (label2adj.find(graph.labels[from]) == label2adj.end())
                { // label[from] does not exist in label2adj
                    label2adj[graph.labels[from]].insert(make_pair(from, set<int>()));
                }
                label2adj[graph.labels[from]][from].insert(to);

                if (label2adj.find(graph.labels[to]) == label2adj.end())
                { // label[to] does not exist in label2adj
                  // label2adj.insert(make_pair(graph.labels[to], set<int>()));
                    label2adj[graph.labels[to]].insert(make_pair(to, set<int>()));
                }
                label2adj[graph.labels[to]][to].insert(from);
            }
        }

#ifdef DEBUG
    cout << "Initial label2adj" << endl;
    PrintMap(label2adj);
#endif

    std::copy(matched_subgraph.begin(), matched_subgraph.end(),
              std::inserter(current_node_set, current_node_set.end()));

    GetCandidates(label2adj, current_node_set, motif, graph, candidates);
#ifdef DEBUG
    cout << "candidates: ";
    Print(candidates);
#endif

    // set<SetTrieNode *> stn_cands;
    set<int> NOT;
    //    if(!candidates.empty())
    //        InitializeSetTrieNodeCandidates(current_node_set,
    //                candidates.size(), stn_cands);
    if (APPROXIMATION)
    {
        if (GETEXACTMMC)
        {
            // DFSMaximal_Approx(label2adj, current_node_set, motif, graph, candidates,
            //   NOT, -1, maximal_motif_cliques);
        }
        else
        {
            extern vector<double> approx_improvement;

            set<int> candidates_temp(candidates.begin(), candidates.end());

            struct timeval start, end;
            long long mtime_exact, mtime_approx, seconds, useconds;
            gettimeofday(&start, NULL);

            DFSMaximal(label2adj, current_node_set, motif, graph, candidates,
                       NOT, maximal_motif_cliques);

            gettimeofday(&end, NULL);
            seconds = end.tv_sec - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;
            mtime_exact = seconds * 1000000 + useconds;

            NOT.clear();
            vector<set<int>> temp;

            gettimeofday(&start, NULL);

            // DFSMaximal_Approx(label2adj, current_node_set, motif, graph, candidates_temp,
            //                   NOT, -1, temp);

            gettimeofday(&end, NULL);
            seconds = end.tv_sec - start.tv_sec;
            useconds = end.tv_usec - start.tv_usec;
            mtime_approx = seconds * 1000000 + useconds;
            if (mtime_approx > 100)
                approx_improvement.push_back(mtime_exact / mtime_approx);
        }
    }
    else
    {
        DFSMaximal(label2adj, current_node_set, motif, graph, candidates,
                   NOT, maximal_motif_cliques);
    }
    return;
}

int GetRandomElement(const set<int> &elements)
{
    if (elements.empty())
        return -1;
    int id = rand() % elements.size();
    auto iter = elements.begin();
    while (id--)
        iter++;
    return *iter;
}

int Pick(const set<int> &current_node_set, const Graph &motif, const Graph &graph, const set<int> &candidates,
         const set<int> &NOT)
{
    // try to find a node x which can not be included into the result set with element_in_not simultaneously.

    extern map<int, set<int>> motif_label2nodes;

    if (NOT.empty() || RandomSelection)
    {
        return GetRandomElement(candidates);
    }

    int element_in_not = GetRandomElement(NOT);
    //    int degree=100000, element_in_not, temp_degree;
    //    for(const auto &e:NOT){
    //        temp_degree=0;
    //        for(const auto &w :candidates){
    //            if(graph.adjList[e].find(w)!=graph.adjList[e].end()) temp_degree++;
    //        }
    //        for(const auto &w :current_node_set){
    //            if(graph.adjList[e].find(w)!=graph.adjList[e].end()) temp_degree++;
    //        }
    //        if(temp_degree<degree){
    //            degree=temp_degree;
    //            element_in_not = e;
    //        }
    //    }

    int label = graph.labels[element_in_not];
    for (const auto &e : candidates)
    {
        if (!IsomorphismCheck(current_node_set, e, element_in_not, motif, graph))
        {
            return e;
        }
    }

    if (EarlyStopCheck(current_node_set, motif, graph, candidates, NOT))
    {
        extern int early_stop_cnt;
        early_stop_cnt += 1;
        return -1;
    }
    else
        return GetRandomElement(candidates);
}

// For test
// bool ContainPreviousMatchedSubgraph(set<int> &current_node_set) {
//    extern int global;
//    extern vector<vector<int> > subgraphs;
//    for (int i = 0; i < global; ++i) {
//        if (CheckSubset(subgraphs[i], current_node_set))
//            return true;
//    }
//    return false;
//}

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

bool FindEndPointInSetTrieNode(const set<int> &current_node_set)
{
    extern int motif_node_size;
    extern SetTrieNode *root;
    set<SetTrieNode *> temp_set;
    set<SetTrieNode *> stn_cands;
    SetTrieNode *temp;
    stn_cands.insert(root);
    int n = current_node_set.size();
    for (const auto &e : current_node_set)
    {
        n--;
        temp_set.clear();
        for (const auto &stn : stn_cands)
        {
            temp = stn->GetChild(e);
            if (temp != NULL)
            {
                if (temp->IsEndPoint())
                    return true;
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

// void RemoveImpossibleSetTrieNode(set<SetTrieNode*> &stn_cands, int cand_size) {
//     for (auto it = stn_cands.begin(); it != stn_cands.end();) {
//         if ((*it)->GetHeight() + cand_size < motif_node_size) {
//             stn_cands.erase(it++);
//         } else {
//             it++;
//         }
//     }
// }

bool EarlyStopCheck(const set<int> &current_node_set, const Graph &motif, const Graph &graph, const set<int> &candidates,
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
        for (const auto &e : current_node_set)
        {
            if (graph.labels[e] == label)
            {
                flag = true;
                for (const auto &w : current_node_set)
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
                for (const auto &w : candidates)
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

int getExactMMC(set<int> curr_node_set, set<int> candidates, const Graph &motif, const Graph &graph)
{

    while (!candidates.empty())
    {
        int new_node = GetRandomElement(candidates);

        candidates.erase(new_node);

        if (IsomorphismCheck(curr_node_set, new_node, motif, graph))
        {
            curr_node_set.insert(new_node);
        }
    }
    return curr_node_set.size();
}

void DFSMaximal_Approx(map<int, set<int>> &label2adj, set<int> &current_node_set,
                       const Graph &motif, const Graph &graph, set<int> &candidates, set<int> &NOT, int step,
                       vector<set<int>> &maximal_motif_cliques)
{
    if (MMC_LIMIT != -1 && maximal_motif_cliques.size() > MMC_LIMIT)
        return;
    // cout << "In DfsMaximal_Approx******" << endl;

    bool flag;
    if ((candidates.empty() || step >= OMEGA) && NOT.empty())
    {

        double exact_size;
        if (GETEXACTMMC && !candidates.empty())
        {
            exact_size = getExactMMC(current_node_set, candidates, motif, graph);
        }
        else
        {
            exact_size = candidates.size() + current_node_set.size();
        }
        if (!candidates.empty())
        {

            current_node_set.insert(candidates.begin(), candidates.end());
            //            for (auto e: candidates) {
            //                current_node_set.insert(e);
            //            }
        }

        //        if (DUPLICATION) {
        //            /**Naive duplication checking. After implementing FindEndPointInSetTrieNode, we can remove duplication checking here.*/
        //            flag = true;
        //            for (int i = 0; i < maximal_motif_cliques.size(); ++i) {
        //                if (Check(maximal_motif_cliques[i], current_node_set)) {
        //                    flag = false;
        //                    duplicated_answer_num++;
        //                    break;
        //                }
        //            }
        //            if (!flag) return;
        //            //assert(flag == true);
        //        }

        maximum_clique_size = max(maximum_clique_size, (int)current_node_set.size());
        maximal_motif_cliques.push_back(current_node_set);

        extern vector<double> real_approx_ratio;
        extern vector<double> real_failure_prob;
        extern double R;

        double approx_ratio_value = current_node_set.size() * 1.0 / exact_size;

        real_approx_ratio.push_back(approx_ratio_value);
        if (approx_ratio_value > R)
        {
            real_failure_prob.push_back(1);
        }
        else
        {
            real_failure_prob.push_back(0);
        }

        if (!candidates.empty())
        {
            for (auto e : candidates)
            {
                current_node_set.erase(e);
            }
        }
        // cout << "result size: " << maximal_motif_cliques.size() << endl;
        return;
    }
    // if (candidates.empty()) return;

    set<int> NOT_new;
    set<int> candidates_new;

    while (!candidates.empty())
    {
        // cout<<"Here "<<candidates.size()<<endl;
        if (MMC_LIMIT != -1 && maximal_motif_cliques.size() >= MMC_LIMIT)
            return;

        if (!NOT.empty() && EarlyStopCheck(current_node_set, motif, graph, candidates, NOT))
        {
            return;
        }
        int new_node = GetRandomElement(candidates);

        candidates.erase(new_node);

        bool IsoCheckForU = false;
        int label = graph.labels[new_node];
        flag = true;
        if (!directed && !WithoutIsoCheckPruning)
        {
            for (const auto element : label2adj.at(label))
            {
                if (graph.adjList[new_node].find(element) == graph.adjList[new_node].end())
                {
                    flag = false;
                    break;
                }
            }
        }
        if (WithoutIsoCheckPruning || directed)
        {
            if (step == -1 || IsomorphismCheck(current_node_set, new_node, motif, graph))
                IsoCheckForU = true;
        }
        else if (step == -1 || flag || IsomorphismCheck(current_node_set, new_node, motif, graph))
        {
            IsoCheckForU = true;
        }
        if (IsoCheckForU)
        {
            current_node_set.insert(new_node);
            if (DUPLICATION || !FindEndPointInSetTrieNode(current_node_set))
            {
                // Update label2adj.
                set<int> new_set;
                int label = graph.labels[new_node];
                for (auto e : current_node_set)
                {
                    if (graph.adjList[new_node].find(e) != graph.adjList[new_node].end() && motif_labelAdj[label].find(graph.labels[e]) != motif_labelAdj[label].end())
                    {
                        label2adj[graph.labels[e]].insert(new_node);
                        if (label2adj[label].find(e) == label2adj[label].end())
                        {
                            new_set.insert(e);
                            label2adj[label].insert(e);
                        }
                    }
                }

                candidates_new.clear();
                NOT_new.clear();

                candidates_new.insert(candidates.begin(), candidates.end());

                // generate NOT_new
                for (const auto &nid : NOT)
                {
                    int label = graph.labels[nid];
                    flag = true;
                    if (!directed)
                    {
                        for (const auto element : label2adj.at(label))
                        {
                            if (graph.adjList[nid].find(element) == graph.adjList[nid].end())
                            {
                                flag = false;
                                break;
                            }
                        }
                    }
                    if (WithoutIsoCheckPruning || directed)
                    {
                        if (IsomorphismCheck(current_node_set, new_node, nid, motif, graph))
                            NOT_new.insert(nid);
                    }
                    else if (flag || IsomorphismCheck(current_node_set, new_node, nid, motif, graph))
                    {
                        NOT_new.insert(nid);
                    }
                }
                if (step != -1)
                {
                    DFSMaximal_Approx(label2adj, current_node_set, motif, graph, candidates_new, NOT_new, step + 1,
                                      maximal_motif_cliques);
                }
                else
                {
                    DFSMaximal_Approx(label2adj, current_node_set, motif, graph, candidates_new, NOT_new, 1,
                                      maximal_motif_cliques);
                }

                // Recover label2adj.
                if (label2adj[label].find(new_node) != label2adj[label].end())
                    label2adj[label].erase(new_node);
                for (auto e : label2adj[label])
                {
                    label2adj[graph.labels[e]].erase(new_node);
                }
                for (auto e : new_set)
                {
                    label2adj[label].erase(e);
                }
            }
            NOT.insert(new_node);
            current_node_set.erase(new_node);
        }
        else
        {
            step = 0;
        }
    }
}

void DFSMaximal(map<int, map<int, set<int>>> &label2adj, set<int> &current_node_set,
                const Graph &motif, const Graph &graph, set<int> &candidates, set<int> &NOT,
                vector<set<int>> &maximal_motif_cliques)
{
    if (MMC_LIMIT != -1 && maximal_motif_cliques.size() > MMC_LIMIT)
        return;
    // cout << "In DfsMaximal******" << endl;

    if (candidates.empty() && NOT.empty())
    {
        if (DUPLICATION)
        {
            /**Naive duplication checking. After implementing FindEndPointInSetTrieNode, we can remove duplication checking here.*/
            bool flag = true;
            for (int i = 0; i < maximal_motif_cliques.size(); ++i)
            {
                if (Check(maximal_motif_cliques[i], current_node_set))
                {
                    // maximal_motif_cliques[i] == current_node_set
                    flag = false;
                    duplicated_answer_num++;
                    break;
                }
            }
            if (!flag)
                return;
            // assert(flag == true);
        }

        maximum_clique_size = max(maximum_clique_size, (int)current_node_set.size());
        maximal_motif_cliques.push_back(current_node_set);
        // Print(current_node_set);
        // cout<<"result size: "<< maximal_motif_cliques.size()<<endl;
        // if (MMC_LIMIT != -1 && maximal_motif_cliques.size() >= MMC_LIMIT) return;
        return;
    }
    if (candidates.empty())
    {
        // cout << "NOT is not empty size = " << NOT.size() << endl;
        return;
    }

    set<int> NOT_new;
    set<int> candidates_new;

    // Pruning for the maximum problem
    // if (current_node_set.size() + candidates.size() < maximum_clique_size) return;

    while (!candidates.empty())
    {
        if (MMC_LIMIT != -1 && maximal_motif_cliques.size() >= MMC_LIMIT)
            return;
        int new_node = Pick(current_node_set, motif, graph, candidates, NOT);
        // new_node equals -1 means all nodes are "containable" with at least one node in NOT (i.e., the first element of NOT)
        if (new_node == -1)
            return;
        candidates.erase(new_node);
        current_node_set.insert(new_node);

        // FindEndPointInSetTrieNode will return false if no end point is found.
        // if (!FindEndPointInSetTrieNode(new_node, current_node_set, stn_cands, candidates_new.size(), stn_cands_new)) {
        // new_set means
        if (DUPLICATION || !FindEndPointInSetTrieNode(current_node_set))
        {
            // Update label2adj.
            set<int> new_set;
            int label = graph.labels[new_node];
            for (const auto &e : current_node_set)
            {
                if (graph.adjList[new_node].find(e) != graph.adjList[new_node].end() && motif_labelAdj[label].find(graph.labels[e]) != motif_labelAdj[label].end())
                {

                    label2adj[graph.labels[e]][e].insert(new_node);
                    new_set.insert(e);
                    if (label2adj[label].find(new_node) == label2adj[label].end())
                    {

                        label2adj[label][new_node].insert(e);
                    }
                    else if (label2adj[label][new_node].find(e) == label2adj[label][new_node].end())
                    {
                        // new_set.insert(e);
                        label2adj[label][new_node].insert(e);
                    }
                }
            }

            candidates_new.clear();
            NOT_new.clear();

            // Compute the new candidates set and the new NOT set after adding the current node.
            GetCandidates(current_node_set, new_node, label2adj, candidates, motif,
                          graph, NOT, NOT_new, candidates_new);
            if (!candidates_new.empty() || NOT_new.empty())
                DFSMaximal(label2adj, current_node_set, motif, graph, candidates_new, NOT_new,
                           maximal_motif_cliques);

            // Recover label2adj.
            // if (label2adj[label].find(new_node) != label2adj[label].end())
            //     label2adj[label].erase(new_node);
            // for (auto adj : label2adj[label])
            // {
            //     int label = adj.first;
            //     for (auto &e : adj.second)
            //     {
            //         label2adj[label][e].erase(new_node);
            //     }
            // }
            label2adj[graph.labels[new_node]].erase(new_node);
            for (auto e : new_set)
            {
                label2adj[graph.labels[e]][e].erase(new_node);
            }
        }
        current_node_set.erase(new_node);

        NOT.insert(new_node);
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
