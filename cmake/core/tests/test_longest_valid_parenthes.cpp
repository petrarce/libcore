#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include <core/leetcode_solutions/longest_valid_parenthes.h>
#include "utils.h"
using namespace libcore::solutions;

BOOST_AUTO_TEST_SUITE(TestLongestValidParenthes)

BOOST_AUTO_TEST_CASE(TestIgnoreNonParenthes)
{
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve(""), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("abcx"), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("a"), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("1234abcd"), 0);
}
BOOST_AUTO_TEST_CASE(TestTrivialParenthes)
{
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("("), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve(")"), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve(")("), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("(("), 0);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("))"), 0);
}

BOOST_AUTO_TEST_CASE(TestValidSequences)
{
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("()"), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("(())"), 4);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("()()"), 4);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("()(())"), 6);
}

BOOST_AUTO_TEST_CASE(TestInvalidSequences)
{
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("(()"), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve(")()"), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("))()"), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("())"), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("())("), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("))(()("), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("))(()()"), 4);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("())()())()()()"), 6);
}

BOOST_AUTO_TEST_CASE(TestIncompleteSequence)
{
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("()(()"), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("(()("), 2);
	BOOST_CHECK_EQUAL(longest_valid_parenthes().solve("(())()(()(("), 6);
}

BOOST_AUTO_TEST_SUITE_END()
