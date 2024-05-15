#include"Graph.h"
#include"global-variable.h"
#include"small-graph.h"
#include <map>

void resize_small_graph(Graph &dst){
	dst.adjList.resize(dst.nodeNum);
	dst.InAdjList.resize(dst.nodeNum);
	dst.OutAdjList.resize(dst.nodeNum);
	dst.labels.assign(dst.nodeNum,-1);
}

map<int,int> new_id_map;

void get_new_id_map(const Graph &src, Graph &dst, int part, int total, set<int> &keep_nodes){
	new_id_map.clear();
	int new_id = 0;
	for(int node_id: keep_nodes){
		if(new_id_map.find(node_id) == new_id_map.end()){
			new_id_map[node_id] = new_id ++;
			if(new_id + 1 >= dst.nodeNum) return;
		}
	}
	for(int i = 0; i < src.nodeNum; i = i + total){
		for(int j = 0; j < part; ++j){
			int old_id = i + j;
			if(new_id_map.find(old_id) == new_id_map.end()){
				new_id_map[old_id] = new_id ++;
				if(new_id + 1 >= dst.nodeNum) return;
			}
		}
	}
	dst.nodeNum = new_id_map.size();
}

void get_new_edges(const Graph &src, Graph &dst){
	for(auto e: src.edges){
		
		if(new_id_map.find(e.from) != new_id_map.end() && new_id_map.find(e.to) != new_id_map.end()){
			if(e.from == 175189) cerr<<e.to<<endl;
			int new_from = new_id_map[e.from];
			int new_to   = new_id_map[e.to];
			dst.adjList[new_from].insert(new_to);
			dst.edgeNum ++;
			dst.edges.push_back(EDGE(new_from,new_to));
		}
	}
}

void get_labels(const Graph &src, Graph &dst){
	for(int old_id = 0; old_id < src.labels.size(); old_id ++){
		if(new_id_map.find(old_id) != new_id_map.end()){
			int new_id = new_id_map[old_id];
			dst.labels[new_id] = src.labels[old_id];
		}
	}
}

void generate_small_graph(const Graph &src, Graph &dst, int part, int total, set<int> &keep_nodes){
	dst.clear();
	dst.nodeNum = 1ll * src.nodeNum * part / total;
	dst.edgeNum = 0;
	resize_small_graph(dst);
	get_new_id_map(src,dst,part,total,keep_nodes);
	get_new_edges(src,dst);
	get_labels(src,dst);
}