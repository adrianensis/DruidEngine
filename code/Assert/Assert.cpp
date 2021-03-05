#include "Assert/Assert.hpp"
#include "Log/Log.hpp"
#include <string>   // String
#include <cstdlib>  // abort

//#include <experimental/filesystem>

namespace DE {

void assert(const bool condition, const String &file, u32 line, const String &function,
		const String &message) {
	if (!condition) {
		std::cout << "DE_ASSERT > [" << function << ":"
				<< std::to_string(line) << "] > " << message << std::endl;
		throw;
		//abort();
	}
}

}
