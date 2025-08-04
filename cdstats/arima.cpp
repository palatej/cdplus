#include "arima.h"
#include "polynomials.h"

using namespace NUMCPP;
using namespace CD_STATS;

Arima::~Arima() {}

Arima::Arima() : m_star(Polynomial::ONE), m_ar(Polynomial::ONE),m_delta(Polynomial::ONE), m_ma(Polynomial::ONE), m_var(1){}

Arima::Arima(const Arima& arima) : m_star(arima.m_star), m_ar(arima.m_ar),m_delta(arima.m_delta), m_ma(arima.m_ma), m_var(arima.m_var) {}

Arima& Arima::operator=(const Arima& arima) {
	if (this != &arima) {
		m_ar = arima.m_ar;
		m_star = arima.m_star;
		m_delta = arima.m_delta;
		m_var = arima.m_var;
	}
	return *this;
}

Arima::Arima(const Polynomial& ar, const Polynomial& delta, const Polynomial& ma, double var)
	:m_star(ar), m_ar(ar*delta), m_delta(delta), m_ma(ma), m_var(var) {
}



