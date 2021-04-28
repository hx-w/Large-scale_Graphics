#pragma once

#include <QVector>
#include <QMap>
#include <QDebug>

class Hierholzer {
// ���еĶ������ݶ���������ԭʼ���ݲ�����������
// ��ȷ��ͼ�ܹ���ŷ����·�������е�Ķȶ���ż��
public:
	Hierholzer();
	~Hierholzer();

	void connet_points(int v1, int v2);
	void run();
	QVector<int> get_answer();

private:
	void dfs_plus(int currV);

	QMap<int, QVector<int>> p2ps; // �ڽӾ���   v | vi vj vk ...  �����ظ���
	QVector<int> euler_trace; // ���
};
