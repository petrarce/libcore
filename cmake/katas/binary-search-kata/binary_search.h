//
// Created by ivan on 9/9/25.
//

#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H
#include <vector>
namespace libcore
{
namespace katas
{

class binary_search_array_traversal
{
public:
	static int find(std::vector<int> values, int value);
};

// this binary search algo will find a lower bound of the array given a value
// and then
class binary_search_array_lower_bound
{
public:
	static int find(std::vector<int> values, int value);
};
} // namespace katas
} // namespace libcore

#endif // BINARY_SEARCH_H
