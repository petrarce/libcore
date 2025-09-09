#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/utils/ring_buffer.h>
using namespace libcore::utils;
BOOST_AUTO_TEST_SUITE(TestRingBuffer)

BOOST_AUTO_TEST_CASE(TestPushPopFrontBack)
{
	ring_buffer<int> rb(3);
	BOOST_CHECK_EQUAL(rb.size(), 0);
	rb.push_back(1);
	rb.push_back(2);
	rb.push_back(3);
	BOOST_CHECK_EQUAL(rb.size(), 3);
	BOOST_CHECK_EQUAL(rb.front(), 1);
	BOOST_CHECK_EQUAL(rb.back(), 3);
	rb.pop_front();
	BOOST_CHECK_EQUAL(rb.front(), 2);
	BOOST_CHECK_EQUAL(rb.back(), 3);
	BOOST_CHECK_EQUAL(rb.size(), 2);
	rb.push_back(4);
	BOOST_CHECK_EQUAL(rb.back(), 4);
	BOOST_CHECK_EQUAL(rb.size(), 3);
}
BOOST_AUTO_TEST_SUITE_END()
