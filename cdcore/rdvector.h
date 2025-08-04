#ifndef __rdvector_h
#define __rdvector_h

#include "rd001.h"

//****************************************************************************
//
//	Templates  _vector<T> et Vector<T> .
//
//****************************************************************************

namespace NUMCPP {

	template<class T> class _vector;
	template<class T> class _svector;
	template<class T> class _svp;
	template<class T> class _cvector;

	class _vectorallocator
	{
	public:

		static void* alloc(size_t);
		static void free(void*);
	};

	template<class T>
	class _safeT
	{
	public:

		operator T& () { return t; }
		_safeT<T>& operator=(const T& u) { t = u; return *this; }

		T t;

		void* operator new[](size_t size) { return _vectorallocator::alloc(size); }
		void operator delete[](void* pt) { _vectorallocator::free(pt); }

	};

	template <class T>
	class _vector
	{
	public:

		//	Constructeurs / destructeurs

		_vector();
		explicit _vector(int sz);
		//	Alloue la place pour sz éléments
		_vector(const _vector<T>&);
		//	Constructeur de copie. L'allocateur du copieur peut différer de
		//	l'allocateur du copié!
		_vector(const _vector<T>&, int first, int sz);
		//	Constructeur de copie. L'allocateur du copieur peut différer de
		//	l'allocateur du copié!
		_vector(const T*, int sz);
		//	Copie les sz éléments contenus dans T*
		_vector(const T&, int sz);
		//	Copie sz fois l'élément donné par référence.

		~_vector();

		//  Opérateurs

		_vector<T>& operator=(const _vector<T>&);
		T* getPtr();
		//	Donne accès en Read/Write au données de l'objet.
		const T* getCPtr()const;
		//	Donne accès en Read au données de l'objet.

		T& operator[](int)const;
		//	donne accès en Read / Write à la donnée précisée dans l'indice.

//	Fonctions

		int length()const;
		//	Nombre d'éléments du vecteur.
		bool isValid()const;
		//	1 si la mémoire a été initialisée, 0 sinon.
		void captureData(_vector<T>& v);
		//	Prend les données de v, qui se retrouve vide.
		void alloc(int);
		//	Alloue la mémoire à la taille désirée, après avoir éventuellement
		//	nettoyé la mémoire existante.
		void free();
		//	Nettoie la mémoire existante (Size() == 0 après)


		// pour stl...
		bool operator==(const _vector<T>& v)const { return _data == v._data; }
		bool operator<(const _vector<T>& v)const { return _data < v._data; }

		//		template <class InputIterator, class T> friend void Init(InputIterator i0, int sz, _vector<T>&);

	protected:

		void _init(int);
		//	Initialisation de la mémoire dans les constructeurs.

		_safeT<T>* _data;
		int _size;

		friend _svp<T>;
		friend _svector<T>;
		friend _cvector<T>;
	};

	template<class T>
	inline _vector<T>::_vector() :_data(NULL), _size(0) {}

	template<class T>
	inline _vector<T>::_vector(int sz) { _init(sz); }

	template<class T>
	_vector<T>::_vector(const _vector<T>& v) :_data(0), _size(0)
	{
		if (v._size)
		{
			_init(v._size);
			for (int u = 0; u < _size; u++)
				_data[u] = v._data[u];
		}
	}

	template<class T>
	_vector<T>::_vector(const _vector<T>& v, int first, int sz)
	{
		_init(sz);
		for (int u = 0, w = first; u < _size; u++, w++)
			_data[u] = v._data[w];
	}

	template<class T>
	_vector<T>::_vector(const T* pt, int sz)
	{
		_init(sz);
		for (int u = 0; u < _size; u++)
			_data[u] = pt[u];
	}

	template<class T>
	_vector<T>::_vector(const T& t, int sz)
	{
		_init(sz);
		for (int i = 0; i < _size; i++)
			_data[i] = t;
	}

	template<class T>
	inline _vector<T>::~_vector()
	{
		if (_data != NULL)
			delete[] _data;
	}

	template<class T>
	inline T* _vector<T>::getPtr()
	{
		return reinterpret_cast<T*>(_data);
	}

	template<class T>
	inline const T* _vector<T>::getCPtr()const
	{
		return reinterpret_cast<T*>(_data);
	}


	template<class T>
	inline T& _vector<T>::operator[](int u)const
	{
		assert(u < _size);	return _data[u];
	}

	template<class T>
	_vector<T>& _vector<T>::operator=(const _vector<T>& v)
	{
		if (this != &v)
		{
			alloc(v._size);
			for (int u = 0; u < _size; u++)
				_data[u] = v._data[u];
		}
		return *this;
	}

	template<class T>
	inline bool _vector<T>::isValid()const
	{
		return _data != NULL;
	}

	template<class T>
	inline int _vector<T>::length()const
	{
		return _size;
	}

	template<class T>
	void _vector<T>::alloc(int sz)
	{
		if (_data != NULL)
			delete[] _data;
		_init(sz);
	}

