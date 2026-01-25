#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "core/math/factorization.hpp"
#include "utils.h"

using namespace libcore::math;
BOOST_AUTO_TEST_SUITE(TestFactorization)

using ListFactors = std::vector<std::pair<unsigned, unsigned int> >;
BOOST_AUTO_TEST_CASE(TestSimpleFactorization)
{

	BOOST_CHECK_EQUAL(simple_factorize(0u), ListFactors());
	BOOST_CHECK_EQUAL(simple_factorize(1u), ListFactors());
	BOOST_CHECK_EQUAL(simple_factorize(2u), ListFactors({ { 2, 1 } }));
	BOOST_CHECK_EQUAL(simple_factorize(2u << 1), ListFactors({ { 2, 2 } }));
	BOOST_CHECK_EQUAL(simple_factorize(2u << 2), ListFactors({ { 2, 3 } }));
	BOOST_CHECK_EQUAL(simple_factorize(2u << 3), ListFactors({ { 2, 4 } }));
	BOOST_CHECK_EQUAL(simple_factorize(2u << 4), ListFactors({ { 2, 5 } }));
	BOOST_CHECK_EQUAL(simple_factorize(3u * 3u * 3u), ListFactors({ { 3, 3 } }));

	BOOST_CHECK_EQUAL(simple_factorize(3u * 3u * 5u * 7u * 7u * 2),
					  ListFactors({ { 2, 1 }, { 3, 2 }, { 5, 1 }, { 7, 2 } }));
}

BOOST_AUTO_TEST_SUITE_END()
