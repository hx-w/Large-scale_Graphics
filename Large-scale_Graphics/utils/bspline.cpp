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
			int p_max = (count - j - 1) / m; // 整除
			float sum_p = 0;

			for (int p = 0; p <= p_max; p++) {
				float sum_i = 0;
			
				for (int i = -20; i <= 20; i++) {
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
	return cache_list;
}

vlist BsplineMethod::up_sampling_bsp(const vlist origin) {
	vlist us_res;

	// calc T(k)
	vlist T;
	int N_old = origin.count();
	for (int k = 0; k < N_old; k++) {
		float val = 0;
		for (int i = std::max(-4, k - N_old + 1); i <= std::min(k, 4); i++) {
			val += S_1_n(i) * origin[k - i];
		}
		T.append(val);
	}

	// calc res
	int N_new = N_old * m;
	for (int k = 0; k < N_new; k++) {
		float val = 0;
		for (int i = std::max(k - N_new + 1, 0); i <= std::min(k, 3 * m); i++) {
			if ((k - i) % m == 0) {
				int idx = (k - i) / m;
				val += Beta_m_n(i) * T[idx];
			}
		}
		us_res.append(val);
	}

	return us_res;
}

float BsplineMethod::LSD(const vlist vl1, const vlist vl2) {
	float ans = 0;
	int count = std::min(vl1.count(), vl2.count());

	for (int idx = 0; idx < count; idx++) {
		ans += std::pow(vl1[idx] - vl2[idx], 1);
	}

	return ans;
}

vlist BsplineMethod::gauss_filter(const vlist origin) {
	vlist res;
	for (int k = 0; k < origin.count(); k++) {
		float val = 0;
		for (int i = std::max(0, k - origin.count() + 1); i < std::min(3 * m, k); i++) {
			val += origin[k - i] * Gauss(i);
		}
		res.append(val);
	}
	return res;
}

float BsplineMethod::Gauss(int x) {
	float pi = 3.14159265358979323846264333827950288489786939937510;
	float e = 2.7182818286;
	float delta = 0.2;
	float fir = 1.0 / (delta * sqrtf(2 * pi));
	float thi = pow((x - 1.5), 2) / (2.0 * delta * delta);

	return fir * pow(e, -thi);
}
