#include "Debug.h"
#include "BasicTypes.h"

namespace DE {

void echo(const std::string& message) {
  #ifdef DE_DEBUG
  std::cout << "D > " << message << std::endl;
  #endif
}

void error(const std::string& message) {
  #ifdef DE_DEBUG
  std::cout << "ERR > " << message << std::endl;
  #endif
}

void brline() { // break line
  #ifdef DE_DEBUG
  std::cout << std::endl;
  #endif
}

}
