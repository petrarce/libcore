#define BOOST_TEST_MODULE TestInfInt
#include <InfInt.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE (InfIntTest)
{
	InfInt v1 = "333333333333333333333333333333";
	InfInt v2 = "-333333333333333333333333333333";
	InfInt res = v1 + v2;
	BOOST_CHECK (res.toString () == "0");
	res = v1 - v2;
	BOOST_CHECK (res.toString () == "666666666666666666666666666666");
	BOOST_CHECK ((v1 * 2).toString () == "666666666666666666666666666666");
	BOOST_CHECK (res / 2 == v1);
}
