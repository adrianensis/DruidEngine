#pragma once

#include <cstdint> // std::uintptr_t
#include <string>

using ptr = std::uintptr_t;
using ptrdiff = std::ptrdiff_t;

using f32 = float;
using f64 = double;

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using byte = std::uint8_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using ClassId = u32;
using ObjectId = u32;