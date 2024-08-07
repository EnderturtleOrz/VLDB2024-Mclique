/*
 * File:   main.cpp
 * Author: zyl
 *
 *
 *
 */

#include <cstdio>
#include <cstring>
#include "SetTrieNode.h"
#include "subgraph_match.h"
#include "global-variable.h"
#include "maximal-motif-clique.h"
#include "./include/vf3_impl.hpp"
#include "small-graph.h"
#include <sstream>
#include <sys/time.h>
#include <cmath>

using namespace std;

unsigned int split(const string &txt, vector<string> &strs, char ch)
{
    // this is the general case
    size_t pos = txt.find(ch);
    size_t initialPos = 0;
    strs.clear();
    // Decompose statement
    while (pos != string::npos)
    {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = txt.find(ch, initialPos);
    }
    // Add the last one
    strs.push_back(txt.substr(initialPos, min(pos, txt.size()) - initialPos));
    // return the size of the vector
    return int(strs.size());
}

void ReadId2Name(const string &name2id_path)
{
    ifstream ifile;
    ifile.open(name2id_path);
    string str, name;
    int id;

    while (getline(ifile, str))
    {
        vector<string> v;
        split(str, v, '\t');
        name = v[0];
        char *pEnd;
        id = strtol(v[1].c_str(), &pEnd, 10);
        id2name[id] = name;
    }
    ifile.close();
    ifile.clear();
}

void PrintNodeMeaning(const set<int> &p, const Graph &graph, const string &name2id_path)
{
    // If it is the first time to invoke this func., we need to load the name2id file of the DBLP dataset.
    if (id2name.empty())
    {
        ReadId2Name(name2id_path);
    }
    // Output the name of authors
    for (const auto &e : p)
    {
        cout << "id=" << e << ": " << graph.labels[e] << "\t" << id2name[e] << endl;
    }
}

void PrintiAnalysis(const int &index, const int &sp_size, const int &all_size, const string &output_path)
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

    fprintf(fout, "The %d th MMC: ", (index));
    fprintf(fout, "   pruning size by sp: %d | all tree nodes: %d \n", (sp_size), (all_size));

    // fprintf(fout, "   compute time is: %lld ms \n", (time));
    fclose(fout);
}
void PrintResults(const vector<set<int>> &result, const string &output_path, const Graph &graph,
                  const string &name2id_path)
{

    if (name2id_path.empty())
        return;
    FILE *fout;
    fout = fopen(output_path.c_str(), "w");
    int index = 0;

    for (const auto &e : result)
    {

        // if(e.size() < 6 ) continue;
        index++;

        map<int, int> label_count;
        for (const auto u : e)
        {
            if (label_count.find(graph.labels[u]) == label_count.end())
            {
                label_count[graph.labels[u]] = 1;
            }
            else
                label_count[graph.labels[u]]++;
        }

        //        bool flag = true;
        //        for (auto x : label_count) {
        //            if (x.second < 3) {
        //                flag = false;
        //                break;
        //            }
        //        }
        //        if (!flag) continue;

        fprintf(fout, "The %d th MMC: ", (index));
        fprintf(fout, "num_nodes: %d", int(e.size()));
        for (const auto u : e)
        {
            fprintf(fout, "\t%d", u);
        }
        fprintf(fout, "\n");
        if (id2name.empty())
        {
            ReadId2Name(name2id_path);
        }
        fprintf(fout, "id\tproduct-name\ttype\n");
        for (const auto u : e)
        {
            fprintf(fout, "%d\t%s\t%d\n", u, id2name[u].c_str(), graph.labels[u]);
        }
        fprintf(fout, "Print the graph:\n");
        for (const auto u : e)
        {
            for (const auto w : e)
            {
                if (u != w && u < w)
                {
                    if (!directed)
                    {
                        if (graph.adjList[u].find(w) != graph.adjList[u].end())
                        {
                            fprintf(fout, "%d\t%d\n", u, w);
                        }
                    }
                    else
                    {
                        if (graph.InAdjList[u].find(w) != graph.InAdjList[u].end())
                        {
                            fprintf(fout, "%d\t%d\n", w, u);
                        }

                        if (graph.OutAdjList[u].find(w) != graph.OutAdjList[u].end())
                        {
                            fprintf(fout, "%d\t%d\n", u, w);
                        }
                    }
                }
            }
        }
    }
    fclose(fout);
}

void PrintResults(const int &index, const int &size, const string &output_path, const long long &time)
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

    fprintf(fout, "The %d th MMC: ", (index));
    fprintf(fout, "   motif size: %d", (size));

    fprintf(fout, "   compute time is: %lld ms \n", (time));
    fclose(fout);
}

