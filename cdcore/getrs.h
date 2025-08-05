#ifndef __lcpp_getrs_h
#define __lcpp_getrs_h

#include "matrix_0.h"
#include "cblas_3.h"

namespace LCPP {
    /// <summary>
    /// solves a system of linear equations 
    /// A* X = B or A' * X = B
    /// with a general N x N matrix A using the LU factorization computed by GETRF
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class GETRS {
    public:

        GETRS() {}

        void operator()(bool tA, int n, int nrhs, const T* A, int lda, int* piv, T* B, int ldb);
 
        int info() {
            return m_info;
        }

    private:

        int m_info;
    };

    
    template <typename T>
    void GETRS<T>::operator()(bool tA, int n, int nrhs, const T* A, int lda, int* piv, T* B, int ldb) {
        int info = 0;
        if (n < 0)
            info = -2;
        else if (nrhs < 0)
            info = -3;
        else if (lda < std::max(1, n))
            info = -5;
        else if (ldb < std::max(1, n))
            info = -8;
        if (info != 0)
            throw lcpp_exception("DGETRS", -info);
        // Quick return if possible
        if (n == 0 || nrhs == 0)
            return;
        LASWP<T> laswp;
        TRSM<T> trsm;
        T one = NUMCPP::CONSTANTS<T>::one;
        if (!tA) {
            //  Solve A* X = B.
            //    Apply row interchanges to the right hand sides.
            laswp(nrhs, b, ldb, 1, n, piv, 1);
            // Solve L * X = B, overwriting B with X.
            trsm(Side::Left, Triangular::Lower, false, false, n, nrhs,
                one, A, lda, B, ldb);
            // Solve U* X = B, overwriting B with X.
            trsm(Side::Left, Triangular::Upper, false, true, n, nrhs,
                one, A, lda, B, ldb);
        }
        else {
            trsm(Side::Left, Triangular::Upper, false, true, n, nrhs,
                one, A, lda, B, ldb);
            trsm(Side::Left, Triangular::Lower, false, false, n, nrhs,
                one, A, lda, B, ldb);
            laswp(nrhs, b, ldb, 1, n, piv, -1);
        }
    }
}

#endif