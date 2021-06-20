#pragma once

#include <string> // std::string
#include "Core/BasicTypes.hpp"

static const std::string emptyAssert = "?";

void assert_msg(const bool condition, const std::string &file, u32 line, const std::string &function,
				const std::string &message = emptyAssert);

#define ASSERT_MSG(condition, message) assert_msg((condition), __FILE__, __LINE__, __FUNCTION__, "[ " #condition " ] " message);
