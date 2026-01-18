#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "core/euler_project/problem_28.h"

using namespace euler_project;

namespace euler_project
{
class test_problem_28
{

public:
	uint64_t level_sum(uint64_t level) { return pr.level_sum(level); }
	uint64_t sum_till_level(uint64_t level) { return pr.sum_till_level(level); }
	problem_28 pr;
};
} // namespace euler_project

BOOST_AUTO_TEST_SUITE(TestProblem28)

BOOST_FIXTURE_TEST_CASE(TestLevelSum, test_problem_28)
{
	BOOST_CHECK_EQUAL(level_sum(0), 1);
	BOOST_CHECK_EQUAL(level_sum(1), 3 + 5 + 7 + 9);
	BOOST_CHECK_EQUAL(level_sum(2), 13 + 17 + 21 + 25);
}

BOOST_FIXTURE_TEST_CASE(TestSumTillLevel, test_problem_28)
{
	BOOST_CHECK_EQUAL(sum_till_level(2), 101);
}
BOOST_AUTO_TEST_SUITE_END()
