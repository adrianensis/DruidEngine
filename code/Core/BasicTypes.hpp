#pragma once

#include <cstdint> // std::uintptr_t

#include <vector>
#include <list>
#include <array>
#include <map>
#include <set>

#include <string>
#include <iostream>
#include <fstream>

#include <type_traits>

#include <functional>

#include "Core/Macros.hpp"

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

// S for "std"
#define SVec std::vector
#define SLst std::list
#define SArr std::array
#define SMap std::map
#define SSet std::set

#define SStr std::string

#define SFun std::function