#ifndef __cblas_2_h
#define __cblas_2_h

#include "cblas_1.h"

namespace LCPP {
	/// <summary>
	///  performs the rank 1 operation
	/// A = alpha * x * y' + A,
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	class GER {
	public:

		GER() {}

		void operator()(int m, int n, T alpha, const T* x, int incx, const T* y, int incy, T* A, int lda);

	};

    template <typename T>
    void GER<T>::operator()(int m, int n, T alpha, const T* x, int incx, const T* y, int incy, T* A, int lda) {
        int info = 0;
        if (m < 0)
            info = 1;
        else if (n < 0)
            info = 2;
        else if (incx == 0)
            info = 5;
        else if (incy == 0)
            info = 7;
        else if (lda < std::max(1, m))
            info = 9;
        if (info != 0)
            throw lcpp_exception("ger", info);
        T zero = NUMCPP::CONSTANTS<T>::zero;
        if (m == 0 || n == 0 || alpha == zero)
            return;
        int jy = 0;
        if (incy < 0)
            jy = incy * (n - 1);
        if (incx == 1) {
            for (int j = 0; j < n; ++j, jy+=incy) {
                T ycur = y[jy];
                if (ycur != zero) {
                    T tmp = alpha * ycur;
                    T* C = A + lda * j;
                    for (int i = 0; i < m; ++i) {
                        C[i] += x[i] * tmp;
                    }
                }
            }
        }
        else {
            int kx = 0;
            if (incx < 0)
                kx = incx * (m - 1);
            for (int j = 0; j < n; ++j, jy += incy) {
                T ycur = y[jy];
                if (ycur != zero) {
                    T tmp = alpha * ycur;
                    T* C = A + lda * j;
                    for (int i = 0, ix=kx; i < m; ++i, ix+=incx) {
                        C[i] += x[ix] * tmp;
                    }
                }
            }
        }
    }
}

#endif