//
// Created by ivan on 8/30/25.
//

#ifndef TWOSUMCALCULATOR_H
#define TWOSUMCALCULATOR_H
#include <algorithm>
#include <numeric>
#include <optional>
#include <vector>
#include <absl/strings/str_format.h>
#include <boost/mpl/assert.hpp>
#include <iostream>
namespace libcore {
namespace solutions {
namespace detail
{
class Solution {

public:

std::vector<int> twoSum(const std::vector<int>& nums, int target)
{
	if (nums.empty())
		return {};

	if (nums.size() == 1)
		return nums[0] == target
			? std::vector{0}
			: std::vector<int>{};


	if (nums.size() == 2 )
		return nums[0] + nums[1] != target
			? std::vector<int>{}
			: std::vector{0, 1};
	// O(n log_n) traversal
	std::vector<int> indices(nums.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::sort(
		indices.begin(),
		indices.end(),
		[&](const int l, const int r) { return nums[l] < nums[r]; });
	for (int i = 0; i < indices.size(); ++i)
	{
		const auto v1idx = indices[i];
		const auto v1 = nums[v1idx];
		const auto potential_v2idx_iter = std::lower_bound(
			indices.begin() + i + 1,
			indices.end(),
			target,
			[&](auto v2idx, auto) { return v1 + nums[v2idx] < target; });

		if (potential_v2idx_iter == indices.end())
			continue;

		if (nums[*potential_v2idx_iter] + v1 != target)
			continue;

		return v1idx > *potential_v2idx_iter
			? std::vector{ *potential_v2idx_iter, v1idx }
			: std::vector{ v1idx, *potential_v2idx_iter };
	}
	return {};
}

};

}

class two_sum_calculator {
public:
	static std::optional<std::pair<int, int>> solve(const std::vector<int>& nums, int target)
	{
		const auto solution = detail::Solution().twoSum(nums, target);
		assert(solution.size() <= 2);
		if (solution.empty())
			return std::nullopt;
		if (solution.size() == 1)
			return std::pair{solution[0], solution[0]};
		return  std::optional(std::pair{solution[0], solution[1]});
	}
};

} // solutions
} // libcore

#endif //TWOSUMCALCULATOR_H