void GenerateMotifImpl(const Graph &g, int motif_size, Graph &new_motif)
{
    int round = 0;
    set<int> curr_node_set;
    vector<int> curr_node_list;
    while (round < 200)
    {
        int id = int(rand() % g.nodeNum);
        // if(g.labels[id] ==3) continue;
        curr_node_set.clear();
        curr_node_list.clear();
        curr_node_set.insert(id);
        curr_node_list.push_back(id);
        int second_tier_round = 0;
        while (curr_node_set.size() < motif_size && second_tier_round < 100)
        {
            second_tier_round++;
            int x = curr_node_list[int(rand() % curr_node_list.size())];
            if (g.adjList[x].size() == 0)
                continue;
            int l = int(rand() % g.adjList[x].size());
            auto it = g.adjList[x].begin();
            while (l--)
                it++;
            if (curr_node_set.find(*it) != curr_node_set.end())
            {
                continue;
            }
            else
            {
                // if(g.labels[*it] ==3) continue;
                curr_node_set.insert(*it);
                curr_node_list.push_back(*it);
            }
        }
        if (curr_node_list.size() == motif_size)
            break;
        round++;
    }

    if (curr_node_list.size() != motif_size)
    {
        //        cout<<"ERROR"<<endl;
        //        exit(0);
    }

    // store curr_node_set to motif.
    new_motif.nodeNum = curr_node_set.size();
    // assert(motif_size == curr_node_set.size());
    new_motif.adjList.resize(curr_node_set.size());
    for (int i = 0; i < curr_node_list.size(); ++i)
    {
        for (int j = 0; j < curr_node_list.size(); ++j)
        {
            if (i == j)
                continue;
            if (g.adjList[curr_node_list[i]].find(curr_node_list[j]) != g.adjList[curr_node_list[i]].end())
            {
                new_motif.adjList[i].insert(j);
                new_motif.edges.push_back(EDGE(i, j));
            }
        }
    }
    new_motif.edgeNum = new_motif.edges.size();
    for (int i = 0; i < curr_node_list.size(); ++i)
    {
        new_motif.labels.push_back(g.labels[curr_node_list[i]]);
    }
}

bool cmp(Graph A, Graph B)
{
    return A.edgeNum > B.edgeNum;
}
void WriteMotifsToFile(string output_path, vector<Graph> &motifs)
{
    ofstream ofile;
    ofile.open(output_path);
    // sort(motifs.begin(), motifs.end(),cmp);

    //    int m_size = 0;
    //    for(auto & motif : motifs){
    //        //if(motif.edgeNum/2 < min(motif.nodeNum+2, motif.nodeNum*(motif.nodeNum-1)/2)) break;
    //        m_size++;
    //    }

    ofile << motifs.size() << endl;

    for (int i = 0; i < motifs.size(); ++i)
    {
        // set<int> labels(motifs[i].labels.begin(), motifs[i].labels.end());
        // ofile<<"labels: "<<labels.size()<<endl;
        motifs[i].Print(ofile);
    }

    ofile.close();
    ofile.clear();
}

void GenerateMotif(string input_path, string output_path, int motif_size, int motif_num)
{
    Graph g;
    g.Read(input_path);
    vector<Graph> motifs;
    // map<Graph, int> motif_maps;
    for (int i = 0; i < motif_num;)
    {

        Graph new_motif;
        GenerateMotifImpl(g, motif_size, new_motif);
        if (motif_size != new_motif.nodeNum)
        {
            ++i;
            continue;
        }
        int label = new_motif.labels[0];
        set<int> labels(new_motif.labels.begin(), new_motif.labels.end());
        set<int> total_labels(g.labels.begin(), g.labels.end());
        // if(labels.size() == 1) continue;
        if (labels.size() < min(motif_size - 1, (int)total_labels.size()))
            continue;

        //        if(motif_size==4 || motif_size==5)
        //        {
        //            if(new_motif.edgeNum/2 < (motif_size*(motif_size-1)/2 + motif_size-1)/2 ) continue;
        //        }else
        //        if(motif_size >=4)
        //        {
        //            if(new_motif.edgeNum/2 < min(motif_size, motif_size*(motif_size-1)/2) ) continue;
        //        }
        ++i;

        bool flag = true;
        extern vector<vector<int>> temp_matched_subgraphs;
        for (auto M : motifs)
        {
            if (M.edgeNum != new_motif.edgeNum)
                continue;
            temp_matched_subgraphs.clear();
            GetMatchedSubgraph(M, new_motif, temp_matched_subgraphs, true);
            if (temp_matched_subgraphs.size() != 0)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            motifs.push_back(new_motif);
            // motif_maps[new_motif] += 1;
        }
    }
    cout << "Write motifs to the output file." << endl;
    // sort (motifs_.begin(), motifs_.end(), [&motif_maps](Graph &a, Graph &b) {
    //     return motif_maps[a] > motif_maps[b];
    // });
    stringstream ssize, snum;
    ssize << motif_size;
    snum << motif_num;
    string strsize = ssize.str(), strnum = snum.str();
    output_path = output_path + "_" + strsize + "_" + strnum + ".txt";
    // for (int i = 0; i <  20; i++) {
    //     motifs.emplace_back(motifs_[i]);
    // }
    WriteMotifsToFile(output_path, motifs);
    cout << "Done. (Write)" << endl;
}

