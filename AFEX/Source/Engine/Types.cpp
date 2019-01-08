#include <Engine/Types.h>

#define AFEX_CHECK_SIZE(name, size) static_assert(sizeof(afex:: name) == size, #name " should be " #size " bytes");

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

// prevent warning about not exporting any symbols.
namespace afex { namespace internal { void TypesDummyFunction() {} } }