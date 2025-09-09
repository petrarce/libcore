#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "utils.h"
#include "core/leetcode_solutions/merge_strings_alternately.h"

using namespace libcore::solutions;
BOOST_AUTO_TEST_SUITE(TestMergeAlternately)

BOOST_AUTO_TEST_CASE(TestAlternateintWords)
{
	BOOST_CHECK_EQUAL(merge_strings_alternately::solve("abc", "def"), "adbecf");
	BOOST_CHECK_EQUAL(merge_strings_alternately::solve("abc", "defgh"), "adbecfgh");
	BOOST_CHECK_EQUAL(merge_strings_alternately::solve("abcde", "def"), "adbecfde");
	BOOST_CHECK_EQUAL(merge_strings_alternately::solve("", "def"), "def");
	BOOST_CHECK_EQUAL(merge_strings_alternately::solve("abc", "abc"), "aabbcc");
}

BOOST_AUTO_TEST_SUITE_END()
