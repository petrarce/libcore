#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/euler_project/problem_29.h>

namespace euler_project
{
class tets_problem_29 : problem_29
{
public:
	template<class... Args>
	auto number_unique_values(Args&&... args)
	{
		return pr.number_unique_values(std::forward<Args>(args)...);
	}

private:
	problem_29 pr;
};
} // namespace euler_project
using namespace euler_project;

BOOST_AUTO_TEST_SUITE(TestFactorization)

using ListFactors = std::vector<std::pair<unsigned, unsigned int> >;
BOOST_AUTO_TEST_CASE(TestProblem29)
{
	BOOST_CHECK_EQUAL(tets_problem_29().number_unique_values(2, 2), 1);
	BOOST_CHECK_EQUAL(tets_problem_29().number_unique_values(2, 3), 2);
	BOOST_CHECK_EQUAL(tets_problem_29().number_unique_values(5, 5), 15);
	BOOST_CHECK_EQUAL(tets_problem_29().number_unique_values(100, 100), 9183);
}

BOOST_AUTO_TEST_SUITE_END()
