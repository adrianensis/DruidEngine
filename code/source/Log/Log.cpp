#include "Log.h"
#include "BasicTypes.h"

#include <experimental/filesystem>

namespace DE {

void log(const std::string file, u32 line, const std::string function, const std::string message) {
  std::cout << "DE_LOG > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
}

void echo(const std::string& message) {
  std::cout << "DE_ECHO > " << message << std::endl;
}

void error(const std::string& message) {
  std::cout << "DE_ERROR > " << message << std::endl;
}

void brline() { // break line
  std::cout << std::endl;
}

}
