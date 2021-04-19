#pragma once

#include <QVector>
#include <iostream>

class BsplineMethod {
public:
	BsplineMethod() { };
	~BsplineMethod() { };


	void init(int n, int m, QVector<float> origin);
	void split();

	QVector<float> join();

	void pre_calcbspline();

	float calc_bsplineKernel(int k); // b样条卷积核
	
	float factorial(int x); // 阶乘

	float binomialC(int down, int up); // 二项式系数

	void downSampling();

	void downSampling(int idx);

private:
	int n; // n阶
	int m; // m倍
	QVector<float> origin;
	QVector<float> splits[3];
	QVector<float> res[3];
	QVector<float> bspline;
};

