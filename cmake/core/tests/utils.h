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

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	os << "{";
	std::ranges::for_each(vec, [&](const auto& v) { os << " " << v << ","; });
	return os << "}";
}

template<class... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...> t)
{
	os << "(";
	std::apply([&](auto... elements) { ((os << elements << ", "), ...); }, t);
	return os << ")";
}
} // namespace std

#endif // UTILS_H
