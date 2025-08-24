#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>
#include <core/filters/soundex.h>
using namespace libcore::filters;
namespace std
{
std::ostream& operator<<(std::ostream& os, const soundex::CharCodePair& val)
{
	return os << "{ " << val.first << " " << val.second << " }";
}

std::ostream& operator<<(std::ostream& os, const std::optional<soundex::CharCodePair>& val)
{
	if (val.has_value())
		return os << val.value();

	return os << "undefined";
}

std::ostream& operator<<(std::ostream& os, const std::nullopt_t&) { return os << "undefined"; }

} // namespace std

BOOST_AUTO_TEST_SUITE(TestSoundexClass)

BOOST_AUTO_TEST_CASE(TestSoundexEncoding)
{
	// test invalid strings
	BOOST_CHECK_EQUAL(soundex::encode(""), std::nullopt);
	BOOST_CHECK_EQUAL(soundex::encode("12dsa"), std::nullopt);
	BOOST_CHECK_NE(soundex::encode("abc"), std::nullopt);
	// test valid words
	BOOST_CHECK_EQUAL(soundex::encode("a"), soundex::CharCodePair({ 'a', 0 }));
	BOOST_CHECK_EQUAL(soundex::encode("aouyhwie"), soundex::CharCodePair({ 'a', 0 }));
	BOOST_CHECK_EQUAL(soundex::encode("abc"), soundex::CharCodePair({ 'a', 120 }));
	BOOST_CHECK_EQUAL(soundex::encode("abb"), soundex::CharCodePair({ 'a', 100 }));
	BOOST_CHECK_EQUAL(soundex::encode("abeucidhlwnyre"), soundex::CharCodePair({ 'a', 123 }));

	// test merges
	BOOST_CHECK_EQUAL(soundex::encode("bobob"), soundex::CharCodePair({ 'b', 110 }));
	BOOST_CHECK_EQUAL(soundex::encode("bb"), soundex::CharCodePair({ 'b', 0 }));
	BOOST_CHECK_EQUAL(soundex::encode("bwb"), soundex::CharCodePair({ 'b', 0 }));
	BOOST_CHECK_EQUAL(soundex::encode("bwwbwb"), soundex::CharCodePair({ 'b', 100 }));
}

BOOST_AUTO_TEST_CASE(TestSoundexRealWordsFromWikipedia)
{
	/* Using this algorithm, both "Robert" and "Rupert" return the same string "R163"
	 * while "Rubin" yields "R150".
	 * "Ashcraft" and "Ashcroft" both yield "A226".
	 * "Tymczak" yields "T522" not "T520" (the chars 'z' and 'k' in the name are coded as 2 twice
	 * since a vowel lies in between them). "Pfister" yields "P236" not "P123" (the first two
	 * letters have the same number and are coded once as 'P'), and "Honeyman" yields "H555".*/
	BOOST_CHECK_EQUAL(soundex::encode("Robert"), soundex::CharCodePair({ 'r', 163 }));
	BOOST_CHECK_EQUAL(soundex::encode("Rupert"), soundex::CharCodePair({ 'r', 163 }));
	BOOST_CHECK_EQUAL(soundex::encode("Rubin"), soundex::CharCodePair({ 'r', 150 }));
	BOOST_CHECK_EQUAL(soundex::encode("Ashcraft"), soundex::CharCodePair({ 'a', 261 }));
	BOOST_CHECK_EQUAL(soundex::encode("Ashcroft"), soundex::CharCodePair({ 'a', 261 }));
	BOOST_CHECK_EQUAL(soundex::encode("Tymczak"), soundex::CharCodePair({ 't', 522 }));
	BOOST_CHECK_EQUAL(soundex::encode("Pfister"), soundex::CharCodePair({ 'p', 236 }));
	BOOST_CHECK_EQUAL(soundex::encode("Honeyman"), soundex::CharCodePair({ 'h', 555 }));
}

BOOST_AUTO_TEST_SUITE_END()
