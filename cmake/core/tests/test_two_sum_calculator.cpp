#define BOOST_TEST_NO_MAIN

#include <vector>
#include <boost/test/unit_test.hpp>
#include <core/leetcode_solutions //two_sum_calculator.h>
#include "utils.h"
using namespace libcore::solutions;

constexpr int operator"" _i(long double v) { return static_cast<int>(v); }
static_assert(45.3_i == 45);

BOOST_AUTO_TEST_SUITE(TestMath)

BOOST_AUTO_TEST_CASE(TestTwoSumEmptyOutput)
{
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{}, 1), std::nullopt);
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{}, 0), std::nullopt);
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 2, 5, 8 }, 0), std::nullopt);
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 1, 5, 3 }, 2), std::nullopt);
	// consider only either 2 indices or nothing
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 2 }, 2), std::pair(0, 0));
}

BOOST_AUTO_TEST_CASE(TestTrivialSums)
{
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 1, 1 }, 2), std::pair(0, 1));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 5, 6 }, 11), std::pair(0, 1));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 7, 5 }, 12), std::pair(0, 1));
}

BOOST_AUTO_TEST_CASE(TestMoreComplex)
{
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 1, 2, 3, 5 }, 7),
					  std::pair(1, 3));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 5, 3, 2, 1 }, 7),
					  std::pair(0, 2));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 2, 5, 1, 3 }, 7),
					  std::pair(0, 1));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 2, 7, 11, 15 }, 9),
					  std::pair(0, 1));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 5, 7, 9, -5 }, 0),
					  std::pair(0, 3));
}

BOOST_AUTO_TEST_CASE(TestLimits)
{
	// -10^9 <= nums[i] <= 10^92
	// -10^9 <= target <= 10^9
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 9 * 1e8_i, 1e8_i }, 1e9_i),
					  std::pair(0, 1));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(std::vector<int>{ 9 * -1e8_i, -1e8_i }, -1e9_i),
					  std::pair(0, 1));
	BOOST_CHECK_EQUAL(two_sum_calculator::solve(
						  std::vector<int>{ 1e6_i, 1e8_i, 1e7_i, 1e5_i, 1e2_i }, 1e7_i + 1e2_i),
					  std::pair(2, 4));
	BOOST_CHECK_EQUAL(
		two_sum_calculator::solve(std::vector<int>{ -1e6_i, -1e8_i, -1e7_i, -1e5_i, -1e2_i },
								  -1e7_i - 1e2_i),
		std::pair(2, 4));
}

BOOST_AUTO_TEST_SUITE_END()
