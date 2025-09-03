#define BOOST_TEST_NO_MAIN

#include <vector>
#include <boost/test/unit_test.hpp>
#include <core/leetcode_solutions/max_average_subarray.h>
#include "utils.h"
using namespace libcore::solutions;
using namespace test_utils;
BOOST_AUTO_TEST_SUITE(TestMaxAverageSubarray)

BOOST_AUTO_TEST_CASE(TestFullArrayAverage)
{
	BOOST_CHECK_CLOSE(max_average_subarray::solve({ 1, 1, 1, 1, 1 }, 5), 1, 1e-5);
	BOOST_CHECK_CLOSE(max_average_subarray::solve({ 2, 2, 2, 2, 2 }, 5), 2, 1e-5);
	BOOST_CHECK_CLOSE(max_average_subarray::solve({ 1e4_i, 1e4_i, 1e4_i, 1e4_i, 1e4_i }, 5), 1e4,
					  1e-5);
	BOOST_CHECK_CLOSE(max_average_subarray::solve({ -1e4_i, -1e4_i, -1e4_i, -1e4_i, -1e4_i }, 5),
					  -1e4, 1e-5);
	BOOST_CHECK_CLOSE(max_average_subarray::solve({ 5 }, 1), 5, 1e-5);
}

BOOST_AUTO_TEST_CASE(TestK1IsMaximum)
{
	{
		std::vector<int> arr = { -1, -5, -8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		BOOST_CHECK_CLOSE(max_average_subarray::solve(arr, 1), std::ranges::max(arr), 1e-5);
	}
	{
		std::vector<int> arr = { -1e4_i, -5e3_i, -800,	1e5_i, 2e3_i, 3e3_i, 4e2_i,
								 5e1_i,	 6e3_i,	 7e2_i, 8e3_i, 9e1_i, 10 };
		BOOST_CHECK_CLOSE(max_average_subarray::solve(arr, 1), std::ranges::max(arr), 1e-5);
	}
}

BOOST_AUTO_TEST_CASE(TestSlidingAverageArbitraryK)
{
	BOOST_CHECK_CLOSE(max_average_subarray::solve({ 1, 12, -5, -6, 50, 3 }, 4), 12.75, 1e-5);
}

BOOST_AUTO_TEST_SUITE_END()
