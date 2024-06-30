#define BOOST_TEST_MODULE MyTestSuite
#include <boost/test/included/unit_test.hpp>
#include <math.h>
#include <absl/strings/str_format.h>
#include <vector>
BOOST_AUTO_TEST_CASE (MyTestCase)
{
	BOOST_CHECK (libcore::math::gcd (20, 15) == 5);
	BOOST_CHECK (libcore::math::gcd (50, 48) == 2);
	BOOST_CHECK (libcore::math::gcd (50, 48) == 2);
	BOOST_CHECK (libcore::math::gcd (6, 2) == 2);
	BOOST_CHECK (libcore::math::gcd (12, 11) == 1);
}

BOOST_AUTO_TEST_CASE (LogarythmWithArbitraryBase) {

	std::vector<std::pair<int64_t, int64_t>> squarePairs;
	for(int i = 2; i < 10000; i+= 313)
	{
		std::cout << absl::StrFormat("Testing: %d\n", i);
		BOOST_CHECK_CLOSE(libcore::math::log_b<float>(i, i * i), 2, 1e-6);
		BOOST_CHECK_CLOSE(libcore::math::log_b<float>(i * i, i), 0.5, 1e-6);
		BOOST_CHECK_CLOSE(libcore::math::log_b<float>(i, 1. / (i * i)), -2, 1e-6);
	}

}
