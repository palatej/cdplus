#ifndef __lcpp_matrix_0_h
#define __lcpp_matrix_0_h

#include <exception>
#include <iostream>

namespace LCPP {

	enum Triangular {
		Lower, Upper
	};

	enum Side {
		Left, Right
	};

	struct lcpp_exception : std::exception {
	public:
		lcpp_exception(const std::string routine, int info) :m_routine(routine), m_info(info) {}

		const std::string& source()const { return m_routine; }
		int info()const { return m_info; }

	private:
		std::string m_routine;
		int m_info;
	};

}

#endif

