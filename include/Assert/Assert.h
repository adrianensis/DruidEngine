#ifndef DE_ASSERT_H_
#define DE_ASSERT_H_

#include <string>       // std::string

namespace DE {

static const std::string emptyAssert = "?";

void assert(const bool condition, const std::string message = emptyAssert);

#define DE_ASSERT(condition, message) assert((condition), "[ " #condition " ] " message);

} /* namespace DE */

#endif /* DE_ASSERT_H_ */
