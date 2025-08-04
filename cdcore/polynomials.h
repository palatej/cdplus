#ifndef __polynomials_h
#define __polynomials_h

#include "rdvector.h"
#include <sequence.h>
#include <ostream>
#include <complex>

namespace NUMCPP {

    class Polynomial {
    public:

        static const Polynomial ZERO, ONE;

        Polynomial() {
        }

        Polynomial(const Polynomial&);

        Polynomial(int degree, double val);
        Polynomial(int degree, double* pval);

        ~Polynomial();

        Polynomial& operator=(const Polynomial&);

        bool isValid()const {
            return m_c.isValid();
        }

        int getDegree()const { return m_c.length()-1; }

        double operator[](int i) const {
            return m_c[i];
        }

        Polynomial operator*(const Polynomial& r)const;
        Polynomial operator+(const Polynomial& r)const;
        Polynomial operator-(const Polynomial& r)const;
        Polynomial operator+(double a)const;
        Polynomial operator-(double a)const;
        Polynomial operator*(double a)const;
        Polynomial operator/(double a)const;
        Polynomial operator+=(double a){
            return *this + a;
        }
        Polynomial operator-= (double a){
            return *this - a;
        }
        Polynomial operator*=(double a) {
            return *this * a;
        }
        Polynomial operator/=(double a) {
            return *this / a;
        }

        double evaluateAt(double x)const;
        std::complex<double> evaluateAt(const std::complex<double>& x)const;
        std::complex<double> evaluateAtFrequency(double w)const;

        void rationalFunctionExpansion(const Polynomial& denom, int n, double* buffer)const;

        friend std::ostream& operator<<(std::ostream& os, const Polynomial& P) {
            int d = P.getDegree();

            os << P[0];
            for (int i = 1; i <= d; ++i) {
                os << '\t' << P[i];
            }
            return os;
        }

    private:

        _cvector<double> m_c;
    };

	class Polynomials {

    public:

		template <typename T>
		static DataBlock<T> times(const Sequence<T>& l, const Sequence<T>& r);
	};

	template <typename T>
	DataBlock<T> Polynomials::times(const Sequence<T>& l, const Sequence<T>& r) {
        int nl = l.length(), nr = r.length();
        int d = nl + nr - 1;
        DataBlock<T> result(d, CONSTANTS<T>::zero);
        for (int i = 0; i < d; ++i)
            result(i) = 0;
        auto lbeg=l.cbegin(), lend = l.cend();
        auto rend = r.cend();
        int il = 0;
        while (lbeg != lend) {
            T lcur = *lbeg++;
            if (lcur != CONSTANTS<T>::zero) {
                auto rbeg = r.cbegin();
                int ir = 0;
                while (rbeg != rend) {
                    T rcur = *rbeg++;
                    if (rcur != CONSTANTS<T>::zero) {
                        result(il+ir) += lcur * rcur;
                    }
                    ++ir;
                }

            }
            ++il;
        }
        return result;

	}

   

}

#endif
