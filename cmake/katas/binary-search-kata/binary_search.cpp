//
// Created by ivan on 9/9/25.
//

#include "binary_search.h"

namespace libkor
{
namespace katas
{

} // namespace katas
} // namespace libkor
int libcore::katas::binary_search_array_traversal::find(std::vector<int> values, int value)
{
	if (values.empty())
		return -1;
	int start = 0, end = values.size() - 1;
	while (start != end)
	{
		const int mid = start + (end - start) / 2;
		if (values[mid] == value)
			start = end = mid;
		if (values[mid] < value)
			start = mid == start ? end : mid + 1;
		else
			end = mid == end ? start : mid - 1;
	}
	// bad - this makes it O(n) algo in worst case
	while (start != 0 && values[start - 1] == value)
		start--;
	return values[start] == value ? start : -1;
}

int libcore::katas::binary_search_array_lower_bound::find(std::vector<int> values, int value)
{
	if (values.empty())
		return -1;
	if (values.size() == 1)
		return values[0] == value ? 0 : -1;

	int start = 0, end = values.size() - 1, mid = 0;
	while (start != end)
	{
		mid = start + (end - start) / 2;
		if (values[mid] > value || mid != start && values[mid - 1] == value)
			// cut from right side
			end = mid == start ? start : mid - 1;
		else if (values[mid] < value)
			// shrink array from left side
			start = mid + 1;
		else
			// first item = value found:
			start = end = mid;
	}
	return values[start] == value ? start : -1;
}
