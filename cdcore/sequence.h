#ifndef __sequence_h
#define __sequence_h

#include <iostream>
#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <iterator>
#include <cstddef>  
#include "constants.h"

namespace NUMCPP {

    enum Fill {
        None, Sum, NegSum, Zero
    };

    template <typename T>
    struct SequenceIterator;

    /// <summary>
    /// Sequence of elements of type T that cannot be modified
    /// To be noted that the sequence itself can be modified (not its items)
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T>
    struct Sequence
    {
        struct ConstIterator {

            using iterator_category = std::input_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = const T*;
            using reference = const T&;

            ConstIterator(pointer ptr, int inc) :mPtr(ptr), mInc(inc) {}

            pointer operator->()const {
                return mPtr;
            }

            reference operator*()const {
                return *mPtr;
            }

            //++T
            ConstIterator operator++() {
                mPtr += mInc;
                return *this;
            }

            //T++
            ConstIterator operator++(int) {
                ConstIterator tmp = *this;
                mPtr += mInc;
                return tmp;
            }

            friend bool operator==(const ConstIterator& l, const ConstIterator& r) {
                return l.mPtr == r.mPtr;
            }

            friend bool operator!=(const ConstIterator& l, const ConstIterator& r) {
                return l.mPtr != r.mPtr;
            }

        private:

            pointer mPtr;
            int mInc;
        };


        struct Iterator {

            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;


            Iterator(pointer ptr, int inc) :mPtr(ptr), mInc(inc) {}

            pointer operator->()const {
                return mPtr;
            }

            reference operator*()const {
                return *mPtr;
            }

            //++T
            Iterator operator++() {
                mPtr += mInc;
                return *this;
            }

            //T++
            Iterator operator++(int) {
                Iterator tmp = *this;
                mPtr += mInc;
                return tmp;
            }

            friend bool operator==(const Iterator& l, const Iterator& r) {
                return l.mPtr == r.mPtr;
            }

            friend bool operator!=(const Iterator& l, const Iterator& r) {
                return l.mPtr != r.mPtr;
            }

        private:

            pointer mPtr;
            int mInc;
        };


        Sequence() :m_data(nullptr), m_inc(0), m_n(0) {

        }

        Sequence(T* p0, T* p1, int inc) :m_data(p0), m_inc(inc) {
            m_n = (p1 - p0) / inc;
        }
        Sequence(T* p0, T* p1) :m_data(p0), m_inc(1), m_n(p1 - p0) {
        }

        Sequence(T* p0, int n, int inc) :m_data(p0), m_inc(inc), m_n(n) {
        }

        Sequence(T* p0, int n) :m_data(p0), m_inc(1), m_n(n) {
        }

        bool isEmpty() const {
            return m_n == 0;
        }

        Iterator begin()const {
            return Iterator(m_data, m_inc);
        }

        Iterator end()const {
            return Iterator(m_data + m_inc * m_n, m_inc);
        }

        ConstIterator cbegin() const {
            return ConstIterator(m_data, m_inc);
        }

        ConstIterator cend() const {
            return ConstIterator(m_data + m_inc * m_n, m_inc);
        }

        Sequence<T> left(int n)const {
            return Sequence<T>(m_data, n, m_inc);
        }

        Sequence<T> right(int n)const {

            return Sequence<T>(m_data + m_inc * (m_n - n), n, m_inc);
        }

        Sequence<T> drop(int nl, int nr)const {
            int nc = nl + nr;
            if (nc >= m_n)
                return Sequence();
            return Sequence<T>(m_data + m_inc * nl, m_n - nc, m_inc);
        }

        Sequence<T> extend(int nl, int nr)const {
            return drop(-nl, -nr);
        }

        Sequence<T> extract(int start, int n)const {
            int nc = start + n;
            if (nc >= m_n)
                return Sequence();
            return Sequence<T>(m_data + m_inc * start, n, m_inc);
        }

        Sequence<T> reverse() const {
            return Sequence<T>(m_data + (m_n - 1) * m_inc, m_n, -m_inc);
        }

        T& operator()(int idx)const {
            return *(m_data + idx * m_inc);
        }

        void copy(Sequence<T> src)const;

        T dot(Sequence<T> src)const;

        void rand()const;

        int length() const {
            return m_n;
        }

        int increment() const {
            return m_inc;
        }

