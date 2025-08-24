//
// Created by ivan on 8/24/25.
//

#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>
#include <optional>

namespace libcore
{
namespace filters
{

/// The class implements Soundex algorythm
/**
 * According to Soundex algorythm given a word the encoding sequence,
 * of the forman BXXX will be returned, where B is a letter and X is a
 * number between [0-9].
 */
class soundex
{
public:
	using CharCodePair = std::pair<char, uint16_t>;
	static std::optional<CharCodePair> encode(const std::string& string);
};

} // namespace filters
} // namespace libcore

#endif // SOUNDEX_H