	template<class T>
	void _vector<T>::free()
	{
		if (_data != NULL)
		{
			delete[] _data;
			_data = NULL; _size = 0;
		}
	}

	template<class T>
	void _vector<T>::_init(int sz)
	{
		_size = sz;
		if (_size)
		{
			_data = new _safeT<T>[sz];
			if (_data == 0)
				throw memException();
		}
		else
			_data = 0;
	}

	template<class T>
	void _vector<T>::captureData(_vector<T>& v)
	{
		if (_data != 0)
			delete[] _data;
		_size = v._size;
		_data = v._data;
		v._size = 0;
		v._data = 0;
	}

	///////////////////////////////////////////////////////////

	template <class InputIterator, class T>
	void initialize(InputIterator i0, int sz, _vector<T>& t)
	{
		t.alloc(sz);
		for (int u = 0; u < sz; ++u, ++i0)
			t[u] = *i0;
	}

	template <class InputIterator, class T>
	void initialize(InputIterator i0, _vector<T>& t)
	{
		int sz = t.length();
		for (int u = 0; u < sz; ++u, ++i0)
			t[u] = *i0;
	}

	template <class InputIterator, class T>
	void initialize(InputIterator i0, InputIterator i1, _vector<T>& t)
	{
		int sz = 0;
		InputIterator itmp = i0;
		while (itmp != i1)
		{
			++sz, ++itmp;
		}
		initialize(i0, sz, t);
	}


	//**************************************************************************
	//
	//	classes auxiliaires

	class _sharedvectorbase
	{
	public:

		_sharedvectorbase() { nrefs = 1; }
		~_sharedvectorbase() {}

		int addRef() { return ++nrefs; }
		int release() { return --nrefs; }
		int refCount() { return nrefs; }

	private:

		int nrefs;

	};

	template <class T>
	class _vectorrep : public _sharedvectorbase, public _vector<T>
	{
	private:

		_vectorrep();
		_vectorrep(int sz);
		_vectorrep(const _vector<T>& v);
		_vectorrep(const _vector<T>& v, int first, int size);
		_vectorrep(const T* pt, int sz);
		_vectorrep(const T& t, int sz);

		friend _svector<T>;
		friend _svp<T>;
		friend _cvector<T>;

	};

	template<class T>
	inline _vectorrep<T>::_vectorrep() {}

	template<class T>
	inline _vectorrep<T>::_vectorrep(int sz)
		:_vector<T>(sz) {}

	template<class T>
	inline _vectorrep<T>::_vectorrep(const _vector<T>& v)
		: _vector<T>(v) {}

	template<class T>
	inline _vectorrep<T>::_vectorrep(const _vector<T>& v, int first, int size)
		: _vector<T>(v, first, size) {}

	template<class T>
	inline _vectorrep<T>::_vectorrep(const T* pt, int sz)
		: _vector<T>(pt, sz) {}

	template<class T>
	inline _vectorrep<T>::_vectorrep(const T& t, int sz)
		: _vector<T>(t, sz) {}


	//**************************************************************************
	//
	//	La classes _cvector définie ci-dessous est la version avec
	//	compteurs de références de la classes définie plus haut.
	//	Il s'agit d'objets "read only".
	//	Ces classes permettent d'avoir plusieurs objets pointant vers les mêmes
	//	blocs de mémoire.
	//
	//**************************************************************************


	template <class T>
	class _cvector
	{
	public:

		_cvector() :rep(new _vectorrep<T>()) {}
		_cvector(const _vector<T>& v);
		_cvector(_vector<T>& v, bool capture);
		_cvector(const _cvector<T>& v);
		_cvector(const _vector<T>& v, int first, int size);
		_cvector(const _cvector<T>& v, int first, int size);
		_cvector(const T* pt, int sz);
		_cvector(const T& t, int sz);

		~_cvector() { if (rep->release() == 0) delete rep; }

		//  Opérateurs

		_cvector& operator=(const _cvector<T>&);
		const T* getCPtr()const { return reinterpret_cast<T*>(rep->_data); }
		operator const _vector<T>& ()const { return *rep; }
		const T& operator[](int u)const;

		//	Fonctions

		int length()const { return rep->length(); }
		bool isValid()const { return rep->isValid(); }

		// pour stl...
		bool operator==(const _cvector<T>& v)const { return rep == v.rep; }
		bool operator<(const _cvector<T>& v)const { return rep < v.rep; }

	protected:

		_vectorrep<T>* rep;

		friend _svector<T>;
	};

	template <class T>
	inline _cvector<T>::_cvector(const _vector<T>& v) :rep(new _vectorrep<T>(v)) {}

	template <class T>
	inline _cvector<T>::_cvector(_vector<T>& v, bool capture) : rep(new _vectorrep<T>())
	{
		assert(capture);
		rep->CaptureData(v);
	}

	template <class T>
	inline _cvector<T>::_cvector(const _vector<T>& v, int first, int size)
		:rep(new _vectorrep<T>(v, first, size)) {
	}

	template <class T>
	inline _cvector<T>::_cvector(const _cvector<T>& v, int first, int size)
		: rep(new _vectorrep<T>(*v.rep, first, size)) {
	}

