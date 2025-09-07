//
// Created by ivan on 9/6/25.
//

#include "merge_strings_alternately.h"
#include <array>
#include <absl/strings/str_format.h>
#include <absl/strings/internal/str_format/extension.h>
namespace libcore
{
namespace solutions
{
namespace detail
{

class Solution
{
public:
	std::string mergeAlternately(const std::string& word1, const std::string& word2)
	{
		std::string res(word1.size() + word2.size(), 0);
		int i = 0;
		while (i < word1.size() && i < word2.size())
		{
			res[2 * i] = word1[i];
			res[2 * i + 1] = word2[i];
			i++;
		}
		while (i < word1.size())
		{
			res[word2.size() + i] = word1[i];
			i++;
		}
		while (i < word2.size())
		{
			res[word1.size() + i] = word2[i];
			i++;
		}

		return res;
	}
};
} // namespace detail
std::string merge_strings_alternately::solve(const std::string& text1, const std::string& text2)
{
	return detail::Solution().mergeAlternately(text1, text2);
}
} // namespace solutions
} // namespace libcore
