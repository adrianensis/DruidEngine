#include "Log/Log.hpp"
#include "Core/BasicTypes.hpp"

//#include <filesystem>

const std::string Log::emptyMessage = "";
std::ofstream Log::logFile;

void Log::init(){
	logFile.open("log.txt");
}

void Log::terminate(){
	logFile.close();
}

void Log::log(const std::string& str){
	std::cout << str << std::endl;
	logFile << str << std::endl;
}

void Log::trace(const std::string file, u32 line, const std::string function, const std::string message){
	// std::cout << "TRACE > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
	log("TRACE > [" + function + ":" + std::to_string(line) + "] > " + message);
}

void Log::echo(const std::string& message){
	// std::cout << "ECHO > " << message << std::endl;
	log("ECHO > " + message);
}

void Log::error(const std::string& message){
	// std::cout << "ERROR > " << message << std::endl;
	log("ERROR > " + message);
}

void Log::brline(){ // break line
	log(emptyMessage);
}
