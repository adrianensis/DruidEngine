#include "Core/Assert/Assert.hpp"
#include <string>	// SStr
#include <cstdlib>	// abort

//#include <experimental/filesystem>

void assert_msg(bool condition, const SStr &file, u32 line, const SStr &function,
				const SStr &message)
{
	if (!condition)
	{
		std::cout << "ASSERT > [" << function << ":"
				  << std::to_string(line) << "] > " << message << std::endl;
		throw;
		//abort();
	}
}