        T* start()const {
            return m_data;
        }

        const T* cstart() const {
            return m_data;
        }

        void copyTo(T* buffer)const;

        void mul(T value) const;

        void div(T value, bool fast = true) const;

        void add(T value)const;

        const Sequence<T>& operator +=(T value)const {
            add(value);
            return *this;
        }

        const Sequence<T>& operator -=(T value)const {
            add(-value);
            return *this;
        }

        const Sequence<T>& operator *=(T value)const {
            mul(value);
            return *this;
        }

        const Sequence<T>& operator /=(T value)const {
            div(value);
            return *this;
        }

        void addAY(T a, Sequence<T> Y)const;

        void set(T value)const;

        void chs()const;

        T asum()const;

        T sum()const;

        T ssq()const;

        int imax()const;

        T max()const;

        int imin()const;

        T min()const;

        template <class Fn>
        T accumulate(Fn fn)const;

        Sequence<T>& slide(int del);

        Sequence<T>& bexpand() {
            m_data -= m_inc;
            ++m_n;
            return *this;
        }

        Sequence<T>& eexpand() {
            ++m_n;
            return *this;
        }

        Sequence<T>& bshrink() {
            m_data += m_inc;
            --m_n;
            return *this;
        }

        Sequence<T>& eshrink() {
            --m_n;
            return *this;
        }

        Sequence<T>& shrink(int nbeg, int nend) {
            m_data += m_inc * nbeg;
            m_n -= nbeg + nend;
            return *this;
        }

        Sequence<T>& move(int n) {
            m_data += m_inc * n;
            return *this;
        }

        Sequence<T>& next(int n) {
            m_data += m_inc * m_n;
            m_n = n;
            return *this;
        }

        Sequence<T>& previous(int n) {
            m_data -= m_inc * n;
            m_n = n;
            return *this;
        }

        void swap(Sequence<T> other)const;

        template <class Fn>
        void apply(Fn fn)const;

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, Sequence<S> seq);

        static void mul(int n, T value, T* x, int incx);

        static void add(int n, T value, T* x, int incx);

        static void set(int n, T value, T* x, int incx);

    private:

