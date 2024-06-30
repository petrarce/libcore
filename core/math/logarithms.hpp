#ifndef logarithms_hpp_
#define logarithms_hpp_
#include <cmath>
namespace libcore
{
namespace math
{

/// Compute logarythm with base b on value a
/**
 * Error class: Very poor
 * Is +- exact with with  relatively large a and b
 */
template<class T>
T log_b(const T& b, const T& a)
{
	return std::log2(a) / std::log2(b);
}

}
}
#endif // logarithms_hpp_
