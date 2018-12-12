#pragma once

#include <limits>
#include <type_traits>

namespace afex {
namespace internal {

template<typename To_t, typename From_t, bool toIsSigned, bool fromIsSigned>
struct NumericChecker {
	static inline void CheckValue(From_t num) { }
};

template<typename To_t, typename From_t>
struct NumericChecker<To_t, From_t, false, false> {
	static inline void CheckValue(From_t num) {
		AFEX_ASSERT_TRUE(num >= ::std::numeric_limits<To_t>::min() && num <= ::std::numeric_limits<To_t>::max());
	}
};

template<typename To_t, typename From_t>
struct NumericChecker<To_t, From_t, true, true> {
	static inline void CheckValue(From_t num) {
		AFEX_ASSERT_TRUE(num >= ::std::numeric_limits<To_t>::min() && num <= ::std::numeric_limits<To_t>::max());
	}
};

template<typename To_t, typename From_t>
struct NumericChecker<To_t, From_t, true, false> {
	static inline void CheckValue(From_t num) {
		AFEX_ASSERT_TRUE(num <= static_cast<typename ::std::make_unsigned<To_t>::type>(::std::numeric_limits<To_t>::max()));
	}
};

template<typename To_t, typename From_t>
struct NumericChecker<To_t, From_t, false, true> {
	static inline void CheckValue(From_t num) {
		AFEX_ASSERT_TRUE(num >= 0 && static_cast<typename ::std::make_unsigned<From_t>::type>(num) <= ::std::numeric_limits<To_t>::max());
	}
};

template<>
struct NumericChecker<float, double, false, true> {
	static inline void CheckValue(double num) {
		AFEX_ASSERT_TRUE(num <= ::std::numeric_limits<float>::max());
	}
};

template<>
struct NumericChecker<double, float, false, true> {
	static inline void CheckValue(float) {
	}
};

}
}