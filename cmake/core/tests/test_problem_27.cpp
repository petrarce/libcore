#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "core/euler_project/problem_27.h"

using namespace euler_project;

BOOST_AUTO_TEST_SUITE(TestProblem27)

BOOST_AUTO_TEST_CASE(TestCountPrimes)
{
	BOOST_CHECK_EQUAL(problem_27::_count(0, 0), 0);
	BOOST_CHECK_EQUAL(problem_27::_count(0, 1), 3);
	BOOST_CHECK_EQUAL(problem_27::_count(1, 0), 0);
	BOOST_CHECK_EQUAL(problem_27::_count(1, 1), 4);
	BOOST_CHECK_EQUAL(problem_27::_count(1, 2), 1);
	BOOST_CHECK_EQUAL(problem_27::_count(1, 41), 40);
	BOOST_CHECK_EQUAL(problem_27::_count(-79, 1601), 80);
}

BOOST_AUTO_TEST_SUITE_END()
