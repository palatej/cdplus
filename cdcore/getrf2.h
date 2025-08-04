#ifndef __lcpp_getrf2_h
#define __lcpp_getrf2_h

#include "matrix.h"

namespace LCPP {

    /// <summary>
    /// DGETRF2 computes an LU factorization of a general MxN matrix A
    /// using partial pivoting with row interchanges.
    /// The factorization has the form A = P * L* U
    /// where P is a permutation matrix, L is lower triangular with unit
    /// diagonal elements(lower trapezoidal if m > n), and U is upper
    /// triangular(upper trapezoidal if m < n).
    ///  
    /// This is the recursive version of the algorithm.It divides
    /// the matrix into four submatrices :
    ///     [A11|A12] where A11 is n1 by n1 and A22 is n2 by n2
    /// A = [---|---]  with n1 = min(m, n) / 2
    ///     [A21|A22]       n2 = n - n1
    /// 
    ///                                      [A11]
    /// The subroutine calls itself to factor[---],
    ///                                      [A21]
    ///                 [A12]
    /// do the swaps on [---], solve A12, update A22,
    ///                 [A22]
    /// then calls itself to factor A22 and do the swaps on A21.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class GETRF2 {
    public:

        GETRF2() { }

        void operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots);

        void operator() (int m, int n, T* A, int lda, int* piv);

        int info() {
            return m_info;
        }

    private:

        int m_info;

    };

    template<typename T>
    void GETRF2<T>::operator()(NUMCPP::FastMatrix<T> A, NUMCPP::Sequence<T> pivots) {
        m_info = 0;
        if (A.isEmpty())
            return;
        int m = A.getNrows(), n = A.getNcols();
        if (m == 1) {
            pivots(0) = 0;
            if (A(0, 0) == 0)
                m_info=1;
        }
        else if (n == 1) {
          //Use unblocked code for one column case
            T sfmin = NUMCPP::CONSTANTS<T>::safe_min;
            T zero = NUMCPP::CONSTANTS<T>::zero, one = NUMCPP::CONSTANTS<T>::one;
            // Find pivot and test for singularity
            NUMCPP::Sequence<T>col = A.column(0);
            int imax = col.imax();
            pivots(0) = imax;
            T cmax = col(imax);
            if (cmax != zero) {
                if (imax != 0) {
                    col(imax) = col(0);
                    col(0) = cmax;
                }
                col.bshrink();
                col.div(cmax, std::abs(cmax) >= sfmin);
            }
            else
                m_info = 1;
        }
        else {
            // recursive code
            int n1 = std::min(m, n) / 2;
            int n2 = n - n1;
            GETRF2<T> rgtrf2;
            NUMCPP::FastMatrix A1=A.left(n1);
            rgtrf2(A1, pivots.left(n1));
            if (m_info == 0 && rgtrf2.m_info > 0)
                m_info = rgtrf2.m_info;
        }
    }

    template<typename T>
    void GETRF2<T>::operator() (int m, int n, T* A, int lda, int* piv ) {
        m_info = 0;
        if (m < 0)
            m_info = -1;
        else if (n < 0)
            m_info = -2;
        else if (lda < std::max(1, m))
            m_info = -4;
        if (m_info != 0)
            throw std::exception();
    }
}

#endif
