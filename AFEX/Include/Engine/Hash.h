#pragma once

#include <Engine/HashInternal.h>
#include <Engine/Types.h>

namespace afex {
namespace hash {
constexpr hash32 GetHash32(char const* str) {
	return internal::xxh32(str, internal::StrLen(str), XXHASH_CX_XXH32_SEED);
}
constexpr hash32 InvalidHash32 = GetHash32("");

constexpr hash64 GetHash64(char const* str) {
	return internal::xxh64(str, internal::StrLen(str), XXHASH_CX_XXH64_SEED);
}
#pragma warning(push)
// warning C4307: '*': integral constant overflow
#pragma warning(disable : 4307)
constexpr hash64 InvalidHash64 = GetHash64("");
#pragma warning(pop)
}
}