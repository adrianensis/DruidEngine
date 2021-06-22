#include "Core/Assert/Assert.hpp"
#include <string>	// std::string
#include <cstdlib>	// abort
#include <iostream> // abort

//#include <experimental/filesystem>

void assert_msg(bool condition, const std::string &file, u32 line, const std::string &function,
				const std::string &message)
{
	if (!condition)
	{
		std::cout << "ASSERT > [" << function << ":"
				  << std::to_string(line) << "] > " << message << std::endl;
		throw;
		//abort();
	}
}