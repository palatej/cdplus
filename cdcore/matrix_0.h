#ifndef __lcpp_matrix_0_h
#define __lcpp_matrix_0_h

#include <exception>
#include <iostream>

namespace LCPP {

	enum Triangular {
		Lower, Upper
	};

	enum Side {
		Left, Right
	};

	struct lcpp_exception : std::exception {
	public:
		lcpp_exception(const std::string routine, int info) :m_routine(routine), m_info(info) {}

		const std::string& source()const { return m_routine; }
		int info()const { return m_info; }

	private:
		std::string m_routine;
		int m_info;
	};

	/// <summary>
	/// LASWP performs a series of row interchanges on the matrix A.
	/// One row interchange is initiated for each of rows K1 through K2 of A.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class LASWP {
	public:

		LASWP() {}

		void operator()(int n, T* A, int lda, int k1, int k2, int* piv, int incx);

	private:

		static const int BLOCK = 32;
	};

	template<typename T>
	void LASWP<T>::operator()(int n, T* A, int lda, int k1, int k2, int* piv, int incx) {
		int ix0, i1, i2, inc;
		if (incx > 0) {
			ix0 = k1;
			i1 = k1;
			i2 = k2;
			inc = 1;
		}
		else if (incx < 0) {
			ix0 = k1 + (k1 - k2) * incx;
			i1 = k2;
			i2 = k1;
			inc = -1;
		}
		else {
			return;
		}
		int nb = (n / BLOCK) * BLOCK;
		T* C = A;
		if (nb != 0){
			for (int j = 0; j < nb; j += BLOCK, C+=lda) {
				for (int i = i1, ix = ix0; i < i2; i += inc, ix += incx) {
					int ip = piv[ix];
					if (ip != i) {
						T* ci = C + i, * cip = C + ip;
						for (int k = j; k < j+BLOCK; ++k, ci += lda, cip += lda) {
							T tmp = *ci;
							*ci = *cip;
							*cip = tmp;
						}
					}
				}
			}
		}
		if (nb < n) {
			for (int i = i1, ix = ix0; i < i2; i += inc, ix += incx) {
				int ip = piv[ix];
				if (ip != i) {
					T* ci = C + i, * cip = C + ip;
					for (int k = nb; k < n; ++k, ci += lda, cip += lda) {
						T tmp = *ci;
						*ci = *cip;
						*cip = tmp;
					}
				}
			}
		}
	}
}

#endif

