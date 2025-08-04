#include <iostream>

#include "polynomials.h"

using namespace NUMCPP;

Polynomial::~Polynomial() {
}

Polynomial::Polynomial(const Polynomial& p) {
	m_c = p.m_c;
}

Polynomial& Polynomial::operator=(const Polynomial& p) {
	if (&p == this)
		return *this;
	m_c = p.m_c;
	return *this;
}

Polynomial::Polynomial(int degree, double val) {
	double* c = new double[degree + 1];
	for (int i = 0; i <= degree; ++i) {
		c[i] = val;
	}
	m_c = _cvector<double>(c, degree + 1);
}

Polynomial::Polynomial(int degree, double* val) {
	double* c = new double[degree + 1];
	for (int i = 0; i <= degree; ++i) {
		c[i] = val[i];
	}
	m_c = _cvector<double>(c, degree + 1);
}

Polynomial Polynomial::operator+(const Polynomial& r)const {
	if (!m_c.isValid() || !r.m_c.isValid())
		return Polynomial();
	int ld = getDegree(), rd=r.getDegree();
	// swap l and r if l.Degree < r.Degree
	if (ld < rd) {
		return r + (*this);
	}
	double* result = new double[ld+1];
	for (int i = 0; i <= rd; ++i) {
		result[i] = m_c[i]+r.m_c[i];
	}
	for (int i = rd + 1; i <= ld; ++i) {
		result[i] = m_c[i];
	}
	return Polynomial(ld, result);
}

Polynomial Polynomial::operator-(const Polynomial& r)const {
	if (!m_c.isValid() || !r.m_c.isValid())
		return Polynomial();
	int ld = getDegree(), rd = r.getDegree();
	if (ld >= rd) {
		double* result = new double[ld + 1];
		for (int i = 0; i <= rd; ++i) {
			result[i] = m_c[i] - r.m_c[i];
		}
		for (int i = rd + 1; i <= ld; ++i) {
			result[i] = m_c[i];
		}
		return Polynomial(ld, result);
	}
	else {
		double* result = new double[rd + 1];
		for (int i = 0; i <= ld; ++i) {
			result[i] = m_c[i] - r.m_c[i];
		}
		for (int i = ld + 1; i <= rd; ++i) {
			result[i] = -r.m_c[i];
		}
		return Polynomial(rd, result);

	}
}

Polynomial Polynomial::operator*(const Polynomial& r)const {
	if (! m_c.isValid() || !r.m_c.isValid())
		return Polynomial();
	int ldeg = getDegree(), rdeg = r.getDegree();
	if (ldeg==0)
		return r * m_c[0];
	if (rdeg ==0)
		return (*this) * r.m_c[0];
	int n = ldeg+rdeg;
	double* result = new double[n + 1];
	for (int i = 0; i <= n; ++i)
		result[i] = 0;
	for (int i = 0; i <= ldeg; ++i) {
		if (m_c[i] != 0) {
			for (int j = 0; j <= rdeg; ++j) {
				if (r.m_c[j] != 0) {
					result[i+j] += m_c[i] * r.m_c[j];
				}
			}
		}
	}
	return Polynomial(n, result);

}

Polynomial Polynomial::operator*(double a)const {
	if (! m_c.isValid())
		return Polynomial();
	if (a == 0)
		return Polynomial::ZERO;
	if (a == 1)
		return *this;
	int n = m_c.length();
	double* result = new double[n];
	for (int i = 0; i < n; ++i) {
		result[i] = m_c[i] * a;
	}
	return Polynomial(n-1, result);
}

Polynomial Polynomial::operator+(double a)const {
	if (!m_c.isValid())
		return Polynomial();
	if (a == 0)
		return *this;
	int n = m_c.length();
	double* result = new double[n];
	for (int i = 0; i < n; ++i) {
		result[i] = m_c[i] + a;
	}
	return Polynomial(n - 1, result);
}

Polynomial Polynomial::operator-(double a)const {
	if (!m_c.isValid())
		return Polynomial();
	if (a == 0)
		return *this;
	int n = m_c.length();
	double* result = new double[n];
	for (int i = 0; i < n; ++i) {
		result[i] = m_c[i] - a;
	}
	return Polynomial(n - 1, result);
}

Polynomial Polynomial::operator/(double a)const {
	if (!m_c.isValid())
		return Polynomial();
	if (a == 1)
		return *this;
	int n = m_c.length();
	double* result = new double[n];
	for (int i = 0; i < n; ++i) {
		result[i] = m_c[i] / a;
	}
	return Polynomial(n - 1, result);
}

double Polynomial::evaluateAt(double x)const {
	int i = getDegree();
	double f = m_c[i--];
	for (; i >= 0; --i) {
		f = m_c[i] + (f * x);
	}
	return f;
}

std::complex<double> Polynomial::evaluateAt(const std::complex<double>&  x)const {
	int i = getDegree();
	double xr = x.real(), xi = x.imag();
	double fr = m_c[i--], fi = 0;
	for (; i >= 0; --i) {m_c[i];
	double tr = fr * xr - fi * xi + m_c[i];
	double ti = fr * xi + fi * xr;
		fr = tr;
		fi = ti;
	}
	return std::complex(fr, fi);
}

std::complex<double> Polynomial::evaluateAtFrequency(double w)const {
	int d = getDegree();
	std::complex<double> f = m_c[0];
	for (int i = 1; i <= d; ++i) {
		f+=std::polar(m_c[i], w * i);
	}
	return f;
}


const Polynomial Polynomial::ZERO(0, 0.0);
const Polynomial Polynomial::ONE(0, 1.0);

// w = num/denom or w * denom = num
void Polynomial::rationalFunctionExpansion(const Polynomial& denom, int n, double* w)const {
	if (n <= 0)
		return;
	int q = m_c.length();
	int p = denom.m_c.length();
	double d = denom[0];

	int imax = std::min(n, q);
	// trivial case
	if (p == 1) {
		if (d != 1) {
			for (int i = 0; i < imax; ++i) {
				w[i] = m_c[i] / d;
			}
		}
		else {
			for (int i = 0; i < imax; ++i) {
				w[i] = m_c[i];
			}
		}
		for (int i = imax; i < n; ++i) {
			w[i] = 0;
		}
		return;
	}

	w[0] = m_c[0] / d;
	// until p...
	int kmax = std::min(n, p);
	for (int k = 1; k < kmax; ++k) {
		double s = 0;
		for (int l = 1; l <= k; ++l) {
			s += denom.m_c[l] * w[k - l];
		}
		if (k < q) {
			w[k] = (m_c[k] - s) / d;
		}
		else {
			w[k] = -s / d;
		}
	}
	for (int k = kmax; k < imax; ++k) {
		double s = 0;
		for (int l = 1; l < p; ++l) {
			s += denom.m_c[l] * w[k - l];
		}
		w[k] = (m_c[k] - s) / d;
	}
	for (int k = imax; k < n; ++k) {
		double s = 0;
		for (int l = 1; l < p; ++l) {
			s += denom.m_c[l] * w[k - l];
		}
		w[k] = -s / d;
	}
}



