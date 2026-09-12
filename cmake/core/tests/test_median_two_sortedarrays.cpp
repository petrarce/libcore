#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/leetcode_solutions/median_two_sorted_arrays.h>
#include "utils.h"
using namespace leetcode;
BOOST_AUTO_TEST_SUITE(TestMedianTwoSortedArrays)

BOOST_AUTO_TEST_CASE(TestTrivialInput)
{
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::solve({ 1 }, {}), 1.);
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::solve({}, { 1 }), 1.);
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::solve({}, { 1, 2, 5, 6, 8 }), 5.);
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::solve({ 1, 2, 5, 6, 8 }, {}), 5.);
}

BOOST_AUTO_TEST_CASE(TestFindElementN)
{
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::find_element_n({ 1 }, {}, 0), 1);
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::find_element_n({}, { 1 }, 0), 1);
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::find_element_n({}, { 1, 2, 3, 4 }, 2), 3);
	BOOST_CHECK_EQUAL(median_two_sorted_arrays::find_element_n({ 1, 2, 3, 4 }, {}, 2), 3);

	BOOST_CHECK_EQUAL(median_two_sorted_arrays::find_element_n({ 1, 2, 5, 7 }, { 3, 4, 6 }, 2), 3);
}
BOOST_AUTO_TEST_SUITE_END()
