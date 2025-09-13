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
