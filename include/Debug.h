#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include <string>

using namespace std;

namespace DE {

namespace Debug {

void echo(string message);
void var(string varname, auto var);
void var(auto var);
void error(string message);
void br();

};

} /* namespace DE */

#endif /* DEBUG_H_ */
