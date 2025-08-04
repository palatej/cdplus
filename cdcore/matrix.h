#ifndef __numcpp_matrix_h
#define __numcpp_matrix_h

#include <iostream>
#include <functional>
#include <random>
#include "sequence.h"

namespace NUMCPP {

	template<typename T>
	class Matrix;

	template <typename T>
	struct FastMatrix
	{

		T& operator()(int r, int c)const {
			return m_data[r + m_ldim * c];
		}

		~FastMatrix() {}

		int getNrows() const{
			return m_nrows;
		}

		int getNcols() const{
			return m_ncols;
		}

		int getColumnIncrement() const{
			return m_ldim;
		}

		const T* cptr() const{
			return m_data;
		}

		T* ptr()const {
			return m_data;
		}

		Sequence<T> row(int row) const {
			return Sequence<T>(m_data + row, m_data + row + m_ldim * m_ncols, m_ldim);
		}

		Sequence<T> column(int col) const {
			int start = col * m_ldim;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		Sequence<T> diagonal() const {
			int n = std::min(m_nrows, m_ncols), inc = 1 + m_ldim;
			return Sequence<T>(m_data, m_data + inc * n, inc);
		}

		Sequence<T> subDiagonal(int pos) const;

		SequenceIterator<T> rowsIterator() const {
			return SequenceIterator<T>(row(-1), m_nrows, 1);
		}

		SequenceIterator<T> columnsIterator() const{
			return SequenceIterator<T>(column(-1), m_ncols, m_ldim);
		}

		SequenceIterator<T> reverseRowsIterator() const {
			return SequenceIterator<T>(row(m_nrows), m_nrows, -1);
		}

		SequenceIterator<T> reverseColumnsIterator() const {
			return SequenceIterator<T>(column(m_ncols), m_ncols, -m_ldim);
		}

		bool isEmpty() const{
			return m_ncols == 0 || m_nrows == 0;
		}

		bool isSquare() const{
			return m_ncols == m_nrows;
		}

		FastMatrix<T> left(int n) const{
			return FastMatrix(m_data, m_ldim, m_nrows, n);
		}

		FastMatrix<T> right(int n) const {
			int nc = m_ncols - n;
			return FastMatrix(m_data+m_ldim*nc, m_ldim, m_nrows, n);
		}

		FastMatrix<T> top(int n) const{
			return FastMatrix(m_data, m_ldim, n, m_ncols);
		}

		FastMatrix<T> bottom(int n) const{
			int nr = m_nrows - n;
			return FastMatrix(m_data+nr, m_ldim, n, m_ncols);
		}

		FastMatrix<T> topLeft(int m, int n) const {
			return FastMatrix(m_data, m_ldim, m, n);
		}

		FastMatrix<T> bottomRight(int m, int n) const {
			int nc = m_ncols - n;
			int nr = m_nrows - n;
			return FastMatrix(m_data + m_ldim * nc+nr, m_ldim, m, n);
		}

		FastMatrix<T> extract(int r0, int nr, int c0, int nc)const {
			return FastMatrix(m_data + m_ldim * c0+r0, m_ldim, nr, nc);
		}

		void set(T value) const{
			set(m_data, m_ldim, m_nrows, m_ncols, value);
		}

		FastMatrix& bshrink() {
			m_data += m_ldim + 1;
			m_nrows--;
			m_ncols--;
			return *this;
		}

		FastMatrix& eshrink() {
			m_nrows--;
			m_ncols--;
			return *this;
		}

		FastMatrix& next(int nr, int nc) {
			m_data += m_ncols * m_ldim+m_nrows;
			m_nrows = nr;
			m_ncols = nc;
			return *this;
		}

		FastMatrix& hnext(int nc) {
			m_data += m_ncols * m_ldim;
			m_ncols = nc;
			return *this;
		}

		FastMatrix& vnext(int nr) {
			m_data += m_nrows;
			m_nrows = nr;
			return *this;
		}


		template <class Fn>
		void set(Fn fn)const;

		void mul(T value)const {
			mul(m_data, m_ldim, m_nrows, m_ncols, value);
		}

		static void set(T* C, int ldc, int m, int n, T value);

		static void mul(T* C, int ldc, int m, int n, T beta);


		template<typename S>
		friend std::ostream& operator<< (std::ostream& stream, const FastMatrix<S>& matrix);

	private:

		FastMatrix(T* data, int lda, int nrows, int ncols) :
			m_data(data), m_ldim(lda), m_nrows(nrows), m_ncols(ncols)
		{ }

		T* m_data;
		int  m_ldim, m_nrows, m_ncols;

		friend Matrix<T>;
	};

