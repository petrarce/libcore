//
// Created by ivan on 8/6/26.
//

#include "median_two_sorted_arrays.h"
#include <vector>
#include <cassert>
#include <boost/mpl/assert.hpp>
namespace
{
class Solution
{
public:
	static double findMedianSortedArrays(const std::vector<int>& a, const std::vector<int>& b)
	{
		auto trivial_med = [](const std::vector<int>& v) -> double
		{
			if (v.size() % 2 == 1)
				return v[v.size() / 2];
			return v[v.size() / 2 - 1] / 2. + v[v.size() / 2] / 2.;
		};
		if (a.empty() || b.empty())
			return trivial_med(a.empty() ? b : a);
		return 0;
	};
	static double find_element_n(const std::vector<int>& v1, const std::vector<int>& v2,
								 const int idx)
	{
		auto trivial_idx_search = [idx = idx](const std::vector<int>& v) { return v[idx]; };
		if (v1.empty() || v2.empty())
			return trivial_idx_search(v1.empty() ? v2 : v1);
		struct arr_meta
		{
			std::pair<int, int> range{ 0, 0 };
			std::size_t cur{ 0 };
			const std::vector<int>& array;
			bool range_empty() { return !range.first < range.second; }
		};
		// start with middle of the arrays
		arr_meta m1{ { 0, v1.size() - 1 }, v1.size() / 2, v1 };
		arr_meta m2{ { 0, v2.size() - 1 }, v2.size() / 2, v2 };

		std::pair arrays = { &m1, &m2 };

		while (m1.range_empty() || m2.range_empty())
		{
			auto& primary = *arrays.first;
			auto& secondary = *arrays.second;

			if (primary.array[primary.cur] < primary.array[secondary.cur]
					&& primary.cur + secondary.cur < idx
				|| primary.array[primary.cur] > primary.array[secondary.cur]
					   && primary.cur + secondary.cur > idx)
				std::swap(arrays.first, arrays.second);

			if (primary.array[primary.cur] <= primary.array[secondary.cur]
				&& (primary.cur + secondary.cur) >= idx)
				secondary.range.second = secondary.cur;
			else if (primary.array[primary.cur] >= primary.array[secondary.cur]
					 && (primary.cur + secondary.cur) <= idx)
				secondary.range.first = secondary.cur + 1;

			secondary.cur = (secondary.range.first + secondary.range.second) / 2;
		}
		auto& primary = *arrays.first;
		auto& secondary = *arrays.second;

		assert(!primary.range_empty());
		assert(secondary.range_empty());
		assert(primary.cur + secondary.range.second == idx);

		return primary.array[primary.cur];
	}
};
} // namespace
namespace leetcode
{
double median_two_sorted_arrays::solve(const std::vector<int>& v1, const std::vector<int>& v2)
{
	return Solution::findMedianSortedArrays(v1, v2);
}
double median_two_sorted_arrays::find_element_n(const std::vector<int>& v1,
												const std::vector<int>& v2, const int idx)
{
	return Solution::find_element_n(v1, v2, idx);
}
} // namespace leetcode
