#include "Assert.h"

#include <iostream>       // std::cout, std::cerr
#include <string>       // std::string
//#include <exception>      // std::terminate
#include <cstdlib>      // abort

namespace DE {

void assert(bool condition, std::string message){
  if(!condition){
    std::cerr << "ASSERT ERROR: " << message << '\n';
    // std::terminate();
    abort();
  }
}

}
