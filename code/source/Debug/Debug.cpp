#include "Debug.h"
#include "BasicTypes.h"

namespace DE {

void echo(const std::string& message) {
  std::cout << "D > " << message << std::endl;
}

void error(const std::string& message) {
  std::cout << "ERR > " << message << std::endl;
}

void brline() { // break line
  std::cout << std::endl;
}

}