	template<typename T>
	class Matrix
	{
	public:

		Matrix();
		Matrix(int nrows, int ncols);

		template<class Fn>
		Matrix(int nrows, int ncols, Fn fn);

		Matrix(const Matrix<T>& matrix);

		Matrix<T>& operator=(const Matrix<T>& matrix);


		virtual ~Matrix();

		FastMatrix<T> all()const;

		FastMatrix<T> extract(int r0, int nr, int c0, int nc)const;

		int getNrows()const {
			return m_nrows;
		}

		int getNcols()const {
			return m_ncols;
		}

		T& operator()(int r, int c) const{
			return m_data[r + m_nrows * c];
		}

		template<class Fn>
		void set(Fn fn)const {
			FastMatrix<T> a = all();
			a.set(fn);
		}
		

		void rand()const;

		Matrix<T>& operator=(T x);

		int size()const;

		Sequence<T> row(int row)const {
			return Sequence<T>(m_data + row, m_data + row + m_nrows * m_ncols, m_nrows);
		}

		Sequence<T> column(int col)const {
			int start = col * m_nrows;
			return Sequence<T>(m_data + start, m_data + start + m_nrows);
		}

		template<typename S>
		friend Matrix<S> transpose(const FastMatrix<S>& M);

		template<typename S>
		friend std::ostream& operator<< (std::ostream& stream, const Matrix<S>& matrix);

	private:

		T* pos(int r, int c)const {
			return m_data + r + c * m_nrows;
		}

		T* m_data;
		int m_nrows, m_ncols;

	};

	template<typename T>
	inline Matrix<T>::Matrix() {
		m_data = NULL;
		m_nrows = 0;
		m_ncols = 0;
	}

	template<typename T>
	inline FastMatrix<T> Matrix<T>::all() const {
		return FastMatrix<T>(m_data, m_nrows, m_nrows, m_ncols);
	}

	template<typename T>
	inline FastMatrix<T> Matrix<T>::extract(int r0, int nr, int c0, int nc) const {
		return FastMatrix<T>(pos(r0, c0), m_nrows, nr, nc);
	}

	template<typename T>
	Sequence<T> FastMatrix<T>::subDiagonal(int pos) const {
		if (pos >= m_ncols) {
			return Sequence<T>();
		}
		if (-pos >= m_nrows) {
			return Sequence<T>();
		}
		int beg = 0, inc = 1 + m_ldim;
		int n;
		if (pos > 0) {
			beg += pos * m_ldim;
			n =std::min(m_nrows, m_ncols - pos);
		}
		else if (pos < 0) {
			beg -= pos;
			n = std::min(m_nrows + pos, m_ncols);
		}
		else {
			n = std::min(m_nrows, m_ncols);
		}
		return Sequence<T>(m_data+beg, m_data+beg + inc * n, inc);
	}

	template<typename T>
	inline int Matrix<T>::size()const {
		return m_nrows * m_ncols;
	}

	template<typename T>
	Matrix<T>::Matrix(int nrows, int ncols)
		:m_data(new T[nrows * ncols]), m_nrows(nrows), m_ncols(ncols)
	{
	}

