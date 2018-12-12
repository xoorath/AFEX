#pragma once
#include <assert.h>
#include <limits>
#include <string>
#include <type_traits>

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
#else
typedef i32                  iptr;
typedef u32                  uptr;
#endif

typedef ::std::string        string;

namespace internal {
	template<typename To_t, typename From_t, bool toIsSigned, bool fromIsSigned>
	struct NumericChecker {
		static inline void CheckValue(From_t num) { }
	};

	template<typename To_t, typename From_t>
	struct NumericChecker<To_t, From_t, false, false> {
		static inline void CheckValue(From_t num) {
			assert(num >= ::std::numeric_limits<To_t>::min() && num <= ::std::numeric_limits<To_t>::max());
		}
	};

	template<typename To_t, typename From_t>
	struct NumericChecker<To_t, From_t, true, true> {
		static inline void CheckValue(From_t num) {
			assert(num >= ::std::numeric_limits<To_t>::min() && num <= ::std::numeric_limits<To_t>::max());
		}
	};

	template<typename To_t, typename From_t>
	struct NumericChecker<To_t, From_t, true, false> {
		static inline void CheckValue(From_t num) {
			assert(num <= static_cast<typename ::std::make_unsigned<To_t>::type>(::std::numeric_limits<To_t>::max()));
		}
	};

	template<typename To_t, typename From_t>
	struct NumericChecker<To_t, From_t, false, true> {
		static inline void CheckValue(From_t num) {
			assert(num >= 0 && num <= static_cast<typename ::std::make_unsigned<From_t>::type>(from) <= ::std::numeric_limits<To_t>::max());
		}
	};
}
template<
	typename To_t,
	typename From_t, 
	typename = ::std::enable_if<::std::is_arithmetic<From_t>::value && ::std::is_arithmetic<To_t>::value>>
inline To_t numeric_cast(From_t num) {
	::afex::internal::NumericChecker<
		::std::remove_cv<To_t>::type,
		::std::remove_cv<From_t>::type,
		::std::numeric_limits<To_t>::is_signed,
		::std::numeric_limits<From_t>::is_signed>::CheckValue(num);
	return static_cast<To_t>(num);
}
}