//
// Created by ivan on 1/17/26.
//

#include "problem_28.h"
#include <fmt/format.h>
#include <cassert>

#include <tuple>

namespace euler_project
{

std::string problem_28::solve()
{
	// lh = (level + 1) * 2 - 1
	// level = lh + 1 / 2 - 1
	return fmt::format("Matrix 1001x1001 diagonal sum: {}", sum_till_level((1001 + 1) / 2 - 1));
}

uint64_t problem_28::level_sum(uint64_t level)
{
	if (level == 0)
		return 1;
	const uint64_t level_height = (level + 1) * 2 - 1;
	const uint64_t upper_right = level_height * level_height;
	const uint64_t lower_right = upper_right - 3 * (level_height - 1);
	return 2 * (upper_right + lower_right);
}

uint64_t problem_28::sum_till_level(uint64_t level)
{
	uint64_t sum = 0;
	for (size_t i = 0; i <= level; i++)
	{
		const auto ls = level_sum(i);
		assert(sum + ls > sum);
		sum += level_sum(i);
	}
	return sum;
}

} // namespace euler_project