	template<typename T>
	template<class Fn>
	Matrix<T>::Matrix(int nrows, int ncols, Fn op) 
		: m_data(new T[nrows * ncols]), m_nrows(nrows), m_ncols(ncols)
	{
		for (int c = 0, j = 0; c < ncols; ++c) {
			for (int r=0; r<nrows; ++r, ++j)
				m_data[j]=op(r,c);
		}
	}

	template<typename T>
	Matrix<T>::Matrix(const Matrix<T>& matrix)
	{
		int size = matrix.m_nrows * matrix.m_ncols;
		m_data = new T[size];
		for (int i = 0; i < size; ++i) {
			m_data[i] = matrix.m_data[i];
		}
		m_nrows = matrix.m_nrows;
		m_ncols = matrix.m_ncols;

	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
	{
		if (this != &matrix) {
			int size = matrix.m_nrows * matrix.m_ncols;
			if (nullptr != m_data)
				delete[] m_data;
			m_data = new T[size];
			for (int i = 0; i < size; ++i) {
				m_data[i] = matrix.m_data[i];
			}
			m_nrows = matrix.m_nrows;
			m_ncols = matrix.m_ncols;
		}
		return *this;
	}

	template<typename T>
	Matrix<T>::~Matrix() {
		if (nullptr != m_data)
			delete[] m_data;
	}

	template<>
	inline void Matrix<double>::rand()const {
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(1.0, 10.0);
		int sz = size();
		for (int u = 0; u < sz; ++u)
			m_data[u] = dist(mt);
	}

	template<typename T>
	Matrix<T>& Matrix<T>::operator=(T x)
	{
		int sz = size();
		for (int u = 0; u < sz; ++u)
			m_data[u] = x;
		return *this;
	}

	template<typename T>
	Matrix<T> transpose(const FastMatrix<T>& M)
	{
		int nr = M.getNrows(), nc = M.getNcols();
		Matrix<T> R(nc, nr);
		NUMCPP::SequenceIterator<T> rows = M.rowsIterator();
		T* ptr = R.m_data;
		while (rows.hasNext()) {
			rows.next().copyTo(ptr);
			ptr += nc;
		}
		return R;
	}


	template<typename T>
	std::ostream& operator<< (std::ostream& stream, const Matrix<T>& matrix) {
		return stream << matrix.all();
	}

	template<typename T>
	std::ostream& operator<< (std::ostream& stream, const FastMatrix<T>& matrix) {
		if (!matrix.isEmpty())
			for (int i = 0; i < matrix.m_nrows; ++i) {
				stream << matrix(i, 0);
				for (int j = 1; j < matrix.m_ncols; ++j)
					stream << '\t' << matrix(i, j);
				stream << "\n\r";
			}
		return stream;
	}

	template<typename T>
	void FastMatrix<T>::set(T* C, int ldc, int m, int n, T value) {
		T* cstart = C;
		T* const end = C + ldc * n;
		while (cstart != end) {
			T* cur = cstart;
			T* const cend = cur + m;
			while (cur != cend)
				*cur++ = value;
			cstart += ldc;
		}
	}

	template<typename T>
	template<class Fn>
	void FastMatrix<T>::set(Fn fn)const
	{
		T* data = m_data;
		for (int c = 0; c < m_ncols; ++c) {
			T* datac = data;
			for (int r = 0; r < m_nrows; ++r)
				*datac++ = fn(r, c);
			data += m_ldim;
		}
	}

	template<typename T>
	void FastMatrix<T>::mul(T* C, int ldc, int m, int n, T value) {
		if (value == NUMCPP::CONSTANTS<T>::one)
			return;
		if (value == NUMCPP::CONSTANTS<T>::zero) {
			set(m, n, value, C, ldc);
			return;
		}
		T* cstart = C;
		T* const end = C + ldc * n;
		while (cstart != end) {
			T* cur = cstart;
			T* const cend = cur + m;
			while (cur != cend)
				*cur++ *= value;
			cstart += ldc;
		}
	}

}

#endif
