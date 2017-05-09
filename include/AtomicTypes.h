#ifndef ATOMICTYPES_H_
#define ATOMICTYPES_H_


#include <cstdint> // std::uintptr_t

namespace DE {

using ptr = std::uintptr_t;
using ptrdiff = std::ptrdiff_t;

using f32 = float;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

} /* namespace DE */

#endif /* ATOMICTYPES_H_ */
