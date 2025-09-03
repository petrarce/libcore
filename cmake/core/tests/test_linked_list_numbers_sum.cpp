#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include <core/leetcode_solutions/linked_list_number_sums.h>
#include "utils.h"
using namespace libcore::solutions;

ListNode* operator"" _ll(unsigned long long int v) { return ListNode::fromInt(v); }
constexpr int operator"" _i(long double v) { return static_cast<int>(v); }

BOOST_AUTO_TEST_SUITE(TestLinkedListdNumbersSum)

BOOST_AUTO_TEST_CASE(TestToListConversion)
{
	BOOST_CHECK_EQUAL(123_ll->toInt(), 123);
	BOOST_CHECK_EQUAL(512_ll->toInt(), 512);
	BOOST_CHECK_EQUAL(1500_ll->toInt(), 1500);
	BOOST_CHECK_EQUAL(2000_ll->toInt(), 2000);
	BOOST_CHECK_EQUAL(100000_ll->toInt(), 100000);
	BOOST_CHECK_EQUAL(100000000_ll->toInt(), 100000000);
}

BOOST_AUTO_TEST_CASE(TestListSum)
{
	auto sum = [](int a, int b)
	{
		return ListNode::toInt(
			linked_list_number_sums::sum(ListNode::fromInt(a), ListNode::fromInt(b)));
	};
	BOOST_CHECK_EQUAL(sum(23124, 32123), 23124 + 32123);
	BOOST_CHECK_EQUAL(sum(432098, 123), 432098 + 123);
	BOOST_CHECK_EQUAL(sum(904302993, 3278), 904302993 + 3278);
	BOOST_CHECK_EQUAL(sum(0, 1), 1);
	BOOST_CHECK_EQUAL(sum(0, 0), 0);
	BOOST_CHECK_EQUAL(sum(1000, 1000000), 1000 + 1000000);
}

BOOST_AUTO_TEST_SUITE_END()
