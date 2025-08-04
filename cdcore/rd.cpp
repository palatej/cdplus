#include "rdvector.h"

using namespace NUMCPP;

void*
_vectorallocator::alloc(size_t t)
{
	void* rslt;
	if (t)
	{
		rslt = malloc(t);
		if (rslt == 0)
			throw memException();
	}
	else
		rslt = 0;
	return rslt;
}

void
_vectorallocator::free(void* pt) { if (pt) ::free(pt); }

const char*
rdException::what()const throw()
{
	return m_what.c_str();
}


void
memException::initMsg()
{

}

char memException::g_szMsg[32] = { 0 };

