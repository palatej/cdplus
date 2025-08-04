#ifndef __cblas_1_h
#define __cblas_1_h

#include <algorithm>
#include "matrix_0.h"

namespace LCPP {

    template <typename T>
    class IAMAX {
    public:

        IAMAX() {}

        int operator()(int n, const T* x, int incx);

    };

    template<typename T>
    int IAMAX<T>::operator()(int n, const T* x, int incx) {
        if (n < 1 || incx <= 0)
            return -1;
        if (n == 1)
            return 0;
        int iamax = 0;
        if (incx == 1) {
            T amax = std::abs(x[0]);
            for (int i = 1; i < n; ++i) {
                T cur = std::abs(x[i]);
                if (cur > amax) {
                    amax = cur;
                    iamax = i;
                }
            }
        }
        else {
            T amax = std::abs(x[0]);
            for (int i = 1, idx = 0; i < n; ++i) {
                idx += incx;
                T cur = std::abs(x[idx]);
                if (cur > amax) {
                    amax = cur;
                    iamax = i;
                }
            }
        }
        return iamax;
    }

    template <typename T>
    class SWAP {

    public:

        SWAP() {}

        void operator()(int n, T* x, int incx, T* y, int incy);

    };

    template<typename T>
    void SWAP<T>::operator()(int n, T* x, int incx, T* y, int incy) {
        if (n <= 0)
            return;
        if (incx == 1 && incy == 1) {
            for (int i = 0; i < n; ++i) {
                T tmp = x[i];
                x[i] = y[i];
                y[i] = tmp;
            }
        }
        else {
            int ix = 0, iy = 0;
            if (incx < 0) ix = (-n + 1) * incx;
            if (incy < 0) iy = (-n + 1) * incy;
            for (int i = 0; i < n; ++i) {
                T tmp = x[ix];
                x[ix] = y[iy];
                y[iy] = tmp;
                ix += incx;
                iy += incy;
            }
        }
    }

    template <typename T>
    class SCAL {
    public:

        SCAL() {}

        void operator()(int n, T a, T* x, int incx);

    };

    template <typename T>
    void SCAL<T>::operator()(int n, T a, T* x, int incx) {
        if (n <= 0 || incx <= 0)
            return;
        T one = NUMCPP::CONSTANTS<T>::one;
        if (a == one)
            return;
        T zero = NUMCPP::CONSTANTS<T>::zero;
        if (incx == 1) {
            if (a == zero) {
                for (int i = 0; i < n; ++i) {
                    x[i] = zero;
                }
            }
            else {
                for (int i = 0; i < n; ++i) {
                    x[i] *= a;
                }
            }

        }
        else {
            int imax = incx * n;
            if (a == zero) {
                for (int i = 0; i < imax; i += incx) {
                    x[i] = zero;
                }
            }
            else {
                for (int i = 0; i < imax; i += incx) {
                    x[i] *= a;
                }
            }
        }
    }
}

#endif
