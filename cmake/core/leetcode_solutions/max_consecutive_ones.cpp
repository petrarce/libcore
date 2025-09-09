//
// Created by ifolbort on 9/4/25.
//

#include "max_consecutive_ones.h"

#include <deque>
#include <cassert>
#include <core/utils/ring_buffer.h>
namespace libcore
{
namespace solutions
{

namespace detail
{

class Solution
{
public:
	int longestOnes(const std::vector<int>& nums, int k)
	{
		utils::ring_buffer<int> rb(k);
		int arrayStartIdx = 0;
		int maxOnes = 0;
		for (int i = 0; i < nums.size(); ++i)
		{
			const auto n = nums[i];
			if (k == 0 && !n)
				arrayStartIdx = i + 1;
			else if (!n)
			{
				if (rb.size() == k)
				{
					arrayStartIdx = rb.front() + 1;
					rb.pop_front();
				}
				rb.push_back(i);
			}
			maxOnes = std::max(maxOnes, i + 1 - arrayStartIdx);
		}
		return maxOnes;
	}
};
} // namespace detail

int max_consecutive_ones::solve(const std::vector<int>& nums, int k)
{
	return detail::Solution().longestOnes(nums, k);
}

} // namespace solutions
} // namespace libcore
