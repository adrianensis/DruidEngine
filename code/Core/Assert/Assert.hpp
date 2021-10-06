#pragma once

#include <string> // SStr
#include "Core/BasicTypes.hpp"

static const SStr emptyAssert = "?";

void assert_msg(bool condition, const SStr &file, u32 line, const SStr &function,
				const SStr &message = emptyAssert);

#define ASSERT_MSG(condition, message) assert_msg((condition), __FILE__, __LINE__, __FUNCTION__, SStr("[ " #condition " ] ") + message);
