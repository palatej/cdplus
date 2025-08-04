#ifndef __lcpp_getrf_h
#define __lcpp_getrf_h

#include "getrf2.h"


namespace LCPP {

	/// <summary>
	/// Computes an LU factorization of a general M-by-N matrix A
	/// using partial pivoting with row interchanges.
	/// 
	/// The factorization has the form
	/// A = P * L* U
	/// where P is a permutation matrix, L is lower triangular with unit
	/// diagonal elements(lower trapezoidal if m > n), and U is upper
	/// triangular(upper trapezoidal if m < n).
	/// This is the right - looking Level 3 BLAS version of the algorithm.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class GETRF {
	public:

		GETRF() {}

		void operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots);

	private:

		static const int BLOCKSIZE = 64;
	 
	};

	template<typename T>
	void GETRF<T>::operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots) {
		if (A.isEmpty())
			return;
		int m = A.getNrows(), n = A.getNcols;
		int nb = blockSize(m, n);
		GETRF2<T> getrf2;
		if (BLOCKSIZE > std::min(m, n)) {
			getrf2(A, pivots);
		}
		else {
			//use blocked code
			throw std::exception("Not implemented yet");
		}
	}
}

#endif 