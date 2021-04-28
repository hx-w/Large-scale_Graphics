#include "EulerGraph.h"

Hierholzer::Hierholzer() {
	p2ps.clear();
	euler_trace.clear();
}

Hierholzer::~Hierholzer() { }

void Hierholzer::connet_points(int v1, int v2) {
	/*
	auto finder_v1 = std::find(p2ps[v1].begin(), p2ps[v1].end(), v2);
	
	auto finder_v2 = std::find(p2ps[v2].begin(), p2ps[v2].end(), v1);

	if (finder_v1 != p2ps[v1].end() && finder_v2 != p2ps[v2].end()) {
		//p2ps[v1].erase(finder_v1);
		//p2ps[v2].erase(finder_v2);
		return;
	}
	*/
	

	p2ps[v1].append(v2);
	p2ps[v2].append(v1);
}

QVector<int> Hierholzer::get_answer() {
	return euler_trace;
}

void Hierholzer::run() {
	dfs_plus(p2ps.firstKey()); 
}

void Hierholzer::dfs_plus(int currV) {
	while (!p2ps[currV].empty()) {
		int nextV = p2ps[currV].takeFirst();
		dfs_plus(nextV);
	}
	euler_trace.append(currV);
}