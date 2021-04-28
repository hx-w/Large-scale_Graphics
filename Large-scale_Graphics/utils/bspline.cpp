#include "bspline.h"
#include <QDebug>

void BsplineMethod::init(int n, int m) {
	this->n = n;
	this->m = m;
}

QVector<vlist> BsplineMethod::split(const vlist origin) {
	QVector<vlist> xyz{
		vlist{}, vlist{}, vlist{} // x y z
	};
	int nums = origin.count();

	for (int idx = 0; idx < nums; idx++) {
		xyz[idx % 3].append(origin[idx]);
	}
	return xyz;
}

vlist BsplineMethod::join(const QVector<vlist> xyz) {
	// merge x,y,z
	int point_nums = xyz[0].count();
	vlist join_vector;
	for (int pidx = 0; pidx < point_nums; pidx++) {
		for (int sidx = 0; sidx < 3; sidx++) {
			join_vector.append(xyz[sidx][pidx]);
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

float BsplineMethod::Beta_m_n(int k) {
	if (k > m * n || k <= 0) {
		return 0;
	}
	float c = pow(1.0 / m, n + 1);
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

float BsplineMethod::S_1_n(int k) {
	k = abs(k);   // 公式里是绝对值
	if (k > 7) { // 超过阈值
		return 0;
	}
	float alpha = sqrtf(3.0) - 2;

	float _fir = (-6 * alpha) / (1 - alpha * alpha);
	float _sec = powf(alpha, k);
	return _fir * _sec;
}

vlist BsplineMethod::low_pass_filter(const vlist origin) {
	int count = origin.count();
	vlist lpf_res;

	for (int k = 0; k < count; k++) { // 计算每个数
		float val = 0;

		for (int j = 0; j <= m - 1; j++) {
			int p_max = (count - j) / m; // 整除
			float sum_p = 0;

			for (int p = 0; p < p_max; p++) {
				float sum_i = 0;
			
				for (int i = -12; i <= 12; i++) {
					int b_idx = k - j - m * p - m * i;
					float beta_m_n = Beta_m_n(b_idx);
					float s_1_n = S_1_n(i);
					sum_i += beta_m_n * s_1_n;
				}
				float u = origin[m * p + j];

				sum_p += sum_i * u;
			}

			val += sum_p;
		}
		lpf_res.append(val);

		qDebug() << "computing: " << k << " / " << count;
	}

	return lpf_res;
}

vlist BsplineMethod::down_sampling(const vlist origin) {
	int ds_count = origin.count() / m;
	vlist ds_res;

	for (int idx = 0; idx < ds_count; idx++) {
		ds_res.append(origin[m * idx]);
	}

	return ds_res;
}

void BsplineMethod::dump(const std::string filename, const vlist origin) {
	std::ofstream outF(filename.c_str(), std::ios::out);
	if (outF.is_open()) {
		for (auto iter : origin) {
			outF << iter << " ";
		}
	}
	outF.close();
	qDebug() << "dump cache finish";
}

vlist BsplineMethod::load(const std::string filename) {
	vlist cache_list;

	std::ifstream inF(filename.c_str(), std::ios::in);
	if (inF.is_open()) {
		float val = 0;
		while (inF >> val) {
			cache_list.append(val);
		}
	}
	inF.close();
	qDebug() << "load cache finish";
	return cache_list;
}

vlist BsplineMethod::up_sampling_bsp(const vlist origin) {
	vlist us_res;
	int origin_N = origin.count();
	int new_N = origin_N * m;

	for (int k = 0; k < new_N; k++) {
		float sum_p = 0;
		// p loop
		for (int p = k - n; p <= k; p++) {
			float beta_m_n = Beta_m_n(k - p);
			
			float up_sample_c = 0;
			if (p % m == 0) {
				int new_p = p / m;
				// i loop
				for (int i = new_p - 7; i <= new_p; i++) {
					if (i > origin_N || i < 0) continue;
					float s_1_n = S_1_n(new_p - i);
					float val = origin[i];
					up_sample_c += s_1_n * val;
				}
			}
			sum_p += beta_m_n * up_sample_c;
		}
		us_res.append(sum_p);
	}

	return us_res;
}
