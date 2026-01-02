#ifndef gcd_xpp
#define gcd_xpp
#include <cmath>
namespace libcore
{
namespace math
{

template<class T>
T gcd(const T& a, const T& b)
{
	auto aa = a > b ? a : b;
	auto bb = a > b ? b : a;

	while (aa % bb != 0)
	{
		const auto bbb = bb;
		bb = aa % bb;
		aa = bbb;
	}
	return bb;
}

template<std::unsigned_integral T>
bool is_prime(const T& val)
{
	if (val == 1 || val == 2)
		return true;

	if (val % 2 == 0)
		return false;

	T max_divisor = std::sqrt(val);
	for (T i = 3; i <= max_divisor; i += 2)
	{
		if (val % i == 0)
			return false;
	}
	return true;
}
} // namespace math
} // namespace libcore
#endif
