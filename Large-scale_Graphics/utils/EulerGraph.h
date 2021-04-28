#pragma once

#include <QVector>
#include <QMap>
#include <QDebug>

class Hierholzer {
// 所有的顶点数据都是索引，原始数据不保存在这里
// 能确保图能构造欧拉回路，即所有点的度都是偶数
public:
	Hierholzer();
	~Hierholzer();

	void connet_points(int v1, int v2);
	void run();
	QVector<int> get_answer();

private:
	void dfs_plus(int currV);

	QMap<int, QVector<int>> p2ps; // 邻接矩阵   v | vi vj vk ...  包含重复点
	QVector<int> euler_trace; // 结果
};
