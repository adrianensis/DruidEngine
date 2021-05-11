#pragma once

#include <string>  // std::string
#include "Core/BasicTypes.hpp"


static const std::string emptyAssert = "?";

void assert(const bool condition, const std::string& file, u32 line, const std::string& function,
		const std::string& message = emptyAssert);

#define ASSERT(condition, message) assert((condition), __FILE__, __LINE__, __FUNCTION__, "[ " #condition " ] " message);