void GetMatchedSubgraphVF3(ARGraph<int, Empty> &patt_graph, ARGraph<int, Empty> &targ_graph,
                           vector<vector<int>> &matched_subgraphs)
{
    // cout << "Start to enumerate all matched subgraphs using VF3!" << endl;
    vf3Impl(patt_graph, targ_graph, matched_subgraphs);
}

int ArgPos(char *str, int argc, char **argv)
{
    int a;
    for (a = 1; a < argc; a++)
        if (strcmp(str, argv[a]) == 0)
        {
            if (a == argc - 1)
            {
                printf("Argument missing for %s\n", str);
                exit(1);
            }
            return a;
        }
    return -1;
}

double GetDensity(Graph &graph, const set<int> &nodeset)
{
    int n = nodeset.size();
    int m = 0;
    for (auto e : nodeset)
    {
        for (auto w : nodeset)
            if (!directed)
            {
                if (e < w && graph.adjList[e].find(w) != graph.adjList[e].end())
                {
                    m++;
                }
            }
            else
            {
                if (e < w && graph.InAdjList[e].find(w) != graph.InAdjList[e].end())
                {
                    m++;
                }
                if (e < w && graph.OutAdjList[e].find(w) != graph.OutAdjList[e].end())
                {
                    m++;
                }
            }
    }
    return 2.0 * m / (n * (n - 1));
}

