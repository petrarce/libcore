//
// Created by ivan on 8/30/25.
//

#include "two_sum_calculator.h"

namespace libcore
{
namespace solutions
{

namespace detail
{
class Solution
{
public:
	std::vector<int> twoSum(const std::vector<int>& nums, int target)
	{
		if (nums.empty())
			return {};

		if (nums.size() == 1)
			return nums[0] == target ? std::vector{ 0 } : std::vector<int>{};

		if (nums.size() == 2)
			return nums[0] + nums[1] != target ? std::vector<int>{} : std::vector{ 0, 1 };
		// O(n) one-path map traversal
		absl::flat_hash_map<int, int> valueMap;
		for (int idx1 = 0; idx1 < nums.size(); idx1++)
		{
			const auto rest = target - nums[idx1];
			if (valueMap.contains(rest))
			{
				const auto idx2 = valueMap[rest];
				return idx1 > idx2 ? std::vector{ idx2, idx1 } : std::vector{ idx1, idx2 };
			}
			valueMap.insert(std::pair{ nums[idx1], idx1 });
		}
		return {};
	}
};

} // namespace detail

std::optional<std::pair<int, int> > two_sum_calculator::solve(const std::vector<int>& nums,
															  int target)
{
	const auto solution = detail::Solution().twoSum(nums, target);
	assert(solution.size() <= 2);
	if (solution.empty())
		return std::nullopt;
	if (solution.size() == 1)
		return std::pair{ solution[0], solution[0] };
	return std::optional(std::pair{ solution[0], solution[1] });
}
} // namespace solutions
} // namespace libcore
