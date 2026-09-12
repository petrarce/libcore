//
// Created by ivan on 8/6/26.
//

#ifndef LIBCORE_MEDIAN_TWO_SORTED_ARRAYS_H
#define LIBCORE_MEDIAN_TWO_SORTED_ARRAYS_H

#include <vector>
namespace leetcode
{

class median_two_sorted_arrays
{
public:
	static double solve(const std::vector<int>& v1, const std::vector<int>& v2);
	static double find_element_n(const std::vector<int>& v1, const std::vector<int>& v2,
								 const int idx);
};

} // namespace leetcode

#endif // LIBCORE_MEDIAN_TWO_SORTED_ARRAYS_H
