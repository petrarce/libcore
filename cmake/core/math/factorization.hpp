//
// Created by ivan on 1/25/26.
//

#include <vector>

namespace libcore
{
namespace math
{
template<std::unsigned_integral T>
std::vector<std::pair<T, T> > simple_factorize(const T& value)
{
	if (value < 0)
		throw std::invalid_argument("value must be positive");
	if (value == 1 || value == 0)
		return {};

	// computes fraction and reduces value relative the fraction
	auto compute_fraction = [](T& val, const T fraction) -> std::pair<T, T>
	{
		int pow = 0;
		for (; val % fraction == 0; ++pow, val /= fraction)
		{
		}
		return { fraction, pow };
	};
	std::vector<std::pair<T, T> > fractions;
	T val = value;

	const auto twoFraction = compute_fraction(val, 2);
	if (twoFraction.second)
		fractions.push_back(twoFraction);

	for (int frac = 3; val != 1; frac += 2)
	{
		const auto newFrac = compute_fraction(val, frac);
		if (newFrac.second)
			fractions.push_back(newFrac);
	}
	return fractions;
}
} // namespace math
} // namespace libcore
