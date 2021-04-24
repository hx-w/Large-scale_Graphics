#include "EulerGraph.h"

Hierholzer::Hierholzer() {
	p2ps.clear();
	euler_trace.clear();
}

Hierholzer::~Hierholzer() { }

void Hierholzer::connet_points(int v1, int v2) {
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

}