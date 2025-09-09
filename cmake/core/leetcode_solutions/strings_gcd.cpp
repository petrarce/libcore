//
// Created by ivan on 9/7/25.
//

#include "strings_gcd.h"
#include <numeric>

namespace libcore
{
namespace solution
{

namespace detail
{
class Solution
{
public:
	static const bool isStringDivisor(const std::string_view& prefix,
									  const std::string_view& string)
	{

		if (string.size() % prefix.size() != 0)
			return false;
		const auto jpass = string.size() / prefix.size();
		for (int i = 0; i < prefix.size(); i++)
		{
			for (int j = 0; j < jpass; j++)
				if (string[j * prefix.size() + i] != prefix[i])
					return false;
		}
		return true;
	}
	std::string gcdOfStrings(std::string_view s1, std::string_view s2)
	{
		std::string_view vmajor = s1.size() > s2.size() ? s1 : s2;
		std::string_view vminor = s1.size() > s2.size() ? s2 : s1;
		std::string_view prefix = std::string_view(
			vmajor.begin(), vmajor.begin() + std::gcd(vmajor.size(), vminor.size()));
		while (prefix.size() > 1)
		{
			if (isStringDivisor(prefix, s1) && isStringDivisor(prefix, s2))
				return std::string(prefix);
			vmajor = vminor;
			vminor
				= std::string_view(vmajor.begin(), vmajor.begin() + vmajor.size() / prefix.size());
			prefix = std::string_view(vmajor.begin(),
									  vmajor.begin() + std::gcd(vmajor.size(), vminor.size()));
		}
		return isStringDivisor(prefix, s1) && isStringDivisor(prefix, s2) ? std::string{ prefix }
																		  : "";
	}
};
} // namespace detail

std::string strings_gcd::solve(const std::string_view& s1, const std::string_view& s2)
{
	return detail::Solution().gcdOfStrings(s1, s2);
}
const bool strings_gcd::isStringDivisor(const std::string_view& prefix,
										const std::string_view& string)
{
	return detail::Solution().isStringDivisor(prefix, string);
}

} // namespace solution
} // namespace libcore
