#ifndef __arima_h
#define __arima_h

#include "sequence.h"
#include "polynomials.h"

namespace CD_STATS {

	class IArima {

	public:

		virtual ~IArima() {}

		virtual NUMCPP::Polynomial getAr() = 0;
		virtual NUMCPP::Polynomial getStationaryAr() = 0;
		virtual NUMCPP::Polynomial getNonStationaryAr() = 0;
		virtual NUMCPP::Polynomial getMa() = 0;

		virtual int getArOrder() = 0;
		virtual int getStationaryArOrder() = 0;
		virtual int getNonStationaryArOrder() = 0;
		virtual int getMaOrder() = 0;

		virtual double getInnovationVariance() = 0;
		virtual bool isStationary() = 0;

	};

	class Arima : public IArima {

	public:

		static NUMCPP::DataBlock<double> ZERO, ONE;

		Arima();
		Arima(const NUMCPP::Polynomial& ar, const NUMCPP::Polynomial& delta, const NUMCPP::Polynomial& ma, double var);
		Arima(const Arima& arima);
		~Arima();

		Arima& operator=(const Arima& arima);


		NUMCPP::Polynomial getAr();
		NUMCPP::Polynomial getStationaryAr();
		NUMCPP::Polynomial getNonStationaryAr();
		NUMCPP::Polynomial getMa();

		int getArOrder();
		int getStationaryArOrder();
		int getNonStationaryArOrder();
		int getMaOrder();

		double getInnovationVariance();
		bool isStationary();

	private:

		NUMCPP::Polynomial m_star, m_ar, m_delta, m_ma;
		double m_var;
	};

	inline int Arima::getArOrder() {
		return m_ar.getDegree();
	}

	inline int Arima::getStationaryArOrder() {
		return m_star.getDegree();
	}

	inline int Arima::getNonStationaryArOrder() {
		return m_delta.getDegree();
	}

	inline int Arima::getMaOrder() {
		return m_ma.getDegree();
	}

	inline double Arima::getInnovationVariance() {
		return m_var;
	}

	inline bool Arima::isStationary() {
		return m_delta.getDegree() == 0;
	}

	inline NUMCPP::Polynomial Arima::getMa() {
		return m_ma;
	}

	inline NUMCPP::Polynomial Arima::getAr() {
		return m_ar;
	}

	inline NUMCPP::Polynomial Arima::getStationaryAr() {
		return m_star;
	}

	inline NUMCPP::Polynomial Arima::getNonStationaryAr() {
		return m_delta;
	}


}

#endif
