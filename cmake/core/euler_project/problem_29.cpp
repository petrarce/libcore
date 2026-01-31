//
// Created by ivan on 1/25/26.
//

#include "problem_29.h"

#include <algorithm>
#include <set>
#include <core/math/factorization.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <fmt/format.h>
#include <iostream>
#include "../tests/utils.h"
using namespace libcore::math;
namespace euler_project
{

std::string problem_29::solve()
{
	return fmt::format("Number of distinct powers: {}", number_unique_values(100, 100));
}
unsigned int problem_29::number_unique_values(unsigned int a, unsigned int b)
{
	std::set<std::vector<std::pair<unsigned int, unsigned int> > > unique_values;

	auto cross_powers = ranges::v3::view::cartesian_product(ranges::v3::views::iota(2u, a + 1),
															ranges::v3::views::iota(2u, b + 1));
	for (auto power : cross_powers)
		unique_values.insert(factorize_a_pow_b(std::get<0>(power), std::get<1>(power)));
	return unique_values.size();
}
} // namespace euler_project
