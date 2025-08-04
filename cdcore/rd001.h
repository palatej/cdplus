#ifndef __rd001_h
#define __rd001_h

#include <string>
#include <stdexcept>
#include <assert.h>

class rdException: public std::exception
{
public:

	rdException(){}
	rdException(const std::string& s):m_what(s){}
	rdException(const char*  ps):m_what(ps){}
	virtual const char *what() const throw();
protected:

	std::string m_what;

};


class memException: public std::exception
{
public:

	memException(){}
	virtual const char *what() const throw(){ return g_szMsg; }    

	static void initMsg();

private:

	static char g_szMsg[32];

};

class safeptrException: public rdException
{
public:

	safeptrException() {}
};

class RefCount
{

public:

	unsigned GetRefs()const;

	virtual unsigned AddRef()const;
	virtual unsigned Release()const;

	class Lock
	{
	public:

		Lock(const RefCount* prc):m_prc(prc){ if (m_prc) m_prc->AddRef(); }
		~Lock(){if (m_prc)m_prc->Release();}

		const RefCount* m_prc;
	};

protected:

	RefCount();
	virtual ~RefCount();

	virtual void DeleteObject()const;

private:

	RefCount(const RefCount&);
	RefCount& operator=(const RefCount&);
	
	mutable unsigned m_uRefs;
};

inline RefCount::RefCount()
{m_uRefs=0; }

inline RefCount::~RefCount()
{}

inline unsigned
RefCount::GetRefs()const
{ return m_uRefs;}


inline unsigned
RefCount::AddRef()const{return ++m_uRefs;}

inline unsigned
RefCount::Release()const
{
	assert(m_uRefs);
	unsigned rslt=--m_uRefs;
	if (rslt == 0)
		DeleteObject();
	return rslt;
}

inline void
RefCount::DeleteObject()const
{ delete this; }

///////////////////////////////////////////////

class LockCount
{

public:

	unsigned GetLocks()const;

	virtual unsigned LockData(unsigned n=1);
	virtual unsigned UnlockData(unsigned n=1);

	class DataLock
	{
	public:

		DataLock(LockCount* prc):m_prc(prc){ if (m_prc) m_prc->LockData(); }
		~DataLock(){if (m_prc)m_prc->UnlockData();}

		LockCount* m_prc;
	};

protected:

	LockCount();
	virtual ~LockCount();

	virtual void LoadData();
	virtual void FreeData();

private:

	LockCount(const LockCount&);
	LockCount& operator=(const LockCount&);
	
	unsigned m_uRefs;
};
	
inline 
LockCount::LockCount()
{m_uRefs=0; }

inline 
LockCount::~LockCount()
{}

inline unsigned
LockCount::GetLocks()const
{ return m_uRefs;}


inline unsigned
LockCount::LockData(unsigned n){if (m_uRefs == 0) LoadData(); m_uRefs+=n; return m_uRefs;}

inline unsigned
LockCount::UnlockData(unsigned n)
{
	assert(m_uRefs>=n);
	m_uRefs-=n;
	unsigned rslt=m_uRefs;
	if (rslt == 0)
		FreeData();
	return rslt;
}

inline void
LockCount::LoadData()
{}

inline void
LockCount::FreeData()
{}

///////////////////////////////////////////////////////////////////////////////////////

template <class T>	//	T doit implanter T* Clone()const;
class ClonePtr
{
public:

	ClonePtr(const T& t);	 
	ClonePtr(const ClonePtr<T>&);
	~ClonePtr();

	ClonePtr<T>& operator=(const ClonePtr<T>&);
	ClonePtr<T>& operator=(const T&);
	operator T*()const;
	T* operator->()const;

private:

	T* m_pT;
};

template <class T> inline 
ClonePtr<T>::ClonePtr(const T& t)
{m_pT=t.Clone();}	// tant pis pour la performance...

template <class T> inline
ClonePtr<T>::ClonePtr(const ClonePtr<T>& sp)
{ m_pT=sp.m_pT->Clone(); }

template <class T> inline
ClonePtr<T>::~ClonePtr()
{ delete m_pT; }

template <class T>
ClonePtr<T>& ClonePtr<T>::operator=(const ClonePtr<T>& sp)
{
	if (this != &sp)
	{
		delete m_pT;
		m_pT=sp.m_pT->Clone();
	}
	return *this;
}

template <class T> inline
ClonePtr<T>::operator T*()const{return m_pT;}

template <class T> inline
T* ClonePtr<T>::operator->()const
{ return m_pT;}


#endif