	template <class T>
	inline _cvector<T>::_cvector(const T* pt, int sz)
	{
		rep = new _vectorrep<T>(pt, sz);
	}

	template <class T>
	inline _cvector<T>::_cvector(const T& t, int sz)
	{
		rep = new _vectorrep<T>(t, sz);
	}

	template <class T>
	inline _cvector<T>::_cvector(const _cvector<T>& v)
	{
		rep = v.rep; rep->addRef();
	}

	template <class T>
	_cvector<T>& _cvector<T>::operator=(const _cvector<T>& v)
	{
		if (rep != v.rep)
		{
			v.rep->addRef();
			if (rep->release() == 0) delete rep;
			rep = v.rep;
		}
		return *this;
	}

	template <class T>
	inline const T& _cvector<T>::operator[](int u)const
	{
		assert(u < rep->_size);
		return rep->_data[u];
	}



	//******************************************************************************

	template <class T>
	class _svector
	{
	public:

		friend class _svp<T>;

		_svector();
		explicit _svector(int sz);
		_svector(const _svector<T>& v);
		_svector(const _cvector<T>& v);
		_svector(const _vector<T>& v);
		_svector(const T* pt, int sz);
		_svector(const T& t, int sz);
		_svector(const _vector<T>& v, int first, int size);
		_svector(const _cvector<T>& v, int first, int size);
		_svector(const _svector<T>& v, int first, int size);

		~_svector() { if (rep->release() == 0) delete rep; }

		//  Opérateurs

		_svector& operator=(const _svector<T>&);
		T operator[](int u)const;
		_svp<T> operator[](int u);

		//	Fonctions

		int length()const { return rep->Size(); }
		bool isValid()const { return rep->isValid(); }
		void alloc(int u);
		void free();

		bool operator==(const _svector<T>& v)const { return rep == v.rep; }
		bool operator<(const _svector<T>& v)const { return rep < v.rep; }

	protected:

		_vectorrep<T>* rep;

		void clone();

	};

	template <class T>
	class _svp
	{
	public:

		friend class _svector<T>;

		operator T& ()const { return _sv.rep->_data[_index]; }
		_svp& operator=(const T& t);

	private:

		_svp(_svector<T>& sv, int index) :_sv(sv), _index(index) {}

		_svector<T>& _sv;
		int _index;

	};


	template <class T>
	inline _svector<T>::_svector() :rep(new _vectorrep<T>()) {}

	template <class T>
	inline _svector<T>::_svector(int sz) : rep(new _vectorrep<T>(sz)) {}

	template <class T>
	inline _svector<T>::_svector(const T* pt, int sz) : rep(new _vectorrep<T>(pt, sz)) {}

	template <class T>
	inline _svector<T>::_svector(const T& t, int sz) : rep(new _vectorrep<T>(t, sz)) {}

	template <class T>
	inline _svector<T>::_svector(const _vector<T>& v, int first, int size)
		: rep(new _vectorrep<T>(v, first, size)) {
	}

	template <class T>
	inline _svector<T>::_svector(const _svector<T>& v, int first, int size)
		: rep(new _vectorrep<T>(*v.rep, first, size)) {
	}

	template <class T>
	inline _svector<T>::_svector(const _cvector<T>& v, int first, int size)
		: rep(new _vectorrep<T>(*v.rep, first, size)) {
	}

	template <class T>
	inline _svector<T>::_svector(const _svector<T>& v)
	{
		rep = v.rep; rep->addRef();
	}

	template <class T>
	inline _svector<T>::_svector(const _cvector<T>& v)
	{
		rep = v.rep; rep->addRef();
	}

	template <class T>
	inline _svector<T>::_svector(const _vector<T>& v)
	{
		rep = new _vectorrep<T>(v);
	}

	template <class T>
	_svector<T>& _svector<T>::operator=(const _svector<T>& v)
	{
		if (rep != v.rep)
		{
			v.rep->addRef();
			if (rep->release() == 0) delete rep;
			rep = v.rep;
		}
		return *this;
	}

	template <class T>
	inline void _svector<T>::clone()
	{
		if (rep->refCount() > 1)
		{
			rep->release();
			rep = new _vectorrep<T>(*rep);
		}
	}

	template <class T>
	void _svector<T>::alloc(int u)
	{
		clone();
		rep->Alloc(u);
	}

	template <class T>
	void _svector<T>::free()
	{
		if (rep->refCount() > 1)
		{
			rep->release();
			rep = new _vectorrep<T>();
		}
		else
			rep->Free();
	}

	template <class T>
	inline _svp<T>& _svp<T>::operator=(const T& t)
	{
		_sv.clone(); _sv.rep->_data[_index] = t; return *this;
	}


	template <class T>
	T _svector<T>::operator[](int u)const
	{
		assert(u >= rep->_size);
		return rep->_data[u];
	}

	template <class T>
	_svp<T> _svector<T>::operator[](int u)
	{
		assert(u >= rep->_size);
		return _svp<T>(*this, u);
	}

}


#endif



