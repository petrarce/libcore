//
// Created by ivan on 8/24/25.
//

#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <string>
#include <optional>
#include "../utils/StrongTypedef.h"

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
	using SoundexEncoding = StrongTypedef<struct SoundexEncoding_t, uint16_t>;

	static std::optional<SoundexEncoding> encode(const std::string& string);
	static std::string encoding_to_string(const SoundexEncoding& encoding);
};
std::string operator~(soundex::SoundexEncoding encoding);

} // namespace filters
} // namespace libcore

#endif // SOUNDEX_H
