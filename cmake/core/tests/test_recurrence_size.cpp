#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include "utils.h"
#include "core/euler_project/problem_26.h"

using namespace euler_project;
namespace euler_project
{
class test_problem_26 : public problem_26
{
public:
	static auto find_max_recurrent(int upper_bound) { return _find_max_recurrent(upper_bound); }
	static auto recurrence_size(int val) { return _recurrence_size(val); }
};
} // namespace euler_project

BOOST_AUTO_TEST_SUITE(TestProblem26)

BOOST_AUTO_TEST_CASE(TestRecurrenceSize)
{
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(1), 0);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(2), 0);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(5), 0);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(3), 1);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(6), 1);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(7), 6);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(9), 1);
	BOOST_CHECK_EQUAL(test_problem_26::recurrence_size(983), 982);
}

BOOST_AUTO_TEST_CASE(TestFindValueWithRecurrent)
{
	BOOST_CHECK_EQUAL(test_problem_26::find_max_recurrent(10), 7);
	BOOST_CHECK_EQUAL(test_problem_26::find_max_recurrent(1000), 983);
}

BOOST_AUTO_TEST_SUITE_END()
