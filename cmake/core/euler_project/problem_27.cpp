//
// Created by ivan on 12/30/25.
//

#include "problem_27.h"

#include "core/math/gcd.hpp"

#include <math.h>
#include <numeric>
#include <ranges>
#include <fmt/format.h>
namespace euler_project
{
std::string problem_27::solve()
{

	int maxPrimeCount = 0;
	auto maxab = std::pair<int, int>(0, 0);
	for (int a : std::views::iota(-999, 999))
	{
		for (int b : std::views::iota(-1000, 1000))
		{
			int curCount = _count(a, b);
			if (curCount > maxPrimeCount)
			{
				maxPrimeCount = curCount;
				maxab = { a, b };
			}
		}
	}
	return fmt::format("Max a({}) * b({}) = {}, Number of primes is {}", maxab.first, maxab.second,
					   maxab.first * maxab.second, maxPrimeCount);
}

int problem_27::_count(int a, int b)
{
	for (int n = 0; true; n++)
	{
		const int64_t val = n * n + n * a + b;
		if (val <= 0)
			return n;
		if (!libcore::math::is_prime(static_cast<uint64_t>(val)))
			return n;
	}
}
} // namespace euler_project
