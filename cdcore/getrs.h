#ifndef __lcpp_getrs_h
#define __lcpp_getrs_h

#include "matrix.h"

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

        void operator()(bool tA, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B);
    };

    
    template <typename T>
    void GETRS<T>::operator()(bool tA, NUMCPP::FastMatrix<T>& A, NUMCPP::FastMatrix<T>& B) {

    }

}

#endif