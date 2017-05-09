#ifndef ASSERT_H_
#define ASSERT_H_

#include <string>       // std::string

namespace DE {

const std::string emptyAssert = "?";

void assert(bool condition, std::string message = emptyAssert);

} /* namespace DE */

#endif /* ASSERT_H_ */
