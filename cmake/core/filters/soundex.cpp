#include "soundex.h"

#include <optional>
#include <cctype>
#include <absl/strings/internal/str_format/parser.h>
#include <algorithm>
namespace
{
uint8_t to_soundex_digit(char letter)
{
	switch (letter)
	{
	case 'b':
	case 'f':
	case 'p':
	case 'v':
		return 1;

	case 'c':
	case 'g':
	case 'j':
	case 'k':
	case 'q':
	case 's':
	case 'x':
	case 'z':
		return 2;

	case 'd':
	case 't':
		return 3;

	case 'l':
		return 4;

	case 'n':
	case 'm':
		return 5;

	case 'r':
		return 6;
	default:
		assert(false && "no viable conversion for given letter");
	}
}

bool is_soundex_vowel(char letter)
{
	switch (letter)
	{
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
	case 'y':
	case 'h':
	case 'w':
		return true;
	default:
		return false;
	}
}
bool isalpha(const std::string& str)
{
	for (auto letter : str)
		if (!std::isalpha(letter))
			return false;
	return true;
}

} // namespace
namespace libcore
{
namespace filters
{
std::optional<soundex::CharCodePair> soundex::encode(const std::string& str)
{
	// * Algorythm description:
	// * 1. Assign B to a first letter of a word
	// * 2. traverse the word letter by letter
	// * 2.1 ignore vowels: a, e, i, o, u, y, h, w.
	// * 2.2 assign each conconance a number:
	//    b, f, p, v → 1
	//    c, g, j, k, q, s, x, z → 2
	//    d, t → 3
	//    l → 4
	//    m, n → 5
	//    r → 6
	// * 2.3 If two conconances with the same number are adjussent then ignore second one.
	// * 2.4 If two consonances with same number are separated with h, w or y ignore second one
	// * 2.5 2.3 and 2.4 also apply for the first letter
	// * 3. Stop after 3 digits already given. Append with 0-th if less then 3 digits are given.

	if (str.empty())
		return std::nullopt;

	if (!isalpha(str))
		return std::nullopt;

	// remove repeating characters within the string.
	// This resolves rule 2.3.
	// But remove all repeating except w, h and y
	auto str_unique = [str = str]() mutable
	{
		std::transform(str.begin(), str.end(), str.begin(), [](char l) { return std::tolower(l); });

		auto to_remove = std::unique(str.begin(), str.end(),
									 [](char l, char r)
									 {
										 if (!is_soundex_vowel(l) && !is_soundex_vowel(r))
											 return to_soundex_digit(l) == to_soundex_digit(r);
										 return l == r && l != 'w' && l != 'h' && l != 'y';
									 });
		str.erase(to_remove, str.end());
		return str;
	}();

	char firstLetter = str_unique[0];
	uint16_t digit = 0;

	std::optional<int> idx_merged = std::nullopt;
	for (int idx = 1; idx < str_unique.size(); ++idx)
	{
		const auto lc_letter = str_unique[idx];
		if (is_soundex_vowel(lc_letter))
			continue;

		const auto lc_code = to_soundex_digit(lc_letter);
		const auto lc_letter_prev = str_unique[idx - 1];

		// implement rule 2.4
		if (idx >= 2 && !is_soundex_vowel(str_unique[idx - 2])
			&& (lc_letter_prev == 'w' || lc_letter_prev == 'h' || lc_letter_prev == 'y')
			&& to_soundex_digit(str_unique[idx - 2]) == lc_code)
			continue;

		digit = digit * 10 + lc_code;
		idx_merged = idx;
		if (digit > 100)
			break;
	}

	while (digit < 100 && digit != 0)
		digit = digit * 10;

	return std::optional(std::pair{ firstLetter, digit });
}
} // namespace filters
} // namespace libcore
