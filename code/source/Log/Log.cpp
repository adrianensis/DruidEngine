#include "Log.hpp"
#include "BasicTypes.hpp"

//#include <filesystem>

namespace DE {

const std::string Log::emptyMessage = "";
std::ofstream Log::logFile;

Log::Log() :
		DE_Class() {
}

Log::~Log() = default;

void Log::init() {
	logFile.open("log/log.txt");
}

void Log::terminate() {
	logFile.close();
}

void Log::log(const std::string &str) {
	std::cout << str << std::endl;
	logFile << str << std::endl;
}

void Log::trace(const std::string file, u32 line, const std::string function,
		const std::string message) {
	// std::cout << "DE_TRACE > [" /* << std::experimental::filesystem::path(file).filename() << ":" */ << function << ":" << std::to_string(line) << "] > " << message << std::endl;
	log(
			"DE_TRACE > [" + function + ":" + std::to_string(line) + "] > "
					+ message);
}

void Log::echo(const std::string &message) {
	// std::cout << "DE_ECHO > " << message << std::endl;
	log("DE_ECHO > " + message);
}

void Log::error(const std::string &message) {
	// std::cout << "DE_ERROR > " << message << std::endl;
	log("DE_ERROR > " + message);
}

void Log::brline() { // break line
	log(emptyMessage);
}

}
