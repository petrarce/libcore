#define BOOST_TEST_MODULE MyTestSuite
#include <boost/test/included/unit_test.hpp>
#include <gcd.hpp>
BOOST_AUTO_TEST_CASE (MyTestCase)
{
	BOOST_CHECK (libcore::math::gcd (20, 15) == 5);
	BOOST_CHECK (libcore::math::gcd (50, 48) == 2);
	BOOST_CHECK (libcore::math::gcd (50, 48) == 2);
	BOOST_CHECK (libcore::math::gcd (6, 2) == 2);
	BOOST_CHECK (libcore::math::gcd (12, 11) == 1);
}
