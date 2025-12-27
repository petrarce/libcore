//
// Created by ivan on 11/21/25.
//

#include "two_arrays_median.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <boost/mpl/assert.hpp>

namespace libcore
{
namespace solutions
{
namespace
{

double median(const std::span<const int>& nums)
{
	if (nums.empty())
		throw std::invalid_argument("empty array");

	if (nums.size() == 1)
		return nums[0];

	return nums[nums.size() / 2] / 2. + nums[nums.size() / 2 - (nums.size() + 1) % 2] / 2.;
}

std::optional<int> idx_ge_median(idx, target_array)

} // namespace

double two_arrays_median::median_in_mixing_odd(const std::vector<int>& nums1,
											   const std::vector<int>& nums2)
{
	if ((nums1.size() + nums2.size()) % 2 == 0)
		throw std::invalid_argument("size_of(nums1 + nums2) should be odd");

	const int median_idx = (nums1.size() + nums2.size()) / 2;

	// find first elemnt that is less or equal then median
	// _El_________Median_
	const auto r = std::views::iota(static_cast<size_t>(0), nums1.size() - 1);
	const auto nums1_median_lb_rit
		= std::ranges::lower_bound(std::views::reverse(r), median_idx,
								   [&](const auto idx, const auto median)
								   {
									   const auto merged_idx
										   = idx_in_merged_array(idx, nums1, nums2);
									   return merged_idx > median;
								   });
	const auto nums1_median_lb_it
		= nums1_median_lb_rit.base() == r.begin() ? r.end() : nums1_median_lb_rit.base() - 1;

	// find first element that is greater or equal then median
	// _Median_________El_
	const auto nums1_median_ub
		= std::ranges::lower_bound(r, median_idx,
								   [&](const auto idx, const auto median)
								   {
									   const auto merged_idx
										   = idx_in_merged_array(idx, nums1, nums2);
									   return median > merged_idx;
								   });

	// find first elemnt that is less or equal then median
	// _El_________Median_
	const auto rr = std::views::iota(static_cast<size_t>(0), nums1.size() - 1);
	const auto nums2_median_lb
		= std::ranges::lower_bound(std::views::reverse(rr), median_idx,
								   [&](const auto idx, const auto median)
								   {
									   const auto merged_idx
										   = idx_in_merged_array(idx, nums2, nums1, false);
									   return merged_idx > median;
								   })
			  .base()
		  - 1;

	// find first element that is greater or equal then median
	// _Median_________El_
	const auto nums2_median_ub
		= std::ranges::lower_bound(rr, median_idx,
								   [&](const auto idx, const auto median)
								   {
									   const auto merged_idx
										   = idx_in_merged_array(idx, nums2, nums1, false);
									   return median > merged_idx;
								   });

	std::cout << *nums1_median_lb << " " << *nums1_median_ub << " " << *nums2_median_lb << " "
			  << *nums2_median_ub << std::endl;

	if (nums1_median_lb == nums1_median_ub)
		return nums1[*nums1_median_lb];
	else if (nums2_median_lb == nums2_median_ub)
		return nums2[*nums2_median_lb];

	assert(false && "something went wrong. Contact developers");
	return 0;
}

double two_arrays_median::solve(const std::vector<int>& nums1, const std::vector<int>& nums2)
{
	if (nums1.empty() && nums2.empty())
		throw std::invalid_argument("Two arrays median cannot be empty");

	if (nums1.empty())
		return median(std::span(nums2.begin(), nums2.end()));

	if (nums2.empty())
		return median(std::span(nums1.begin(), nums1.end()));

	// the median is definitely at the end of the nums1
	if (nums1.back() <= nums2.front() && nums1.size() > nums2.size())
		return median(std::span(nums1.begin() + nums2.size(), nums1.end()));

	// the median is definitely at the beginning of the nums1
	if (nums1.front() >= nums2.back() && nums1.size() > nums2.size())
		return median(std::span(nums1.begin(), nums1.end() - nums2.size()));

	// the median is definitely at the beginning of the nums2
	if (nums1.back() <= nums2.front() && nums1.size() < nums2.size())
		return median(std::span(nums2.begin(), nums2.end() - nums1.size()));

	// the median is definitely at the end of the nums2
	if (nums1.front() >= nums2.back() && nums1.size() < nums2.size())
		return median(std::span(nums2.begin() + nums1.size(), nums2.end()));

	// array sizes are equal and they are non mixing
	if (nums1.front() >= nums2.back() || nums1.back() <= nums2.front())
	{
		assert(nums1.size() == nums2.size());
		return nums1.front() >= nums2.back() ? (nums1.front() + nums2.back()) / 2.
											 : (nums2.front() + nums1.back()) / 2.;
	}

	auto res_array = nums1;
	res_array.insert(res_array.end(), nums2.begin(), nums2.end());
	std::ranges::sort(res_array, std::greater());
	return median(res_array);
}

int two_arrays_median::idx_in_merged_array(int idx, const std::vector<int>& target_array,
										   const std::vector<int>& mixing_array,
										   bool targetBeforeMixing)
{

	auto less = [](int idx1, int idx2) { return idx1 < idx2; };
	auto less_eq = [](int idx1, int idx2) { return idx1 <= idx2; };

	// find first element that is not before the target_array[idx]
	// if TargetBeforeMixing == true then all elements == target_array[idx]
	// are treated before the target_array[idx]
	const auto it = std::lower_bound(mixing_array.begin(), mixing_array.end(), target_array[idx],
									 targetBeforeMixing ? less : less_eq);

	return std::distance(mixing_array.begin(), it) + idx;
}

} // namespace solutions
} // namespace libcore
