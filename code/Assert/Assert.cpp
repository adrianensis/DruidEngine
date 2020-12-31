#include "Assert/Assert.hpp"
#include "Log/Log.hpp"
#include <string>   // std::string
#include <cstdlib>  // abort

//#include <experimental/filesystem>

namespace DE {

void assert(const bool condition, const std::string &file, u32 line, const std::string &function,
		const std::string &message) {
	if (!condition) {
		std::cout << "DE_ASSERT > [" << /*std::filesystem::path(file).filename() <<*/":" << function << ":"
				<< std::to_string(line) << "] > " << message << std::endl;
		throw;
		//abort();
	}
}

}
