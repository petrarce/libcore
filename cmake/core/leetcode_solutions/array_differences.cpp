//
// Created by ivan on 9/8/25.
//

#include "array_differences.h"

namespace libcore
{
namespace solutions
{
namespace detail
{
class Solution
{
public:
	std::vector<std::vector<int> > findDifference(const std::vector<int>& nums1,
												  const std::vector<int>& nums2)
	{
		std::bitset<2001> b1, b2;
		std::for_each(nums1.begin(), nums1.end(), [&](int i) { b1[i + 1000] = true; });
		std::for_each(nums2.begin(), nums2.end(), [&](int i) { b2[i + 1000] = true; });

		std::vector<std::vector<int> > res(2);
		for (int i = 0; i < b1.size(); i++)
		{
			if (b1[i] && !b2[i])
				res[0].push_back(i - 1000);
			if (b2[i] && !b1[i])
				res[1].push_back(i - 1000);
		}
		return res;
	}
};
} // namespace detail
std::vector<std::vector<int> > array_differences::findDifference(const std::vector<int>& nums1,
																 const std::vector<int>& nums2)
{
	return detail::Solution().findDifference(nums1, nums2);
}

} // namespace solutions
} // namespace libcore
