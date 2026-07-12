//
// Created by ivan on 7/12/26.
//

#include "longest_valid_parenthes.h"

#include <iostream>
#include <ranges>
#include <absl/strings/internal/str_format/extension.h>
#include <fmt/format.h>

namespace
{

class Solution
{
public:
	template<std::ranges::viewable_range R>
	auto longestPrefix(R&& s, char open = '(', char close = ')')
	{
		long opened_parenthes = 0;
		long longest_valid_sequence_length = 0;
		long current_valid_sequence_length = 0;
		auto current_valid_sequence_start = s.begin();

		for (auto idx = s.begin(); idx != s.end(); ++idx)
		{
			const auto& c = *idx;
			if (c == open)
				opened_parenthes++;
			else if (c == close && opened_parenthes > 0)
			{
				opened_parenthes--;
				current_valid_sequence_length += 2;
			}
			else if (c == close)
			{
				longest_valid_sequence_length
					= std::max(current_valid_sequence_length, longest_valid_sequence_length);
				current_valid_sequence_length = 0;
				current_valid_sequence_start = idx;
				current_valid_sequence_start = std::next(current_valid_sequence_start);
			}
		}

		return std::pair{ opened_parenthes == 0 ? std::max(longest_valid_sequence_length,
														   current_valid_sequence_length)
												: longest_valid_sequence_length,
						  std::ranges::subrange(current_valid_sequence_start, s.end()) };
	}

	int longestValidParentheses(const std::string& s)
	{
		auto [largest_prefix_length, suffix] = longestPrefix(s, '(', ')');
		const auto [largets_suffix_length, _]
			= longestPrefix(std::ranges::views::reverse(suffix), ')', '(');
		return std::max(largest_prefix_length, largets_suffix_length);
	}
};
} // namespace

namespace libcore
{
namespace solutions
{

int longest_valid_parenthes::solve(const std::string& str)
{
	return Solution().longestValidParentheses(str);
}

} // namespace solutions
} // namespace libcore
