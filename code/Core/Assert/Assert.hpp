#pragma once

#include <string>  // String
#include "Core/BasicTypes.hpp"

namespace DE {

static const String emptyAssert = "?";

void assert(const bool condition, const String &file, u32 line, const String &function,
		const String &message = emptyAssert);

#define ASSERT(condition, message) assert((condition), __FILE__, __LINE__, __FUNCTION__, "[ " #condition " ] " message);

}

