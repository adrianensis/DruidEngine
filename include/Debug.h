#ifndef DEBUG_H_
#define DEBUG_H_

#include <string>
#include "BasicTypes.h"

using namespace std;

namespace DE {

namespace Debug {

template<typename T>
void echo(T message);
void var(string varname, auto var);
void var(auto var);
void error(string message);
void br();

};

} /* namespace DE */

#endif /* DEBUG_H_ */
