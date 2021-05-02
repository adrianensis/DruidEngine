#include "File/File.hpp"
#include "Log/Log.hpp"

namespace DE {

File::File() : ObjectBase() {

}

File::~File() {

}

void File::init() {
	TRACE()
}

void File::readFile(const String &path, std::function<void(std::ifstream &file)> callback) {

	std::ifstream file;
	file.open(path);

	if (callback) {
		callback(file);
	}

	file.close();
}

void File::writeFile(const String &path, std::function<void(std::ofstream &file)> callback) {

	std::ofstream file;
	file.open(path);

	if (callback) {
		callback(file);
	}

	file.close();
}

}
