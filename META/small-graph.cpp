#include"Graph.h"
#include"global-variable.h"
#include"small-graph.h"
#include <map>

void get_new_edges(const Graph& src, Graph& dst, int part, int total, map<int, int>& id_map) {
	int sz = src.edges.size();
	dst.edgeNum = 0;
	dst.nodeNum = 0;

	dst.adjList.resize(src.nodeNum);

	for (int i = 0; i < sz; i = i + 2 * total) {
		for (int j = 0; j < 2 * part; j += 2) {
			int edge_id = i + j;
			if (i + j >= sz) continue;
			dst.edgeNum += 2;

			int from = src.edges[edge_id].from;
			int to = src.edges[edge_id].to;
			if (id_map.find(from) == id_map.end()) id_map[from] = dst.nodeNum++;
			if (id_map.find(to) == id_map.end()) id_map[to] = dst.nodeNum++;

			from = id_map[from];
			to = id_map[to];

			dst.adjList[from].insert(to);
			dst.adjList[to].insert(from);
			dst.edges.push_back(EDGE(from, to));
			dst.edges.push_back(EDGE(to, from));
		}
	}

	dst.labels.resize(dst.nodeNum);
}

void get_labels(const Graph& src, Graph& dst, map<int, int>& id_map) {
	for (int old_id = 0; old_id < src.labels.size(); old_id++) {
		if (id_map.find(old_id) != id_map.end()) {
			int new_id = id_map[old_id];
			dst.labels[new_id] = src.labels[old_id];
		}
	}
}

void generate_small_graph(const Graph& src, Graph& dst, int part, int total, set<int>& keep_nodes) {
	dst.clear();
	map<int, int> id_map;
	get_new_edges(src, dst, part, total, id_map);
	get_labels(src, dst, id_map);
}