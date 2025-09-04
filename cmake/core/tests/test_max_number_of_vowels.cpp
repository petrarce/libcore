#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/filters/soundex.h>
#include "utils.h"
#include "core/leetcode_solutions/max_number_of_viwels.h"

using namespace libcore::solutions;
BOOST_AUTO_TEST_SUITE(TestMaxNumberOfVowels)

BOOST_AUTO_TEST_CASE(TestCharStrings)
{
	// vowels
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("a", 1), 1);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("e", 1), 1);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("i", 1), 1);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("o", 1), 1);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("u", 1), 1);

	// non-vowels
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("b", 1), 0);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("c", 1), 0);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("d", 1), 0);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("f", 1), 0);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("g", 1), 0);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("h", 1), 0);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("x", 1), 0);
}

BOOST_AUTO_TEST_CASE(TestNormalStrings)
{
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("abcdefghijklmnopqrstuvqxyz", 5), 2);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("abcdefghijklmnopqrstuvqxyz", 2), 1);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("aaaaa", 2), 2);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("aaaaa", 3), 3);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("aaaaa", 4), 4);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("aaaaa", 5), 5);
	BOOST_CHECK_EQUAL(max_number_of_viwels::solve("abciiidef", 3), 3);
}

BOOST_AUTO_TEST_SUITE_END()
