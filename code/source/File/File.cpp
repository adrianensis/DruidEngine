#include "File.hpp"
#include "Log.hpp"

namespace DE {

// ---------------------------------------------------------------------------

File::File() :
		DE_Class() {

}

File::~File() {

}

// ---------------------------------------------------------------------------

void File::init() {
	TRACE();
}

// ---------------------------------------------------------------------------

void File::readFile(const std::string &path,
		std::function<void(std::ifstream &file)> callback) {

	std::ifstream file;
	file.open(path);

	if (callback) {
		callback(file);
	}

	file.close();
}

// ---------------------------------------------------------------------------

void File::writeFile(const std::string &path,
		std::function<void(std::ofstream &file)> callback) {

	std::ofstream file;
	file.open(path);

	if (callback) {
		callback(file);
	}

	file.close();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