void ReadValidNodes(string species_path)
{
    extern set<int> validNodes;
    FILE *fin;
    fin = fopen(species_path.c_str(), "rb");
    if (fin == NULL)
    {
        printf("ERROR: network file not found!\n");
        exit(1);
    }
    int id;
    while (fscanf(fin, "%d", &id) != -1)
    {
        validNodes.insert(id);
    }
    fclose(fin);
}
// According to the motif, the graph is reduced using the following rules:
// Rule 1: Label Reduction
// Rule 2: Link Reduction
// Rule 3: Neighbor-Histogram Reduction
inline void ReduceGraph(const Graph &motif, Graph &graph)
{
    set<int> deleteVertex;
    std::map<int, int> sort2ori;
    std::map<int, map<int, int>> motif_label2cnt;
    set<int> labels(motif.labels.begin(), motif.labels.end());
    for (const int &label : labels)
    {
        if (motif_label2nodes[label].size() > 1)
            continue;

        for (const int &e : motif.adjList[*motif_label2nodes[label].begin()])
        {
            motif_label2cnt[label][motif.labels[e]] += 1;
        }
    }

    int num_vertices = graph.adjList.size();
    vector<int> new_labels;
    // Rule 1
    for (int i = 0; i < num_vertices; i++)
    {
        int label = graph.labels[i];

        if (labels.find(graph.labels[i]) == labels.end())
        {
            deleteVertex.insert(i);
            for (auto &j : graph.adjList[i])
            {
                graph.adjList[j].erase(i);
            }
        }
    }

    int cnt = 0;
    map<int, set<int>> nbMap;

    for (int i = 0; i < num_vertices; i++)
    {
        if (deleteVertex.find(i) == deleteVertex.end() && !graph.adjList[i].empty())
        {
            nbMap[i] = graph.adjList[i];
            sort2ori[i] = cnt++;
        }
    }

    cnt = 0;
    // Rule2
    vector<set<int>> newAdjList(nbMap.size());
    for (const auto &element : nbMap)
    {
        for (auto &e : element.second)
        {
            if (motif_labelAdj[graph.labels[element.first]].find(graph.labels[e]) == motif_labelAdj[graph.labels[element.first]].end())
                continue;
            newAdjList[cnt].insert(sort2ori[e]);
        }
        new_labels.emplace_back(graph.labels[element.first]);
        cnt++;
    }

    graph.adjList = newAdjList;
    graph.labels = new_labels;
    deleteVertex.clear();
    sort2ori.clear();

    // Rule3
    // We want to avoid the unvalid computing, since for some cases, the rule3 is not work
    for (int i = 0; i < nbMap.size(); i++)
    {

        int label = graph.labels[i];
        map<int, int> label2cnt;
        if (motif_label2nodes[label].size() == 1 || motif_label2Orbit[label])
        {
            set<int> stisfyLabelSet;
            bool flag = false;
            // Collect the Histogram information
            for (const int &ner : graph.adjList[i])
            {
                if (stisfyLabelSet.size() == motif_label2cnt[label].size())
                {
                    flag = false;
                    break;
                }
                int label_ner = graph.labels[ner];
                if (stisfyLabelSet.find(label_ner) != stisfyLabelSet.end())
                    continue;
                label2cnt[label_ner] += 1;
                if (label2cnt[label_ner] >= motif_label2cnt[label][label_ner])
                {
                    stisfyLabelSet.insert(label_ner);
                }
            }
            // Compare with the motif
            flag = (stisfyLabelSet.size() < motif_label2cnt[label].size());
            // If flag is true, delete this vertex
            if (flag)
            {
                deleteVertex.insert(i);
                for (auto &j : graph.adjList[i])
                {
                    graph.adjList[j].erase(i);
                }
            }
        }
    }

    // Repeat the above steps
    num_vertices = nbMap.size();
    nbMap.clear();
    cnt = 0;
    for (int i = 0; i < num_vertices; i++)
    {
        if (deleteVertex.find(i) == deleteVertex.end() && !graph.adjList[i].empty())
        {
            nbMap[i] = graph.adjList[i];
            sort2ori[i] = cnt++;
        }
    }

    cnt = 0;

    newAdjList = vector<set<int>>(nbMap.size());
    new_labels.clear();
    for (const auto &element : nbMap)
    {
        for (auto &e : element.second)
        {
            newAdjList[cnt].insert(sort2ori[e]);
        }
        new_labels.emplace_back(graph.labels[element.first]);
        cnt++;
    }

    graph.adjList = newAdjList;
    graph.labels = new_labels;
    graph.nodeNum = nbMap.size();
    graph.edges.clear();
    // Compute the new edgess
    for (int from = 0; from < graph.nodeNum; from++)
    {
        for (const auto &to : graph.adjList[from])
        {

            if (from > to)
                continue;
            // Ignore self-edge
            if (to == from)
            {
                // self_edge++;
                continue;
            }

            graph.edges.emplace_back(EDGE(to, from));
            graph.edges.emplace_back(EDGE(from, to));
        }
    }
    graph.edgeNum = graph.edges.size();
}

