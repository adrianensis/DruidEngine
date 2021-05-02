#include "Log/Log.hpp"
#include "Core/BasicTypes.hpp"

//#include <filesystem>

namespace DE {

const String Log::emptyMessage = "";
std::ofstream Log::logFile;

Log::Log() : ObjectBase() {
}

Log::~Log() = default;

void Log::init() {
	logFile.open("log.txt");
}

void Log::terminate() {
	logFile.close();
}

void Log::log(const String &str) {
	std::cout << str << std::endl;
	logFile << str << std::endl;
}

void Log::trace(const String file, u32 line, const String function, const String message) {
	// std::cout << "TRACE > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
	log("TRACE > [" + function + ":" + std::to_string(line) + "] > " + message);
}

void Log::echo(const String &message) {
	// std::cout << "ECHO > " << message << std::endl;
	log("ECHO > " + message);
}

void Log::error(const String &message) {
	// std::cout << "ERROR > " << message << std::endl;
	log("ERROR > " + message);
}

void Log::brline() { // break line
	log(emptyMessage);
}

}
