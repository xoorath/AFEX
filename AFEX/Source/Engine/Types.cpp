#include <Engine/Types.h>
#include <stdint.h>

#define AFEX_CHECK_SIZE(name, compareWith) static_assert(sizeof(afex:: name) == compareWith, #name " should be " #compareWith " bytes");

AFEX_CHECK_SIZE(byte, 1);
AFEX_CHECK_SIZE(i8, 1);
AFEX_CHECK_SIZE(u8, 1);

AFEX_CHECK_SIZE(i16, 2);
AFEX_CHECK_SIZE(u16, 2);

AFEX_CHECK_SIZE(i32, 4);
AFEX_CHECK_SIZE(u32, 4);
AFEX_CHECK_SIZE(f32, 4);

AFEX_CHECK_SIZE(i64, 8);
AFEX_CHECK_SIZE(u64, 8);
AFEX_CHECK_SIZE(f64, 8);

AFEX_CHECK_SIZE(iptr, sizeof(void*));
AFEX_CHECK_SIZE(uptr, sizeof(void*));

AFEX_CHECK_SIZE(isize, sizeof(std::ptrdiff_t));
AFEX_CHECK_SIZE(usize, sizeof(std::size_t));

// prevent warning about not exporting any symbols.
namespace afex { namespace internal { void TypesDummyFunction() {} } }