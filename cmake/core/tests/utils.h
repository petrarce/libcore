//
// Created by ivan on 8/30/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <optional>
namespace std
{

inline std::ostream& operator<<(std::ostream& os, const std::nullopt_t&)
{
	return os << "no-value";
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& v)
{
	if (!v.has_value())
		return os << std::nullopt;
	return os << *v;
}

template<class T1, class T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& v)
{
	return os << "{" << v.first << ", " << v.second << "}";
}
} // namespace std
namespace test_utils
{
constexpr int operator"" _i(long double v) { return static_cast<int>(v); }

} // namespace test_utils

#endif // UTILS_H
