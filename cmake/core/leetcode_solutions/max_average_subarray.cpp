//
// Created by ivan on 9/3/25.
//

#include "max_average_subarray.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <bits/ostream.tcc>
namespace libcore::solutions
{
namespace detail
{
class Solution
{
public:
	double findMaxAverage(const std::vector<int>& nums, int k)
	{
		/*
		 *1. Compute average for the first k elements of the subarray.
		 *2. from i in [k's...nums.end]: to the end dor next
		 *3.	potentialAverage = curAverage - nums[i - k] / k + nums[i] / k
		 *3		if (curAverage < potentialAverage)
		 *4.			curAverage = curAverage - nums[i - k] / k + nums[i] / k;
		 *				updateRange with [i - k + 1, i] => this is a new range with max average
		 */
		double average = 0;
		std::for_each(nums.begin(), nums.begin() + k,
					  [&average, k](int n) { average += static_cast<double>(n) / k; });
		double slidingAverage = average;
		for (int i = k; i < nums.size(); ++i)
		{
			slidingAverage = slidingAverage - static_cast<double>(nums[i - k]) / k
							 + static_cast<double>(nums[i]) / k;
			average = std::max(slidingAverage, average);
		}
		return average;
	}
};

} // namespace detail
double max_average_subarray::solve(const std::vector<int>& nums, int k)
{
	assert(k <= nums.size());
	return detail::Solution().findMaxAverage(nums, k);
}
} // namespace libcore::solutions
