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

	float calc_bsplineKernel(int k); // b���������
	
	float factorial(int x); // �׳�

	float binomialC(int down, int up); // ����ʽϵ��

	void downSampling();

	void downSampling(int idx);

private:
	int n; // n��
	int m; // m��
	QVector<float> origin;
	QVector<float> splits[3];
	QVector<float> res[3];
	QVector<float> bspline;
};

