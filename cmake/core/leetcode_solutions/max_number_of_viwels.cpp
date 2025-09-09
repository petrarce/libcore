//
// Created by ifolbort on 9/4/25.
//

#include "max_number_of_viwels.h"

#include <algorithm>
#include <vector>
namespace libcore
{
namespace solutions
{
namespace detail
{
bool isVovel(char c)
{
	switch (c)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return true;
	default:
		return false;
	}
}
class Solution
{
public:
	int maxVowels(const std::string& s, int k)
	{
		int maxVowels = 0;
		std::for_each(s.begin(), s.begin() + k, [&](char c) { maxVowels += isVovel(c); });
		int currentVowels = maxVowels;
		for (int i = k; i < s.size(); ++i)
		{
			currentVowels += isVovel(s[i]) - isVovel(s[i - k]);
			maxVowels = std::max(maxVowels, currentVowels);
		}
		return maxVowels;
	}
};
} // namespace detail
int max_number_of_viwels::solve(const std::string& str, int k)
{
	return detail::Solution().maxVowels(str, k);
}

} // namespace solutions
} // namespace libcore
