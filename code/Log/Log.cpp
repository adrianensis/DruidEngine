#include "Log/Log.hpp"
#include "Core/BasicTypes.hpp"

//#include <filesystem>

const std::string Log::emptyMessage = "";
std::ofstream Log::logFile;

void Log::init()
{
	logFile.open("log.txt");
}

void Log::terminate()
{
	logFile.close();
}

void Log::log(const std::string &str)
{
	std::cout << str << std::endl;
	logFile << str << std::endl;
}

void Log::append(const std::string &str)
{
    std::cout << '\r';
	std::cout << str;
	logFile << str;
    std::cout.flush();
}

void Log::trace(const std::string file, u32 line, const std::string function, const std::string message)
{
	// std::cout << "TRACE > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
	log("TRACE > [" + function + ":" + std::to_string(line) + "] > " + message);
}

void Log::echo(const std::string &message, bool newLine /*= true*/)
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

void Log::custom_echo(const std::string &tag, const std::string &message, bool newLine /*= true*/)
{
    if(newLine)
    {
	    log(tag + " > " + message);
    }
    else
    {
	    append(tag + " > " + message);
    }
}

void Log::error(const std::string &message)
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