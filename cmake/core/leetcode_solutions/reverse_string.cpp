
#include "reverse_string.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <ranges>
#include <span>
#include <boost/range/end.hpp>

namespace libcore
{
namespace solutions
{

std::string reverse_string::solve(const std::string& s)
{

	// 1. ideas: parse untill space, then copy to new string reverse. At the end return the new
	// string
	// 2. in place algo?
	// 2.1 Use partitioning algorythm to acheive this
	if (s.empty())
		return "";

	if (s.size() == 1)
		return std::isspace(s[0]) ? "" : s;

	if (s.size() == 2)
	{
		auto res = s;
		const auto new_end
			= std::remove_if(res.begin(), res.end(), [](const auto c) { return std::isspace(c); });
		res.erase(new_end, res.end());
		return res;
	}

	std::string res;
	res.reserve(s.size());
	int begin_idx = 0;
	for (int end_idx = 1; end_idx < s.length(); end_idx++)
	{

		// 4. in the middle of the spaces
		if (std::isspace(s[end_idx]) && std::isspace(s[end_idx - 1]))
			continue;

		// 3. in the middle of the word
		if (end_idx + 1 != s.size() && !std::isspace(s[end_idx - 1]) && !std::isspace(s[end_idx]))
			continue;

		// 1. begin of the word
		if (std::isspace(s[end_idx - 1]) && !std::isspace(s[end_idx]))
		{
			begin_idx = end_idx;
			continue;
		}

		// 2. end of the word
		const int offset = std::isspace(s[end_idx]) ? end_idx : end_idx + 1;

		if (!res.empty())
			res.push_back(' ');

		auto old_end = res.size();
		res.append(s.begin() + begin_idx, s.begin() + offset);
		std::reverse(res.begin() + old_end, res.end());
	}
	std::reverse(res.begin(), res.end());
	return res;
}

} // namespace solutions
} // namespace libcore
