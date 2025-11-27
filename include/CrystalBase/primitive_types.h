#ifndef CRYSTAL_BASE_PRIMITIVE_TYPES_H_
#define CRYSTAL_BASE_PRIMITIVE_TYPES_H_

#include <cstdint> // int8 ...
#include <cstddef> // size_t
#include <uchar.h> // char16_t, char32_t

namespace crystal {
/* Signed Integers */
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
/* Unsigned Integers */
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
/* Characters */
using char8 = char;
using char16 = char16_t;
using char32 = char32_t;
using wchar = wchar_t;
/* Floats */
using float32 = float;
using float64 = double;
using float128 = long double;
/* Booleans */
//using bool = bool;
/* Other */
using byte = std::byte;
using size = size_t;
using nullptr_t = std::nullptr_t;
//using void = void;
} // namespace crystal

#endif
