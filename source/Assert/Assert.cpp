#include "Assert.h"
#include "Debug.h"
#include <string>       // std::string
#include <cstdlib>      // abort

namespace DE {

void assert(const bool condition, const std::string file, const u32 line, const std::string function, const std::string message){
  if(!condition){
    ERROR("DE_ASSERT > [ FILE: " + file + " FUNCTION: " + function + " LINE: " + std::to_string(line) + " ] " + message);
    abort();
  }
}

}
