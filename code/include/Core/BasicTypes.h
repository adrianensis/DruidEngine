#ifndef DE_BASICTYPES_H
#define DE_BASICTYPES_H

#include <cstdint> // std::uintptr_t
#include <type_traits>

// FOR LOOPS

#define FOR_RANGE(it, start, size) for (u32 it = start; it < size; ++it)

#define FOR_ARRAY(it, array) FOR_RANGE(it, 0, array->getLength())
#define FOR_ARRAY_COND(it, array, condition) for (u32 it = 0; it < array->getLength() && condition; ++it)

#define FOR_LIST(it, list) for (auto it = list->getIterator(); !it.isNull(); it.next())
#define FOR_LIST_COND(it, list, cond) for (auto it = list->getIterator(); (!it.isNull()) && cond; it.next())

namespace DE {

using ptr = std::uintptr_t;
using ptrdiff = std::ptrdiff_t;

using f32 = float;
using f64 = double;

using c8 = char;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using byte = std::uint8_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using ClassId = u16;

} /* namespace DE */

#endif /* DE_BASICTYPES_H */
