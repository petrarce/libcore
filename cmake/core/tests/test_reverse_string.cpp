
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/filters/soundex.h>
#include "utils.h"
#include "core/leetcode_solutions/reverse_string.h"

using namespace libcore::solutions;
BOOST_AUTO_TEST_SUITE(TestReverseString)

auto solve = &reverse_string::solve;

BOOST_AUTO_TEST_CASE(TestEmptyString)
{
	BOOST_CHECK_EQUAL(solve("  "), "");
	BOOST_CHECK_EQUAL(solve(""), "");
	BOOST_CHECK_EQUAL(solve("              "), "");
}

BOOST_AUTO_TEST_CASE(TestOneWord)
{
	BOOST_CHECK_EQUAL(solve("hello"), "hello");
	BOOST_CHECK_EQUAL(solve("   hello"), "hello");
	BOOST_CHECK_EQUAL(solve("hello  "), "hello");
	BOOST_CHECK_EQUAL(solve(" hello "), "hello");
	BOOST_CHECK_EQUAL(solve(" Hello "), "Hello");
}

BOOST_AUTO_TEST_CASE(TestMultipleWords)
{
	BOOST_CHECK_EQUAL(solve("The brown little fox"), "fox little brown The");
	BOOST_CHECK_EQUAL(solve(" The  brown    little   fox   "), "fox little brown The");
}

BOOST_AUTO_TEST_SUITE_END()
