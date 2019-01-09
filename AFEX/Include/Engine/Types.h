#pragma once
#include <Engine/Log.h>

#include <string>
#include <type_traits>

#include <Engine/TypesInternal.h>

namespace afex {
typedef signed char           i8;
typedef unsigned char         u8;

typedef signed short          i16;
typedef unsigned short        u16;

typedef signed int            i32;
typedef unsigned int          u32;
typedef float                 f32;

typedef signed long long      i64;
typedef unsigned long long    u64;
typedef double                f64;

#if _WIN64 || __x86_64__ || __ppc64__
typedef i64                  iptr;
typedef u64                  uptr;
typedef i64                  isize;
typedef u64                  usize;
#else
typedef i32                  iptr;
typedef u32                  uptr;
typedef i32                  isize;
typedef u32                  usize;
#endif

typedef u32                  hash32;
typedef u64                  hash64;

typedef ::std::string        string;

template<
	typename To_t,
	typename From_t, 
	typename = ::std::enable_if_t<::std::is_arithmetic<From_t>::value && ::std::is_arithmetic<To_t>::value>>
inline To_t numeric_cast(From_t num) {
	::afex::internal::NumericChecker<
		::std::remove_cv<To_t>::type,
		::std::remove_cv<From_t>::type,
		::std::numeric_limits<To_t>::is_signed,
		::std::numeric_limits<From_t>::is_signed>::CheckValue(num);
	return static_cast<To_t>(num);
}

template<
	typename To_t,
	typename From_t,
	typename = ::std::enable_if_t<::std::is_pod<To_t>::value && ::std::is_pod<From_t>::value>>
inline To_t pun_cast(From_t num) {
	union {
		::std::remove_cv<From_t>::type asFrom;
		To_t asTo;
	};
	asFrom = num;
	return asTo;
}

}