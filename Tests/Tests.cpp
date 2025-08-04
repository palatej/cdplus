
#include <iostream>
#include <chrono>
#include <ratio>
#include <ctime>
#include "sequence.h"
#include "arima.h"
#include "getf2.h"

#include "matrix.h"

using namespace NUMCPP;
using namespace CD_STATS;

CONSTANTS<double> cnt;

int main() {

	double ar[4] = { 1, 0, 0,-.3 };
	double ma[3] = { 1,-.2,.1 };

	DataBlock<double> all(10);
	all.rand();

	Polynomial AR(3, ar);
	Polynomial MA(2, ma);

	Arima arima(AR, AR, AR, 1);
	Polynomial par = arima.getAr();

	par += 0;
	Polynomial par2 = par - 0;

	std::cout << par << std::endl;
	//std::cout << par.evaluateAtFrequency(45.123);

	const auto start = std::chrono::steady_clock::now();
	std::complex s(0.0);
	for (int i = 1; i < 1000000; ++i)
		s += par.evaluateAtFrequency(1000.0 / i);

	const auto end = std::chrono::steady_clock::now();
	const auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << int_ms.count() << "  " << std::endl;

	double* x = new double[20];

	par.rationalFunctionExpansion(MA, 20, x);
	//for (int i = 0; i < 20; ++i) {
	//	std::cout << x[i] << std::endl;
	//}
	delete[] x;

	LCPP::GETF2<double> getf2;
	int m = 500, n = 500;
	Matrix<double> A(m, n);
	A.rand();

	FastMatrix<double> a=A.all();
	int* piv = new int[n];
	const auto start2 = std::chrono::steady_clock::now();
	getf2(m, n, &a(0, 0), m, piv);
	const auto end2 = std::chrono::steady_clock::now();
	const auto int_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
	std::cout << int_ms2.count() << "  " << std::endl;

	//for (int i = 0; i < n; ++i) {
	//	std::cout << piv[i] << std::endl;
	//}
	delete[]piv;
	std::cout << getf2.info();
//	std::cout << A;
}

