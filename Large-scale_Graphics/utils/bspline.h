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

	vlist low_pass_filter(const vlist origin); // ��ͨ�˲������и�������

	vlist down_sampling(const vlist origin); // �²���

	float Beta_m_n(int k); // b���������
	
	float S_1_n(int k); // b�����������
	
	vlist up_sampling_bsp(const vlist origin);

	void dump(const std::string filename, const vlist origin);

	vlist load(const std::string filname);

private:
	int n; // n��
	int m; // m��
	
	// �ڲ�����
	float factorial(int x); // �׳�
	float binomialC(int down, int up); // ����ʽϵ��
};