inline void ConstructGraph(Graph &g, const set<int> &keepSet)
{
    g.adjList.resize(motif.nodeNum);
    for (const int &from : keepSet)
    {
        for (const int &to : motif.adjList[from])
        {
            if (keepSet.find(to) == keepSet.end())
                continue;
            if (g.adjList[from].find(to) != g.adjList[from].end())
            {
                continue;
            }
            g.adjList[from].insert(to);
            g.edges.push_back(EDGE(from, to));
            g.edges.push_back(EDGE(to, from));
            g.adjList[to].insert(from);
        }
        g.labels.push_back(motif.labels[from]);
    }
    g.nodeNum = motif.nodeNum;
    g.edgeNum = g.edges.size();
}
inline bool CheckSet(const set<int> &A, const set<int> &B)
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
// Checks if vertices u and v in the given motif are in the same orbit.
// Vertices u and v are considered to be in the same orbit if:
// 1. They are of the same type.
// 2. The are automorphism in the graph
//  Actually, we maintain a std::map<int, int>motif_label2Orbit to represent the orbits of all vertices of a specific type in the motif.
//  The key in the map would be the vertex label, and the value would be the orbit identifier.
inline void GetMotifOrbit(const Graph &g, bool &hasDifference, map<int, int> &motif_label2Orbit)
{
    motif_label2Orbit.clear();
    hasDifference = false;
    vector<vector<int>> matched_subgraphs;
    ARGraph<int, Empty> patt_graph(g);
    ARGraph<int, Empty> targ_graph(g);
    vf3ImpOrbitCheck(patt_graph, targ_graph, matched_subgraphs);
    for (const auto &label_nodes : motif_label2nodes)
    {
        if (label_nodes.second.size() == 1)
            continue;
        int pos = *label_nodes.second.begin();
        set<int> autoMorphismSet;
        for (const vector<int> &matched_subgraph : matched_subgraphs)
        {
            autoMorphismSet.insert(matched_subgraph[pos]);
        }
        if (CheckSet(autoMorphismSet, label_nodes.second))
        {
            motif_label2Orbit[label_nodes.first] = 1;
        }
        else
        {
            motif_label2Orbit[label_nodes.first] = 0;
            hasDifference = true;
        }
    }
}
inline bool Search(set<int> subgraph)
{
    extern SetTrieNode *root;
    auto iter = subgraph.begin();

    SetTrieNode *temp = root;
    while (!temp->IsEndPoint())
    {
        temp = temp->GetChild(*iter);
        iter++;
        if (temp == NULL)
            return false;
    }
    return true;
}
int main(int argc, char **argv)
{

    int i;
    if (argc == 1)
    {
        printf("On Maximal Motif Clique Enumeration\n\n");
        printf("Options:\n");
        printf("Parameters:\n");
        printf("\t-genMotif\n");
        printf("\t\tGenerate motifs from the inputted graph\n");
        printf("\t-mmc\n");
        printf("\t\tEnumerate MMCs.\n");
        printf("\t-graph <file>\n");
        printf("\t\tthe target graph\n");
        printf("\t-motif <int>\n");
        printf("\t\tthe file of inputted motifs\n");
        printf("\t-output <file>\n");
        printf("\t\tSave the MMCs for each motif in <file>_motifId.txt\n");
        printf("\t-name2idpath <file>\n");
        printf("\t\tThe file contains all mappings from node names to ids.\n");
        printf("\t-vf3\n");
        printf("\t\tUse VF3 in subgraph matching.\n");
        printf("\t-motif_size <int>\n");
        printf("\t\tthe size of each motif to be generated.\n");
        printf("\t-motif_num <int>\n");
        printf("\t\tnumber of motifs to be generated.\n");
        printf("\t-subgraphlimit <int>\n");
        printf("\t\tThe maximum number of subgraphs to be explored.\n");
        printf("\t-mmclimit <int>\n");
        printf("\t\tThe maximum number of mmcs to be explored.\n");
        printf("\t-containnode <int>\n");
        printf("\t\tThe assigned node to be contained in detected MMCs.\n");

        printf("\t-directed <int>\n");
        printf("\t\tConsider directed graphs.\n");

        printf("\t-RandomSelection\n");
        printf("\t\tRandomly return a node in the Pick function.\n");
        printf("\t-DUPLICATION\n");
        printf("\t\tDon't set-trie to avoid duplication\n");

        printf("\t-WithoutIsoCheckPruning\n");
        printf("\t\tDon't use IsoCheck pruning.\n");

        printf("\nExample (Generate motifs):\n");
        printf("./motif_src -genMotif -graph network.txt -output motif.txt -motif_size 3 -motif_num 100\n\n");

        printf("\nExample (Enumerate MMCs):\n");
        printf("./motif_src -mmc -vf3 -graph net.txt -motif motif.txt -name2idpath name2idpath.txt -output mmcs.txt -subgraphlimit 100 -mmclimit 100 \n\n");

        printf("\nExample (Enumerate MMCs with an assigned node):\n");
        printf("./motif_src -mmc -vf3 -graph net.txt -containnode 1 -motif motif.txt -name2idpath name2idpath.txt -output mmcs.txt -subgraphlimit 100 -mmclimit 100 \n\n");
        return 0;
    }

    string graph_path;
    string motif_path;
    string output_path;
    string name2id_path;

    string species_path;

    // Generate undirected motifs
    if ((i = ArgPos((char *)"-genMotif", argc, argv)) > 0)
    {
        int motif_size, motif_num;
        if ((i = ArgPos((char *)"-graph", argc, argv)) > 0)
            graph_path = string(argv[i + 1]);
        if ((i = ArgPos((char *)"-output", argc, argv)) > 0)
            output_path = string(argv[i + 1]);
        if ((i = ArgPos((char *)"-motif_size", argc, argv)) > 0)
            motif_size = atoi(argv[i + 1]);
        if ((i = ArgPos((char *)"-motif_num", argc, argv)) > 0)
            motif_num = atoi(argv[i + 1]);
        cout << "Start to generate " << motif_num << " motifs with size=" << motif_size << " from " << graph_path
             << ", and write to " << output_path << endl;
        srand(time(NULL));
        GenerateMotif(graph_path, output_path, motif_size, motif_num);
        return 0;
    }

    assert((i = ArgPos((char *)"-mmc", argc, argv)) > 0);

    bool IsVF3 = false;
    bool SaveMMCs = false;
    bool EFFECTIVENESS = false;
    bool UseSCA = false;
    int SCA = 0;
    bool PrintAnalysis = false;
    int MustContainNodeId = -1;

    if ((i = ArgPos((char *)"-graph", argc, argv)) > 0)
        graph_path = string(argv[i + 1]);
    if ((i = ArgPos((char *)"-motif", argc, argv)) > 0)
        motif_path = string(argv[i + 1]);
    if ((i = ArgPos((char *)"-output", argc, argv)) > 0)
    {
        SaveMMCs = true;
        output_path = string(argv[i + 1]);
    }

    if ((i = ArgPos((char *)"-CHECKSPECIES", argc, argv)) > 0)
    {
        CHECKSPECIES = true;
        species_path = string(argv[i + 1]);
    }

    if ((i = ArgPos((char *)"-EFFECTIVENESS", argc, argv)) > 0)
        EFFECTIVENESS = true;
    if ((i = ArgPos((char *)"-name2idpath", argc, argv)) > 0)
        name2id_path = string(argv[i + 1]);
    if ((i = ArgPos((char *)"-vf3", argc, argv)) > 0)
        IsVF3 = true;
    if ((i = ArgPos((char *)"-RandomSelection", argc, argv)) > 0)
        RandomSelection = true;
    if ((i = ArgPos((char *)"-DUPLICATION", argc, argv)) > 0)
        DUPLICATION = true;
    if ((i = ArgPos((char *)"-NOEARLYSTOPCHECK", argc, argv)) > 0)
        NOEARLYSTOPCHECK = true;
    if ((i = ArgPos((char *)"-WithoutIsoCheckPruning", argc, argv)) > 0)
        WithoutIsoCheckPruning = true;

    // Handle directed graph or not
    if ((i = ArgPos((char *)"-directed", argc, argv)) > 0)
        directed = true;

    if ((i = ArgPos((char *)"-approx", argc, argv)) > 0)
        APPROXIMATION = true;
    if ((i = ArgPos((char *)"-GETEXACTMMC", argc, argv)) > 0)
        GETEXACTMMC = true;

    if ((i = ArgPos((char *)"-R", argc, argv)) > 0)
        R = atof(argv[i + 1]);
    if ((i = ArgPos((char *)"-DELTA", argc, argv)) > 0)
        DELTA = atof(argv[i + 1]);
    if (APPROXIMATION)
    {
        OMEGA = ceil(log(1.0 / DELTA) / log(R));
    }

    if ((i = ArgPos((char *)"-subgraphlimit", argc, argv)) > 0)
        SUBGRAPH_LIMIT = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-mmclimit", argc, argv)) > 0)
        MMC_LIMIT = atoi(argv[i + 1]);

    if ((i = ArgPos((char *)"-containnode", argc, argv)) > 0)
        MustContainNodeId = atoi(argv[i + 1]);

    if ((i = ArgPos((char *)"-ORDER", argc, argv)) > 0)
        ORDER = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-PIVOT", argc, argv)) > 0)
        PIVOT = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-InitVF3", argc, argv)) > 0)
        InitCandidate = false;

    if ((i = ArgPos((char *)"-NotReduce", argc, argv)) > 0)
        NotReduce = true;
    if ((i = ArgPos((char *)"-PARALLELRUN", argc, argv)) > 0)
        PARALLELRUN = true;
    if ((i = ArgPos((char *)"-WORKS", argc, argv)) > 0)
        WORKS = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-START", argc, argv)) > 0)
        START = atoi(argv[i + 1]);
    if ((i = ArgPos((char *)"-SCA", argc, argv)) > 0)
    {
        UseSCA = true;
        SCA = atoi(argv[i + 1]);
    }
    if ((i = ArgPos((char *)"-BaseIP", argc, argv)) > 0)
    {
        PrintAnalysis = true;
        UseAP = !(atoi(argv[i + 1]));
    }
    if (CHECKSPECIES)
    {
        ReadValidNodes(species_path);
    }

    cout << "Read the target graph" << endl;
    graph.Read(graph_path);
    IsVisited = new bool[graph.nodeNum];
    copy_graph = Graph(graph);
    // For Scalability test:
    // 20%, 40%, 60%, 80%
    if (UseSCA)
    {
        set<int> keep_nodes;
        keep_nodes.clear();
        generate_small_graph(copy_graph, graph, SCA, 100, keep_nodes);
    }

    copy_graph = Graph(graph);

    int num_motifs;

    FILE *fin;
    fin = fopen(motif_path.c_str(), "rb");
    if (fin == NULL)
    {
        printf("ERROR: network file not found!\n");
        exit(1);
    }
    fscanf(fin, "%d", &num_motifs);

    // cout << "Start" << endl;
    struct timeval start, end, per_start;
    long long mtime, seconds, useconds, per_time;
    gettimeofday(&start, NULL);

    // num_motifs = min(100, num_motifs);

    if (APPROXIMATION)
    {
        MMC_LIMIT = -1;
        num_motifs = min(40, num_motifs);
    }
    ARGraph<int, Empty> targ_graph;
    if (NotReduce)
    {
        targ_graph = ARGraph<int, Empty>(graph);
        sort_subgraphs(ORDER);
    }
    int skip_motif = 1;
    if (PARALLELRUN)
    {
        num_motifs = num_motifs / WORKS;
        skip_motif = START;
    }
    for (int cas = 0; cas < num_motifs; ++cas)
    {

        // Initialization
        if (cas != 0)
            skip_motif = 1;
        while (skip_motif--)
        {
            motif.clear();
            for (auto e : maximal_motif_cliques)
                e.clear();
            maximal_motif_cliques.clear();
            for (auto e : subgraphs)
                e.clear();
            subgraphs.clear();

            if (cas % 20 == 0)
                printf("Process the %dth motif\n", cas);

            fscanf(fin, "%d %d", &motif.nodeNum, &motif.edgeNum);
            // printf("#Nodes: %d  #Edegs: %d\n", motif.nodeNum, motif.edgeNum);
            int from, to;
            motif.adjList.resize(motif.nodeNum);
            if (directed)
            {
                motif.InAdjList.resize(motif.nodeNum);
                motif.OutAdjList.resize(motif.nodeNum);
            }
            int self_edge = 0;

            for (int i = 0; i < motif.edgeNum; ++i)
            {
                fscanf(fin, "%d %d", &from, &to);
                // Ignore self-edge
                if (to == from)
                {
                    self_edge++;
                    continue;
                }

                if (motif.adjList[from].find(to) != motif.adjList[from].end())
                {
                    continue;
                }
                motif.adjList[from].insert(to);
                motif.edges.push_back(EDGE(from, to));

                if (!directed)
                {
                    // insert its reverse edge, the input file includes undirected graphs.
                    motif.edges.push_back(EDGE(to, from));
                    motif.adjList[to].insert(from);
                }
                else
                {
                    motif.InAdjList[to].insert(from);
                    motif.OutAdjList[from].insert(to);
                }
            }
            int label = 0;
            for (int i = 0; i < motif.nodeNum; ++i)
            {
                fscanf(fin, "%d", &label);
                motif.labels.push_back(label);
            }
        }

        assert(motif.labels.size() == motif.nodeNum);
        motif.edgeNum = motif.edges.size();

        GetLabel2NodesMap(motif, motif_label2nodes);

        GetLabelAdj(motif, motif_labelAdj);

        GetMotifOrbit(motif, hasDifference, motif_label2Orbit);

        motif_node_size = motif.nodeNum;

        if (motif.edgeNum == 0)
        {
            cout << "There's no edge in the inputted motif. Please input a valid motif!" << endl;
            return 0;
        }

        global_hash_id = 0;
        root = new SetTrieNode(-1, false, 0, global_hash_id++);
        if (MustContainNodeId != -1)
        {
            GetMatchedSubgraph(motif, graph, subgraphs,
                               MustContainNodeId); // Use the basic subgraph matching method, the outputted MMCs must contain MustContainNodeId.
        }
        else if (IsVF3)
        {
            unsigned long long start = clock();
            gettimeofday(&per_start, NULL);
            if (!NotReduce)
            {
                if (ORDER == 2)
                {

                    sort_subgraphs(ORDER);
                    ReduceGraph(motif, graph);
                }
                else
                {
                    // Note {\tt POMA-R}, which means "//" the following code:
                    ReduceGraph(motif, graph);
                    sort_subgraphs(ORDER);
                }
            }
            ARGraph<int, Empty> patt_graph(motif);
            if (!NotReduce)
            {
                targ_graph = ARGraph<int, Empty>(graph);
            }
            reduced_vertex += graph.nodeNum;
            reduced_edge += (graph.edgeNum >> 1);
            // cerr << "pre-compute time is: " << ((double)(clock() - start) / CLOCKS_PER_SEC) << endl;
            reduceHIN_time += ((double)(clock() - start) / CLOCKS_PER_SEC);

            GetMatchedSubgraphVF3(patt_graph, targ_graph, subgraphs);
            if (!NotReduce)
            { // Need to add the deep copy time:
                start = clock();
                graph = Graph(copy_graph);
                reduceHIN_time += ((double)(clock() - start) / CLOCKS_PER_SEC);
            }
        }
        else
        {
            cout << "enter the isVF3 = false here" << endl;
            GetMatchedSubgraph(motif, graph, subgraphs); // Use the basic subgraph matching method
        }

        root->release();
        delete root;
        gettimeofday(&end, NULL);

        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = seconds * 1000000 + useconds;
        per_time = ((end.tv_sec - per_start.tv_sec) * 1000000 + (end.tv_usec - per_start.tv_usec)) / 1000;

        extern int compute_motif_neighbor;

        extern double use_pivot;

        extern double use_duplicate;
        extern double update_candidates;
        extern double pick_time;
        mclique_number += maximal_motif_cliques.size();

        cout << maximal_motif_cliques.size() << " maximal motif cliques have been found!"
             << "compute vf3 time: " << vf3_time << " reduced vertex: " << reduced_vertex << "  reduced  edge: " << reduced_edge << " mclique size: " << mclique_size << " mclique number: " << mclique_number << " reduce_HIN time: " << reduceHIN_time << " init_time: " << init_time << endl;

        // cout<<maximal_motif_cliques[0].size()<<endl;

        // cout<<subgraphs.size()<<endl;

        if (SaveMMCs)
        {
            stringstream sid;
            sid << cas;
            string str_motif_id = sid.str();

            cout << "Start to save all maximal motif cliques into " << output_path + "_" + str_motif_id + ".txt"
                 << endl;
            // For case study.
            // PrintResults(maximal_motif_cliques.size(), output_path + "_" + str_motif_id + ".txt", graph, name2id_path);

            // For print the number of maximal m-cliques.
            PrintResults(cas, maximal_motif_cliques.size(), output_path + ".txt", per_time);

            // For print the vistied vertices.
            //  PrintResults(cas, visited_vertices, output_path + ".txt", per_time);
            if (PrintAnalysis)
            {
                extern int Pruning_vertices_sp;
                extern int All_vertices_sp;
                PrintiAnalysis(cas, Pruning_vertices_sp, All_vertices_sp, output_path + ".txt");
            }
            cout << "Done!" << endl;
        }

        if (EFFECTIVENESS)
        {
            for (const auto &e : maximal_motif_cliques)
            {
                average_size_mmc += e.size();
                average_density_mmc += GetDensity(graph, e);
            }
            average_number_mmc += maximal_motif_cliques.size();
        }
    }
    fclose(fin);

    gettimeofday(&end, NULL);
    seconds = end.tv_sec - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = seconds * 1000000 + useconds;

    cout << "each motif can be processed in " << mtime / num_motifs / 1000 << " ms on average!" << endl;

    if (EFFECTIVENESS)
    {
        cout << "average_number_mmc: " << average_number_mmc * 1.0 / num_motifs << endl;
        cout << "average_size_mmc: " << average_size_mmc * 1.0 / average_number_mmc << endl;
        cout << " average_density_mmc:" << average_density_mmc / average_number_mmc << endl;
    }

    cout << "Average #duplicated MMCs generated: " << duplicated_answer_num * 1.0 / num_motifs << endl;
    cout << "Average #Iso_check: " << iso_check_num * 1.0 / num_motifs << endl;

    if (APPROXIMATION)
    {

        extern vector<double> approx_improvement;
        extern vector<double> real_approx_ratio;
        extern vector<double> real_failure_prob;

        double sum = 0;
        if (!GETEXACTMMC)
        {
            assert(approx_improvement.size() != 0);
            for (auto e : approx_improvement)
            {
                sum += e;
            }
            cout << "Improvement of the approximation algorithm for each embedding: " << sum / approx_improvement.size()
                 << endl;
        }
        else
        {
            sum = 0;
            assert(real_approx_ratio.size() != 0);
            for (auto e : real_approx_ratio)
            {
                sum += e;
            }
            cout << "Avg. approx. ratio: " << sum / real_approx_ratio.size() << endl;

            sum = 0;
            assert(real_failure_prob.size() != 0);
            for (auto e : real_failure_prob)
            {
                sum += e;
            }
            cout << "Avg. failure prob: " << sum / real_failure_prob.size() << endl;
        }
    }

    if (temp_graph != NULL)
        temp_graph->clear();
    delete temp_graph;
    delete[] IsVisited;
    return 0;
}