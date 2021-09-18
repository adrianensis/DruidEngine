#include "Log/Log.hpp"
#include "Core/BasicTypes.hpp"

//#include <filesystem>

CNS SStr Log::emptyMessage = "";
std::ofstream Log::logFile;

void Log::init()
{
	logFile.open("log.txt");
}

void Log::terminate()
{
	logFile.close();
}

void Log::log(CNS SStr &str)
{
	std::cout << str << std::endl;
	logFile << str << std::endl;
}

void Log::trace(CNS SStr file, u32 line, CNS SStr function, CNS SStr message)
{
	// std::cout << "TRACE > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
	log("TRACE > [" + function + ":" + std::to_string(line) + "] > " + message);
}

void Log::echo(CNS SStr &message)
{
	// std::cout << "ECHO > " << message << std::endl;
	log("ECHO > " + message);
}

void Log::error(CNS SStr &message)
{
	// std::cout << "ERROR > " << message << std::endl;
	log("ERROR > " + message);
}

void Log::brline()
{ // break line
	log(emptyMessage);
}
