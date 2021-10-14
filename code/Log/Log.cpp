#include "Log/Log.hpp"
#include "Core/BasicTypes.hpp"

//#include <filesystem>

const SStr Log::emptyMessage = "";
std::ofstream Log::logFile;

void Log::init()
{
	logFile.open("log.txt");
}

void Log::terminate()
{
	logFile.close();
}

void Log::log(const SStr &str)
{
	std::cout << str << std::endl;
	logFile << str << std::endl;
}

void Log::append(const SStr &str)
{
    std::cout << '\r';
	std::cout << str;
	logFile << str;
    std::cout.flush();
}

void Log::trace(const SStr file, u32 line, const SStr function, const SStr message)
{
	// std::cout << "TRACE > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
	log("TRACE > [" + function + ":" + std::to_string(line) + "] > " + message);
}

void Log::echo(const SStr &message, bool newLine /*= true*/)
{
	// std::cout << "ECHO > " << message << std::endl;
    if(newLine)
    {
	    log("ECHO > " + message);
    }
    else
    {
	    append("ECHO > " + message);
    }
}

void Log::error(const SStr &message)
{
	// std::cout << "ERROR > " << message << std::endl;
	log("ERROR > " + message);
}

void Log::brline()
{ // break line
	log(emptyMessage);
}

void Log::backspace()
{
	std::cout << "\b \b";
    logFile << "\b \b";
}