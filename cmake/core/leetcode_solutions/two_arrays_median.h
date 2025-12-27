

#ifndef TWO_ARRAYS_MEDIAN_H
#define TWO_ARRAYS_MEDIAN_H

#include <functional>
#include <vector>
namespace libcore
{
namespace solutions
{

class two_arrays_median
{
public:
	static double solve(const std::vector<int>& nums1, const std::vector<int>& nums2);

	// TargetBeforeMixing equal elements of target goes before elements of mixing array in a merged
	// array
	static int idx_in_merged_array(int idx, const std::vector<int>& target_array,
								   const std::vector<int>& mixing_array,
								   bool targetBeforeMixing = true);
	static double median_in_mixing_odd(const std::vector<int>& nums1,
									   const std::vector<int>& nums2);
};
} // namespace solutions
} // namespace libcore

#endif // TWO_ARRAYS_MEDIAN_H
