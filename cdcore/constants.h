#ifndef __numcpp_constants_h
#define __numcpp_constants_h

#include <algorithm>
#include <numeric>
#include <cmath>
namespace NUMCPP {

	template<typename T>
	struct CONSTANTS {

	public:

		static const T zero;

        static const T one;

        static const T two;
        
        static const T half;

        static const T radix;

        static const T safe_min;

        static const T safe_max;

        static const T huge;

        static const T tiny;

        static const T tsml, tbig, ssml, sbig;

    };


    template <>
    inline const double CONSTANTS<double>::zero = 0;

    template <>
    inline const double CONSTANTS<double>::one=1;

    template <>
    inline const double CONSTANTS<double>::two = 2;

    template <>
    inline const double CONSTANTS<double>::half = 0.5;

    template <typename T>
    inline const T CONSTANTS<T>::radix=std::numeric_limits<T>::radix;
    
    template <typename T>
    inline const T CONSTANTS<T>::safe_min= std::pow<T>(radix, std::min(std::numeric_limits<T>::min_exponent - 1,
            1 - std::numeric_limits<T>::max_exponent));

    template <typename T>
    inline const T CONSTANTS<T>::safe_max=std::pow<T>(radix, std::max(std::numeric_limits<T>::max_exponent - 1,
            1 - std::numeric_limits<T>::min_exponent));
    
    template <typename T>
    inline const T CONSTANTS<T>::tiny=std::numeric_limits<T>::min();

    template <typename T>
    inline const T CONSTANTS<T>::huge= std::numeric_limits<T>::max();

    template <>
    inline const double CONSTANTS<double>::tsml = std::pow<double>(radix, std::ceil(
        std::numeric_limits<double>::min_exponent - 1) * 0.5);
    
    template <>
    inline const double CONSTANTS<double>::tbig = std::pow<double>(radix, std::floor(
        std::numeric_limits<double>::max_exponent - std::numeric_limits < double>::digits + 1) * 0.5);
    
    template <>
    inline const double CONSTANTS<double>::ssml = std::pow<double>(radix, -std::floor(
        std::numeric_limits<double>::min_exponent - std::numeric_limits < double>::digits) * 0.5);
    
    template <>
    inline const double CONSTANTS<double>::sbig = std::pow<double>(radix, -std::ceil(
        std::numeric_limits<double>::max_exponent + std::numeric_limits < double>::digits - 1) * 0.5);


}

#endif