        T* m_data;
        int m_inc;
        int m_n;

    };


    template <typename T>
    inline Sequence<T>& Sequence<T>::slide(int del) {
        m_data += del;
        return *this;
    }

    template <typename T>
    struct SequenceIterator {

        SequenceIterator(const Sequence<T>& start, int niter, int inc)
            :m_data(start), m_end(niter), m_inc(inc), m_pos(0) {
        }

        bool hasNext()const {
            return m_pos < m_end;
        }

        Sequence<T> next() {
            m_pos++;
            m_data.slide(m_inc);
            return m_data;
        }

        void reset(int newpos) {
            int del = m_pos - newpos;
            if (del != 0) {
                del *= m_inc;
                m_data.slide(-del);
                m_pos = newpos;
            }
        }

        void begin() {
            reset(0);
        }

    private:

        Sequence<T> m_data;
        int m_end, m_inc, m_pos;

    };


    template <typename T>
    class DataBlock
    {
    public:

        DataBlock() {
            m_data = NULL;
            m_size = 0;
        }

        DataBlock(int n) {
            m_data = new T[n];
            m_size = n;
        }

        DataBlock(int n, T x);

        DataBlock(int n, T* px);

        DataBlock(int n, std::function<T(int)> fn);

        DataBlock(const DataBlock<T>& x);

        DataBlock(const Sequence<T>& x);

        DataBlock<T>& operator=(const DataBlock<T>& x);

        Sequence<T> all() {
            return Sequence<T>(m_data, m_size);
        }

        int length() {
            return m_size;
        }

        T& operator()(int idx) {
            return m_data[idx];
        }

        void rand();

        virtual ~DataBlock();

        template<typename S>
        friend std::ostream& operator<< (std::ostream& stream, DataBlock<S>& seq);


    private:

        T* m_data;
        int m_size;
    };

    template<typename T>
    DataBlock<T>::DataBlock(int n, T* px) {
        m_data = new T[n];
        m_size = n;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = px[u];
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(int n, T x) {
        m_data = new T[n];
        m_size = n;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = x;
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(int n, std::function<T(int)> fn) {
        m_data = new T[n];
        m_size = n;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = fn(u);
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(const DataBlock<T>& x) {
        m_data = new T[x.m_size];
        m_size = x.m_size;
        for (int u = 0; u < m_size; ++u) {
            m_data[u] = x.m_data[u];
        }
    }

    template<typename T>
    DataBlock<T>::DataBlock(const Sequence<T>& x) {
        m_size = x.length();
        m_data = new T[m_size];
        auto cur = x.cbegin();
        auto end = x.cend();
        int i = 0;
        while (cur != end) {
            m_data[i] = *cur++;
        }
    }

    template<typename T>
    DataBlock<T>& DataBlock<T>::operator=(const DataBlock<T>& x) {
        if (this != &x) {
            delete[] m_data;
            m_data = new T[x.m_size];
            m_size = x.m_size;
            for (int u = 0; u < m_size; ++u) {
                m_data[u] = x.m_data[u];
            }
        }
        return *this;
    }


    template<typename T>
    DataBlock<T>::~DataBlock() {
        delete[] m_data;
    }

    template<typename T>
    inline void Sequence<T>::copy(Sequence<T> src)const
    {
        if (m_n == 0)
            return;
        T* x = m_data;
        T* y = src.m_data;
        T* const e = x + m_inc * m_n;
        int yinc = src.m_inc;
        while (x != e) {
            *x = *y;
            y += yinc;
            x += m_inc;
        }
    }

    template<typename T>
    inline void Sequence<T>::swap(Sequence<T> other)const {
        if (m_n == 0)
            return;
        T* x = m_data;
        T* y = other.m_data;
        T* const e = x + m_inc * m_n;
        int oinc = other.m_inc;
        while (x != e) {
            T tmp = *x;
            *x = *y;
            *y = tmp;
            y += oinc;
            x += m_inc;
        }
    }

    template<typename T>
    void NUMCPP::Sequence<T>::copyTo(T* buffer) const {
        if (m_n == 0)
            return;
        T* x = m_data;
        T* const e = x + m_inc * m_n;
        while (x != e) {
            *buffer++ = *x;
            x += m_inc;
        }
    }


    template<typename T>
    std::ostream& operator<< (std::ostream& stream, DataBlock<T>& data) {
        return stream << data.all();
    }


    template<>
    inline void Sequence<double>::rand() const {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(0, 1);
        double* x = m_data, * const e = x + m_inc * m_n;
        while (e != x) {
            *x = dist(mt);
            x += m_inc;
        }
    }

    template<>
    inline void DataBlock<double>::rand() {
        all().rand();
    }

    template<typename T>
    void Sequence<T>::mul(int n, T value, T* x, int incx) {
        if (value == NUMCPP::CONSTANTS<T>::one)
            return;
        if (value == NUMCPP::CONSTANTS<T>::zero) {
            set(n, value, x, incx);
            return;
        }
        int imax = incx * n;
        for (int i = 0; i != imax; i += incx)
            x[i] *= value;
    }

    template<typename T>
    void Sequence<T>::mul(T value)const {
        if (value == NUMCPP::CONSTANTS<T>::one)
            return;
        if (value == NUMCPP::CONSTANTS<T>::zero) {
            set(value);
            return;
        }
        T* x = m_data, * const e = x + m_inc * m_n;
        while (e != x) {
            *x *= value;
            x += m_inc;
        }
    }

    template<typename T>
    void Sequence<T>::add(T value)const {
        if (value == NUMCPP::CONSTANTS<T>::zero)
            return;
        T* x = m_data, * const e = x + m_inc * m_n;
        while (e != x) {
            *x += value;
            x += m_inc;
        }
    }

    template<typename T>
    void Sequence<T>::add(int n, T value, T* x, int incx) {
        if (value == NUMCPP::CONSTANTS<T>::zero)
            return;
        int imax = incx * n;
        for (int i = 0; i != imax; i += incx)
            x[i] += value;
    }

    template<typename T>
    void Sequence<T>::set(int n, T value, T* x, int incx) {
        int imax = incx * n;
        for (int i = 0; i != imax; i += incx)
            x[i] = value;
    }

    template<typename T>
    void Sequence<T>::set(T value)const {
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc)
            m_data[i] = value;
    }

    template<typename T>
    void Sequence<T>::addAY(T a, Sequence<T> Y) const {
        if (a == NUMCPP::CONSTANTS<T>::zero)
            return;
        int imax = m_inc * m_n;
        for (int i = 0, j = 0; i != imax; i += m_inc, j += Y.m_inc) {
            m_data[i] += a * Y.m_data[j];
        }
    }

    template<typename T>
    inline T Sequence<T>::asum()const {
        return accumulate([](T s, T cur) {return s + std::abs(cur); });
    }

    template<typename T>
    inline T Sequence<T>::sum()const {
        T s = NUMCPP::CONSTANTS<T>::zero;
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            s += m_data[i];
        }
        return s;
    }

    template<typename T>
    T Sequence<T>::ssq()const {
        T s = NUMCPP::CONSTANTS<T>::zero;
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            T cur = m_data[i];
            s += cur * cur;
        }
        return s;
    }

    template<typename T>
    T Sequence<T>::dot(Sequence<T> Y)const {
        T s = NUMCPP::CONSTANTS<T>::zero;
        T* x = m_data, * y = Y.m_data, * const e = x + m_inc * m_n;
        while (e != x) {
            s += *x * *y;
            x += m_inc;
            y += Y.m_inc;
        }
        return s;
    }

    template<typename T>
    template<class Fn>
    T Sequence<T>::accumulate(Fn fn) const {
        T s = NUMCPP::CONSTANTS<T>::zero;
        int incx = m_inc;
        T* x = m_data, * const e = x + incx * m_n;
        while (e != x) {
            s = fn(s, *x);
            x += incx;
        }
        return s;
    }

    template<typename T>
    void Sequence<T>::div(T value, bool fast)const {
        T one = NUMCPP::CONSTANTS<T>::one;
        if (one == value)
            return;
        int incx = m_inc;
        T* x = m_data, * const e = x + incx * m_n;
        if (fast) {
            T inv = one / value;
            while (e != x) {
                *x *= inv;
                x += incx;
            }
        }
        else
            while (e != x) {
                *x /= value;
                x += incx;
            }
    }

    template<typename T>
    template<class Fn>
    void Sequence<T>::apply(Fn fn)const {
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            m_data[i] = fn(m_data[i]);
        }
    }

    template<typename T>
    void Sequence<T>::chs() const {
        int imax = m_inc * m_n;
        for (int i = 0; i != imax; i += m_inc) {
            m_data[i] = -m_data[i];
        }
    }

    template<typename T>
    int Sequence<T>::imax() const {
        if (isEmpty())
            return -1;
        if (m_n == 1)
            return 0;
        int nmax = m_inc * m_n;
        T cmax = m_data[0];
        int imax = 0;
        for (int i = 1; i != nmax; i += m_inc) {
            T cur = m_data[i];
            if (cur > cmax) {
                cmax = cur;
                imax = i;
            }
        }
        return imax;
    }

    template<typename T>
    T Sequence<T>::max() const {
        if (isEmpty())
            return NUMCPP::CONSTANTS<T>::zero;
        if (m_n == 1)
            return m_data[0];
        int nmax = m_inc * m_n;
        T cmax = m_data[0];
        for (int i = 1; i != nmax; i += m_inc) {
            T cur = m_data[i];
            if (cur > cmax) {
                cmax = cur;
            }
        }
        return cmax;
    }

    template<typename T>
    int Sequence<T>::imin() const {
        if (isEmpty())
            return -1;
        if (m_n == 1)
            return 0;
        int nmax = m_inc * m_n;
        T cmin = m_data[0];
        int imin = 0;
        for (int i = 1; i != nmax; i += m_inc) {
            T cur = m_data[i];
            if (cur > cmin) {
                cmin = cur;
                imin = i;
            }
        }
        return imin;
    }

    template<typename T>
    T Sequence<T>::min() const {
        if (isEmpty())
            return NUMCPP::CONSTANTS<T>::zero;
        if (m_n == 1)
            return m_data[0];
        int nmax = m_inc * m_n;
        T cmin = m_data[0];
        for (int i = 1; i != nmax; i += m_inc) {
            T cur = m_data[i];
            if (cur < cmin) {
                cmin = cur;
            }
        }
        return cmin;
    }

    template<typename T>
    std::ostream& operator<< (std::ostream& stream, Sequence<T> seq) {
        if (seq.isEmpty())
            return stream;
        auto p = seq.cbegin(), e = seq.cend();
        stream << *p++;
        while (p != e)
            stream << '\t' << *p++;
        return stream;
    }


}
#endif
