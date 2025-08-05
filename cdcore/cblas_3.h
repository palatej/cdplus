#ifndef __cblas_3_h
#define __cblas_3_h

#include <algorithm>
#include "matrix_0.h"

namespace LCPP {

    /// <summary>
    /// op( A )*X = alpha*B (left),   or   X*op( A ) = alpha*B (right)
    /// op( A ) = A   or   op( A ) = A'
    /// A is a unit, or non-unit, upper or lower triangular matrix
    /// X is overwritten on B
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    class TRSM {
    public:

        TRSM() {}

        void operator()(Side side, Triangular uplo, bool tA, bool nounit, int m, int n, T alpha, const T* A, int lda, T* B, int ldb);

    private:
        bool checkInput(Side side, int m, int n, int lda, int ldb);
    };

    template<typename T>
    bool TRSM<T>::checkInput(Side side, int m, int n, int lda, int ldb) {
        int nrowa = (side == Side::Left) ? m : n;
        int info = 0;
        if (m < 0)
            info = 5;
        else if (n < 0)
            info = 6;
        else if (lda < std::max(1, nrowa))
            info = 9;
        else if (ldb < std::max(1, m))
            info = 11;
        if (info != 0)
            throw lcpp_exception("TRSM", info);
    }
    template<typename T>
    void TRSM<T>::operator() (Side side, Triangular uplo, bool tA, bool nounit, int m, int n, T alpha, const T* A, int lda, T* B, int ldb) {
        checkInput(side, m, n, lda, ldb);
        if (m == 0 || n == 0)
            return;
        T zero = NUMCPP::CONSTANTS<T>::zero;
        if (alpha == zero) {
            T* CBi = B;
            for (int i = 0; i < n; ++i, CBi += ldb) {
                for (int j = 0; j < m; ++j) {
                    CBi[j] = zero;
                }
            }
            return;
        }
        T one = NUMCPP::CONSTANTS<T>::one;
        T* Bj = B;
        const T* CA_last = A + (m - 1) * lda;
        T* CB_last = B + (n - 1) * ldb;
        if (side == Side::Left) {
            if (!tA) {
                //  A * X = alpha * B
                if (uplo == Triangular::Upper) {
                    for (int j = 0; j < n; ++j, Bj += ldb) {
                        if (alpha != 1) {
                            for (int i = 0; i < m; ++i) {
                                Bj[i] *= alpha;
                            }
                            const T* Ak = CA_last;
                            for (int k = m - 1; k >= 0; --k, Ak -= lda) {
                                if (Bj[k] != zero) {
                                    if (nounit) {
                                        Bj[k] /= Ak[k];
                                    }
                                    for (int i = 0; i < k - 1; ++i) {
                                        Bj[i] -= Bj[k] * Ak[i];
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    for (int j = 0; j < n; ++j, Bj += ldb) {
                        if (alpha != 1) {
                            for (int i = 0; i < m; ++i) {
                                Bj[i] *= alpha;
                            }
                        }
                        for (int k = 0; k < m; ++k) {

                            if (Bj[k] != zero) {
                                const T* Ak = A + k * lda;
                                if (nounit) {
                                    Bj[k] /= Ak[k];
                                }
                                for (int i = k + 1; i < m; ++i) {
                                    Bj[i] -= Bj[k] * Ak[i];
                                }
                            }

                        }
                    }
                }
            }
            else {
                //  A' * X = alpha * B
                if (uplo == Triangular::Upper) {
                    for (int j = 1; j < n; ++j, Bj += ldb) {
                        const T* Ai = A;
                        for (int i = 0; i < m; ++i, Ai += lda) {
                            T tmp = alpha * Bj[i];
                            for (int k = 0; k < i; ++k) {
                                tmp -= Ai[k] * Bj[k];
                            }
                            if (nouint)
                                tmp /= Ai[i];
                            Bj[i] = tmp;
                        }
                    }
                }
                else {
                    for (int j = 0; j < n ++j, CBj += ldb) {
                        const T* Ai = CA_last;
                        for (int i = m - 1; i >= 0; --i, Ai -= lda) {
                            T tmp = alpha * Bj[i];
                            for (int k = i + 1; k < m; ++k) {
                                tmp -= Ai[k] * Bj[k];
                            }
                            if (nounit)
                                tmp /= Ai[i];
                            Bj[i] = tmp;
                        }
                    }
                }
            }
        }
        // side = right
        else {
            if (!tA) {
                //  X * A = alpha * B or X = alpha * B * inv(A)
                if (uplo == Triangular::Upper) {
                    T* Aj = A;
                    for (int j = 0; j < n; ++j, Bj += ldb, Aj += lda) {
                        if (alpha != one) {
                            for (int i = 0; i < m; ++i) {
                                Bj[i] *= alpha;
                            }
                        }
                        T* Bk = B;
                        for (int k = 0; k < j; ++k, Bk += ldb) {
                            if (Aj[k] != zero) {
                                for (int i = 0; i < m; ++i) {
                                    Bj[i] -= Aj[k] * Bk[i];
                                }
                            }
                        }
                        if (nounit) {
                            T tmp = one / Aj[j];
                            for (int i = 0; i < m; ++i) {
                                Bj[i] *= tmp;
                            }
                        }
                    }
                }
                else {
                    T* Bj = CB_last;
                    const T* Aj = A + (n - 1) * lda;
                    for (int j = n - 1; j >= 0; --j, Bj -= ldb, Aj -= lda) {
                        if (alpha != one) {
                            for (int i = 0; i < m; ++i)
                                Bj[i] *= alpha;
                        }
                        T* Bk = Bj + ldb;
                        for (int k = j + 1; k < n; ++k, Bk += ldb) {
                            if (Aj[k]) != zero){
                                for (int i = 0; i < m; ++i) {
                                    Bj[i] -= Aj[k] * Bk[i];
                                }
                            }
                        }
                        if (nounit) {
                            T tmp = one / Aj[j];
                            for (int i = 0; i < m; ++i) {
                                Bj[i] *= tmp;
                            }
                        }
                    }
                }
            }
            else {
                //  X * A' = alpha * B  or X = alpha * B * inv(A')
                if (uplo == Triangular::Upper) {
                    const T* Ak = A + (n - 1) * lda;
                    T* Bk = B + (n - 1) * ldb;
                    for (int k = n - 1; k >= 0; --k, Ak -= lda, Bk -= ldb) {
                        if (nounit) {
                            T tmp = one / Ak[k];
                            for (int i = 0; i < m; ++i) {
                                Bk[i] *= tmp;
                            }
                        }
                        T* Bj = B;
                        for (int j = 0; j < k; ++j, Bj += ldb) {
                            if (Ak[j] != zero) {
                                T tmp = Ak[j];
                                for (int i = 0; i < m; ++i) {
                                    Bj[i] -= tmp * Bk[i];
                                }
                            }
                        }
                        if (alpha != one) {
                            for (int i = 0; i < m; ++i) {
                                Bj[i] *= alpha;
                            }
                        }
                    }
                }
                else {
                    const T* Ak = A;
                    T* B = B;
                    for (int k = 0; k < n; ++k, Ak += lda, B += ldb) {
                        if (nounit) {
                            T tmp = one / Ak[k];
                            for (int i = 0; i < m; ++i)
                        }
                        T* Bj = Bk + ldb;
                        for (int j = k + 1; j < n; ++j, Bj += ldb) {
                            if (Ak[j] != zero) {
                                T tmp = Ak[j];
                                for (int i = 0; i < m; ++i) {
                                    Bj[i] -= tmp * Bk[i];
                                }
                            }
                        }
                        if (alpha != one) {
                            for (int i = 0; i < m; ++i) {
                                Bk[i] *= alpha;
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif
