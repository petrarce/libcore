#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>

#include "utils.h"

#include <ranges>

BOOST_AUTO_TEST_SUITE(TestRangesV3)

BOOST_AUTO_TEST_CASE(TestCartesianProduct)
{
	auto v1 = std::vector<int>{ 1, 2 };
	auto v2 = std::vector<std::string>{ "a", "b" };
	auto cp = ranges::v3::cartesian_product_view(v1, v2);
	using VecType = std::vector<std::tuple<int, std::string> >;
	const auto res = VecType(cp.begin(), cp.end());
	const auto exp = VecType{ { 1, "a" }, { 1, "b" }, { 2, "a" }, { 2, "b" } };
	// VecType exp = std::vector{std::pair{1,"a"}, std::pair{1,"b"}, std::pair{2,"a"},
	// std::pair{2,"b"}};
	BOOST_CHECK_EQUAL(ranges::v3::size(cp), 4);
	BOOST_CHECK_EQUAL(res.size(), 4);
	BOOST_CHECK_EQUAL(res, exp);
}

BOOST_AUTO_TEST_CASE(TestCartesianIntView)
{
	auto cp = ranges::v3::views::cartesian_product(ranges::v3::views::iota(0, 10),
												   ranges::v3::views::iota(0, 10));
	std::vector<std::tuple<int, int> > rng(cp.begin(), cp.end());
	BOOST_CHECK_EQUAL(ranges::v3::size(cp), 100);
}
BOOST_AUTO_TEST_SUITE_END()
