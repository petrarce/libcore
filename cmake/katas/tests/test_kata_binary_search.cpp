#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>
#include <katas/binary-search-kata/binary_search.h>

using namespace libcore::katas;

BOOST_AUTO_TEST_SUITE(TestBinarySearchKatas)

template<class T>
class BinarySearchFixture
{
public:
	void test()
	{
		auto bsf = &T::find;
		BOOST_CHECK_EQUAL(bsf({ 1 }, 1), 0);
		BOOST_CHECK_EQUAL(bsf({ 1 }, 0), -1);
		BOOST_CHECK_EQUAL(bsf({ 1, 2, 3 }, 3), 2);
		BOOST_CHECK_EQUAL(bsf({ 1, 2, 3 }, 1), 0);
		BOOST_CHECK_EQUAL(bsf({ 1, 2, 3, 4, 5 }, 3), 2);
		BOOST_CHECK_EQUAL(bsf({ 1, 2, 2, 2, 3 }, 2), 1);
		BOOST_CHECK_EQUAL(bsf({ 1, 2 }, 1), 0);
		BOOST_CHECK_EQUAL(bsf({ 1, 2 }, 2), 1);
		BOOST_CHECK_EQUAL(bsf({ 1, 2 }, 0), -1);
		BOOST_CHECK_EQUAL(bsf({ 1, 2 }, 3), -1);
	}
};

// BOOST_FIXTURE_TEST_CASE(TestBinarySearchTraversal,
// BinarySearchFixture<binary_search_array_traversal>)
// {
// 	test();
// }
BOOST_FIXTURE_TEST_CASE(TestBinarySearthLowerBound,
						BinarySearchFixture<binary_search_array_lower_bound>)
{
	test();
}

BOOST_AUTO_TEST_SUITE_END()
