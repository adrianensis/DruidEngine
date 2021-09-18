#pragma once

#include <string> // SStr
#include "Core/BasicTypes.hpp"

STC CNS SStr emptyAssert = "?";

void assert_msg(bool condition, CNS SStr &file, u32 line, CNS SStr &function,
				CNS SStr &message = emptyAssert);

#define ASSERT_MSG(condition, message) assert_msg((condition), __FILE__, __LINE__, __FUNCTION__, SStr("[ " #condition " ] ") + message);
