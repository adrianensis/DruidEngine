#include "Core/Assert/Assert.hpp"
#include <string>	// SStr
#include <cstdlib>	// abort

//#include <experimental/filesystem>

void assert_msg(bool condition, CNS SStr &file, u32 line, CNS SStr &function,
				CNS SStr &message)
{
	if (!condition)
	{
		std::cout << "ASSERT > [" << function << ":"
				  << std::to_string(line) << "] > " << message << std::endl;
		throw;
		//abort();
	}
}