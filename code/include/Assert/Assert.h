#ifndef DE_DE_ASSERT_H
#define DE_DE_ASSERT_H

#include <string>  // std::string
#include "BasicTypes.h"

namespace DE {

static const std::string emptyAssert = "?";

void assert(const bool condition, const std::string file, const u32 line, const std::string function, const std::string message = emptyAssert);

#define DE_ASSERT(condition, message) assert((condition), __FILE__, __LINE__, __FUNCTION__, "[ " #condition " ] " message);

} /* namespace DE */

#endif /* DE_DE_ASSERT_H */
