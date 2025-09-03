#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/filters/soundex.h>
#include "utils.h"
using namespace libcore::filters;


namespace std
{
std::ostream& operator<<(std::ostream& os, const soundex::SoundexEncoding& val)
{
	return os << "{ " << ~val << " }";
}

}
BOOST_AUTO_TEST_SUITE(TestSoundexClass)

BOOST_AUTO_TEST_CASE(TestSoundexEncoding)
{
	// clang-format off
	// test invalid strings
	BOOST_CHECK_EQUAL(soundex::encode(""),      std::nullopt);
	BOOST_CHECK_EQUAL(soundex::encode("12dsa"), std::nullopt);
	BOOST_CHECK_NE(soundex::encode("abc"),      std::nullopt);
	// test valid words
	BOOST_CHECK_EQUAL(~soundex::encode("a").value(),        "a000" );
	BOOST_CHECK_EQUAL(~soundex::encode("aouyhwie").value(), "a000" );
	BOOST_CHECK_EQUAL(~soundex::encode("abc").value(),      "a120"  );
	BOOST_CHECK_EQUAL(~soundex::encode("abb").value(),      "a100"  );
	BOOST_CHECK_EQUAL(~soundex::encode("abeucidhlwnyre").value(), "a123" );

	// test merges
	BOOST_CHECK_EQUAL(~soundex::encode("bobob").value(),  "b110" );
	BOOST_CHECK_EQUAL(~soundex::encode("bb").value(),     "b000" );
	BOOST_CHECK_EQUAL(~soundex::encode("bwb").value(),    "b000" );
	BOOST_CHECK_EQUAL(~soundex::encode("rwwrwr").value(), "r600" );
	// clang-format on
}

BOOST_AUTO_TEST_CASE(TestSoundexRealWordsFromWikipedia)
{
	/* Using this algorithm, both "Robert" and "Rupert" return the same string "R163"
	 * while "Rubin" yields "R150".
	 * "Ashcraft" and "Ashcroft" both yield "a261".
	 * "Tymczak" yields "T522" not "T520" (the chars 'z' and 'k' in the name are coded as 2 twice
	 * since a vowel lies in between them). "Pfister" yields "P236" not "P123" (the first two
	 * letters have the same number and are coded once as 'P'), and "Honeyman" yields "H555".*/
	// clang-format off

	BOOST_CHECK_EQUAL(~soundex::encode("Robert").value(),   "r163");
	BOOST_CHECK_EQUAL(~soundex::encode("Rupert").value(),   "r163");
	BOOST_CHECK_EQUAL(~soundex::encode("Rubin").value(),    "r150");
	BOOST_CHECK_EQUAL(~soundex::encode("Ashcraft").value(), "a261");
	BOOST_CHECK_EQUAL(~soundex::encode("Ashcroft").value(), "a261");
	BOOST_CHECK_EQUAL(~soundex::encode("Tymczak").value(),  "t522");
	BOOST_CHECK_EQUAL(~soundex::encode("Pfister").value(),  "p236");
	BOOST_CHECK_EQUAL(~soundex::encode("Honeyman").value(), "h555");

	// clang-format on
}

BOOST_AUTO_TEST_SUITE_END()
