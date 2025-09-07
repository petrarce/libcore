//
// Created by ivan on 9/7/25.
//

#ifndef STRINGS_GCD_H
#define STRINGS_GCD_H
#include <string>

namespace libcore
{
namespace solution
{

class strings_gcd
{
public:
	static std::string solve(const std::string_view& s1, const std::string_view& s2);
	static const bool isStringDivisor(const std::string_view& prefix,
									  const std::string_view& string);
};

} // namespace solution
} // namespace libcore

#endif // STRINGS_GCD_H
