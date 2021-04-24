#include "bspline.h"

void BsplineMethod::init(int n, int m, QVector<float> origin) {
	for (int idx = 0; idx < 3; idx++) {
		splits[idx].clear();
		res[idx].clear();
	}
	bspline.clear();
	this->n = n;
	this->m = m;
	this->origin = origin;
}

void BsplineMethod::split() {
	int nums = origin.count();

	for (int idx = 0; idx < nums; idx++) {
		splits[idx % 3].append(origin[idx]);
	}
}

QVector<float> BsplineMethod::join() {
	int point_nums = res[0].count();
	QVector<float> join_vector;
	for (int pidx = 0; pidx < point_nums; pidx++) {
		for (int sidx = 0; sidx < 3; sidx++) {
			join_vector.append(res[sidx][pidx]);
		}
	}
	return join_vector;
}

float BsplineMethod::factorial(int x) {
	if (x == 0) return 1;
	float res = 1;
	for (int idx = 1; idx <= x; idx++) {
		res *= idx;
	}
	return res;
}

float BsplineMethod::binomialC(int down, int up) {
	float _up = factorial(up);
	float _down = factorial(down) * factorial(up - down);
	return _up / _down;
}

void BsplineMethod::pre_calcbspline() {
	int old_num = splits[0].count();
	for (int idx = 0; idx < old_num; idx++) {
		bspline.append(calc_bsplineKernel(idx));
	}
}

float BsplineMethod::calc_bsplineKernel(int k) {
	int old_num = splits[0].count();
	if (k > n) {
		return 0;
	}
	float c = pow(1.0 / m, n);
	float _down = factorial(n);

	float _sum = 0;

	for (int j = 0; j <= n + 1; j++) {
		float _up = 1;
		if (j % 2 == 1) _up = -1;
		float _fir = _up / _down;
		float _sec = binomialC(j, n + 1);
		float _thi = pow(float(k) - m * j, n);
		float _fou = 1;
		if (k - m * j < 0) _fou = 0;
		
		_sum += _fir * _sec * _thi * _fou;
	}

	return c * _sum;
}

void BsplineMethod::downSampling() {
	for (int idx = 0; idx < 3; idx++) {
		downSampling(idx);
	}
}

void BsplineMethod::downSampling(int idx) {
	int old_num = splits[0].count();
	int new_num = old_num / m;

	for (int new_idx = 0; new_idx < new_num; new_idx++) {
		int up_idx = new_idx * m;

		// function T
		float t_res = 0;
		
		for (int i = 0; i <= old_num - 1; i++) {  // g * beta
			if (up_idx - i < 0) break;
			if (i > n) break;
			float bspk = bspline[i];
			t_res += splits[idx][up_idx - i] * bspk;
		}

		res[idx].append(t_res);
	}
}

