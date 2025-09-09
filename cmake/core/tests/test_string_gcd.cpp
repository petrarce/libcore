#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "utils.h"
#include "core/leetcode_solutions/strings_gcd.h"

using namespace libcore::solution;
BOOST_AUTO_TEST_SUITE(TestStringsGcd)

BOOST_AUTO_TEST_CASE(TestPrefixGcdCheck)
{
	const auto sdiv = &strings_gcd::isStringDivisor;
	BOOST_CHECK_EQUAL(sdiv("abc", "abc"), true);
	BOOST_CHECK_EQUAL(sdiv("ab", "abc"), false);
	BOOST_CHECK_EQUAL(sdiv("ab", "abab"), true);
	BOOST_CHECK_EQUAL(sdiv("abc", "abcabcabc"), true);
	BOOST_CHECK_EQUAL(sdiv("leet", "code"), false);
	BOOST_CHECK_EQUAL(sdiv("leet", "leetleet"), true);
	BOOST_CHECK_EQUAL(sdiv("the", "thecodethecodethecodethecode"), false);
	BOOST_CHECK_EQUAL(sdiv("code", "thecodethecodethecodethecode"), false);
	BOOST_CHECK_EQUAL(sdiv("the", "thesensorisoperational"), false);
	BOOST_CHECK_EQUAL(sdiv("thecode", "thecodethecodethecodethecode"), true);
	BOOST_CHECK_EQUAL(sdiv("thecodethecodethecodethecode", "thecode"), false);
}

BOOST_AUTO_TEST_CASE(TestStringsGcd)
{
	const auto gcd = &strings_gcd::solve;
	BOOST_CHECK_EQUAL(gcd("abc", "abc"), "abc");
	BOOST_CHECK_EQUAL(gcd("aaaaa", "aaa"), "a");
	BOOST_CHECK_EQUAL(gcd("aaaaaa", "aa"), "aa");
	BOOST_CHECK_EQUAL(gcd("ababab", "ab"), "ab");
	BOOST_CHECK_EQUAL(gcd("ababab", "abab"), "ab");
	BOOST_CHECK_EQUAL(gcd("LEET", "CODE"), "");
}

BOOST_AUTO_TEST_SUITE_END()
