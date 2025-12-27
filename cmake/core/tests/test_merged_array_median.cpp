#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "utils.h"
#include "core/leetcode_solutions/two_arrays_median.h"

using namespace libcore::solutions;

double compute_median_and_test_commutation(const std::vector<int>& nums1,
										   const std::vector<int>& nums2)
{
	const auto median1 = two_arrays_median::solve(nums1, nums2);
	const auto median2 = two_arrays_median::solve(nums2, nums1);
	BOOST_CHECK_EQUAL(median1, median2);
	return median1;
}
BOOST_AUTO_TEST_SUITE(TestMergedArraysMedian)

BOOST_AUTO_TEST_CASE(TestAllArraysEmpty)
{
	BOOST_CHECK_THROW(two_arrays_median::solve({}, {}), std::invalid_argument);
}
BOOST_AUTO_TEST_CASE(TestEmptyArray)
{
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 4 }, {}), 4);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({}, { 4 }), 4);

	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2, 3, 4, 5 }, {}), 3);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({}, { 1, 2, 3, 4, 5 }), 3);

	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2, 3, 4, 5, 6 }, {}), 3.5);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({}, { 1, 2, 3, 4, 5, 6 }), 3.5);
}

BOOST_AUTO_TEST_CASE(TestTwoArrays)
{
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2 }, { 3, 4 }), 2.5);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 3, 4 }, { 1, 2 }), 2.5);

	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2, 3, 4 }, { 1, 2, 3, 4 }), 2.5);

	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2, 3 }, { 4, 5 }), 3);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 4, 5 }, { 1, 2, 3 }), 3);
}

BOOST_AUTO_TEST_CASE(TestNonMixingArrays)
{
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2, 3 }, { 3, 4 }), 3);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 2, 3, 4 }, { 1, 2 }), 2);

	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 3, 4 }, { 1, 2, 3 }), 3);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2 }, { 2, 3, 4 }), 2);

	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 2, 3 }, { 4, 5, 6 }), 3.5);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 4, 5, 6 }, { 1, 2, 3 }), 3.5);
}

BOOST_AUTO_TEST_CASE(TestMixingArrays)
{
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 3, 5 }, { 2, 4, 6 }), 3.5);
	BOOST_CHECK_EQUAL(compute_median_and_test_commutation({ 1, 3, 5, 7 }, { 4 }), 4);
}

BOOST_AUTO_TEST_CASE(TestComputeIdxInMerged)
{
	BOOST_CHECK_EQUAL(
		two_arrays_median::idx_in_merged_array(1, std::vector{ 1, 3, 5 }, std::vector{ 2, 4, 6 }),
		2);
	BOOST_CHECK_EQUAL(
		two_arrays_median::idx_in_merged_array(2, std::vector{ 1, 3, 5 }, std::vector{ 2, 4, 6 }),
		4);
	BOOST_CHECK_EQUAL(two_arrays_median::idx_in_merged_array(1, std::vector{ 1, 3, 5 },
															 std::vector{ 2, 3, 3, 3, 4, 6 }),
					  2);
	BOOST_CHECK_EQUAL(two_arrays_median::idx_in_merged_array(
						  1, std::vector{ 1, 3, 5 }, std::vector{ 2, 3, 3, 3, 4, 6 }, false),
					  5);
}

BOOST_AUTO_TEST_CASE(TestMedianInMixingOdd)
{
	BOOST_CHECK_THROW(two_arrays_median::median_in_mixing_odd({}, {}), std::invalid_argument);
	BOOST_CHECK_EQUAL(two_arrays_median::median_in_mixing_odd({ 2, 4, 6, 8 }, { 1, 3, 5, 7, 9 }),
					  5);
	BOOST_CHECK_EQUAL(two_arrays_median::median_in_mixing_odd({ 2, 4, 6, 8 }, { 9 }), 6);
	BOOST_CHECK_EQUAL(two_arrays_median::median_in_mixing_odd({ 2 }, { 3, 5, 7, 9 }), 5);
	BOOST_CHECK_EQUAL(two_arrays_median::median_in_mixing_odd({ 6, 8 }, { 1, 3, 5, 7, 9 }), 6);
	BOOST_CHECK_EQUAL(two_arrays_median::median_in_mixing_odd({ 6, 8 }, { 1, 3, 5, 7, 9 }), 6);
	BOOST_CHECK_EQUAL(two_arrays_median::median_in_mixing_odd({ 2, 4, 6 }, { 5, 7 }), 5);
}
BOOST_AUTO_TEST_SUITE_END()
