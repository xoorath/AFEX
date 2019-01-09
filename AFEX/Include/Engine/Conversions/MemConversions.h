#pragma once

#include <Engine/Types.h>

namespace afex {
constexpr u64 operator""_KiB(u64 amount) {
	return amount* 1024ULL;
}

constexpr u64 operator""_MiB(u64 amount) {
	return amount* 1024ULL * 1024ULL;
}

constexpr u64 operator""_GiB(u64 amount) {
	return amount * 1024ULL * 1024ULL * 1024ULL;
}

constexpr u64 operator""_TiB(u64 amount) {
	return amount * 1024ULL * 1024ULL * 1024ULL * 1024ULL;
}

}