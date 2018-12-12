#pragma once

#include <Engine/HashInternal.h>
#include <Engine/Types.h>

namespace afex {
namespace hash {

constexpr u32 GetHash32(char const* str, u32 len) {
	return ~::afex::hash::internal::GetCrc32(str, len);
}

constexpr u32 GetHash32(char const* str) {
	return ~::afex::hash::internal::GetCrc32(str, ::afex::hash::internal::StrLen(str));
}

}
}