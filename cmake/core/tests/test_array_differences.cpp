#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "utils.h"
#include "core/leetcode_solutions/array_differences.h"

using namespace libcore::solutions;
BOOST_AUTO_TEST_SUITE(TestArrayDifferences)

BOOST_AUTO_TEST_CASE(TestArrayDifferences)
{
	auto adiff = &array_differences::findDifference;
	auto vset = [](std::vector<int> v1, std::vector<int> v2) { return std::vector{ v1, v2 }; };
	BOOST_CHECK_EQUAL(adiff({ 0, 1 }, { 2, 3 }), vset({ 0, 1 }, { 2, 3 }));
	BOOST_CHECK_EQUAL(adiff({ 0, 1 }, { 1, 2 }), vset({ 0 }, { 2 }));
}

BOOST_AUTO_TEST_SUITE_END()
