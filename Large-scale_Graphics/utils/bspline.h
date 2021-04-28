#pragma once

#include <QVector>
#include <iostream>
#include <fstream>

typedef QVector<float> vlist;

class BsplineMethod {
public:
	BsplineMethod() { };
	~BsplineMethod() { };


	void init(int n, int m);

	QVector<vlist> split(const vlist origin);

	vlist join(const QVector<vlist> xyz);

	vlist low_pass_filter(const vlist origin); // 低通滤波，序列个数不变

	vlist down_sampling(const vlist origin); // 下采样

	float Beta_m_n(int k); // b样条卷积核
	
	float S_1_n(int k); // b样条反卷积核
	
	vlist up_sampling_bsp(const vlist origin);

	void dump(const std::string filename, const vlist origin);

	vlist load(const std::string filname);

private:
	int n; // n阶
	int m; // m倍
	
	// 内部方法
	float factorial(int x); // 阶乘
	float binomialC(int down, int up); // 二项式系数
};

